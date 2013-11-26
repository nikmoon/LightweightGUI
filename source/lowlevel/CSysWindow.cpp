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
	// отменяем регистрацию оконного класса
	// можно делать это не боясь, т.к. если это не последнее окно данного класса,
	// отмена не будет выполнена
	if (m_WndProc)
	{
		::UnregisterClass(m_ClassName.c_str(), m_hInst);
	}
#endif
}


//
//	Обработчик по-умолчанию
//	Вызывается для всех сообщений, не имеющих собственного обработчика
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
		case WM_DESTROY:
			OnEvent_Destroy(ev);

			// при необходимости отменяем сабклассинг
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


//
//	Создание системного окна и заполнение всех полей экземпляра
//
void
CSysWindow::CreateSysWindow(const string &wname, CSysWindow *parent, int x, int y, int cx, int cy, const string &clname, DWORD clstyle, DWORD style,
		DWORD exstyle, HMENU hmenu, WNDPROC wproc)
{
#ifdef WIN32
	// проверим глобальный флаг m_hInst
	if (!m_hInst)
		throw -1;

	// получим описатель родительского окна
	HWND hparent = (parent == NULL) ? NULL : parent->GetHWnd();

	// при необходимости регистрируем класс
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

	// создаем системное окно
	m_hWnd = ::CreateWindowEx(exstyle, clname.c_str(), wname.c_str(), style, x, y, cx, cy, hparent, hmenu, m_hInst, NULL);

	// в случае ошибки - исключение
	if (m_hWnd == NULL)
		throw ::GetLastError();

	// осуществляем привязку экземпляра и системного окна
	::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	// при необходимости регистрируем класс
	if (wproc) // сабклассинг не нужен
	{
		m_WndProc = NULL;
	}
	else
	{
		// выполняем сабклассинг
		m_WndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)EventWndProc);
	}

	// инициализируем остальные поля
	m_ClassName = clname;

#else

#endif
	// инициализируем общие поля
	m_Parent = parent;
}



#ifdef WIN32
//
//	Главная оконная процедура для любого окна
//	Всегда будет вызываться раньше классовой.
//
LRESULT CALLBACK
CSysWindow::EventWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
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
