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


#include "lptwriter.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct lptport {
	int fd;
};

struct lptport* OpenLPTPort(int portnum)
{
	char filename[32];
	struct lptport* port;
	int mode, fd;
	
	// Define the device to open
	if (portnum<0)
		portnum = 0;
	snprintf(filename, 31, "/dev/parport%i", portnum);
	
	// Open the device
	fd = open(filename, O_RDONLY);
	if (fd<0)
		return NULL;

	// Set the device parameters
	/*if (ioctl(fd, PPEXCL)<0) {
		close(fd);
		return NULL;
	}*/
	if (ioctl(fd, PPCLAIM)<0) {
		close(fd);
		return NULL;
	}
	mode = IEEE1284_MODE_COMPAT;
	ioctl(fd, PPNEGOT, &mode);
	
	
	// Allocate the struct
	port = malloc(sizeof(*port));
	if (!port) {
		close(fd);
		return NULL;
	}
	port->fd = fd;
	return port;
	
}


void CloseLPTPort(struct lptport* port)
{
	if (port) {
		close(port->fd);
		free(port);
	}
}

void SetLPTData(struct lptport* port, unsigned char data)
{
	if (port)
		ioctl(port->fd, PPWDATA, &data);
}

