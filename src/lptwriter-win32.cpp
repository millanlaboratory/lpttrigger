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


