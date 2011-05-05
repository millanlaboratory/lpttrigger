#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "lptwriter.h"
#include "lptpwm.h"

#if !HAVE_CLOCK_GETTIME
# include "../lib/clock_gettime.h"
#endif
#if !HAVE_CLOCK_NANOSLEEP
# include "../lib/clock_nanosleep.h"
#endif



struct lptpwm
{
	struct lptport* port;
	unsigned long duration;
	long* delays;
	unsigned int* data;
	unsigned int numch;

	int run_th;
	pthread_mutex_t	update_mtx;
	pthread_t thread_id;
};

#define NSEC_PER_SEC 1000000000

static
void add_to_timestamp(struct timespec* ts, long nsec)
{
	nsec += ts->tv_nsec;		
	ts->tv_sec += (nsec/NSEC_PER_SEC);
	ts->tv_nsec = nsec%NSEC_PER_SEC;
}

static
void* pulse_width_modulation_thread(void* arg)
{
	struct lptpwm* pwm = arg;
	struct timespec ts, evt_ts;
	struct lptport* port = pwm->port;
	pthread_mutex_t* update_mtx = &(pwm->update_mtx);
	int running = 1;
	long* delays = NULL;
	long frame_duration = pwm->duration;
	unsigned int i, numch = pwm->numch;

	
	// Init 
	delays = malloc(numch*sizeof(*(pwm->delays)));
	SetLPTData(port, 0);
	clock_gettime(CLOCK_REALTIME, &ts);


	while (running) {
		// update pulse length
		pthread_mutex_lock(update_mtx);
		memcpy(delays, pwm->delays, numch*sizeof(*(pwm->delays)));
		running = pwm->run_th;
		pthread_mutex_unlock(update_mtx);

		evt_ts = ts;

		// Generate each pulse (assume that delays are in
		// increasing order)
		for(i=0; i<numch; i++) {
			// a non positive delay means that the pulse should
			// be skipped
			if (delays[i]<=0)
				continue;

			// Change the corresponding levels to parallel port
			SetLPTData(port, pwm->data[i]);

			// Prepare and wait for the next pulse change
			add_to_timestamp(&evt_ts, delays[i]);
			while (clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME,
			                &evt_ts, NULL));
		}

		// We have finish one full frame, so update ts to reflect
		// the timestamp of the beginning of the next frame
		add_to_timestamp(&ts, frame_duration);
	}
	
	SetLPTData(port, 0);
	free(delays);
	
	return NULL;
}


API_EXPORTED
struct lptpwm *lptpwm_open(int freq, unsigned int numch, int portnum)
{
	struct lptpwm* pwm = NULL;
	long* delays;
	unsigned int* data;
	unsigned int i;
	struct lptport* port = NULL;

	numch++;


	// Ressources allocation
	pwm = malloc(sizeof(*pwm));
	delays = malloc(sizeof(*delays)*numch);
	data = malloc(sizeof(*data)*numch);
	if (!pwm || !delays || !data)
		goto error;

	port = OpenLPTPort(portnum);
	if (!port)
		goto error;

	// Fill the struct
	pwm->delays = delays;
	pwm->duration = NSEC_PER_SEC / freq;
	pwm->data = data;
	pwm->numch = numch;
	pwm->port = port;

	for (i=0; i<numch-1; i++) {
		delays[i] = -1;
		data[i] = 1 << i;
	}
	data[numch-1] = 0;
	delays[numch-1] = pwm->duration;

	
	// Create the thread
	pwm->run_th = 1;
	pthread_mutex_init(&(pwm->update_mtx), NULL);
	if (pthread_create(&(pwm->thread_id), NULL, pulse_width_modulation_thread, pwm)) 
		goto error;
	

	return pwm;

error:
	free(delays);
	free(data);
	free(pwm);
	CloseLPTPort(port);
	return NULL;
}

API_EXPORTED
void lptpwm_close(struct lptpwm *pwm)
{
	if (pwm) {
		pthread_mutex_lock(&(pwm->update_mtx));	
		pwm->run_th = 0;
		pthread_mutex_unlock(&(pwm->update_mtx));
		
		pthread_join(pwm->thread_id, NULL);
		pthread_mutex_destroy(&(pwm->update_mtx));

		CloseLPTPort(pwm->port);

		free(pwm->delays);
		free(pwm->data);
		free(pwm);
	}
}


API_EXPORTED
void lptpwm_setlevels(struct lptpwm *pwm, const float val[])
{
	unsigned int i;
	long remaining = pwm->duration;

	for (i=0; i<pwm->numch-1; i++) {
		if (val[i] <= 0.0f)
			pwm->delays[i] = -1;
		else {
			pwm->delays[i] = (long)(val[i] * (float)(pwm->duration));
			remaining -= pwm->delays[i];
		}
	}
	pwm->delays[pwm->numch-1] = remaining;
}

