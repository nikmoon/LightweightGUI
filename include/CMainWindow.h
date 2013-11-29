/*
 * CMainWindow.h
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CMAINWINDOW_H_
#define CMAINWINDOW_H_

#include "CTopLevelWindow.h"

namespace LightweightGUI
{

class CMainWindow : public CTopLevelWindow
{
public:
	CMainWindow() {};
	CMainWindow(const string &wname, const SWndGeom &geom);
	CMainWindow(const string &wname, int x, int y, int cx, int cy);
	virtual ~CMainWindow();

	virtual void OnEvent_Destroy(CEventInfo &ev);

	static DWORD sm_DefFlags;
	static DWORD sm_DefBackColor;
};

} /* namespace LightweightGUI */

#endif /* CMAINWINDOW_H_ */
