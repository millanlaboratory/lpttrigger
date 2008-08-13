#include <stdio.h>
#include <lpttrigger.h>

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(duration) Sleep((duration*1000))
#endif

int main(int argc, char* argv[])
{
	struct lpttrigger* trigg;
	int i;

	trigg = OpenLPTTrigger(0, 200, -1);
	if (!trigg)
		return 1;

	for (i=0; i<10; i++) {
		printf("%i\n",i);
		SignalTrigger(trigg, i);
		sleep(1);
	}

	CloseLPTTrigger(trigg);
	return 0;
}
