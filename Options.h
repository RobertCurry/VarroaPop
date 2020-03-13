#if !defined(AFX_OPTIONS_H__A4F3FA80_BC1B_11D6_B486_00A024E290BA__INCLUDED_)
#define AFX_OPTIONS_H__A4F3FA80_BC1B_11D6_B486_00A024E290BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Options.h : header file
//
#include "varroapopdoc.h"
/////////////////////////////////////////////////////////////////////////////
// COptions dialog

class COptions : public CDialog
{
// Construction
public:
	COptions(CVarroaPopDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptions)
	enum { IDD = IDD_OPTIONS };
	BOOL	m_ColTitles;
	BOOL	m_InitConds;
	BOOL	m_Version;
	BOOL	m_WeatherColony;
	int		m_FieldDelimiter;
	int		m_DispFrequency;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CVarroaPopDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(COptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_H__A4F3FA80_BC1B_11D6_B486_00A024E290BA__INCLUDED_)
