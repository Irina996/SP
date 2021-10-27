// lr12.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr12.h"

#define MAX_LOADSTRING 100
#define DRAW_STAR 101
#define DRAW_SQUARE 102
#define DRAW_ELLIPSE 103

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HANDLE threads[3];
HANDLE event;
HWND mainWnd;
//bool figure[3] = { false };
CRITICAL_SECTION CriticalSection;

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
    LoadStringW(hInstance, IDC_LR12, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR12));

    MSG msg;

    SetTimer(mainWnd, NULL, 1000, nullptr);

    for (int i = 0; i < 3; i++)
    {
        ResumeThread(threads[i]);
    }

    SetEvent(event);
   
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            SetEvent(event);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR12));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR12);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

struct ThreadArgs {
    bool draw;
} args[3];

DWORD WINAPI ThreadProc(ThreadArgs* args) {
    for (;;) {
        DWORD waitResult = WaitForSingleObject(event, 10);
        if (waitResult == WAIT_OBJECT_0) { 
            EnterCriticalSection(&CriticalSection);

            args->draw = true;
            Sleep(1000);
            args->draw = false;
            while (!ResetEvent(event)) {}

            LeaveCriticalSection(&CriticalSection);
        }
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
      CW_USEDEFAULT, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

   mainWnd = hWnd;

   event = CreateEvent(NULL, TRUE, FALSE, NULL);

   if (!event)
   {
       return -1;
   }

   for (int i = 0; i < 3; i++)
   {
       args[i].draw = false;
       threads[i] = CreateThread(nullptr, 0u, (LPTHREAD_START_ROUTINE)ThreadProc, args + i, 0, nullptr);
   }

   if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
       return -1;

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
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
            SelectObject(hdc, brush);

            if (args[0].draw)
            {
                POINT points[10];
                int length = 50;

                points[0].x = 250;
                points[0].y = 250 - length;

                points[1].x = 250 + 3 * length / 10;
                points[1].y = 250 - length / 2;

                points[2].x = 250 + length;
                points[2].y = 250 - length / 2;

                points[3].x = 250 + 13 * length / 27;
                points[3].y = 250 - length / 7;

                points[4].x = 250 + 2 * length / 3;
                points[4].y = 250 + length / 2;

                points[5].x = 250;
                points[5].y = 250 + length / 6;

                points[6].x = 250 - 2 * length / 3;
                points[6].y = 250 + length / 2;

                points[7].x = 250 - 13 * length / 27;
                points[7].y = 250 - length / 7;

                points[8].x = 250 - length;
                points[8].y = 250 - length / 2;

                points[9].x = 250 - 3 * length / 10;
                points[9].y = 250 - length / 2;

                Polygon(hdc, points, 10);

            }
            if (args[1].draw)
            {
                POINT points[4];
                points[0].x = 200;
                points[0].y = 300;
                points[1].x = 200;
                points[1].y = 200;
                points[2].x = 300;
                points[2].y = 200;
                points[3].x = 300;
                points[3].y = 300;

                MoveToEx(hdc, points[0].x, points[0].y, NULL);
                Polygon(hdc, points, 4);
            }
            if (args[2].draw)
            {
                MoveToEx(hdc, 250, 250, NULL);
                Ellipse(hdc, 200, 200, 300, 300);
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

