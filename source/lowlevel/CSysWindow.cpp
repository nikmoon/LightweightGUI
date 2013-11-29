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

// имена стандартных системных классов WinAPI
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

// информация для создания системных классов
struct SStdClassData
{
	int m_ClassNameIndex;	// индекс имени класса WinAPI
	DWORD m_Flags;			// специфические для данного класса флаги WinAPI
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
	// создаем окно стандартного системного класса
	m_hWnd = ::CreateWindowEx(
		0,
		StdSysClassName[StdClassData[ctype].m_ClassNameIndex],
		wname.c_str(),
		StdClassData[ctype].m_Flags|GetWinAPIFlags(flags),
		x, y, cx, cy,
		parent->m_hWnd, NULL,
		m_hInst, NULL);
	// проверяем успешность создания
	if (!m_hWnd)
		throw ::GetLastError();
	// задаем адрес дефолтной процедуры обработки сообщений и устанавливаем первой свою
	m_DefWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)CWndClassWrapper::EventWndProc);
	// делаем привязку данного экземпляра к системному окну
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
	if (m_DefWndProc == ::DefWindowProc)	// значит не системное окно
	{
		// отменяем регистрацию оконного класса
		CommonWndClass.UnregisterWndClass();
	}
#endif
}


//
//	Создание системного окна и заполнение всех полей экземпляра
//
void
CSysWindow::CreateSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom)
{
#ifdef WIN32
	// создаем окно
	m_hWnd = ::CreateWindowEx(0, CommonWndClass.GetName().c_str(), wname.c_str(), GetWinAPIFlags(flags), geom.X, geom.Y, geom.cX, geom.cY,
			(parent == NULL)?NULL:parent->m_hWnd, NULL, m_hInst, NULL);
	if (!m_hWnd)
		throw ::GetLastError();
	// задаем адрес дефолтной системной функции обработки событий
	m_DefWndProc = ::DefWindowProc;
	// делаем привязку данного экземпляра к системному окну
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	// после привязки необходимо заполнить задний фон
	::InvalidateRect(m_hWnd,NULL,TRUE);
#endif
	m_Geom = geom;
	m_WndName = wname;
	m_Flags = flags;
	m_BackColor = bcolor;
	m_WndParent = parent;
}

#ifdef WIN32
// преобразование кроссплатформенных флагов для создания окон во флаги WinAPI
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

	// разбираем флаги
	if (!(flags & WNDFLAG_NEEDED))	// если один из требуемых флагов не задан
		flags |= WNDFLAG_DEFAULT;	// установим флаг по-умолчанию

	DWORD resf = 0;		// флаги в формате Win32 WinAPI (WS_...)
	DWORD fbit = 0x01;	// текущий проверяемый бит заданного флага

	// побитово проверим заданные флаги
	// index - индекс, сопоставляющий порядковый номер бита флага, набору флагов WinAPI
	for(DWORD index = 0; index < (sizeof(ws_flags)/sizeof(DWORD)); index++)
	{
		if (flags & fbit)	// если бит текущего флага установлен
		{
			resf |= ws_flags[index];	// добавляем нужные биты во флаг в формате WinAPI
		}
		fbit <<= 1;	// следующий битовый флаг
	}
	return resf;
}
#endif	// WIN32

//
//	Обработчик по-умолчанию
//	Вызывается для всех сообщений, не имеющих собственного обработчика
//
void
CSysWindow::OnEvent_Default(CEventInfo &ev)
{
#ifdef WIN32
	ev.m_Result = m_DefWndProc(m_hWnd, ev.m_Msg, ev.m_WP, ev.m_LP);
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
			if (m_DefWndProc != ::DefWindowProc)	// значит системное окно
			{
				// восстанавливаем адрес системной оконной процедуры
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


// Уничтожение системного окна, экземпляр остается
void
CSysWindow::Destroy()
{
#ifdef WIN32
	::DestroyWindow(m_hWnd);
#endif
}

} /* namespace LightweightGUI */
