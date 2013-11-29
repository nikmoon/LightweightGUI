/*
 * CSysWindow.h
 *
 *  Created on: 25 нояб. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CSYSWINDOW_H_
#define CSYSWINDOW_H_

#include <string>
#include <vector>
#include "CEventInfo.h"

#ifdef WIN32
#include <commctrl.h>
#endif

using namespace std;

namespace LightweightGUI
{

// предварительное описание типа
class CSysWindow;


#ifndef WIN32
	typedef unsigned int DWORD;
#endif


//
//	Флаги для задания различных элементов и характеристик окна при создании
//
const DWORD WNDFLAG_BORDER		= 0x00000001;	// 0x01 1 рамка для окна
const DWORD WNDFLAG_CAPTION		= 0x00000002;	// 0x02 2 заголовок (автомат. включает рамку)
const DWORD WNDFLAG_CLOSEBUTTON = 0x00000004;	// 0x04 3 кнопка закрытия окна (автомат. включает заголовок)
const DWORD WNDFLAG_MINBUTTON	= 0x00000008;	// 0x08 4 кнопка сворачивания окна (автомат. включает кнопку закрытия и заголовок)
const DWORD WNDFLAG_MAXBUTTON	= 0x00000010;	// 0x10 5 кнопка разворачивания окна (автомат. включает кнопку закрытия и заголовок)
const DWORD WNDFLAG_SIZEBOX		= 0x00000020;	// 0x20 6 можно менять размер окна за рамку

const DWORD WNDFLAG_CHILD		= 0x00000040;	// 0x40 7 расположение внутри родительского окна (не сочетается с DEFAULT)
const DWORD WNDFLAG_DEFAULT		= 0x00000080;	// 0x80 8 просто прямоугольник (не сочетается с CHILD)

const DWORD WNDFLAG_NEEDED		= 0x000000C0;	// должен быть задан один из двух флагов выше, иначе будет установлен DEFAULT
const DWORD WNDFLAG_MAINWINDOW	= 0x000000BF;	// флаги для создания главного окна приложения по-умолчанию



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
	static LRESULT CALLBACK EventWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
private:
	bool m_Registered;
	string m_ClassName;
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

enum ESystemControl
{
	ESCTRL_BUTTON = 0,
	ESCTRL_BUTTON_FIXED,

	ESCTRL_CHECKBOX2,
	ESCTRL_CHECKBOX3,

	ESCTRL_COMBOBOX,
	ESCTRL_EDITBOX,
	ESCTRL_GROUPBOX,
	ESCTRL_MENUBAR,
	ESCTRL_SCROLLBAR_H,
	ESCTRL_SCROLLBAR_V,
	ESCTRL_STATUSBAR,
	ESCTRL_TEXTLABEL,
	ESCTRL_TOOLBAR,
	ESCTRL_TREEVIEW
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
	CSysWindow(ESystemControl ctype, const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, int x, int y, int cx, int cy);
	void CreateSysWindow(const string &wname, DWORD flags, DWORD bcolor, CSysWindow *parent, const SWndGeom &geom);
	virtual ~CSysWindow();
	virtual void OnEvent_Default(CEventInfo &ev);
	virtual void OnEvent(CEventInfo &ev);
	virtual void OnEvent_Destroy(CEventInfo &ev);
	virtual void OnEvent_EraseBackGround(CEventInfo &ev);
	virtual void OnEvent_RedrawClientArea(CEventInfo &ev);
	virtual void OnEvent_LButtonDown(CEventInfo &ev);
	virtual void OnEvent_LButtonUp(CEventInfo &ev);

	// Геттеры свойств окна
	SWndGeom GetWndGeom() const { return m_Geom; };
	string GetWndName() const { return m_WndName; };
	DWORD GetFlags() const { return m_Flags; };
	DWORD GetBackColor() const { return m_BackColor; };
	CSysWindow *GetParentWindow() const { return m_WndParent; };

	// Сеттеры свойств окна

	// Методы
	void Show();
	void Hide();
	void Destroy();

	virtual void ExecuteAction(DWORD aindex, CSysWindow &owned) {};

private:
	//
	// объявления и определения для ОС Windows
	//
#ifdef WIN32
public:
	static HINSTANCE m_hInst;
	DWORD GetWinAPIFlags(DWORD flags);

	HWND GetHWnd() const { return m_hWnd; };
private:
	static CWndClassWrapper CommonWndClass;
	HWND m_hWnd;
	WNDPROC m_DefWndProc;
#endif	// WIN32

};

} /* namespace LightweightGUI */

#endif /* CSYSWINDOW_H_ */
