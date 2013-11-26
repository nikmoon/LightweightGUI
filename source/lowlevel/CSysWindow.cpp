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

HINSTANCE
CSysWindow::m_hInst = NULL;

#endif



CSysWindow::CSysWindow()
{
#ifdef WIN32
	m_hWnd = NULL;
	m_WndProc = NULL;
	m_Parent = NULL;
#endif
}


#ifdef WIN32
CSysWindow::CSysWindow(const string &wname, CSysWindow *parent, int x, int y, int cx, int cy, const string &clname, DWORD clstyle, DWORD style,
	DWORD exstyle, HMENU hmenu, WNDPROC wproc)
{
	CreateSysWindow(wname, parent, x, y, cx, cy, clname, clstyle, style, exstyle, hmenu, wproc);
}
#endif

CSysWindow::~CSysWindow()
{
#ifdef WIN32
	// �������� ����������� �������� ������
	// ����� ������ ��� �� �����, �.�. ���� ��� �� ��������� ���� ������� ������,
	// ������ �� ����� ���������
	if (m_WndProc)
	{
		::UnregisterClass(m_ClassName.c_str(), m_hInst);
	}
#endif
}


//
//	���������� ��-���������
//	���������� ��� ���� ���������, �� ������� ������������ �����������
//
void
CSysWindow::OnEvent_Default(CEventInfo &ev)
{
#ifdef WIN32
	if (m_WndProc)
	{
		ev.m_Result = m_WndProc(m_hWnd, ev.m_Msg, ev.m_WP, ev.m_LP);
	}
	else
	{
		ev.m_Result = ::DefWindowProc(m_hWnd, ev.m_Msg, ev.m_WP, ev.m_LP);
	}
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
		case WM_DESTROY:
			OnEvent_Destroy(ev);

			// ��� ������������� �������� �����������
			if (m_WndProc)
			{
				::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_WndProc);
			}

			break;
		case WM_PAINT:
			OnEvent_RedrawClientArea(ev);
			break;
		default:
			OnEvent_Default(ev);
			break;
	}
#else
	OnEvent_Default(ev);
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


//
//	�������� ���������� ���� � ���������� ���� ����� ����������
//
void
CSysWindow::CreateSysWindow(const string &wname, CSysWindow *parent, int x, int y, int cx, int cy, const string &clname, DWORD clstyle, DWORD style,
		DWORD exstyle, HMENU hmenu, WNDPROC wproc)
{
#ifdef WIN32
	// �������� ���������� ���� m_hInst
	if (!m_hInst)
		throw -1;

	// ������� ��������� ������������� ����
	HWND hparent = (parent == NULL) ? NULL : parent->GetHWnd();

	// ��� ������������� ������������ �����
	if (wproc)
	{
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = clstyle;
		wc.lpfnWndProc = EventWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInst;
		wc.hIcon = ::LoadIcon(NULL,IDI_APPLICATION);
		wc.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = clname.c_str();
		::RegisterClassEx(&wc);
	}

	// ������� ��������� ����
	m_hWnd = ::CreateWindowEx(exstyle, clname.c_str(), wname.c_str(), style, x, y, cx, cy, hparent, hmenu, m_hInst, NULL);

	// � ������ ������ - ����������
	if (m_hWnd == NULL)
		throw ::GetLastError();

	// ������������ �������� ���������� � ���������� ����
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	// ��� ������������� ������������ �����
	if (wproc) // ����������� �� �����
	{
		m_WndProc = NULL;
	}
	else
	{
		// ��������� �����������
		m_WndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)EventWndProc);
	}

	// �������������� ��������� ����
	m_ClassName = clname;

#else

#endif
	// �������������� ����� ����
	m_Parent = parent;
}



#ifdef WIN32
//
//	������� ������� ��������� ��� ������ ����
//	������ ����� ���������� ������ ���������.
//
LRESULT CALLBACK
CSysWindow::EventWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
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
#endif

void
CSysWindow::Show()
{
#ifdef WIN32
	::ShowWindow(m_hWnd, SW_SHOW);
#endif
}

void
CSysWindow::Hide()
{
#ifdef WIN32
	::ShowWindow(m_hWnd, SW_HIDE);
#endif
}

} /* namespace LightweightGUI */
