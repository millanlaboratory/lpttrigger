#include "lptsynchro.h"
#include <stdlib.h>

#ifdef WIN32

#include <windows.h>
#include <Mmsystem.h>
#include "winio.h"
struct lptsynchro {
	HANDLE syncHandle[2];
	DWORD baseLevel;
	LARGE_INTEGER dueTime;
};

inline unsigned long GetTimeStamp()
{
	return timeGetTime();
}

struct lptsynchro *OpenLPTSynchro(unsigned int base_level,
				  unsigned int duration) 
{
	int bResult = FALSE;
	struct lptsynchro *synchro = NULL;

	synchro = malloc(sizeof(*synchro));
	if (synchro == NULL)
		return NULL;
	synchro->syncHandle[0] = CreateWaitableTimer(NULL, FALSE, NULL);
	synchro->syncHandle[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	bResult = InitializeWinIo();
	if (synchro->syncHandle[0] && synchro->syncHandle[1] && bResult) {
		SetPortVal(0x378, synchro->baseLevel, 1);
		CreateThread(NULL, 0, ResetSignalThread, synchro, 0, 0);
	} else {
		if (synchro->syncHandle[0])
			CloseHandle(synchro->syncHandle[0]);
		if (synchro->syncHandle[1])
			CloseHandle(synchro->syncHandle[1]);
		if (bResult)
			ShutdownWinIo();
		synchro->syncHandle[0] = synchro->syncHandle[1] = NULL;
		free(synchro);
		return NULL;
	}

	SetSynchroDuration(synchro, duration);

	return synchro;
}

CloseLPTSynchro(struct lptsynchro * synchro)
{
	if (synchro->syncHandle[0]) {
		SetEvent(synchro->syncHandle[1]);
		
		    //ShutdownWinIo();
	}
}
unsigned long SignalSynchro(struct lptsynchro *synchro,
			      unsigned int message)
{
	uint32_t pins;
	if (!synchro->syncHandle[0])
		return 0;
	uint32_t pins = message;
	SetPortVal(0x378, pins ^ (synchro->baseLevel), 1);
	SetWaitableTimer(syncHandle[0], &dueTime, 0, NULL, NULL, FALSE);
	return GetTimeStamp();
}
int SetSynchroDuration(struct lptsynchro *synchro,
			 unsigned int duration)
{
	if (!synchro->syncHandle[0])
		return 0;
	synchro->dueTime.LowPart = duration * 10000;
	synchro->dueTime.HighPart = 0;
	synchro->dueTime.QuadPart *= -1;
	return 1;
}

/********************************************************************/
DWORD WINAPI ResetSignalThread(LPVOID lpParameter)
{
	struct lptsynchro *synchro = lpParameter;
	HANDLE syncHandles[2];
	DWORD BaseLevel;

	syncHandles[0] = synchro->syncHandle[0];
	syncHandles[1] = synchro->syncHandle[1];
	BaseLevel = synchro->baseLevel;

	while (WaitForMultipleObjects(2, syncHandles, FALSE, INFINITE) ==
	       WAIT_OBJECT_0)
		SetPortVal(0x378, BaseLevel, 1);

	SetPortVal(0x378, BaseLevel, 1);

	CloseHandle(synchro->syncHandles[0]);
	CloseHandle(synchro->syncHandles[1]);

	ShutdownWinIo();
	return 0;
}

#endif				//WIN32
