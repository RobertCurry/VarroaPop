// VarroaPop.h : main header file for the VARROAPOP application
//

#if !defined(AFX_VARROAPOP_H__8C6C4187_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_VARROAPOP_H__8C6C4187_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <afxtempl.h>       // ApplWizard doesn't include this!!
#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CVarroaPopApp:
// See VarroaPop.cpp for the implementation of this class
//

class CVarroaPopApp : public CWinApp
{
protected:
	CStdioFile m_LogFile;

public:
	CString m_ModuleFileName;
	CString m_SessionFileName;
	CString m_InputFileName;
	CString m_OutputGraphFileName;
	CString m_OutputResultsFileName;
	CString m_OutputLogFileName;
	

public:
	bool RunBackground();
	int Run();
	CVarroaPopApp();
	CString GetModuleFileName() {return m_ModuleFileName;}
	bool OpenLogFile(CString LogFileName);
	bool WriteToLogFile(CString Line);
	bool CloseLogFile(bool Save = true);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVarroaPopApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVarroaPopApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelpReportaproblem();
};


//Free Functions
int MyMessageBox( LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 );
enum PELEMENT {DRV = 1, DIR = 2, FNAME = 3, EXT =4};
CString SplitPath(CString PathString, PELEMENT PathElement);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARROAPOP_H__8C6C4187_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
