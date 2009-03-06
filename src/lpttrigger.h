/**
 * \file lpttrigger.h
 * \brief Definitions of the LPTtrigger API
 * \author Nicolas Bourdaud
 * \version 0.2
 * \date 14/08/2008
 *
 * This is the definition of the API of LPTtrigger
 *
 */
#ifndef _LPTTRIGGER_H_
#define _LPTTRIGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct lpttrigger;

struct lpttrigger *OpenLPTTrigger(unsigned char base_level, unsigned int duration, int portnum); //!< Open the LPT port and allocate the ressource
void CloseLPTTrigger(struct lpttrigger *trigg); //!< Close the connection to the port and free ressources
void SignalTrigger(struct lpttrigger *trigg, unsigned int message); //!< Send a trigger signal

#ifdef __cpluplus
}
#endif

#endif //_LPTTRIGGER_H_
