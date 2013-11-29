/*
 * CMainWindow.cpp
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#include "CMainWindow.h"

namespace LightweightGUI
{

#define CMAINWINDOW_CPP
#include "Application.h"


DWORD
	CMainWindow::sm_DefFlags = WNDFLAG_MAINWINDOW;

DWORD
	CMainWindow::sm_DefBackColor = (0xCC | (0xCC << 8) | (0xCC << 16));


CMainWindow::CMainWindow(const string &wname, const SWndGeom &geom)
	: CTopLevelWindow(wname, sm_DefFlags, sm_DefBackColor, NULL, geom)
{

}

CMainWindow::CMainWindow(const string &wname, int x, int y, int cx, int cy)
	: CTopLevelWindow(wname, sm_DefFlags, sm_DefBackColor, NULL, x, y, cx, cy)
{

}


CMainWindow::~CMainWindow()
{
	// TODO Auto-generated destructor stub
}

void
CMainWindow::OnEvent_Destroy(CEventInfo &ev)
{
	CloseApp(0);
}

} /* namespace LightweightGUI */
