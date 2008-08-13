#include <stdio.h>
#include <unistd.h>
#include "lpttrigger.h"

int main(int argc, char* argv[])
{
	struct lpttrigger* trigg;
	int i;

	trigg = OpenLPTTrigger(0, 200, -1);
	if (!trigg)
		return 1;

	for (i=0; i<5; i++) {
		SignalTrigger(trigg, i);
		printf("%i\n",i);
		sleep(1);
	}

	CloseLPTTrigger(trigg);
	return 0;
}
