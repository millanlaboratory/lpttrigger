#include <stdio.h>
#include <unistd.h>
#include <lptpwm.h>


#define NUMCH	4
#define FREQ	100

int main(int argc, char* argv[])
{
	struct lptpwm* pwm;
	float val[NUMCH] = {0.1f, 0.1f, 0.1f, 0.1f};
	int i,j;

	pwm = lptpwm_open(FREQ, NUMCH, -1);
	if (!pwm) {
		fprintf(stderr, "OPening the port failed\n");
		return 1;
	}

		lptpwm_setlevels(pwm, val);
	for (i=0; i<20; i++) {
	//	for (j=0; j<NUMCH; j++) {
	//		val[j] = 0.5f;
	//	}
	//	lptpwm_setlevels(pwm, val);
		sleep(1);
	}


	lptpwm_close(pwm);

	return 0;
}
