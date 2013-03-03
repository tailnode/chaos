// Recorder.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Recorder.h"
#include "mouseHook.h"
#include <windows.h>
#include <stdio.h>
#include "debugOutput.h"
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
FILE* g_file = NULL;

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RECORDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// 加载钩子
	if (!loadHook())
	{
		MessageBox(NULL, L"Error!", L"加载DLL失败", 0);
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RECORDER));

#if DEBUG_OUTPUT_CONSOLE
	// 打开console
	AllocConsole();
	SetConsoleTitle(L"Debug Output(Main)");
	freopen("CONOUT$","w",stdout);
#endif

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


bool loadHook(void)
{
	HINSTANCE hIns = LoadLibrary(L"mouseHook.dll");

	if (NULL == hIns)
	{
		return false;
	}
	
	setMouseHook = (void (__cdecl *)(HWND))GetProcAddress(hIns, "setMouseHook");
	unSetMouseHook = (void (__cdecl *)(void))GetProcAddress(hIns, "unSetMouseHook");

	if (NULL == setMouseHook || NULL == unSetMouseHook)
	{
		return false;
	}
	
	return true;
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RECORDER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RECORDER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 300, 100, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	static int num = 0;
	wchar_t title[40] = {0};

	switch (message)
	{
	case WM_CREATE:
		createButton(hWnd, lParam);
		break;
	case WM_USER_MOUSEPOSTION:
		OUTPUT("receive user message\n");
		POINT* point;
		point = (POINT*)lParam;
		recMousePos(*point);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_START_STOP:
			startStopButtonProc(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void createButton(HWND hWnd, LPARAM lParam)
{
	CreateWindow(TEXT("BUTTON"),		//控件"类名称"
			TEXT("开始"),
			WS_CHILD | WS_VISIBLE |BS_PUSHBUTTON,
			10,
			10,
			50,
			30,
			hWnd,
			(HMENU)IDB_START_STOP,			//控件ID
			((LPCREATESTRUCT) lParam)->hInstance,	//实例句柄
			NULL);
}

void startStopButtonProc(HWND hWnd)
{
	HWND buttonHandle = GetDlgItem(hWnd, IDB_START_STOP);
	wchar_t text[3] = {0};
	GetWindowText(buttonHandle, text, 3);

	if (0 == wcscmp(text, L"开始"))
	{
		SetWindowText(buttonHandle, L"停止");
		
		// 安装钩子
		setMouseHook(hWnd);
		OUTPUT("hook is installed\n");

		// 打开文件用于记录鼠标数据
		g_file = fopen("E://mouseInfo.log", "a");
		if (NULL == g_file)
		{
			OUTPUT("file open fail\n");
			return;
		}
	}
	else
	{
		SetWindowText(buttonHandle, L"开始");

		// 卸载钩子
		unSetMouseHook();
		OUTPUT("hook is uninstalled\n");

		// 关闭文件
		if (NULL != g_file)
		{
			fclose(g_file);
			g_file = NULL;	
		}
	}
}

void recMousePos(POINT point)
{
	char data[15] = {0};
	sprintf(data, "%d %d\n", point.x, point.y);
	fwrite(data, strlen(data), 1, g_file);
	fflush(g_file);
}