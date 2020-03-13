//{{AFX_INCLUDES()
#include "chart2d.h"
//}}AFX_INCLUDES
#if !defined(AFX_OLECTRAGRAPHDLG_H__4A9373A0_6B9A_11D4_AC54_444553546170__INCLUDED_)
#define AFX_OLECTRAGRAPHDLG_H__4A9373A0_6B9A_11D4_AC54_444553546170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Matrix.h"
// OlectraGraphDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COlectraGraphDlg dialog

class COlectraGraphDlg : public CDialog
{
// Construction
public:
	COlectraGraphDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COlectraGraphDlg)
	enum { IDD = IDD_OLECTRAGRAPHDLG };
	CChart2D	m_Graph;
	//}}AFX_DATA
	CMatrix* m_pData;
	CStringList* m_pLabels;
	CStringList* m_pAxisLabels;
	COleDateTime m_StartDate;
	static int m_GraphDlgCount;  // The next number of a graph dialog title
	static int m_OpenGraphDlgs;  // The current number of displayed graph dialogs

	bool m_InitialUpdate;	// Has dialog been displayed yet
	bool m_Dragging;		// Is a mouse move a drag action
	double m_PlotMin;		// Minimum value of data on a chart
	double m_PlotMax;		// Maximum value of data on a chart
	CWnd* m_pMouseLabel;	// Window that will contain mouse coords


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COlectraGraphDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	int m_nPts;
	SAFEARRAY m_safeArray;
	bool m_AutoScaleYAxis;
	double m_YAxisMin;
	double m_YAxisMax;



// Implementation
public:
	void MouseLabel(CString LabelStg, double x, double y);
	void DoChart(CString OutputFileName = "");
	void SetTitle(CString title) {m_Title = title;}
	void SetData(CMatrix* pData) {m_pData = pData;}
	void SetYAxisMinMax(bool Auto, double Min = 0, double Max = 0);
	void SetLabels(CStringList* pLabels) {m_pLabels = pLabels;}
	void SetAxisLabels(CStringList* pAxLabels) {m_pAxisLabels = pAxLabels;}
	void SetStartDate(COleDateTime theDate) {m_StartDate = theDate;}


protected:
	CString m_Title;
	// Generated message map functions
	//{{AFX_MSG(COlectraGraphDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPrinter();
	afx_msg void OnFile();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnModifyEndOlectrachart();
	afx_msg void OnMouseDownOlectrachart(short Button, short Shift, long X, long Y);
	afx_msg void OnMouseMoveOlectrachart(short Button, short Shift, long X, long Y);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLECTRAGRAPHDLG_H__4A9373A0_6B9A_11D4_AC54_444553546170__INCLUDED_)
