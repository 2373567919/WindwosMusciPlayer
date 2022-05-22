#ifndef FUN_H
#define FUN_H
#define UNICODE  1
#define _UNICODE 1
#include "tchar.h"
#include <shlobj.h>
#include "windows.h"
#include <vector>
#include "include/bass.h"
#include "include/bassflac.h"
#include <string>
void Open(PTCHAR szFileName);
void Play();
void Pause();
void Stop();
LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL EumeFile(PTCHAR lpszPath);
void CALLBACK endCallback(HSYNC handle, DWORD channel, DWORD data, void* pTarget);

#endif