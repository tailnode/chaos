#include <objbase.h>
#include <stdio.h>
#include <tchar.h>
#include "mouseHook.h"


HINSTANCE g_hIns = 0;
HHOOK g_hook  = 0;
HWND g_hWnd = 0;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hIns = (HINSTANCE)hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{	
	// Êó±ê×ó/ÓÒ¼ü°´ÏÂ
	if (WM_LBUTTONDOWN == wParam ||WM_RBUTTONDOWN == wParam)
	{
		POINT point = ((PMSLLHOOKSTRUCT)lParam)->pt;

		printf("MouseProc is involked!\tx = %d, y = %d\n", point.x, point.y);
		
		SendMessage(g_hWnd, WM_USER_MOUSEPOSTION, 0, (LPARAM)&point);
	}

	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}
extern "C" _declspec(dllexport) void setMouseHook(HWND hWnd)
{
	if (NULL == g_hook)
	{
		g_hWnd = hWnd;
		g_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, g_hIns, 0);
	}
}

extern "C" _declspec(dllexport) void unSetMouseHook()
{
	if (NULL != g_hook)
	{
		UnhookWindowsHookEx(g_hook);
		g_hook = NULL;
	}
}
