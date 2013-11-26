/*
 * CTopLevelWindow.h
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CTOPLEVELWINDOW_H_
#define CTOPLEVELWINDOW_H_

#include "CSysWindow.h"

namespace LightweightGUI
{

class CTopLevelWindow : public CSysWindow
{
public:
	CTopLevelWindow() {};
#ifdef WIN32
	CTopLevelWindow(const string &wname, CSysWindow *parent, int x, int y, int cx, int cy, const string &clname, DWORD clstyle, DWORD style,
		DWORD exstyle, HMENU hmenu, WNDPROC wproc);
#endif
	virtual ~CTopLevelWindow();
};

} /* namespace LightweightGUI */

#endif /* CTOPLEVELWINDOW_H_ */
