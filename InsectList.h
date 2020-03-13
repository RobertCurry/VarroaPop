// InsectList.h: interface for the CInsectList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSECTLIST_H__FD312327_96D6_11D3_8919_44455354616F__INCLUDED_)
#define AFX_INSECTLIST_H__FD312327_96D6_11D3_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Insect.h"

class CInsectList : public CObList  
{
protected:
	int m_MaxAgeDD;

public:
	CInsectList();
	CInsectList(int Age);
	virtual ~CInsectList();
	double GetQuantity();
//	double GetQuantity( double MinAge = -1);
	void SetMaxAge(int MaxAge) {m_MaxAgeDD = MaxAge;}
	int GetMaxAge() {return m_MaxAgeDD;}
	void ChangeQuantity( double DeltQuant);
	void IncrementAge(double DegreeDays);
	void Populate(int Boxcars, double Quantity);
	void Clear();
};

#endif // !defined(AFX_INSECTLIST_H__FD312327_96D6_11D3_8919_44455354616F__INCLUDED_)
