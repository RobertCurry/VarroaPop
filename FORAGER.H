// Forager.h: interface for the CForager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORAGER_H__8C6C41B6_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_FORAGER_H__8C6C41B6_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Bee.h"

class CForager : public CBee  {
  private:
	float daysInLifetime;

  public:
	CForager();
	CForager(int quantity);
	CForager(CForager* oldForager);
	void Serialize(CArchive &ar);
	virtual ~CForager();

	CForager& operator=(const CForager& theForager);

};

#endif // !defined(AFX_FORAGER_H__8C6C41B6_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
