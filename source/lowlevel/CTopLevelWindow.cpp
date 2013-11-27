/*
 * CTopLevelWindow.cpp
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#include "CTopLevelWindow.h"

namespace LightweightGUI
{

CTopLevelWindow::CTopLevelWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom)
	: CSysWindow(wname, flags, bcolor, parent, geom)
{
	// TODO Auto-generated constructor stub
}

CTopLevelWindow::CTopLevelWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, int x, int y, int cx, int cy)
	: CSysWindow(wname, flags, bcolor, parent, x, y, cx, cy)
{

}

CTopLevelWindow::~CTopLevelWindow()
{
}


} /* namespace LightweightGUI */
