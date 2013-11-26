/*
 * CTopLevelWindow.cpp
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#include "CTopLevelWindow.h"

namespace LightweightGUI
{


#ifdef WIN32
CTopLevelWindow::CTopLevelWindow(const string &wname, CSysWindow *parent, int x, int y, int cx, int cy, const string &clname, DWORD clstyle, DWORD style,
	DWORD exstyle, HMENU hmenu, WNDPROC wproc)
	: CSysWindow(wname,parent,x,y,cx,cy,clname,clstyle,style,exstyle,hmenu,wproc)
{
	// TODO Auto-generated constructor stub
}
#endif

CTopLevelWindow::~CTopLevelWindow()
{
	// TODO Auto-generated destructor stub
}


} /* namespace LightweightGUI */
