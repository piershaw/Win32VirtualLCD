// Win32VirtualLCD.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32VirtualLCD.h"
#include <mmsystem.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Sprite.h"
#include "BackBuffer.h"
#include "resource.h"

#define MAX_LOADSTRING 100
using namespace std;
HWND hWnd; // main window

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//Stuff I added 
#define SCREEN_HEIGHT 1040
#define SCREEN_WIDTH  468
/* Sprint Class for just static bitmaps for now */
Sprite * breadBoard;
Sprite * largeLCD;
/* This is a Handle to device context the video card */
HDC ghSpriteDC = 0;
/*BackBuffer for sprites double buffering to show only the cutout area of the video memory copyied from the bitmap to the client area */
BackBuffer * gBackBuffer = 0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32VIRTUALLCD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32VIRTUALLCD));

	MSG msg;
	Vec2 pos = Vec2(1024/2,412/2);

	breadBoard = new Sprite(hInstance, IDB_BITMAP1, IDB_BITMAP5, pos);

	ZeroMemory(&msg, sizeof(MSG));

	// Get the performance timer frequency.
	__int64 cntsPerSec = 0;
	bool perfExists = QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec) != 0;
	if (!perfExists)
	{
		MessageBox(0, L"Performance timer does not exist!", 0, 0);
		return 0;
	}

	double timeScale = 1.0 / (double)cntsPerSec;
	// Get the current time.
	__int64 lastTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

	double timeElapsed = 0.0f;

	while (msg.message != WM_QUIT)
	{
		// IF there is a Windows message then process it.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ELSE, do game stuff.
		else
		{
			// Get the time now.

			__int64 currTime = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			// Compute the differences in time from the last
			// time we checked.  Since the last time we checked
			// was the previous loop iteration, this difference
			// gives us the time between loop iterations...
			// or, I.e., the time between frames.
			double deltaTime = (double)(currTime - lastTime) * timeScale;

			timeElapsed += deltaTime;

			// Only update once every 1/100 seconds.
			if (timeElapsed >= 0.01)
			{
				// Update the game and draw everything.
				breadBoard->update((float)timeElapsed);
				// Draw every frame.
				breadBoard->draw(gBackBuffer->getDC(), ghSpriteDC);
				
			

				drawFramesPerSecond((float)timeElapsed);

				// Now present the backbuffer contents to the main
				// window client area.
				gBackBuffer->present();

				timeElapsed = 0.0;
			}

			// We are at the end of the loop iteration, so
			// prepare for the next loop iteration by making
			// the "current time" the "last time."
			lastTime = currTime;
		}
	}

    return (int) msg.wParam;
}

// Name: DrawFramesPerSecond
// Desc: This function is called every frame and updates
// the frame per second display in the main window
// caption.
//=========================================================
void drawFramesPerSecond(float deltaTime){
	// Make static so the variables persist even after
	// the function returns.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	static char buffer[256];

	++frameCnt;
	// last frame.
	timeElapsed += deltaTime;
	// Has one second passed?
	if (timeElapsed >= 1.0f)
	{
		wstringstream wis;
		wis << "FPS ";
		wis << frameCnt;
		SetWindowText(hWnd,wis.str().c_str());
		// Reset the counters to prepare for the next time
		// we compute the frames per second.
		frameCnt = 0;
		timeElapsed = 0.0f;
	}
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32VIRTUALLCD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32VIRTUALLCD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_HEIGHT , SCREEN_WIDTH, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message){
		// Create application resources.
	case WM_CREATE:
		// Create system memory DCs 
		ghSpriteDC = CreateCompatibleDC(0);

		// Create the backbuffer.
		gBackBuffer = new BackBuffer(
			hWnd,
			SCREEN_HEIGHT,
			SCREEN_WIDTH);

		return 0;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
				delete breadBoard;
	
				delete gBackBuffer;
				DeleteDC(ghSpriteDC);
				PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
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

// Message handler for about box.
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
