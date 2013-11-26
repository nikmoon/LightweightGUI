/*
 * CMainWindow.cpp
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#include "CMainWindow.h"

namespace LightweightGUI
{


#ifdef WIN32
CMainWindow::CMainWindow(const string &wname, int x, int y, int cx, int cy)
	: CTopLevelWindow(wname,NULL,x,y,cx,cy,"CMainWindowClass",CS_HREDRAW|CS_VREDRAW,WS_OVERLAPPEDWINDOW|WS_VISIBLE,0,NULL,EventWndProc)
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
	CTopLevelWindow::OnEvent_Destroy(ev);
#ifdef WIN32
	::PostQuitMessage(0);
	ev.m_Result = 0;
#endif
}

} /* namespace LightweightGUI */
