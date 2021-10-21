// lr8.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr8.h"

#define MAX_LOADSTRING 100

#define BUTTON_DRAW_ID 1
#define BUTTON_CLEAR_ID 2

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

bool isVisible = false;

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
    LoadStringW(hInstance, IDC_LR8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR8));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LR8);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   HWND addButton = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
       20, 320, 60, 60, hWnd, (HMENU)BUTTON_DRAW_ID, NULL, NULL);
   HWND clearButton = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
       90, 320, 60, 60, hWnd, (HMENU)BUTTON_CLEAR_ID, NULL, NULL);


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
            case BUTTON_DRAW_ID:
                isVisible = true;
                InvalidateRect(hWnd, NULL, true);
                break;
            case BUTTON_CLEAR_ID:
                isVisible = false;
                InvalidateRect(hWnd, NULL, true);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DRAWITEM:
        {
        DRAWITEMSTRUCT* draw = (DRAWITEMSTRUCT*)lParam;
        HBRUSH brush;
        HPEN pen;

        switch (draw->CtlID)
        {
        case BUTTON_DRAW_ID:
        {
            HDC hdc = draw->hDC;

            if (draw->itemAction == ODA_SELECT) {
                brush = CreateSolidBrush(RGB(0, 0, 255));
            }
            else {
                brush = CreateSolidBrush(RGB(0, 255, 0));
            }


            pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));

            SelectObject(hdc, brush);
            SelectObject(hdc, pen);

            Ellipse(hdc, 0, 0, 60, 60);
            MoveToEx(hdc, 10, 30, NULL);
            LineTo(hdc, 50, 30);
            MoveToEx(hdc, 30, 10, NULL);
            LineTo(hdc, 30, 50);
        }
        break;
        case BUTTON_CLEAR_ID:
        {
            HDC hdc = draw->hDC;

            if (draw->itemAction == ODA_SELECT) {
                brush = CreateSolidBrush(RGB(0, 0, 255));
            }
            else {
                brush = CreateSolidBrush(RGB(255, 0, 0));
            }
            pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));

            SelectObject(hdc, brush);
            SelectObject(hdc, pen);

            Ellipse(hdc, 0, 0, 60, 60);
            MoveToEx(hdc, 20, 20, NULL);
            LineTo(hdc, 40, 40);
            MoveToEx(hdc, 40, 20, NULL);
            LineTo(hdc, 20, 40);
        }
        break;
        default:
            break;
        }
        }
    case WM_PAINT:
        {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (isVisible)
        {
            HBRUSH brush = CreateSolidBrush(RGB(255, 255, 00));
            SelectObject(hdc, brush);

            Ellipse(hdc, 30, 30, 300, 300);

            brush = CreateSolidBrush(RGB(0, 255, 255));
            SelectObject(hdc, brush);
            Ellipse(hdc, 70, 90, 130, 150);
            Ellipse(hdc, 190, 90, 250, 150);

            POINT points[3];
            brush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, brush);
            MoveToEx(hdc, 80, 250, NULL);
            points[0].x = 80;
            points[0].y = 250;
            points[1].x = 170;
            points[1].y = 280;
            points[2].x = 250;
            points[2].y = 250;
            PolyBezierTo(hdc, points, 3);

            MoveToEx(hdc, 80, 250, NULL);
            points[0].x = 80;
            points[0].y = 250;
            points[1].x = 170;
            points[1].y = 350;
            points[2].x = 250;
            points[2].y = 250;
            PolyBezierTo(hdc, points, 3);


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
