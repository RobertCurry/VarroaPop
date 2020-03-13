#if !defined(AFX_IMMIGRATIONDLG_H__7CD760E4_EC59_11D2_8919_44455354616F__INCLUDED_)
#define AFX_IMMIGRATIONDLG_H__7CD760E4_EC59_11D2_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImmigrationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImmigrationDlg dialog

class CImmigrationDlg : public CDialog
{
// Construction
public:
//	CImmigrationDlg(CWnd* pParent = NULL);   // standard constructor
	CImmigrationDlg(CString ImType, CWnd* pParent = NULL);  
	bool IsImmigrationEnabled() {return m_Enabled;}
	void SetEnabled( bool en ) {m_Enabled = en;}
	CString GetImmigrationType() {return m_ImmigrationType;}
	int GetImmigrationMiteNum() {return m_MiteNum;}
	COleDateTime GetImmigrationStart() {return m_ImmigrationStart;}
	COleDateTime GetImmigrationStop() {return m_ImmigrationEnd;}
	void GetDataFromDoc();
	void SendDataToDoc();
	void SetDocument(CVarroaPopDoc* pDoc) {m_pDoc = pDoc;}
	BOOL OnInitDialog();

// Dialog Data
	//{{AFX_DATA(CImmigrationDlg)
	enum { IDD = IDD_IMMIGRATION };
	int		m_MiteNum;
	COleDateTime	m_ImmigrationEnd;
	COleDateTime	m_ImmigrationStart;
	double	m_MitePctResistant;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImmigrationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_ImmigrationType;
	bool m_Enabled;
	CVarroaPopDoc* m_pDoc;


	// Generated message map functions
	//{{AFX_MSG(CImmigrationDlg)
	virtual void OnOK();
	afx_msg void OnDatetimechangeImStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeImEnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGraphcos();
	afx_msg void OnGraphexp();
	afx_msg void OnGraphlog();
	afx_msg void OnGraphpoly();
	afx_msg void OnGraphsin();
	afx_msg void OnGraphtan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMMIGRATIONDLG_H__7CD760E4_EC59_11D2_8919_44455354616F__INCLUDED_)
