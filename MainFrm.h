// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8C6C418B_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_MAINFRM_H__8C6C418B_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CString m_WeatherFileName;
	CString m_SessionFileName;
	CStatusBar  m_wndStatusBar;
	bool m_EnableImmigration;


	int updatecnt;  // test
// Operations
public:
	void EnableDialogBar(bool bEnable);
	afx_msg void OnUpdateWeather(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSession(CCmdUI *pCmdUI);
	afx_msg void OnUpdateResFreq(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWeatherStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWeatherStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImType(CCmdUI *pCmdUI);
	void SetImmigration(bool On) {if (On) OnRadioImOn(); else OnRadioImOff();}
	void InitializeDateCtrls();
	void AddLogEvents(CString EventString);
	void ClearLogEvents();


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;

public:
	CDialogBar	m_SimulationBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonGo();
	afx_msg void OnButtonImSetup();
	afx_msg void OnRadioImOff();
	afx_msg void OnRadioImOn();
	afx_msg void OnUpdateFileSaveResults(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveSession(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditcurrentweather(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelp();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8C6C418B_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
