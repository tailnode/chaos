// Recorder.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Recorder.h"
#include "mouseHook.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
FILE* g_file = NULL;

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RECORDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// ���ع���
	if (!loadHook())
	{
		MessageBox(NULL, L"Error!", L"����DLLʧ��", 0);
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RECORDER));

	// ��console
	AllocConsole();
	SetConsoleTitle(L"Debug Output(Main)");
	freopen("CONOUT$","w",stdout);

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		cout << "receive user event" << endl;
		POINT* point;
		point = (POINT*)lParam;
		recMousePos(*point);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
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

// �����ڡ������Ϣ�������
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
	CreateWindow(TEXT("BUTTON"),		//�ؼ�"������"
			TEXT("��ʼ"),
			WS_CHILD | WS_VISIBLE |BS_PUSHBUTTON,
			10,
			10,
			50,
			30,
			hWnd,
			(HMENU)IDB_START_STOP,			//�ؼ�ID
			((LPCREATESTRUCT) lParam)->hInstance,	//ʵ�����
			NULL);
}

void startStopButtonProc(HWND hWnd)
{
	HWND buttonHandle = GetDlgItem(hWnd, IDB_START_STOP);
	wchar_t text[3] = {0};
	GetWindowText(buttonHandle, text, 3);

	if (0 == wcscmp(text, L"��ʼ"))
	{
		SetWindowText(buttonHandle, L"ֹͣ");
		
		// ��װ����
		setMouseHook(hWnd);
		cout << "hook is installed" << endl;

		// ���ļ����ڼ�¼�������
		g_file = fopen("E://mouseInfo.log", "a");
		if (NULL == g_file)
		{
			cout << "file open fail" << endl;
			return;
		}
	}
	else
	{
		SetWindowText(buttonHandle, L"��ʼ");

		// ж�ع���
		unSetMouseHook();
		cout << "hook is uninstalled" << endl;

		// �ر��ļ�
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