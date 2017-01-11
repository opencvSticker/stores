// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "common.h"

#include <process.h>
#include <windows.h>
#include <WinUser.h>
#include <map>

using namespace std;

map<HWND, DWORD> hWndTothreadMap;
map<DWORD, HWND> threadToHChildWndMap;
const char childWndClassName[] = "_childWndClassName_";
const char * HexBmpViewWndClassName = "_HexBmpViewWndClassName_";

BOOL regWndClass(HMODULE hModule, WNDPROC proc);

LRESULT CALLBACK childWndProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
)
{
	if (uMsg == WM_CREATE)
	{
		DWORD threadID = GetCurrentThreadId();
		threadToHChildWndMap.insert(make_pair(threadID, hwnd));
	}

	if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hDC = NULL;
		hDC = ::BeginPaint(hwnd,&ps);

		char txt[1025];
		for (int i = 0; i< 1024; i++)
		{
			txt[i] = 'w';
		}
		txt[1024] = 0;
		TextOutA(hDC, 10, 70, txt, strlen(txt));

		::EndPaint(hwnd, &ps);		
	}

	if (uMsg == WM_SIZE_WND)
	{
		MoveWindow(hwnd, 0, 0, LOWORD(lParam), HIWORD(lParam),TRUE);
	}
	
	return  ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

unsigned _stdcall createChildWnd(void* param)
{
	HWND hParent = (HWND)param;
	
#ifdef _WIN64
	HINSTANCE hIns = (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE);
#else
	HINSTANCE hIns = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
#endif

	RECT rect;
	::GetClientRect(hParent, &rect);
	HWND hWnd = ::CreateWindowA(childWndClassName, NULL, WS_CHILDWINDOW | WS_VISIBLE,
		rect.left, rect.top, rect.right,rect.bottom, hParent, NULL, hIns, NULL);


	DWORD startPoint = GetTickCount();
	DWORD checkPoint = startPoint;
	
	MSG msg;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		}
	}
	return 0;
}


LRESULT CALLBACK windowProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
)
{
	if (uMsg == WM_CREATE)
	{
#ifdef _WIN64
		LONG styleValue = ::GetWindowLongPtr(hwnd, GWL_STYLE);
		::SetWindowLongPtr(hwnd, GWL_STYLE, styleValue | WS_CLIPCHILDREN);
#else
		LONG styleValue = ::GetWindowLong(hwnd, GWL_STYLE);
		::SetWindowLong(hwnd, GWL_STYLE, styleValue | WS_CLIPCHILDREN);
#endif

		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, createChildWnd, hwnd, NULL, NULL);
		
		hWndTothreadMap[hwnd] = GetThreadId(handle);
	}
	else if (uMsg == WM_SIZE)
	{
		map<HWND, DWORD>::iterator iter = hWndTothreadMap.find(hwnd);
		if (iter != hWndTothreadMap.end())
		{
			map<DWORD, HWND>::iterator childIter;
			childIter = threadToHChildWndMap.find(iter->second);
			if (childIter != threadToHChildWndMap.end())
			{
				HWND hChildWnd = childIter->second;
				PostMessage(hChildWnd, WM_SIZE_WND, wParam, lParam);
			}
		}
	}

	return  ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL regWndClass(HMODULE hModule, WNDPROC proc, const char * className)
{
	WNDCLASSA wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = ::LoadCursor(NULL, IDC_IBEAM);
	wndClass.hIcon = NULL;

	HINSTANCE hIns = GetModuleHandle(NULL);
	wndClass.hInstance = hIns;

	wndClass.lpfnWndProc = proc;
	wndClass.lpszClassName = className;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!::RegisterClassA(&wndClass))
	{
		return false;
	}
	return true;
}

BOOL unregWndClass(HMODULE hModule)
{
	if (!::UnregisterClassA(HexBmpViewWndClassName, hModule))
	{
		return FALSE;
	}

	if (!::UnregisterClassA(childWndClassName, hModule))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL regis(HMODULE hModule)
{
	if (!regWndClass(hModule, windowProc, HexBmpViewWndClassName))
	{
		return false;
	}

	if (!regWndClass(hModule, childWndProc, childWndClassName))
	{
		return false;
	}

	return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		return regis(hModule);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		return unregWndClass(hModule);
		break;
	}
	return TRUE;
}

