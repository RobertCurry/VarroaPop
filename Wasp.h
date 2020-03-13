// Wasp.h: interface for the CWasp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WASP_H__6B576143_E5E0_11D3_8D9A_00A024E290BA__INCLUDED_)
#define AFX_WASP_H__6B576143_E5E0_11D3_8D9A_00A024E290BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWasp : public CObject  
{
private:
	// Attributes
	double m_AgeDD;
	double m_Quantity;

public:
	// Operations
	CWasp();
	virtual ~CWasp();
	void SetAgeDD( double Age ) {m_AgeDD = Age;}
	double GetAgeDD() { return m_AgeDD; }
	void SetQuantity( double Quan ) { m_Quantity = Quan; }
	double GetQuantity() { return m_Quantity; }

};

#endif // !defined(AFX_WASP_H__6B576143_E5E0_11D3_8D9A_00A024E290BA__INCLUDED_)
