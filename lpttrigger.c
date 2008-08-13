#include "lpttrigger.h"
#include "lptwriter.h"
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

#define STOP_THREAD	1
#define RESTART_TIMER	2


struct lpttrigger {
	unsigned int duration;
	struct lptport* port;
	volatile int cond_var;
	pthread_cond_t cond;
	pthread_mutex_t cond_mtx;
	pthread_t thread;
	unsigned char base_level;
	unsigned char current_level;
};

#define increase_timespec(timeout, delay)       do {                    \
	unsigned int nsec_duration = (delay) + (timeout).tv_nsec;       \
	(timeout).tv_sec += nsec_duration/1000000000;                   \
	(timeout).tv_nsec = nsec_duration%1000000000;                   \
} while (0)


void* ResettingTriggerFunc(void* arg)
{
	struct lpttrigger* trigg = arg;
	int retcode = 0;
	int run_signal = 0;
	struct timespec timeout;
	pthread_cond_t* cond = &(trigg->cond);
	pthread_mutex_t* mutex = &(trigg->cond_mtx);

	pthread_mutex_lock(&(trigg->cond_mtx));
	while (!(trigg->cond_var & STOP_THREAD)) {
		// Wait for event
		if (run_signal)
			retcode = pthread_cond_timedwait(cond, mutex, &timeout);
		else
			retcode = pthread_cond_wait(cond, mutex);

		if (trigg->cond_var & RESTART_TIMER) {
			// increase the timeout
			clock_gettime(CLOCK_REALTIME, &timeout);
			increase_timespec(timeout, trigg->duration*100000);

			// update the state
			trigg->cond_var &= ~RESTART_TIMER;
			run_signal = 1;

		} else if (retcode == ETIMEDOUT) {
			SetLPTData(trigg->port, trigg->base_level);
			run_signal = 0;
		}
	}
	SetLPTData(trigg->port, trigg->base_level);
	pthread_mutex_unlock(&(trigg->cond_mtx));

	return NULL;
}


struct lpttrigger *OpenLPTTrigger(unsigned char base_level, unsigned int duration, int portnum)
{
	struct lpttrigger* trigg;
	struct lptport* port;

	// Allocate and reserve ressources
	trigg = malloc(sizeof(*trigg));
	port = OpenLPTPort(portnum);
	if (!trigg || !port)
		goto error;

	// Set the parameters
	trigg->port = port;
	trigg->duration = duration;
	trigg->base_level = base_level;

	// Create the resetting thread
	pthread_cond_init(&(trigg->cond), NULL);
	pthread_mutex_init(&(trigg->cond_mtx), NULL);
	if (pthread_create(&(trigg->thread), NULL, ResettingTriggerFunc, trigg) < 0)
		goto error;
	
	SetLPTData(port, base_level);
	return trigg;

error:
	free(trigg);
	CloseLPTPort(port);
	return NULL;
}


void CloseLPTTrigger(struct lpttrigger *trigg)
{
	if (!trigg)
		return;

	// Send Stop signal to resetting thread	
	pthread_mutex_lock(&(trigg->cond_mtx));
	trigg->cond_var |= STOP_THREAD;
	pthread_cond_signal(&(trigg->cond));
	pthread_mutex_unlock(&(trigg->cond_mtx));

	// Wait for the end of the resetting thread
	pthread_join(trigg->thread, NULL);

	// Free ressources
	CloseLPTPort(trigg->port);
	free(trigg);
}


void SignalTrigger(struct lpttrigger *trigg, unsigned int message)
{
	char level;
	if (!trigg)
		return;

	// Send Stop signal to resetting thread	
	pthread_mutex_lock(&(trigg->cond_mtx));
	level = message | trigg->current_level;
	SetLPTData(trigg->port, level);
	trigg->cond_var |= RESTART_TIMER;
	pthread_cond_signal(&(trigg->cond));
	pthread_mutex_unlock(&(trigg->cond_mtx));
}
