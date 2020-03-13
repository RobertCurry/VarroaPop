#if !defined(AFX_SIMDLGBAR_H__0C725B00_E2D8_11D2_8918_44455354616F__INCLUDED_)
#define AFX_SIMDLGBAR_H__0C725B00_E2D8_11D2_8918_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimDlgBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimDlgBar dialog

class CSimDlgBar : public CDialog
{
// Construction
public:
	CSimDlgBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSimDlgBar)
	enum { IDD = IDD_SIMULATION_BAR };
	int		m_FAge;
	int		m_Sperm;
	CString	m_SimStart;
	CString	m_SImStop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimDlgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSimDlgBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMDLGBAR_H__0C725B00_E2D8_11D2_8918_44455354616F__INCLUDED_)
