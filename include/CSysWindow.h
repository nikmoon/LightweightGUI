/*
 * CSysWindow.h
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CSYSWINDOW_H_
#define CSYSWINDOW_H_

#include <string>
#include "CEventInfo.h"

using namespace std;

namespace LightweightGUI
{

class CSysWindow;

//
//	»нформаци€ дл€ создани€ окна
//
struct SCreateWindowData
{
	// общие данные дл€ всех систем
	string m_WinName;
	CSysWindow *m_Parent;
	int x, y;
	int cx, cy;
#ifdef WIN32
	string m_ClassName;
	DWORD m_ClassStyle;
	DWORD m_Style;
	DWORD m_ExStyle;
	HMENU m_hMenu;
	WNDPROC m_WndProc;
#endif
};


//
//	ќбертка дл€ системного окна
//
class CSysWindow
{
public:
	CSysWindow();
#ifdef WIN32
	CSysWindow(const string &wname, CSysWindow *parent, int x, int y, int cx, int cy, const string &clname, DWORD clstyle, DWORD style,
		DWORD exstyle, HMENU hmenu, WNDPROC wproc);
#endif
	virtual ~CSysWindow();

	//
	// общий дл€ окон любого типа интерфейс обработки сообщений
	//
	virtual void OnEvent_Default(CEventInfo &ev);
	virtual void OnEvent(CEventInfo &ev);

	virtual void OnEvent_Destroy(CEventInfo &ev);
	virtual void OnEvent_RedrawClientArea(CEventInfo &ev);

#ifdef WIN32
	string GetWndClassName() const { return m_ClassName; };
	HWND GetHWnd() const { return m_hWnd; };

	static HINSTANCE m_hInst;
	static LRESULT CALLBACK EventWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
#endif

	CSysWindow *GetParentWindow() const { return m_Parent; };

	void Show();
	void Hide();
private:
	CSysWindow *m_Parent;
#ifdef WIN32
	HWND m_hWnd;
	string m_ClassName;
	WNDPROC m_WndProc;
#endif
	void CreateSysWindow(const string &wname, CSysWindow *parent, int x, int y, int cx, int cy, const string &clname, DWORD clstyle, DWORD style,
		DWORD exstyle, HMENU hmenu, WNDPROC wproc);
};

} /* namespace LightweightGUI */

#endif /* CSYSWINDOW_H_ */
