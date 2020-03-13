// INSECT.h: interface for the CInsect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSECT_H__FD312323_96D6_11D3_8919_44455354616F__INCLUDED_)
#define AFX_INSECT_H__FD312323_96D6_11D3_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInsect : public CObject  
{
	// Attributes
private:
	double m_Age;  // In Degree-Days
	double m_Quantity;

	// Operations
public:
	CInsect();
	virtual ~CInsect();
	double GetAge() {return m_Age;}
	void SetAge(double theAge) {m_Age = theAge;}
	double GetQuantity() {return m_Quantity;}
	void SetQuantity(double theQuantity);
	virtual void Serialize(CArchive &ar);

};

#endif // !defined(AFX_INSECT_H__FD312323_96D6_11D3_8919_44455354616F__INCLUDED_)
