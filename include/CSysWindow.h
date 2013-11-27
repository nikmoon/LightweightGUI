/*
 * CSysWindow.h
 *
 *  Created on: 25 нояб. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CSYSWINDOW_H_
#define CSYSWINDOW_H_

#include <string>
#include "CEventInfo.h"

using namespace std;

namespace LightweightGUI
{

// предварительное описание типа
class CSysWindow;


#ifndef WIN32
	typedef unsigned int DWORD;
#endif



const DWORD WNDFLAG_BORDER		= 0x00000001;	// 0x01 1 рамка для окна
const DWORD WNDFLAG_CAPTION		= 0x00000002;	// 0x02 2 заголовок (автомат. включает рамку)
const DWORD WNDFLAG_CLOSEBUTTON = 0x00000004;	// 0x04 3 кнопка закрытия окна (автомат. включает заголовок)
const DWORD WNDFLAG_MINBUTTON	= 0x00000008;	// 0x08 4 кнопка сворачивания окна (автомат. включает кнопку закрытия и заголовок)
const DWORD WNDFLAG_MAXBUTTON	= 0x00000010;	// 0x10 5 кнопка разворачивания окна (автомат. включает кнопку закрытия и заголовок)
const DWORD WNDFLAG_SIZEBOX		= 0x00000020;	// 0x20 6 можно менять размер окна за рамку
const DWORD WNDFLAG_CHILD		= 0x00000040;	// 0x40 7 расположение внутри родительского окна
const DWORD WNDFLAG_DEFAULT		= 0x00000080;	// 0x80 8 включает все, кроме WNDFLAG_CHILD



//
//	Обертка для создания оконных классов WinAPI
//
#ifdef WIN32
class CWndClassWrapper
{
public:
	CWndClassWrapper();
	const string &GetName();
	void RegisterWndClass();
	void UnregisterWndClass();
private:
	bool m_Registered;
	string m_ClassName;
	static LRESULT CALLBACK EventWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};
#endif


// Положение и размеры окна
struct SWndGeom
{
	int X, Y;		// расположение окна
	int cX, cY;		// размеры окна
};

//
//	Информация для создания окна
//
struct SCreateData
{
	string m_WndName;			// имя окна
	DWORD m_Flags;				// флаги
	DWORD m_BackColor;			// цвет фона
	CSysWindow * m_WndParent;	// родительское окно
	SWndGeom m_Geom;			// геометрия окна
};

//
//	Обертка для системного окна
//
class CSysWindow : private SCreateData
{
	//
	// общий для окон любого типа интерфейс обработки сообщений
	//
public:
	CSysWindow();
	CSysWindow(const SCreateData &data);
	CSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom);
	CSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, int x, int y, int cx, int cy);
	void CreateSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom);
	virtual ~CSysWindow();
	virtual void OnEvent_Default(CEventInfo &ev);
	virtual void OnEvent(CEventInfo &ev);
	virtual void OnEvent_Destroy(CEventInfo &ev);
	virtual void OnEvent_EraseBackGround(CEventInfo &ev);
	virtual void OnEvent_RedrawClientArea(CEventInfo &ev);

	// Геттеры свойств окна
	SWndGeom GetWndGeom() const { return m_Geom; };
	string GetWndName() const { return m_WndName; };
	DWORD GetFlags() const { return m_Flags; };
	DWORD GetBackColor() const { return m_BackColor; };
	CSysWindow *GetParentWindow() const { return m_WndParent; };

	// Методы
	void Show();
	void Hide();

	//
	// объявления и определения для ОС Windows
	//
#ifdef WIN32
public:
	static HINSTANCE m_hInst;
private:
	static CWndClassWrapper CommonWndClass;
	HWND m_hWnd;
#endif	// WIN32

};

} /* namespace LightweightGUI */

#endif /* CSYSWINDOW_H_ */
