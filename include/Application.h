/*
 * Application.h
 *
 *  Created on: 26 ����. 2013 �.
 *      Author: karpachevnk
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "CSysWindow.h"

/*
 *		��� ����� ������������ ������ � ����� CMainWindow.cpp
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
 *		��� ����� ������������ � ����������� ����� ������������
 */
#else	// CMAINWINDOW_CPP not defined

void Init();
void Done();

#ifdef WIN32

int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR CmdLine, int CmdShow)
{
	// �������� ��������� ������, ������� ����� �������������� ��� �������� ���� ���� � ��������� ����������
	LightweightGUI::CSysWindow::m_hInst = hInst;

	// ������� ������������, ��������� ������� ����
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

	// ������� ������������, ��������� ������� ����
	Done();

	return msg.wParam;
}

#endif	// WIN32
#endif	// CMAINWINDOW_CPP
#endif /* APPLICATION_H_ */
