/*
 * CSysWindow.cpp
 *
 *  Created on: 25 нояб. 2013 г.
 *      Author: karpachevnk
 */

#include "CSysWindow.h"

namespace LightweightGUI
{


#ifdef WIN32

// описатель модуля
HINSTANCE
	CSysWindow::m_hInst = ::GetModuleHandle(NULL);

// создаем стандартный оконный класс
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

// вызывать, только если m_Registered == false
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
//	Главная оконная процедура для любого окна
//
LRESULT CALLBACK
CWndClassWrapper::EventWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	// получаем адрес экземпляра, связанный с системным окном
	CSysWindow *pWindow = (CSysWindow *)::GetWindowLongPtr(hwnd,GWLP_USERDATA);
	if (pWindow)
	{
		CEventInfo ev(msg, wp, lp);	// создаем класс события
		pWindow->OnEvent(ev);		// вызываем обработчик событий
		return ev.m_Result;			// возвращаем результат обработки
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


CSysWindow::~CSysWindow()
{
#ifdef WIN32
	// отменяем регистрацию оконного класса
	CommonWndClass.UnregisterWndClass();
#endif
}


//
//	Создание системного окна и заполнение всех полей экземпляра
//
void
CSysWindow::CreateSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom)
{
#ifdef WIN32
	static DWORD ws_flags[] =
	{
		WS_POPUP,
		WS_BORDER | WS_CAPTION,
		WS_BORDER | WS_CAPTION | WS_SYSMENU,
		WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU,
		WS_SIZEBOX,
		WS_CHILD,
		WS_OVERLAPPEDWINDOW
	};

	// разбираем флаги
	DWORD f = 0;
	int index = 0;
	DWORD bit = 0x01;

	while (flags)
	{
		if (flags & bit)
		{
			f |= ws_flags[index];
			flags &= ~bit;
		}
		index++;
		bit <<= 1;
	}

	flags = f;


	// создаем окно
	m_hWnd = ::CreateWindowEx(0, CommonWndClass.GetName().c_str(), wname.c_str(), flags, geom.X, geom.Y, geom.cX, geom.cY,
			(parent == NULL)?NULL:parent->m_hWnd, NULL, m_hInst, NULL);
	if (!m_hWnd)
		throw ::GetLastError();
	// делаем привязку данного экземпляра к системному окну
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	::InvalidateRect(m_hWnd,NULL,TRUE);
#endif
	m_Geom = geom;
	m_WndName = wname;
	m_Flags = flags;
	m_BackColor = bcolor;
	m_WndParent = parent;
}

//
//	Обработчик по-умолчанию
//	Вызывается для всех сообщений, не имеющих собственного обработчика
//
void
CSysWindow::OnEvent_Default(CEventInfo &ev)
{
#ifdef WIN32
	ev.m_Result = ::DefWindowProc(m_hWnd, ev.m_Msg, ev.m_WP, ev.m_LP);
#else
	#error "CSysWindow::OnEvent_Default(CEventInfo &ev) реализован только для WIN32"
#endif
}


//
//	Главный обработчик событий.
//	Все сообщения сначала попадают сюда, отсюда направляются соответствующим
//	конкретным обработчикам.
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
			OnEvent_Destroy(ev);
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


void
CSysWindow::OnEvent_EraseBackGround(CEventInfo &ev)
{
#ifdef WIN32
	HDC hDC = (HDC)ev.m_WP;
	RECT clRect;
	::GetClientRect(m_hWnd,&clRect);
	::FillRect(hDC,&clRect,(HBRUSH)m_BackColor);
	ev.m_Result = 0;
#endif
}

//
//	Вызывается, когда требуется перерисовать содержимое клиентской части окна
//
void
CSysWindow::OnEvent_RedrawClientArea(CEventInfo &ev)
{
	OnEvent_Default(ev);
}


//
//	Вызывается, когда уничтожается окно
//
void
CSysWindow::OnEvent_Destroy(CEventInfo &ev)
{
	OnEvent_Default(ev);
}


// Отобразить окна
void
CSysWindow::Show()
{
#ifdef WIN32
	::ShowWindow(m_hWnd, SW_SHOW);
#endif
}


// Скрыть окно
void
CSysWindow::Hide()
{
#ifdef WIN32
	::ShowWindow(m_hWnd, SW_HIDE);
#endif
}

} /* namespace LightweightGUI */
