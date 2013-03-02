#pragma once

#include <windows.h>
#include "resource.h"


void (*setMouseHook)(HWND);
void (*unSetMouseHook)(void);
bool loadHook(void);
void createButton(HWND hWnd, LPARAM lParam);
void startStopButtonProc(HWND hWnd);
void recMousePos(POINT point);