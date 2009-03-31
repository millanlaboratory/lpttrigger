#ifndef WINIO_H
#define WINIO_H

#ifdef WINIO_DLL
#define WINIO_API _declspec(dllexport)
#else
#define WINIO_API _declspec(dllimport)
#endif

extern "C"
{
  WINIO_API bool __stdcall InitializeWinIo();
  WINIO_API void __stdcall ShutdownWinIo();
  WINIO_API PBYTE __stdcall MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle);
  WINIO_API bool __stdcall UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr);
  WINIO_API bool __stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
  WINIO_API bool __stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
  WINIO_API bool __stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
  WINIO_API bool __stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
  WINIO_API bool __stdcall InstallWinIoDriver(PSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
  WINIO_API bool __stdcall RemoveWinIoDriver();
}

extern bool IsNT;
extern HANDLE hDriver;
extern bool IsWinIoInitialized;

bool __stdcall StartWinIoDriver();
bool __stdcall StopWinIoDriver();

#endif
