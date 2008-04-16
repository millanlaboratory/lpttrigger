#ifndef LPTSYNCHRO_H
#define LPTSYNCHRO_H

struct lptsynchro;
struct lptsynchro* OpenLPTSynchro(uint32_t base_level, unsigned int duration);
CloseLPTSynchro(struct lptsynchro* synchro);
int SetSynchroDuration(struct lptsynchro* synchro, unsigned int duration);

#endif	//LPTSYNCHRO_H
