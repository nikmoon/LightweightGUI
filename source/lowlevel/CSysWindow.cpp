/*
 * CSysWindow.cpp
 *
 *  Created on: 25 ����. 2013 �.
 *      Author: karpachevnk
 */

#include "CSysWindow.h"

namespace LightweightGUI
{


#ifdef WIN32

// ��������� ������
HINSTANCE
	CSysWindow::m_hInst = ::GetModuleHandle(NULL);

// ������� ����������� ������� �����
CWndClassWrapper
	CSysWindow::CommonWndClass;


CWndClassWrapper::CWndClassWrapper()
{
	m_Registered = false;
	m_ClassName = "CCommonWindowClass";
}

const string &
CWndClassWrapper::GetName()
{
	if (!m_Registered)
		RegisterWndClass();
	return m_ClassName;
}

// ��������, ������ ���� m_Registered == false
void
CWndClassWrapper::RegisterWndClass()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = EventWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_ClassName.c_str();
	wc.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = CSysWindow::m_hInst;

	if (!::RegisterClassEx(&wc))
			throw ::GetLastError();

	m_Registered = true;
}

void
CWndClassWrapper::UnregisterWndClass()
{
	if (m_Registered)
	{
		BOOL result = ::UnregisterClass(m_ClassName.c_str(), CSysWindow::m_hInst);
		if (!result)
		{
			DWORD error = ::GetLastError();
			if (error != ERROR_CLASS_HAS_WINDOWS)
			{
				throw error;
			}
		}
		else
		{
			m_Registered = false;
		}
	}
}

//
//	������� ������� ��������� ��� ������ ����
//
LRESULT CALLBACK
CWndClassWrapper::EventWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// �������� ����� ����������, ��������� � ��������� �����
	CSysWindow *pWindow = (CSysWindow *)::GetWindowLongPtr(hwnd,GWLP_USERDATA);
	if (pWindow)
	{
		CEventInfo ev(msg, wp, lp);	// ������� ����� �������
		pWindow->OnEvent(ev);		// �������� ���������� �������
		return ev.m_Result;			// ���������� ��������� ���������
	}
	else
	{
		return ::DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

#endif	// WIN32


CSysWindow::CSysWindow()
{
#ifdef WIN32
	m_hWnd = NULL;
	m_DefWndProc = ::DefWindowProc;
#endif
}


CSysWindow::CSysWindow(const SCreateData &data)
{
	CreateSysWindow(data.m_WndName, data.m_Flags, data.m_BackColor, data.m_WndParent, data.m_Geom);
}


CSysWindow::CSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom)
{
	CreateSysWindow(wname, flags, bcolor, parent, geom);
}

CSysWindow::CSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, int x, int y, int cx, int cy)
{
	SWndGeom geom = {x, y, cx, cy};
	CreateSysWindow(wname, flags, bcolor, parent, geom);
}


#ifdef WIN32

// ����� ����������� ��������� ������� WinAPI
LPCTSTR StdSysClassName[] =
{
	WC_BUTTON,
	WC_COMBOBOX,
	WC_EDIT,
	WC_SCROLLBAR,
	WC_STATIC,
	TOOLBARCLASSNAME,
	WC_TREEVIEW
};

// ���������� ��� �������� ��������� �������
struct SStdClassData
{
	int m_ClassNameIndex;	// ������ ����� ������ WinAPI
	DWORD m_Flags;			// ������������� ��� ������� ������ ����� WinAPI
}
StdClassData[] =
{
	{0, BS_PUSHBUTTON | WS_CHILD},						// button
	{0, BS_AUTOCHECKBOX | BS_PUSHLIKE | WS_CHILD},		// button_fixed
	{0, BS_AUTOCHECKBOX | WS_CHILD},					// checkbox2
	{0, BS_AUTO3STATE | WS_CHILD}						// checkbox3
};
#endif	// WIN32




CSysWindow::CSysWindow(ESystemControl ctype, const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, int x, int y, int cx, int cy)
{
#ifdef WIN32
	// ������� ���� ������������ ���������� ������
	m_hWnd = ::CreateWindowEx(
		0,
		StdSysClassName[StdClassData[ctype].m_ClassNameIndex],
		wname.c_str(),
		StdClassData[ctype].m_Flags|GetWinAPIFlags(flags),
		x, y, cx, cy,
		parent->m_hWnd, NULL,
		m_hInst, NULL);
	// ��������� ���������� ��������
	if (!m_hWnd)
		throw ::GetLastError();
	// ������ ����� ��������� ��������� ��������� ��������� � ������������� ������ ����
	m_DefWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)CWndClassWrapper::EventWndProc);
	// ������ �������� ������� ���������� � ���������� ����
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
#endif
	m_Geom.X = x; m_Geom.Y = y; m_Geom.cX = cx; m_Geom.cY = cy;
	m_WndName = wname;
	m_Flags = flags;
	m_BackColor = bcolor;
	m_WndParent = parent;
}


CSysWindow::~CSysWindow()
{
#ifdef WIN32
	if (m_DefWndProc == ::DefWindowProc)	// ������ �� ��������� ����
	{
		// �������� ����������� �������� ������
		CommonWndClass.UnregisterWndClass();
	}
#endif
}


