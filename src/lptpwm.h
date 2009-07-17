#ifndef LPTPWM_H
#define LPTPWM_H


#ifdef __cplusplus
extern "C" {
#endif

struct lptpwm;

struct lptpwm* lptpwm_open(int freq, unsigned int numch, int portnum);
void lptpwm_close(struct lptpwm *pwm);
void lptpwm_setlevels(struct lptpwm *pwm, const float val[]);

#ifdef __cplusplus
}
#endif

#endif
