/*
	Copyright (C) 2009  EPFL (Ecole Polytechnique Fédérale de Lausanne)
	Nicolas Bourdaud <nicolas.bourdaud@epfl.ch>

    This file is part of the act2demux library

    The act2demux library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#if HAVE_CONFIG_H
# include <config.h>
#endif
#include "clock_gettime.h"

#if HAVE_GETTIMEOFDAY
# include <sys/time.h>
#endif //HAVE_GETTIMEOFDAY

#include <errno.h>

#if HAVE__FTIME || HAVE_FTIME
# include <sys/timeb.h>
# if !HAVE_FTIME
#  define ftime _ftime
#  define timeb _timeb
#  define HAVE_FTIME
# endif //!HAVE_FTIME
#endif //HAVE__FTIME

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	(void)clk_id;
	if (!tp) {
		errno = EFAULT;
		return -1;
	}

#ifdef HAVE_GETTIMEOFDAY
	{
		struct timeval tv;
	
		gettimeofday(&tv, NULL);
		tp->tv_sec = tv.tv_sec;
		tp->tv_nsec = tv.tv_usec*1000;
	}
#elif HAVE_FTIME
	{
		struct timeb now;

		ftime(&now);
		tp->tv_sec = now.time;
		tp->tv_nsec = now.millitm*1000000;
	}
#else
# error There is no replacement for clock_gettime
#endif
	return 0;
}

