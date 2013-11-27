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
	CTopLevelWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom);
	CTopLevelWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, int x, int y, int cx, int cy);
	virtual ~CTopLevelWindow();
};

} /* namespace LightweightGUI */

#endif /* CTOPLEVELWINDOW_H_ */
