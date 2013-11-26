/*
 * CEventInfo.h
 *
 *  Created on: 25 но€б. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CEVENTINFO_H_
#define CEVENTINFO_H_

#ifdef WIN32
	#include <windows.h>
#endif

namespace LightweightGUI
{

class CEventInfo
{
public:
#ifdef WIN32
	CEventInfo() :   m_Result(-1), m_Msg(0), m_WP(0), m_LP(0) {};
	CEventInfo(UINT msg, WPARAM wp, LPARAM lp) :  m_Result(-1), m_Msg(msg), m_WP(wp), m_LP(lp) {};

	int m_Result;			// код, возвращаемый обработчиком данного событи€
	UINT m_Msg;
	WPARAM m_WP;
	LPARAM m_LP;
#endif
	virtual ~CEventInfo() {};
};

} /* namespace LightweightGUI */

#endif /* CEVENTINFO_H_ */
