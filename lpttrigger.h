#ifndef _LPTTRIGGER_H_
#define _LPTTRIGGER_H_


struct lpttrigger;

struct lpttrigger *OpenLPTTrigger(unsigned char base_level, unsigned int duration, int portnum);
void CloseLPTTrigger(struct lpttrigger *trigg);
void SignalTrigger(struct lpttrigger *trigg, unsigned int message);

#endif //_LPTTRIGGER_H_
