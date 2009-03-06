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
#ifndef _LPTWRITER_H_
#define _LPTWRITER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct lptport;


struct lptport* OpenLPTPort(int portnum);
void CloseLPTPort(struct lptport* port);
void SetLPTData(struct lptport* port, unsigned char data);


#ifdef __cplusplus
}
#endif

#endif //_LPTWRITER_H_
