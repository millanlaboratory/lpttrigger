#ifndef LPTSYNCHRO_H
#define LPTSYNCHRO_H

#ifdef __cplusplus
extern "C" {
#endif

struct lptsynchro;

struct lptsynchro *OpenLPTSynchro(unsigned int base_level, unsigned int duration);
void CloseLPTSynchro(struct lptsynchro *synchro);
int SetSynchroDuration(struct lptsynchro *synchro, unsigned int duration);
unsigned long SignalSynchro(struct lptsynchro *synchro, unsigned int message);

#ifdef __cplusplus
}
#endif

#endif				//LPTSYNCHRO_H
