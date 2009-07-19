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
/**
 * \file lpttrigger.h
 * \brief Definitions of the LPTtrigger API
 * \author Nicolas Bourdaud
 * \version 0.2
 * \date 14/08/2008
 *
 * This is the definition of the API of LPTtrigger
 *
 */
#ifndef _LPTTRIGGER_H_
#define _LPTTRIGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct lpttrigger;

struct lpttrigger *OpenLPTTrigger(unsigned char base_level, unsigned int duration, int portnum); //!< Open the LPT port and allocate the ressource
void CloseLPTTrigger(struct lpttrigger *trigg); //!< Close the connection to the port and free ressources
void SignalTrigger(struct lpttrigger *trigg, unsigned int message); //!< Send a trigger signal

const char* lpttrigger_get_string(void);

#ifdef __cplusplus
}
#endif

#endif //_LPTTRIGGER_H_
