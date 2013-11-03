// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
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

void openRealtekMng();
HWND findWindow(TCHAR* name);
extern "C" _declspec(dllexport) void muteOn();
extern "C" _declspec(dllexport) void muteOff();
void showWindow();
void hideWindow();

// 打开“Realtek高清晰音频管理器”
void openRealtekMng()
{
	const TCHAR file[] = L"RAVCpl64.exe";
	const TCHAR path[] = L"C:\\Program Files\\Realtek\\Audio\\HDA";

	ShellExecute(NULL, NULL, file, NULL, path, SW_SHOW);
}

// 取得“Realtek高清晰音频管理器”窗口的句柄
HWND findWindow(TCHAR* name)
{
	HWND hWnd = 0;

	while (0 == hWnd)
	{
		hWnd = FindWindow(NULL, name);
		Sleep(100);
	}

	return hWnd;
}

// 打开"该设备应被验证为："窗口
void showWindow()
{
	RECT rect;

	openRealtekMng();

	HWND hWnd = findWindow(L"Realtek高清晰音频管理器");
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	GetWindowRect(hWnd, &rect);
	SetCursorPos(rect.left, rect.top);

	mouse_event(MOUSEEVENTF_MOVE, 750, 220, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 750, 220, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, 750, 220, 0, 0);
	
	Sleep(100);

	mouse_event(MOUSEEVENTF_MOVE, 5, 5, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	hWnd = findWindow(L"该设备应被验证为：");
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	GetWindowRect(hWnd, &rect);
	SetCursorPos(rect.left, rect.top);
}

void hideWindow()
{
	HWND hWnd = findWindow(L"Realtek高清晰音频管理器");	
	ShowWindow(hWnd, SW_HIDE);
}


extern "C" _declspec(dllexport) void muteOn()
{
	POINT backup;
	GetCursorPos(&backup);

	showWindow();

	mouse_event(MOUSEEVENTF_MOVE, 25, 70, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

	hideWindow();

	SetCursorPos(backup.x, backup.y);
}

extern "C" _declspec(dllexport) void muteOff()
{
	POINT backup;
	GetCursorPos(&backup);

	showWindow();

	mouse_event(MOUSEEVENTF_MOVE, 25, 105, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	
	hideWindow();

	SetCursorPos(backup.x, backup.y);
}