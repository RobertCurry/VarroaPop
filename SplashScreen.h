#if !defined(AFX_SPLASHSCREEN_H__1D96B2C5_3DD9_11D4_8D9A_00A024E290BA__INCLUDED_)
#define AFX_SPLASHSCREEN_H__1D96B2C5_3DD9_11D4_8D9A_00A024E290BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashScreen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen dialog

class CSplashScreen : public CDialog
{
// Construction
public:
	CSplashScreen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplashScreen)
	enum { IDD = IDD_SPLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashScreen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTime m_startTime;
	CTime m_endTime;
	CTimeSpan m_elapsedTime;

public:

	void Display();
	void Kill();
	void KillAfter(int Seconds);


	// Generated message map functions
	//{{AFX_MSG(CSplashScreen)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHSCREEN_H__1D96B2C5_3DD9_11D4_8D9A_00A024E290BA__INCLUDED_)
