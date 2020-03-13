#if !defined(AFX_SIMDLG_H__7CD760E3_EC59_11D2_8919_44455354616F__INCLUDED_)
#define AFX_SIMDLG_H__7CD760E3_EC59_11D2_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimDlg dialog

class CSimDlg : public CDialogBar
{
// Construction
public:
	CSimDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSimDlg)
	enum { IDD = IDD_SIMULATION_BAR1 };
	int		m_ImmigrationOn;
	CString	m_SimStart;
	CString	m_SimStop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSimDlg)
	afx_msg void OnButtonGo();
	afx_msg void OnButtonImSetup();
	afx_msg void OnDeltaposSpinStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinStop(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMDLG_H__7CD760E3_EC59_11D2_8919_44455354616F__INCLUDED_)
