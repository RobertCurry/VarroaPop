// FlyList.h: interface for the CFlyList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLYLIST_H__FD312327_96D6_11D3_8919_44455354616F__INCLUDED_)
#define AFX_FLYLIST_H__FD312327_96D6_11D3_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Insect.h"
#include "InsectList.h"

class CFlyList : public CInsectList  
{

public:
	double GetQuantity( double MinAge = -1);
	CInsect* RemoveMatures();
};

#endif // !defined(AFX_FLYLIST_H__FD312327_96D6_11D3_8919_44455354616F__INCLUDED_)
