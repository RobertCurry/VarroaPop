#if !defined(AFX_COMBREMDLG_H__96559B83_C49D_11D5_B486_00A024E290BA__INCLUDED_)
#define AFX_COMBREMDLG_H__96559B83_C49D_11D5_B486_00A024E290BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CombRemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCombRemDlg dialog

class CCombRemDlg : public CDialog
{
// Construction
public:
	CCombRemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCombRemDlg)
	enum { IDD = IDD_REMOVECOMB };
	COleDateTime	m_RemovalDate;
	BOOL	m_IsEnabled;
	double	m_Percent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCombRemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCombRemDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBREMDLG_H__96559B83_C49D_11D5_B486_00A024E290BA__INCLUDED_)
