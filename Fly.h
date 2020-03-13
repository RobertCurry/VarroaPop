// Fly.h: interface for the CFly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLY_H__FD312323_96D6_11D3_8919_44455354616F__INCLUDED_)
#define AFX_FLY_H__FD312323_96D6_11D3_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "insect.h"

class CFly : public CInsect  
{
	// Attributes
private:
	double m_Age;  // In Degree-Days
	double m_Quantity;

	// Operations
public:
	CFly();
	virtual ~CFly();
	double GetAge() {return m_Age;}
	void SetAge(double theAge) {m_Age = theAge;}
	double GetQuantity() {return m_Quantity;}
	void SetQuantity(double theQuantity);
	virtual void Serialize(CArchive &ar);

};

#endif // !defined(AFX_FLY_H__FD312323_96D6_11D3_8919_44455354616F__INCLUDED_)
