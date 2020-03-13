#if !defined(AFX_BEE_H__8C6C41B3_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_BEE_H__8C6C41B3_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Bee.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CBee command target

class CBee : public CCmdTarget
{
	DECLARE_DYNCREATE(CBee)

	CBee();           // protected constructor used by dynamic creation

// Attributes
public:
	int number;
	float age;  // days
	bool Alive;

// Operations
public:
	virtual ~CBee();
	CBee operator = (CBee& bee);
	CBee::CBee(CBee& bee);
	CBee::CBee(int Num);
	void SetNumber(int Num) {number = Num;}
	int GetNumber() {return number;}
	void Kill() {Alive = false; number = 0;}
	bool IsAlive() {return Alive;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBee)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBee)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEE_H__8C6C41B3_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
