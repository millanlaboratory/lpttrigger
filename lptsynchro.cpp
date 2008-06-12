#include "lptsynchro.h"
#include <stdlib.h>

//#ifdef WIN32

#include <windows.h>
#include <Mmsystem.h>
#include "winio.h"

#ifndef _DEBUG
#pragma comment(lib, "winio.lib")
#else
#pragma comment(lib, "winiod.lib")
#endif

#pragma comment(lib, "Winmm.lib")

struct lptsynchro {
	HANDLE syncHandle[2];
	HANDLE thread;
	DWORD baseLevel;
	LARGE_INTEGER dueTime;
};

DWORD WINAPI ResetSignalThread(LPVOID lpParameter);

unsigned long GetTimeStamp()
{
	return timeGetTime();
}

struct lptsynchro *OpenLPTSynchro(unsigned int base_level, unsigned int duration) 
{
	int bResult = FALSE;
	struct lptsynchro *synchro = NULL;

	synchro = (struct lptsynchro*)malloc(sizeof(*synchro));
	if (synchro == NULL)
		return NULL;
    
	synchro->syncHandle[0] = CreateWaitableTimer(NULL, FALSE, NULL);
	synchro->syncHandle[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	synchro->baseLevel = base_level;
	bResult = InitializeWinIo();
    
	if (synchro->syncHandle[0] && synchro->syncHandle[1] && bResult) {
		SetPortVal(0x378, synchro->baseLevel, 1);
		synchro->thread = CreateThread(NULL, 0, ResetSignalThread, synchro, 0, 0);
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

void CloseLPTSynchro(struct lptsynchro * synchro)
{
	if (!synchro)
		return;
	
	
	SetEvent(synchro->syncHandle[1]);
	WaitForSingleObject(synchro->thread, INFINITE);
	
	CloseHandle(synchro->syncHandle[0]);
	CloseHandle(synchro->syncHandle[1]);
	
	ShutdownWinIo();
	
	free(synchro);
}


unsigned long SignalSynchro(struct lptsynchro *synchro,	unsigned int message)
{
	unsigned int pins;
	if (!synchro)
		return 0;
	
	pins = message;
	SetPortVal(0x378, pins ^ (synchro->baseLevel), 1);
	SetWaitableTimer(synchro->syncHandle[0], &synchro->dueTime, 0, NULL, NULL, FALSE);
	return GetTimeStamp();
}


int SetSynchroDuration(struct lptsynchro *synchro, unsigned int duration)
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
	struct lptsynchro *synchro = (struct lptsynchro*) lpParameter;
	HANDLE syncHandles[2];
	DWORD BaseLevel;
	
	syncHandles[0] = synchro->syncHandle[0];
	syncHandles[1] = synchro->syncHandle[1];
	BaseLevel = synchro->baseLevel;
	
	while (WaitForMultipleObjects(2, syncHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
		   SetPortVal(0x378, BaseLevel, 1);
	
	SetPortVal(0x378, BaseLevel, 1);
	
	return 0;
}

//#endif				//WIN32
