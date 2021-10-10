// lr6.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr6.h"

#define START 6
#define STOP 7
#define MAX_LOADSTRING 100
#define MAIN_WINDOW_WIDTH 800
#define MAIN_WINDOW_HEIGHT 400

int X = MAIN_WINDOW_WIDTH / 2 - 50; // x coordinate of text
int Y = MAIN_WINDOW_HEIGHT / 2 - 50; // y coordinate of text
bool leftDirFlag = false;  // flag of direction if move
const int TIMER_ID = 1; // timer idz 


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LR6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR6));

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
    wcex.hIcon          = (HICON)LoadImage(nullptr, _T("./icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wcex.hCursor        = (HICON)LoadImage(nullptr, _T("./cursor.cur"), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
    wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = (HICON)LoadImage(wcex.hInstance, _T("./icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

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

   HWND hWnd = CreateWindow(szWindowClass, _T("Star Wars"),
       WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, NULL, NULL,
       hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   HMENU hMenuBar = CreateMenu();
   HMENU hSubMenu = CreateMenu();

   AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hSubMenu, _T("Menu"));
   AppendMenu(hSubMenu, MF_STRING, START, _T("Start"));
   AppendMenu(hSubMenu, MF_STRING, STOP, _T("Stop"));

   SetMenu(hWnd, hMenuBar);

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
            case START:
                // Set the timer
                SetTimer(hWnd, TIMER_ID, 100, NULL);
                break;
            case STOP:
                KillTimer(hWnd, TIMER_ID);
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
        if (X >= MAIN_WINDOW_WIDTH - 100)
        {
            leftDirFlag = true;
        }
        if (X <= 10)
        {
            leftDirFlag = false;
        }
        if (!leftDirFlag)
        {
            X += 10;
        }
        else
        {
            X -= 10;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        {
            const char text[] = "star wars";
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            TextOutA(hdc, X, Y, text, strlen(text));
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
