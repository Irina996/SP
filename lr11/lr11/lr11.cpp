// lr11.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr11.h"
#include <cmath>

#define MAX_LOADSTRING 100

#define START_BTN 1
#define STOP_BTN 2
#define PI 3.14159265

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE threads[3];
int xCenter = 300, yCenter = 300;
bool active = false;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LR11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR11));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

struct ThreadArgs {
    double length;
    double x;
    double y;
    double angle;
    int sleepTime;
    double divClock;
} args[3];

DWORD WINAPI RotateStick(ThreadArgs* args) {
    for (;;) {
        args->angle = args->angle + args->divClock;
        if (args->angle > 360)
            args->angle = args->angle - 360;
        args->x = int(xCenter + cos(args->angle * PI / 180) * args->length);
        args->y = int(yCenter + sin(args->angle * PI / 180) * args->length);
        Sleep(1000*args->sleepTime);
    }
    return 0;
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE,
       20, 10, 150, 30, hWnd, (HMENU)START_BTN, hInstance, nullptr);
   CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE,
       200, 10, 150, 30, hWnd, (HMENU)STOP_BTN, hInstance, nullptr);

   for (int i = 0; i < 3; ++i) {
       args[i].x = xCenter;
       args[i].y = yCenter - args[i].length;
       args[i].angle = 270;
   }

   args[0].length = args[1].length = 100; // секундная и минутная стрелки
   args[2].length = 50; // часовая стрелка

   args[0].sleepTime = 1;
   args[1].sleepTime = 60;
   args[2].sleepTime = 3600;

   args[0].divClock = 6;
   args[1].divClock = 6;
   args[2].divClock = 30;

   for (int i = 0; i < 3; ++i) {
       threads[i] = CreateThread(nullptr, 0u, (LPTHREAD_START_ROUTINE)RotateStick, args + i, 0, nullptr);
   }

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case START_BTN:
                if (!active) {
                    for (int i = 0; i < 3; ++i) {
                        ResumeThread(threads[i]);
                    }

                    SetTimer(hWnd, NULL, 20, nullptr);

                    active = true;
                }
                break;
            case STOP_BTN:
                if (active) {
                    for (int i = 0; i < 3; ++i) {
                        SuspendThread(threads[i]);
                    }
                    active = false;
                }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_TIMER:
        InvalidateRect(hWnd, nullptr, true);
        if (active)
            SetTimer(hWnd, NULL, 20, nullptr);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            SelectObject(hdc, pen);

            MoveToEx(hdc, xCenter, yCenter, nullptr);
            LineTo(hdc, args[0].x, args[0].y);

            pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
            SelectObject(hdc, pen);
            for (int i = 1; i < 3; ++i) {
                MoveToEx(hdc, xCenter, yCenter, nullptr);
                LineTo(hdc, args[i].x, args[i].y);
            }

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
