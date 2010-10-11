/*
	Copyright (C) 2009  EPFL (Ecole Polytechnique Fédérale de Lausanne)
	Nicolas Bourdaud <nicolas.bourdaud@epfl.ch>

    This file is part of the lpttrigger library

    The lpttrigger library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \internal
 * \file lpttrigger.c
 * \author Nicolas Bourdaud
 * \version 0.1
 * \date 14/08/2008
 *
 * This is the implementation of the LPTtrigger
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

#if !HAVE_CLOCK_GETTIME
# include "../lib/clock_gettime.h"
#endif


#include "lpttrigger.h"
#include "lptwriter.h"

#define STOP_THREAD	1
#define RESTART_TIMER	2

const char lpt_version[] = PACKAGE_STRING;

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


static
void* ResettingTriggerFunc(void* arg)
{
	struct lpttrigger* trigg = arg;
	int retcode = 0;
	int run_signal = 0;
	volatile unsigned char* curr_level = &(trigg->current_level);
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
			increase_timespec(timeout, trigg->duration*1000000);

			// update the state
			trigg->cond_var &= ~RESTART_TIMER;
			run_signal = 1;

		} else if (retcode == ETIMEDOUT) {
			SetLPTData(trigg->port, trigg->base_level);
			*curr_level = trigg->base_level;
			run_signal = 0;
		}
	}
	SetLPTData(trigg->port, trigg->base_level);
	pthread_mutex_unlock(&(trigg->cond_mtx));
	pthread_exit(0);
	return NULL;
}


/*!
 * \param base_level 	The rest level of all bits
 * \param duration	The minimum duration of a pulse in milliseconds
 * \param portnum	The value of desired port to be used. A negative value means that the default port should be used
 * \return a pointer to a \c lpttrigger structure refering the port and options used
 * 
 * Open the port specified by \c portnum. \c portnum is a negative value, the port opened is the default port. The meaning of a non negtive value is platform dependent: On unix like OS (Linux and Mac OS X), it refers to the number of the device name (for example /dev/parport1 on linux). On windows, it refers to the address value.
 * The \c base_level parameter is the "rest state" of the pins of the parallel port. It is useful particularly if the system you work with detect falling edge of signals.
 *
 */
API_EXPORTED
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
	trigg->current_level = base_level;

	// Create the resetting thread
	trigg->cond_var = 0;
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

/*!
 * \param trigg 	the \c lpttrigger struct you want to free
 *  
 * \c trigg can be NULL. In that case, the function do nothing.
 */
API_EXPORTED
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

/*!
 * \param trigg 	the \c lpttrigger structure previously created
 * \param message	a bitmask representing the bits whose state should be changed
 *
 * Change the state of the bits specified by \c message for \e at \e least \c trigg->duration milliseconds. The function returns immediatly.
 */
API_EXPORTED
void SignalTrigger(struct lpttrigger *trigg, unsigned int message)
{
	if (!trigg)
		return;

	// Send Stop signal to resetting thread	
	pthread_mutex_lock(&(trigg->cond_mtx));
	trigg->current_level |= message ;
	SetLPTData(trigg->port, trigg->current_level);
	trigg->cond_var |= RESTART_TIMER;
	pthread_cond_signal(&(trigg->cond));
	pthread_mutex_unlock(&(trigg->cond_mtx));
}


API_EXPORTED
const char* lpttrigger_get_string(void)
{
	return lpt_version;
}
