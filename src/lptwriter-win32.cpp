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
#include <tchar.h>
//#include "winio_dll.h"
#include "lptwriter.h"


typedef bool (_stdcall *INITIALIZEWINIOPROC)(void);
typedef void (_stdcall *SHUTDOWNWINIOPROC)(void);
typedef PBYTE (_stdcall *MAPPHYSTOLINPROC)(PBYTE, DWORD, HANDLE*);
typedef bool (_stdcall *UNMAPTHYSICALMEMORYPROC)(HANDLE, PBYTE);
typedef bool (_stdcall *GETPHYSLONGPROC)(PBYTE pbPhysAddr, PDWORD);
typedef bool (_stdcall *SETPHYSLONGPROC)(PBYTE pbPhysAddr, DWORD);
typedef bool (_stdcall *GETPORTVALPROC)(WORD, PDWORD, BYTE);
typedef bool (_stdcall *SETPORTVALPROC)(WORD, DWORD, BYTE);
typedef bool (_stdcall *INSTALLWINIODRIVERPROC)(PSTR, bool);
typedef bool (_stdcall *REMOVEWINIODRIVERPROC)();

INITIALIZEWINIOPROC InitializeWinIo;
SHUTDOWNWINIOPROC ShutdownWinIo;
SETPORTVALPROC SetPortVal;
HMODULE hDLL = NULL;

static int num_use = 0;

struct lptport {
	unsigned short addr;
};


static
bool init_winio(void)
{
	hDLL = LoadLibrary(_T("WinIo.dll"));
	if (!hDLL)
		return false;
	
	InitializeWinIo = (INITIALIZEWINIOPROC) GetProcAddress(hDLL, "InitializeWinIo");
	ShutdownWinIo = (SHUTDOWNWINIOPROC) GetProcAddress(hDLL, "ShutdownWinIo");
	SetPortVal =	(SETPORTVALPROC) GetProcAddress(hDLL, "SetPortVal");
	
	if (!InitializeWinIo || !ShutdownWinIo || !SetPortVal)
		return false;

	return InitializeWinIo();
}


static
int close_winio(void)
{
	if (hDLL) {
		if (ShutdownWinIo)
			ShutdownWinIo();
		FreeLibrary(hDLL);
	}
	hDLL = NULL;
	return 1;
}


API_EXPORTED
struct lptport* OpenLPTPort(int portnum)
{	
	struct lptport* port;
	
	// Initialize the WinIO lib once for all
	if (!num_use)
		if (!init_winio())
			return NULL;
	num_use++;

	// Define the port to use
	if (portnum < 0)
		portnum = 0x378;

	// allocate the ressource
	port = (struct lptport*)std::malloc(sizeof(&port));
	if (!port) {
		if (!--num_use)
			close_winio();
		return NULL;
	}
	
	
	port->addr = portnum;
	return port;	
}


API_EXPORTED
void CloseLPTPort(struct lptport* port)
{
	if (port) {
		if (!--num_use)
			close_winio();
		free(port);
	}
}


API_EXPORTED
void SetLPTData(struct lptport* port, unsigned char data)
{
	if (port)
		SetPortVal(port->addr, data, 1);
}


