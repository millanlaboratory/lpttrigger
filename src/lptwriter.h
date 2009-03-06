#ifndef _LPTWRITER_H_
#define _LPTWRITER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct lptport;


struct lptport* OpenLPTPort(int portnum);
void CloseLPTPort(struct lptport* port);
void SetLPTData(struct lptport* port, unsigned char data);


#ifdef __cplusplus
}
#endif

#endif //_LPTWRITER_H_
