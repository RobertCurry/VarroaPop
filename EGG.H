// Egg.h: interface for the CEgg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EGG_H__5D99D1A1_DDD5_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_EGG_H__5D99D1A1_DDD5_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Bee.h"

class CEgg : public CBee  
{
public:
	CEgg();
	CEgg(int Num) {number = Num;}
	virtual ~CEgg();

};

#endif // !defined(AFX_EGG_H__5D99D1A1_DDD5_11D2_8D9A_0020AF233A70__INCLUDED_)
