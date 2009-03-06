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
#ifndef _CLOCK_GETTIME_H_
#define _CLOCK_GETTIME_H_

#include <config.h>
#include <time.h>

#if !HAVE_DECL_CLOCK_GETTIME
# include "timespec.h"

# if !HAVE_DECL_CLOCKID_T
typedef int clockid_t;
#  define CLOCK_REALTIME	1
# endif //!HAVE_DECL_CLOCKID_T

int clock_gettime(clockid_t clk_id, struct timespec *tp);
# endif //!HAVE_DECL_CLOCK_GETTIME

#endif
