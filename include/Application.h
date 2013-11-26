/*
 * Application.h
 *
 *  Created on: 26 но€б. 2013 г.
 *      Author: karpachevnk
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "CMainWindow.h"

#ifdef WIN32

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR CmdLine, int CmdShow)
{
	CMainWindow::m_hInst = hInst;
	CApplicationWindow AppWindow;

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

	return msg.wParam;
}


#endif

#endif /* APPLICATION_H_ */
