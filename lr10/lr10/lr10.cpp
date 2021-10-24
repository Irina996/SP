// lr10.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lr10.h"
#include <exception>

#define MAX_LOADSTRING 100

#define RED 0
#define GREEN 1
#define BLUE 2

#define RHOMBUS 0
#define SQUARE 1
#define CIRCLE 2
#define STAR 3

#define USER_CLICK WM_USER + 1
#define DRAW WM_USER + 2

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND radiobuttonRed;
HWND radiobuttonGreen;
HWND radiobuttonBlue;

HWND radiobuttonRhombus;
HWND radiobuttonSquare;
HWND radiobuttonCircle;
HWND radiobuttonStar;

HWND checkboxDraw;

HWND mainHWnd;
HWND canvasWindow;

WORD xPos, yPos;

int currentColor = -1, currentShape = -1;

int length = 50;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                MyRegisterClass2(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc2(HWND, UINT, WPARAM, LPARAM);
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
	LoadStringW(hInstance, IDC_LR10, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	MyRegisterClass2(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LR10));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR10));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LR10);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}



//
//  FUNCTION: MyRegisterClass2()
//
//  PURPOSE: Registers second window class.
//
ATOM MyRegisterClass2(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc2;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LR10));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LR10);
	wcex.lpszClassName = L"Child";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		CW_USEDEFAULT, 0, 300, 300, nullptr, nullptr, hInstance, nullptr);
	mainHWnd = hWnd;

	if (!hWnd)
	{
		return FALSE;
	}

	HWND hWndCanvas = CreateWindowW(L"Child", L"Canvas", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWndCanvas)
		return FALSE;

	canvasWindow = hWndCanvas;

	checkboxDraw = CreateWindow(L"BUTTON", L"Draw", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
		20, 110, 100, 20, hWnd, NULL, hInst, NULL);

	radiobuttonRed = CreateWindow(L"BUTTON", L"Red", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
		20, 20, 100, 20, hWnd, NULL, hInst, NULL);
	radiobuttonGreen = CreateWindow(L"BUTTON", L"Green", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		20, 50, 100, 20, hWnd, NULL, hInst, NULL);
	radiobuttonBlue = CreateWindow(L"BUTTON", L"Blue", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		20, 80, 100, 20, hWnd, NULL, hInst, NULL);
	radiobuttonRhombus = CreateWindow(L"BUTTON", L"Rhombus", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
		140, 20, 100, 20, hWnd, NULL, hInst, NULL);
	radiobuttonSquare = CreateWindow(L"BUTTON", L"Square", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		140, 50, 100, 20, hWnd, NULL, hInst, NULL);
	radiobuttonCircle = CreateWindow(L"BUTTON", L"Circle", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		140, 80, 100, 20, hWnd, NULL, hInst, NULL);
	radiobuttonStar = CreateWindow(L"BUTTON", L"Star", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
		140, 110, 100, 20, hWnd, NULL, hInst, NULL);

	if (!radiobuttonRed || !radiobuttonGreen || !radiobuttonBlue) {
		MessageBox(NULL, L"Error: color radiobuttons", L"Error", MB_OK);
	}

	if (!radiobuttonRhombus || !radiobuttonSquare || !radiobuttonCircle || !radiobuttonStar) {
		MessageBox(NULL, L"Error: shape radiobuttons", L"Error", MB_OK);
	}

	if (!checkboxDraw) {
		MessageBox(NULL, L"Error: checkbox", L"Error", MB_OK);
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ShowWindow(hWndCanvas, nCmdShow);
	UpdateWindow(hWndCanvas);

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
	case USER_CLICK: {
		auto isChecked = SendMessage(checkboxDraw, BM_GETCHECK, NULL, NULL);
		if (isChecked != BST_CHECKED) break;

		auto isCheckedRed = SendMessage(radiobuttonRed, BM_GETCHECK, NULL, NULL);
		auto isCheckedGreen = SendMessage(radiobuttonGreen, BM_GETCHECK, NULL, NULL);
		auto isCheckedBlue = SendMessage(radiobuttonBlue, BM_GETCHECK, NULL, NULL);
		if (!(isCheckedRed == BST_CHECKED || isCheckedGreen == BST_CHECKED || isCheckedBlue == BST_CHECKED)) break;

		auto isCheckedRhombus = SendMessage(radiobuttonRhombus, BM_GETCHECK, NULL, NULL);
		auto isCheckedSquare = SendMessage(radiobuttonSquare, BM_GETCHECK, NULL, NULL);
		auto isCheckedCircle = SendMessage(radiobuttonCircle, BM_GETCHECK, NULL, NULL);
		auto isCheckedStar = SendMessage(radiobuttonStar, BM_GETCHECK, NULL, NULL);
		if (!(isCheckedRhombus == BST_CHECKED || isCheckedSquare == BST_CHECKED ||
			isCheckedCircle == BST_CHECKED || isCheckedStar == BST_CHECKED)) break;

		WPARAM wParam;
		LPARAM lParam;
		if (isCheckedRed == BST_CHECKED)
			wParam = (WPARAM)RED;
		if (isCheckedGreen == BST_CHECKED)
			wParam = (WPARAM)GREEN;
		if (isCheckedBlue == BST_CHECKED)
			wParam = (WPARAM)BLUE;
		if (isCheckedRhombus == BST_CHECKED)
			lParam = (LPARAM)RHOMBUS;
		if (isCheckedSquare == BST_CHECKED)
			lParam = (LPARAM)SQUARE;
		if (isCheckedCircle == BST_CHECKED)
			lParam = (LPARAM)CIRCLE;
		if (isCheckedStar == BST_CHECKED)
			lParam = (LPARAM)STAR;

		SendMessageW(canvasWindow, DRAW, wParam, lParam);
		break;
	}
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
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

//
//  FUNCTION: WndProc2(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for second window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_LBUTTONDOWN:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		SendMessage(mainHWnd, USER_CLICK, NULL, NULL);
		break;
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		switch (currentColor) {
		case RED:
			SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
			break;
		case GREEN:
			SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
			break;
		case BLUE:
			SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
			break;
		default:
			break;
		}
		switch (currentShape) {
		case RHOMBUS:
			POINT rhombusPoints[4];
			rhombusPoints[0].x = xPos - length;
			rhombusPoints[0].y = yPos;
			rhombusPoints[1].x = xPos;
			rhombusPoints[1].y = yPos + length;
			rhombusPoints[2].x = xPos + length;
			rhombusPoints[2].y = yPos;
			rhombusPoints[3].x = xPos;
			rhombusPoints[3].y = yPos - length;

			Polygon(hdc, rhombusPoints, 4);

			break;
		case SQUARE:
			Rectangle(hdc, xPos - length, yPos - length, xPos + length, yPos + length);
			break;
		case CIRCLE:
			Ellipse(hdc, xPos - length, yPos - length, xPos + length, yPos + length);
			break;
		case STAR:

			POINT points[10];

			points[0].x = xPos;
			points[0].y = yPos - length;

			points[1].x = xPos + 3 * length / 10;
			points[1].y = yPos - length / 2;

			points[2].x = xPos + length;
			points[2].y = yPos - length / 2;

			points[3].x = xPos + 13 * length / 27;
			points[3].y = yPos - length / 7;

			points[4].x = xPos + 2 * length / 3;
			points[4].y = yPos + length / 2;

			points[5].x = xPos;
			points[5].y = yPos + length / 6;

			points[6].x = xPos - 2 * length / 3;
			points[6].y = yPos + length / 2;

			points[7].x = xPos - 13 * length / 27;
			points[7].y = yPos - length / 7;

			points[8].x = xPos - length;
			points[8].y = yPos - length / 2;

			points[9].x = xPos - 3 * length / 10;
			points[9].y = yPos - length / 2;

			Polygon(hdc, points, 10);

			break;
		default:
			break;
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case DRAW: {
		currentColor = LOWORD(wParam);
		currentShape = LOWORD(lParam);
		SendMessage(canvasWindow, WM_PAINT, wParam, lParam);
		InvalidateRect(canvasWindow, NULL, FALSE);
		break;
	}
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
