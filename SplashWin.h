#if !defined(AFX_SPLASHWIN_H__0DAF2BC2_3F7E_11D4_AC53_444553546170__INCLUDED_)
#define AFX_SPLASHWIN_H__0DAF2BC2_3F7E_11D4_AC53_444553546170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashWin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashWin window

class CSplashWin : public CWnd
{
// Construction
public:
	CSplashWin();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWin)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplashWin();

protected:
	CTime m_startTime;
	CTime m_endTime;
	CTimeSpan m_elapsedTime;

public:

	void Display();
	void Kill();
	void KillAfter(int Seconds);


	// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWin)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHWIN_H__0DAF2BC2_3F7E_11D4_AC53_444553546170__INCLUDED_)
