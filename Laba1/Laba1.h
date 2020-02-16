#pragma once
#include "framework.h"
#include "resource.h"

static int count = 0;
// ind or len(all)
static int who_show;
static bool mode;

class WindowDescription
{
	int id;
	int s_len = 64;
	wchar_t* w_str = new wchar_t[s_len];
	char* c_str = new char[s_len];
	int x = 0;
	int y = 0;
	SYSTEMTIME time;
	const int h_max = 43200, m_max = 3600, s_max = 60;

public:
	int one_event, both_event;
	bool isChecked = false;
	RECT rc;
	HWND hwnd;
	HWND other;

	WindowDescription() {

	}

	WindowDescription(HWND _hwnd) {
		hwnd = _hwnd;
		id = count++;
	}

	/*WindowDescription& operator= (const WindowDescription& a)
	{
		WindowDescription ret;
		ret.id = a.id;
		ret.hwnd = a.hwnd;
		return ret;
	}*/

	void SetOtherHWND(HWND hwnd_other) {
		other = hwnd_other;
	}

	void toggleMode() {
		if (mode) {
			mode = false;
		}
		else {
			mode = true;
		}
	}

	void init(HWND _hwnd) {
		hwnd = _hwnd;
		id = count++;
	}
	void set_time(SYSTEMTIME t) { time = t; }
	void set_show(int ind) { who_show = ind; }
	void set_show() { who_show = id; }

	int get_id() { return id; }

	void set_coords(int _x, int _y) {
		x = _x; y = _y;
	}

	void paint(HDC hdc)
	{
		POINT pt;
		if (isChecked)
		{
			SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 255, 0)));
		}
		else {
			SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0)));

		}
		SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
		//windows[hWnd].hwnd = hWnd;
		GetClientRect(hwnd, &rc);

		MoveToEx(hdc, rc.left, rc.bottom, &pt);
		LineTo(hdc, rc.right, rc.top);
		MoveToEx(hdc, rc.right, rc.bottom, &pt);
		LineTo(hdc, rc.left, rc.top);

		/*SetBkColor(hdc, RGB(255, 255, 255));
		Rectangle(hdc, -1, -1, rc.right + 1, rc.bottom + 1);*/

		if(who_show < 0 || who_show == id)
			s_len = sprintf_s(c_str, -1, "Coords X[%0.4d] Y[%0.4d]", x, y) + 1;
		else
			s_len = sprintf_s(c_str, -1, "Coords dont work here!!") + 1;

		MultiByteToWideChar(CP_ACP, 0, c_str, -1, w_str, s_len);
		DrawText(hdc, w_str, -1, &rc, DT_BOTTOM);


		/*int R = min(rc.bottom, rc.right) / 2;
		int c_x = rc.right/2, c_y = rc.bottom/2;
		MoveToEx(hdc, c_x + R, c_y, NULL);
		AngleArc(hdc, c_x, c_y, R, 0, 360);

		float ang = 0, k = R / 3.0, pi = 3.1419;
		ang = ((float)time.wHour * 60 + (float)time.wHour) * 0.5 - 90 ;
		ang = ang * pi / 180;
		MoveToEx(hdc, c_x, c_y, NULL);
		LineTo(hdc, c_x + k * cos(ang), c_y + k * sin(ang));
		ang = (float)time.wMinute * 6 - 90;
		ang = ang * pi / 180;
		k = R * 2.0 / 3.0;
		MoveToEx(hdc, c_x, c_y, NULL);
		LineTo(hdc, c_x + k * cos(ang), c_y + k * sin(ang));
		ang = (float)time.wSecond * 6 - 90;
		ang = ang * pi / 180;
		k = R;
		MoveToEx(hdc, c_x, c_y, NULL);
		LineTo(hdc, c_x + k * cos(ang), c_y + k * sin(ang));*/
	}
};