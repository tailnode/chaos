#pragma once

#include <windows.h>

#define LOG_FILE_PATH		"E://mouseInfo.log"

#define IDB_START_STOP		0x01	// ��ʼ/������ť
#define IDB_GENERATE_IMAGE	0x02	// ���ɰ�ť


void (*setMouseHook)(HWND);
void (*unSetMouseHook)(void);
bool loadHook(void);
void createButton(HWND hWnd, LPARAM lParam);
void startStopButtonProc(HWND hWnd);
void recMousePos(POINT point);
bool generateImage(HWND hWnd);
ATOM registerImageClass(HINSTANCE hInstance);
LRESULT CALLBACK imageWndProc(HWND, UINT, WPARAM, LPARAM);