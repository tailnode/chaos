#include <objbase.h>
#include <tchar.h>
#include "mouseHook.h"
#include "debugOutput.h"

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

// 处理鼠标事件
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{	
	// 鼠标左/右键按下
	if (WM_LBUTTONDOWN == wParam ||WM_RBUTTONDOWN == wParam)
	{
		POINT point = ((PMSLLHOOKSTRUCT)lParam)->pt;

		OUTPUT("MouseProc is involked!\tx = %d, y = %d\n", point.x, point.y);
		
		// 向调用此DLL的窗口发送鼠标坐标消息
		SendMessage(g_hWnd, WM_USER_MOUSEPOSTION, 0, (LPARAM)&point);
	}

	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

// 启动钩子
extern "C" _declspec(dllexport) void setMouseHook(HWND hWnd)
{
	if (NULL == g_hook)
	{
		g_hWnd = hWnd;
		g_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, g_hIns, 0);
	}
}

// 结束钩子
extern "C" _declspec(dllexport) void unSetMouseHook()
{
	if (NULL != g_hook)
	{
		UnhookWindowsHookEx(g_hook);
		g_hook = NULL;
	}
}