//
//	�������� ���������� ���� � ���������� ���� ����� ����������
//
void
CSysWindow::CreateSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom)
{
#ifdef WIN32
	// ������� ����
	m_hWnd = ::CreateWindowEx(0, CommonWndClass.GetName().c_str(), wname.c_str(), GetWinAPIFlags(flags), geom.X, geom.Y, geom.cX, geom.cY,
			(parent == NULL)?NULL:parent->m_hWnd, NULL, m_hInst, NULL);
	if (!m_hWnd)
		throw ::GetLastError();
	// ������ ����� ��������� ��������� ������� ��������� �������
	m_DefWndProc = ::DefWindowProc;
	// ������ �������� ������� ���������� � ���������� ����
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	// ����� �������� ���������� ��������� ������ ���
	::InvalidateRect(m_hWnd,NULL,TRUE);
#endif
	m_Geom = geom;
	m_WndName = wname;
	m_Flags = flags;
	m_BackColor = bcolor;
	m_WndParent = parent;
}

#ifdef WIN32
// �������������� ������������������ ������ ��� �������� ���� �� ����� WinAPI
DWORD
CSysWindow::GetWinAPIFlags(DWORD flags)
{
	static DWORD ws_flags[] =
	{
		WS_BORDER,
		WS_BORDER | WS_CAPTION,
		WS_BORDER | WS_CAPTION | WS_SYSMENU,
		WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU,
		WS_SIZEBOX,
		WS_CHILD,
		WS_POPUP
	};

	// ��������� �����
	if (!(flags & WNDFLAG_NEEDED))	// ���� ���� �� ��������� ������ �� �����
		flags |= WNDFLAG_DEFAULT;	// ��������� ���� ��-���������

	DWORD resf = 0;		// ����� � ������� Win32 WinAPI (WS_...)
	DWORD fbit = 0x01;	// ������� ����������� ��� ��������� �����

	// �������� �������� �������� �����
	// index - ������, �������������� ���������� ����� ���� �����, ������ ������ WinAPI
	for(DWORD index = 0; index < (sizeof(ws_flags)/sizeof(DWORD)); index++)
	{
		if (flags & fbit)	// ���� ��� �������� ����� ����������
		{
			resf |= ws_flags[index];	// ��������� ������ ���� �� ���� � ������� WinAPI
		}
		fbit <<= 1;	// ��������� ������� ����
	}
	return resf;
}
#endif	// WIN32

//
//	���������� ��-���������
//	���������� ��� ���� ���������, �� ������� ������������ �����������
//
void
CSysWindow::OnEvent_Default(CEventInfo &ev)
{
#ifdef WIN32
	ev.m_Result = m_DefWndProc(m_hWnd, ev.m_Msg, ev.m_WP, ev.m_LP);
#else
	#error "CSysWindow::OnEvent_Default(CEventInfo &ev) ���������� ������ ��� WIN32"
#endif
}


//
//	������� ���������� �������.
//	��� ��������� ������� �������� ����, ������ ������������ ���������������
//	���������� ������������.
//
void
CSysWindow::OnEvent(CEventInfo &ev)
{
#ifdef WIN32
	switch (ev.m_Msg)
	{
		case WM_ERASEBKGND:
			OnEvent_EraseBackGround(ev);
			break;
		case WM_DESTROY:
			if (m_DefWndProc != ::DefWindowProc)	// ������ ��������� ����
			{
				// ��������������� ����� ��������� ������� ���������
				::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_DefWndProc);
			}
			OnEvent_Destroy(ev);
			break;
		case WM_PAINT:
			OnEvent_RedrawClientArea(ev);
			break;
		case WM_LBUTTONDOWN:
			OnEvent_LButtonDown(ev);
			break;
		case WM_LBUTTONUP:
			OnEvent_LButtonUp(ev);
			break;
		default:
			OnEvent_Default(ev);
			break;
	}
#else
	OnEvent_Default(ev);
#endif
}


void
CSysWindow::OnEvent_EraseBackGround(CEventInfo &ev)
{
#ifdef WIN32
	HDC hDC = (HDC)ev.m_WP;
	RECT clRect;
	::GetClientRect(m_hWnd,&clRect);
	HBRUSH br = ::CreateSolidBrush(m_BackColor);
	::FillRect(hDC,&clRect,br);
	::DeleteObject(br);
	ev.m_Result = 0;
#endif
}

//
//	����������, ����� ��������� ������������ ���������� ���������� ����� ����
//
void
CSysWindow::OnEvent_RedrawClientArea(CEventInfo &ev)
{
	OnEvent_Default(ev);
}


//
//	����������, ����� ������������ ����
//
void
CSysWindow::OnEvent_Destroy(CEventInfo &ev)
{
	OnEvent_Default(ev);
}

void
CSysWindow::OnEvent_LButtonDown(CEventInfo &ev)
{
	OnEvent_Default(ev);
}

void
CSysWindow::OnEvent_LButtonUp(CEventInfo &ev)
{
	OnEvent_Default(ev);
}


// ���������� ����
void
CSysWindow::Show()
{
#ifdef WIN32
	::ShowWindow(m_hWnd, SW_SHOW);
#endif
}


// ������ ����
void
CSysWindow::Hide()
{
#ifdef WIN32
	::ShowWindow(m_hWnd, SW_HIDE);
#endif
}


// ����������� ���������� ����, ��������� ��������
void
CSysWindow::Destroy()
{
#ifdef WIN32
	::DestroyWindow(m_hWnd);
#endif
}

} /* namespace LightweightGUI */
