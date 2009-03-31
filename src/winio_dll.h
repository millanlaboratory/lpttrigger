#ifdef WINIO_DLL_H
#define WINIO_DLL_H

#include <windows.h>

#define WINIO_API _declspec(dllimport)

typedef WINIO_API bool (_stdcall *INITIALIZEWINIOPROC)(void);
typedef WINIO_API void (_stdcall *SHUTDOWNWINIOPROC)(void);
typedef WINIO_API PBYTE (_stdcall *MAPPHYSTOLINPROC)(PBYTE, DWORD, HANDLE*);
typedef WINIO_API bool (_stdcall *UNMAPTHYSICALMEMORYPROC)(HANDLE, PBYTE);
typedef WINIO_API bool (_stdcall *GETPHYSLONGPROC)(PBYTE pbPhysAddr, PDWORD);
typedef WINIO_API bool (_stdcall *SETPHYSLONGPROC)(PBYTE pbPhysAddr, DWORD);
typedef WINIO_API bool (_stdcall *GETPORTVALPROC)(WORD, PDWORD, BYTE);
typedef WINIO_API bool (_stdcall *SETPORTVALPROC)(WORD, DWORD, BYTE);
typedef WINIO_API bool (_stdcall *INSTALLWINIODRIVERPROC)(PSTR, bool);
typedef WINIO_API bool (_stdcall *REMOVEWINIODRIVERPROC)();
 
extern bool IsNT;
extern HANDLE hDriver;
extern bool IsWinIoInitialized;






#endif //WINIO_DLL_H
