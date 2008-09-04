#include <stdio.h>
#include "lpttrigger.h"

#ifndef WIN32
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

int getch(void)
{
	int c = 0;

	struct termios org_opts, new_opts;
	int res = 0;
	//-----  store old settings -----------
	res = tcgetattr(STDIN_FILENO, &org_opts);
	assert(res == 0);
	//---- set new terminal parms --------
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &=
	    ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE |
	      ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
	c = getchar();
	//------  restore old settings ---------
	res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res == 0);
	return (c);
}

#else				// WIN32
#include <conio.h>
#endif				// WIN32


#define ESCAPE_CODE 27

unsigned int to_decbin(unsigned int integer)
{
	unsigned int i, binint, inc;

	binint = 0;
	inc = 1;
	for (i = 0; i < 8; i++) {
		binint += (integer & (1 << i)) ? inc : 0;
		inc *= 10;
	}
	return binint;
}


int main(int argc, char *argv[])
{
	struct lpttrigger *trigg;
	unsigned int data;
	int key, retcode;
	int keystroke_mode = 0;
	char value[16];

	trigg = OpenLPTTrigger(0, 200, -1);
	if (!trigg)
		return 1;

	printf("Enter an hexadecimal number to be sent to the parallel port (number will be troncate to an 8 bits integer) or a command:\n"
	"\t'key' to enter into the keystroke mode\n"
	"\t'exit' to from the software\n");

	while (1) {
		if (keystroke_mode) {
			// Keystroke mode
			key = getch();
			if (key == ESCAPE_CODE) {
				printf("normal mode: enter an hexadecimal number or a command ('key' or 'exit')\n");
				keystroke_mode = 0;
				continue;
			}
			if ((key < 0x31) || (key > 0x38))
				continue;
			data = 1 << (key - 0x31);
		} else {
			// Normal mode
			printf("input> ");
			retcode = scanf("%15s",value);
			if (retcode == EOF) 
				break;
			if (strcmp("key", value)==0) {
				printf("keystroke mode. Press ESC to exit the mode.\n");
				keystroke_mode = 1;
				continue;
			}
			if (strcmp("exit", value)==0)
				break;

			sscanf(value, "%x", &data);
		}

		data &= 0xff;
		printf("\tsend 0x%02x  (%08u)\n", data, to_decbin(data));
//              SignalTrigger(trigg, i);
	}

	CloseLPTTrigger(trigg);
	return 0;
}

