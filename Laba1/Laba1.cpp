// Laba1.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Laba1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
std::map<HWND, WindowDescription> windows;
SYSTEMTIME time;

// Отправить объявления функций, включенных в этот модуль кода:
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

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDC_LABA1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA1));

	MSG msg;

	// Цикл основного сообщения:
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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LABA1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//Новый код
HMENU CreateMyMenu(HWND hwnd)
{
	int offset = windows[hwnd].get_id() * 2;
	windows[hwnd].one_event = 1001 + offset;
	windows[hwnd].both_event = 1002 + offset;

	HMENU hMainMenu = CreateMenu();
	HMENU hPopMenuFile = CreatePopupMenu();

	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, L"Настройки");
	{
		AppendMenu(hPopMenuFile, MF_STRING, windows[hwnd].one_event, L"Включить только в этом окне");
		AppendMenu(hPopMenuFile, MF_STRING, windows[hwnd].both_event, L"Включить в обоих окнах");
		AppendMenu(hPopMenuFile, MF_SEPARATOR, 1000, L"");
		AppendMenu(hPopMenuFile, MF_STRING, IDM_EXIT, L"Выход");
	}
	AppendMenu(hMainMenu, MF_STRING, IDM_ABOUT, L"Справка");

	MENUINFO mi;
	mi.cbSize = sizeof(mi);
	mi.hbrBack = CreateSolidBrush(RGB(200, 50, 50));
	mi.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mi.dwStyle = 0;

	SetMenuInfo(hMainMenu, &mi);
	SetMenuInfo(hPopMenuFile, &mi);

	return hMainMenu;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
	//Первое окно
	HWND hWnd_1 = CreateWindowW(szWindowClass, L"Title 1", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	//Второе окно
	HWND hWnd_2 = CreateWindowW(szWindowClass, L"Title 2", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

#pragma region Новый код

	WindowDescription().set_show(-1);
	windows.emplace(hWnd_1, WindowDescription(hWnd_1));
	windows.emplace(hWnd_2, WindowDescription(hWnd_2));
	SetMenu(hWnd_1, CreateMyMenu(hWnd_1));
	SetMenu(hWnd_2, CreateMyMenu(hWnd_2));

	GetSystemTime(&time);
	windows[hWnd_1].set_time(time);
	windows[hWnd_2].set_time(time);
	windows[hWnd_1].SetOtherHWND(hWnd_2);
	windows[hWnd_2].SetOtherHWND(hWnd_1);

	SetTimer(hWnd_1, windows[hWnd_1].get_id() + 1, 100, NULL);
	SetTimer(hWnd_2, windows[hWnd_2].get_id() + 1, 100, NULL);

	RegisterHotKey(hWnd_1, windows[hWnd_1].one_event, MOD_NOREPEAT, 0x31);
	RegisterHotKey(hWnd_2, windows[hWnd_2].one_event, MOD_NOREPEAT, 0x32);
	RegisterHotKey(hWnd_1, windows[hWnd_1].both_event, MOD_NOREPEAT, VK_SPACE);
	RegisterHotKey(hWnd_2, windows[hWnd_2].both_event, MOD_NOREPEAT, VK_SPACE);

#pragma endregion

	if (!hWnd_1 || !hWnd_2)
	{
		return FALSE;
	}

	ShowWindow(hWnd_1, nCmdShow);
	UpdateWindow(hWnd_1);

	ShowWindow(hWnd_2, nCmdShow);
	UpdateWindow(hWnd_2);
	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPARAM lParam2 = lParam;
	POINT pt;
	switch (message)
	{
		//Таймер
	case WM_TIMER:
		GetLocalTime(&time);
		windows[hWnd].set_time(time);
		InvalidateRect(hWnd, &(windows[hWnd].rc), false);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

		//Горячая клавиша
	case WM_HOTKEY:
	{
		int wmId = LOWORD(wParam);
		if (windows[hWnd].one_event == wmId)		
			windows[hWnd].set_show();
		else if (windows[hWnd].both_event == wmId)	
			windows[hWnd].toggleMode();
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
		break;

		//Движение мыши
	case WM_MOUSEMOVE:

		/*for (std::map<HWND, WindowDescription>::iterator it = windows.begin(); it != windows.end(); ++it) {
			if (hWnd != it->first) {
				pt.x = GET_X_LPARAM(lParam2);
				pt.y = GET_Y_LPARAM(lParam2);
				MapWindowPoints(hWnd, it->first,  reinterpret_cast<POINT*>(&(pt)), 1);
				windows[it->first].set_coords(pt.x, pt.y);
			}
			else {
				windows[it->first].set_coords(GET_X_LPARAM(lParam2), GET_Y_LPARAM(lParam2));
			}
			InvalidateRect(it->first, &(windows[it->first].rc), false);
		}*/
		if (mode) {
			pt.x = GET_X_LPARAM(lParam2);
			pt.y = GET_Y_LPARAM(lParam2);
			MapWindowPoints(hWnd, windows[hWnd].other, reinterpret_cast<POINT*>(&(pt)), 1);
			windows[windows[hWnd].other].set_coords(pt.x, pt.y);
			InvalidateRect(windows[hWnd].other, &(windows[windows[hWnd].other].rc), false);
		}
		windows[hWnd].set_coords(GET_X_LPARAM(lParam2), GET_Y_LPARAM(lParam2));
		InvalidateRect(hWnd, &(windows[hWnd].rc), false);
		//windows[hWnd].set_coords(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		//InvalidateRect(hWnd, &(windows[hWnd].rc), false);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

		//Нажатие левой кнопки мыши
	case WM_LBUTTONDOWN:
		windows[hWnd].isChecked = !windows[hWnd].isChecked;
		InvalidateRect(hWnd, &(windows[hWnd].rc), false);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		//ю
		if (wmId > 1000)
		{
			if (windows[hWnd].one_event == wmId)		windows[hWnd].set_show();
			else if (windows[hWnd].both_event == wmId)	windows[hWnd].toggleMode();
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		else
		// Разобрать выбор в меню:
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

		/*POINT pt;
		HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		HPEN hPen2 = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		SelectPen(hdc, hPen);
		SelectPen(hdc, hPen);
		RECT rc;
		windows[hWnd].hwnd = hWnd;
		GetClientRect(hWnd, &rc);
		MoveToEx(hdc, rc.left, rc.bottom, &pt);
		LineTo(hdc, rc.right, rc.top);
		MoveToEx(hdc, rc.right, rc.bottom, &pt);
		LineTo(hdc, rc.left, rc.top);*/
		
		windows[hWnd].paint(hdc); //для часов
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		UnregisterHotKey(hWnd, windows[hWnd].one_event);
		UnregisterHotKey(hWnd, windows[hWnd].both_event);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

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
