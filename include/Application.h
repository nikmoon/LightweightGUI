/*
 * Application.h
 *
 *  Created on: 26 но€б. 2013 г.
 *      Author: karpachevnk
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "CSysWindow.h"

/*
 *		Ёта часть подключаетс€ только в файле CMainWindow.cpp
 */

#ifdef CMAINWINDOW_CPP

void
CloseApp(int code)
{
#ifdef WIN32
	::PostQuitMessage(code);
#endif
}

/*
 *		Ёта часть подключаетс€ в исполн€емом файле пользовател€
 */
#else	// CMAINWINDOW_CPP not defined

void Init();
void Done();

#ifdef WIN32

int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR CmdLine, int CmdShow)
{
	// задаетс€ описатель модул€, который будет использоватьс€ дл€ создани€ всех окон и элементов приложени€
	LightweightGUI::CSysWindow::m_hInst = hInst;

	// функци€ пользовател€, создающа€ главное окно
	Init();

	BOOL bRet;
	MSG msg;

	while ( (bRet = ::GetMessage(&msg, NULL, 0, 0)) != 0  )
	{
		if (bRet == -1)
		{
			throw ::GetLastError();
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// функци€ пользовател€, удал€юща€ главное окно
	Done();

	return msg.wParam;
}

#endif	// WIN32
#endif	// CMAINWINDOW_CPP
#endif /* APPLICATION_H_ */
