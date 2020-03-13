// VarroaPopView.h : interface of the CVarroaPopView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARROAPOPVIEW_H__8C6C418F_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_VARROAPOPVIEW_H__8C6C418F_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxcview.h>
#include "ColonyDataPage.h"
#include "WeatherPage.h"
#include "ActionPage.h"
#include "EPAPage.h"
#include "ResourcePage.h"
#include "SimulationPage.h"
#include "EPAPage.h"
#include "MyPropSheet.h"

#define MaxListClms 50

class CVarroaPopView : public CListView
{
protected: // create from serialization only
	CVarroaPopView();
	DECLARE_DYNCREATE(CVarroaPopView)

public:
	//{{AFX_DATA(CVarroaPopView)
	//}}AFX_DATA

// Attributes
public:
	CVarroaPopDoc* GetDocument();

protected:
	int m_OldPrintY;
	CFont m_fontPrinter;
	UINT m_cxWidth;
	UINT m_cxOffset;
	UINT m_nLinesPerPage;
	UINT m_nLinesPerHeader;
	UINT m_nCharsPerLine;
	UINT m_nLinesTotal;
	UINT m_cyPrinter;
	void PrintPageHeader(CVarroaPopDoc* pDoc, CDC* pDC, UINT PageNumber);
	void PrintPage(CVarroaPopDoc* pDoc, CDC* pDC, UINT PageNumber);
	void SaveColumnInfo();
	void GetColumnInfo();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVarroaPopView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateColumnWidth();
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVarroaPopView();
	CMyPropSheet* GetPropSheet() {return m_pPropSheet;}
	void ShowPropsheet() {OnShowPropsheet();}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:	
	CWnd* m_pMyParentWindow;
	CMyPropSheet* m_pPropSheet;
	CPoint m_ViewportOrigin;
	int m_LastPropSheetPageNum;
	int m_NumDisplayColumns;
	int m_ColumnCount;

public:
	int m_CurrentDisplayAI; // tracks the EPA Page last AI displayed

	// One page for each form so we can use OnInitDialog
	// to initialize the controls
public:
	void ChartData(CVarroaPopDoc* pDoc, bool Display = true);
	CColonyDataPage* m_pColonyDataPage;
	CWeatherPage* m_pWeatherPage;
	CActionPage* m_pActionPage;
	CResourcePage* m_pResourcePage;
	CSimulationPage* m_pSimulationPage;
	CEPAPage* m_pEPAPage;

protected:
	void SaveClmHdrInfo( LPCSTR sKey, int nClms, LPINT paiIndexes, LPINT paiWidths );
	BOOL ReadClmHdrInfo( LPCSTR sKey, int nClms, LPINT paiIndexes, LPINT paiWidths );
	void SaveClmInfo(     LPCSTR sKey ); 
	void UseSavedClmInfo( LPCSTR sKey ); 
	void UpdateEventLog();


// Generated message map functions
protected:
	//{{AFX_MSG(CVarroaPopView)
	afx_msg void OnShowPropsheet();
	afx_msg void OnViewParameters();
	afx_msg void OnColumnExpand();
	afx_msg void OnColumnReduce();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAppExit();
};

#ifndef _DEBUG  // debug version in VarroaPopView.cpp
inline CVarroaPopDoc* CVarroaPopView::GetDocument()
   { return (CVarroaPopDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARROAPOPVIEW_H__8C6C418F_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
