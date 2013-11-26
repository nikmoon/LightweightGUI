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
#ifdef WIN32
	CMainWindow(const string &wname, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT);
#endif
	virtual ~CMainWindow();

	virtual void OnEvent_Destroy(CEventInfo &ev);
};

} /* namespace LightweightGUI */

#endif /* CMAINWINDOW_H_ */
