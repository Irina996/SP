// lr8.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr8.h"

#define MAX_LOADSTRING 100

#define DRAW_BTN 3
#define ERASE_BTN 4

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

   HWND drawButton = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
       650, 10, 60, 60, hWnd, (HMENU)DRAW_BTN, NULL, NULL);
   HWND eraseButton = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
       720, 10, 60, 60, hWnd, (HMENU)ERASE_BTN, NULL, NULL);


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
            case DRAW_BTN:
                isVisible = true;
                InvalidateRect(hWnd, NULL, true);
                break;
            case ERASE_BTN:
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
        case DRAW_BTN:
        {
            HDC hdc = draw->hDC;

            if (draw->itemAction == ODA_SELECT) {
                brush = CreateSolidBrush(RGB(70, 70, 70));
                SetBkColor(hdc, RGB(70, 70, 70));
            }
            else {
                brush = CreateSolidBrush(RGB(66, 135, 245));
                SetBkColor(hdc, RGB(66, 135, 245));
            }
            pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

            SelectObject(hdc, brush);
            SelectObject(hdc, pen);
            
            Rectangle(hdc, 0, 0, 60, 60);

            RECT rectangle;
            SetRect(&rectangle, 0, 0, 60, 60);
            DrawText(hdc, L"DRAW", strlen("DRAW"), &rectangle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
        break;
        case ERASE_BTN:
        {
            HDC hdc = draw->hDC;

            if (draw->itemAction == ODA_SELECT) {
                brush = CreateSolidBrush(RGB(70, 70, 70));
                SetBkColor(hdc, RGB(70, 70, 70));
            }
            else {
                brush = CreateSolidBrush(RGB(66, 135, 245));
                SetBkColor(hdc, RGB(66, 135, 245));
            }
            pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

            SelectObject(hdc, brush);
            SelectObject(hdc, pen);

            Rectangle(hdc, 0, 0, 60, 60);

            RECT rectangle;
            SetRect(&rectangle, 0, 0, 60, 60);
            DrawText(hdc, L"ERASE", strlen("ERASE"), &rectangle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
            HPEN pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
            SelectObject(hdc, pen);
            HBRUSH brush = CreateSolidBrush(RGB(5, 30, 80));
            SelectObject(hdc, brush);

            Rectangle(hdc, 10, 10, 510, 210);

            brush = CreateSolidBrush(RGB(100, 140, 200));
            SelectObject(hdc, brush);
            Rectangle(hdc, 110, 30, 350, 90);

            brush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, brush);
            pen = CreatePen(PS_SOLID, 5, RGB(60, 60, 60));
            SelectObject(hdc, pen);
            Ellipse(hdc, 50, 180, 120, 240);
            Ellipse(hdc, 390, 180, 460, 240);

            pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
            SelectObject(hdc, pen);
            POINT points[5];
            brush = CreateSolidBrush(RGB(10, 80, 10));
            SelectObject(hdc, brush);
            MoveToEx(hdc, 510, 30, NULL);
            points[0].x = 510;
            points[0].y = 30;
            points[1].x = 580;
            points[1].y = 30;
            points[2].x = 650;
            points[2].y = 130;
            points[3].x = 650;
            points[3].y = 210;
            points[4].x = 510;
            points[4].y = 210;
            Polygon(hdc, points, 5);

            brush = CreateSolidBrush(RGB(100, 140, 200));
            SelectObject(hdc, brush);
            MoveToEx(hdc, 530, 50, NULL);
            points[0].x = 530;
            points[0].y = 50;
            points[1].x = 560;
            points[1].y = 50;
            points[2].x = 620;
            points[2].y = 110;
            points[3].x = 620;
            points[3].y = 130;
            points[4].x = 530;
            points[4].y = 130;
            Polygon(hdc, points, 5);

            pen = CreatePen(PS_SOLID, 5, RGB(50, 50, 50));
            SelectObject(hdc, pen);
            brush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, brush);
            Ellipse(hdc, 570, 180, 640, 240);

            pen = CreatePen(PS_SOLID, 5, RGB(10, 200, 130));
            SelectObject(hdc, pen);

            MoveToEx(hdc, 90, 75, NULL);
            ArcTo(hdc, 50, 70, 190, 180, 90, 90, 160, 160);
            MoveToEx(hdc, 400, 100, NULL);
            ArcTo(hdc, 400, 70, 490, 160, 400, 110, 490, 70);

            MoveToEx(hdc, 210, 95, NULL);
            LineTo(hdc, 270, 130);
            MoveToEx(hdc, 290, 100, NULL);
            LineTo(hdc, 185, 200);
            MoveToEx(hdc, 320, 110, NULL);
            LineTo(hdc, 380, 170);
            MoveToEx(hdc, 250, 150, NULL);
            LineTo(hdc, 320, 150);
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
