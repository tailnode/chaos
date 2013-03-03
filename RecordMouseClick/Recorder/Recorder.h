#pragma once

#include <windows.h>
#include "resource.h"

#define LOG_FILE_PATH		"E://mouseInfo.log"
#define IMAGE_FILE_PATH		

void (*setMouseHook)(HWND);
void (*unSetMouseHook)(void);
bool loadHook(void);
void createButton(HWND hWnd, LPARAM lParam);
void startStopButtonProc(HWND hWnd);
void recMousePos(POINT point);
bool generateImage(HWND hWnd);
ATOM registerImageClass(HINSTANCE hInstance);
LRESULT CALLBACK imageWndProc(HWND, UINT, WPARAM, LPARAM);