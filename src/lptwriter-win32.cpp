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
#if HAVE_CONFIG_H
# include <config.h>
#endif


#include <windows.h>
#include <cstdlib>
#include "Winio.h"
#include "lptwriter.h"

static int num_use = 0;

struct lptport {
	unsigned short addr;
};


struct lptport* OpenLPTPort(int portnum)
{	
	struct lptport* port;
	
	// Initialize the WinIO lib once for all
	if (!num_use)
		if (!InitializeWinIo())
			return NULL;
	num_use++;

	// Define the port to use
	if (portnum < 0)
		portnum = 0x378;

	// allocate the ressource
	port = (struct lptport*)std::malloc(sizeof(&port));
	if (!port) {
		if (!--num_use)
			ShutdownWinIo();
		return NULL;
	}
	
	
	port->addr = portnum;
	return port;	
}

void CloseLPTPort(struct lptport* port)
{
	if (port) {
		if (!--num_use)
			ShutdownWinIo();
		free(port);
	}
}

void SetLPTData(struct lptport* port, unsigned char data)
{
	if (port)
		SetPortVal(port->addr, data, 1);
}


