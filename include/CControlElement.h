/*
 * CControlElement.h
 *
 *  Created on: 26 но€б. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CCONTROLELEMENT_H_
#define CCONTROLELEMENT_H_

#include "CChildWindow.h"

namespace LightweightGUI
{

class CControlElement : public CChildWindow
{
public:
	CControlElement();
	virtual ~CControlElement();
};

} /* namespace LightweightGUI */

#endif /* CCONTROLELEMENT_H_ */
