//{{AFX_INCLUDES()
#include "mschart.h"
//}}AFX_INCLUDES
#if !defined(AFX_GRAPHDLG_H__8733C4D3_1102_11D3_8919_44455354616F__INCLUDED_)
#define AFX_GRAPHDLG_H__8733C4D3_1102_11D3_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Matrix.h"
// GraphDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CGraphDlg dialog

class CGraphDlg : public CDialog
{
// Construction
public:
	CGraphDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGraphDlg)
	enum { IDD = IDD_GRAPHDIALOG };
	CMSChart	m_Chart;
	//}}AFX_DATA
	CMatrix* m_pData;
	CStringList* m_pLabels;
	CStringList* m_pAxisLabels;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void DoChart();
	void SetData(CMatrix* pData) {m_pData = pData;}
	void SetLabels(CStringList* pLabels) {m_pLabels = pLabels;}
	void SetAxisLabels(CStringList* pAxLabels) {m_pAxisLabels = pAxLabels;}


protected:

	// Generated message map functions
	//{{AFX_MSG(CGraphDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDLG_H__8733C4D3_1102_11D3_8919_44455354616F__INCLUDED_)
