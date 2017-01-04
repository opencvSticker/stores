// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "noteBookInterface.h"
#include <process.h>
#include <memory>
#include <map>
#include "sticker/stickerRegions.h"
#include "sticker/wndFocusManager.h"

using namespace std;
using namespace stickerEditor;


BOOL regWndClass(HMODULE hModule, WNDPROC proc);

const char childWndClassName[] = "_childWndClassName_";
map<DWORD, shared_ptr<StickerRegions>> stickerRegionsMap;
map<HWND, DWORD> hWndTothreadMap;

void test()
{
	int a;
	a = 0;
}
LRESULT CALLBACK childWndProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
)
{	
	DWORD threadID = GetCurrentThreadId();

	if (uMsg == WM_CREATE)
	{		
		stickerRegionsMap[threadID] = make_shared<StickerRegions>(hwnd);
	}

	WndFocusManager::onMsg(hwnd, uMsg, wParam, lParam);

	map<DWORD, shared_ptr<StickerRegions>>::iterator iter = stickerRegionsMap.find(threadID);
	if (iter!=stickerRegionsMap.end())
	{
		LRESULT re = iter->second->onMsg(hwnd, uMsg, wParam, lParam);
		if (re != 0)
		{
			return 0;
		}		
	}

	return  ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

unsigned _stdcall createChildWnd(void* param)
{
	HWND hParent = (HWND)param;
	HINSTANCE hIns = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);

	RECT rect;
	::GetClientRect(hParent, &rect);
	HWND hWnd = ::CreateWindowA(childWndClassName, NULL, WS_CHILDWINDOW | WS_VISIBLE ,
		rect.left, rect.top, rect.right, rect.bottom, hParent, NULL, hIns, NULL);
	
	
	DWORD startPoint = GetTickCount();
	DWORD checkPoint = startPoint;
	
	RECT rc;
	RECT oldRc;
	memset(&oldRc, 0, sizeof RECT);

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
			checkPoint = GetTickCount();
			if (!stickerRegionsMap.empty() && (checkPoint - startPoint) > 500  )
			{
				startPoint = checkPoint;

				map<DWORD, shared_ptr<StickerRegions>>::iterator iter;
				iter = stickerRegionsMap.begin();
				while (iter != stickerRegionsMap.end())
				{
					iter->second->drawCursor();
					iter++;
				}				
			}	
		}
	}
	return 0;
}

HWND getChildHWndByThreadId(DWORD threadID)
{
	if (threadID == 0)
	{
		return NULL;
	}

	map<DWORD, shared_ptr<StickerRegions>>::iterator iter;
	iter = stickerRegionsMap.find(threadID);
	if (iter != stickerRegionsMap.end())
	{
		return iter->second->getHWnd();
	}

	return NULL;
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
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, createChildWnd, hwnd, NULL, NULL);
		
		hWndTothreadMap[hwnd] = GetThreadId(handle);
	}
	else if (uMsg == WM_SIZE)
	{
		map<HWND, DWORD>::iterator iter;
		iter = hWndTothreadMap.find(hwnd);
		if (iter != hWndTothreadMap.end())
		{
			HWND hChildWnd = getChildHWndByThreadId(iter->second);
			if (hChildWnd != NULL)
			{
				PostMessage(hChildWnd, WM_CHILD_SIZE, wParam, lParam);
			}			
		}		
	}

	return  ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL regWndClass(HMODULE hModule, WNDPROC proc,const char * className)
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
	if (!::UnregisterClassA(noteBookWndClassName, hModule))
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
	if (!regWndClass(hModule, windowProc, noteBookWndClassName))
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

