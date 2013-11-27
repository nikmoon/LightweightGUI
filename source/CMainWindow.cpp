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


#ifdef WIN32
CMainWindow::CMainWindow(const string &wname, const SWndGeom &geom)
	: CTopLevelWindow(wname, WS_OVERLAPPEDWINDOW|WS_VISIBLE, (COLOR_WINDOW+1), NULL, geom)
{

}

CMainWindow::CMainWindow(const string &wname, int x, int y, int cx, int cy)
	: CTopLevelWindow(wname, WNDFLAG_BORDER, (COLOR_BTNFACE+1), NULL, x, y, cx, cy)
{

}
#endif


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
