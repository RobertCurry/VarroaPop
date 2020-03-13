#if !defined(AFX_MYPROPSHEET_H__EFCE53A3_C342_11D2_8918_44455354616F__INCLUDED_)
#define AFX_MYPROPSHEET_H__EFCE53A3_C342_11D2_8918_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPropSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPropSheet

class CVarroaPopView;	// Forward Reference

class CMyPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropSheet)

// Construction
public:
	CMyPropSheet();
	CMyPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMyPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
protected:
    int m_LastPageNumber;  // Keeps track of the last property page that was shown

// Operations
public:
	void SetView(CVarroaPopView* pView) {m_pView = pView;}
	CVarroaPopView* GetView() {return m_pView;}
	void SetLastPageNumber(int number){m_LastPageNumber = number;} 
	int GetLastPageNumber() {return m_LastPageNumber;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPropSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyPropSheet();

	// Generated message map functions
protected:
	CVarroaPopView* m_pView;
	//{{AFX_MSG(CMyPropSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROPSHEET_H__EFCE53A3_C342_11D2_8918_44455354616F__INCLUDED_)
