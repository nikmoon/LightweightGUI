/*
 * CSysWindow.h
 *
 *  Created on: 25 ����. 2013 �.
 *      Author: karpachevnk
 */

#ifndef CSYSWINDOW_H_
#define CSYSWINDOW_H_

#include <string>
#include "CEventInfo.h"

using namespace std;

namespace LightweightGUI
{

// ��������������� �������� ����
class CSysWindow;


#ifndef WIN32
	typedef unsigned int DWORD;
#endif


//
//	����� ��� ������� ��������� ��������� � ������������� ���� ��� ��������
//
const DWORD WNDFLAG_BORDER		= 0x00000001;	// 0x01 1 ����� ��� ����
const DWORD WNDFLAG_CAPTION		= 0x00000002;	// 0x02 2 ��������� (�������. �������� �����)
const DWORD WNDFLAG_CLOSEBUTTON = 0x00000004;	// 0x04 3 ������ �������� ���� (�������. �������� ���������)
const DWORD WNDFLAG_MINBUTTON	= 0x00000008;	// 0x08 4 ������ ������������ ���� (�������. �������� ������ �������� � ���������)
const DWORD WNDFLAG_MAXBUTTON	= 0x00000010;	// 0x10 5 ������ �������������� ���� (�������. �������� ������ �������� � ���������)
const DWORD WNDFLAG_SIZEBOX		= 0x00000020;	// 0x20 6 ����� ������ ������ ���� �� �����

const DWORD WNDFLAG_CHILD		= 0x00000040;	// 0x40 7 ������������ ������ ������������� ���� (�� ���������� � DEFAULT)
const DWORD WNDFLAG_DEFAULT		= 0x00000080;	// 0x80 8 ������ ������������� (�� ���������� � CHILD)

const DWORD WNDFLAG_NEEDED		= 0x000000C0;	// ������ ���� ����� ���� �� ���� ������ ����, ����� ����� ���������� DEFAULT
const DWORD WNDFLAG_MAINWINDOW	= 0x000000BF;	// ����� ��� �������� �������� ���� ���������� ��-���������



//
//	������� ��� �������� ������� ������� WinAPI
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


// ��������� � ������� ����
struct SWndGeom
{
	int X, Y;		// ������������ ����
	int cX, cY;		// ������� ����
};

//
//	���������� ��� �������� ����
//
struct SCreateData
{
	string m_WndName;			// ��� ����
	DWORD m_Flags;				// �����
	DWORD m_BackColor;			// ���� ����
	CSysWindow * m_WndParent;	// ������������ ����
	SWndGeom m_Geom;			// ��������� ����
};

//
//	������� ��� ���������� ����
//
class CSysWindow : private SCreateData
{
	//
	// ����� ��� ���� ������ ���� ��������� ��������� ���������
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

	// ������� ������� ����
	SWndGeom GetWndGeom() const { return m_Geom; };
	string GetWndName() const { return m_WndName; };
	DWORD GetFlags() const { return m_Flags; };
	DWORD GetBackColor() const { return m_BackColor; };
	CSysWindow *GetParentWindow() const { return m_WndParent; };

	// ������� ������� ����

	// ������
	void Show();
	void Hide();

	//
	// ���������� � ����������� ��� �� Windows
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
