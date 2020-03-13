#if !defined(AFX_SELGRAPH_H__5A487580_1C17_11D3_8919_F1C93D681570__INCLUDED_)
#define AFX_SELGRAPH_H__5A487580_1C17_11D3_8919_F1C93D681570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VarroaPopDoc.h"
// SelGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelGraph dialog

class CSelGraph : public CDialog
{
// Construction
public:
	CSelGraph(CVarroaPopDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelGraph)
	enum { IDD = IDD_SELECT_GRAPHS };
	BOOL	m_TM;
	BOOL	m_AD;
	BOOL	m_AW;
	BOOL	m_CS;
	BOOL	m_DB;
	BOOL	m_DE;
	BOOL	m_DL;
	BOOL	m_F;
	BOOL	m_MDB;
	BOOL	m_MWB;
	BOOL	m_PDB;
	BOOL	m_PWB;
	BOOL	m_PRM;
	BOOL	m_RM;
	BOOL	m_WB;
	BOOL	m_WE;
	BOOL	m_WL;
	BOOL	m_MD;
	BOOL	m_PMD;
	BOOL	m_NS;
	BOOL	m_PS;
	BOOL	m_NPC;
	BOOL	m_PPC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelGraph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelGraph)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	CVarroaPopDoc* pDoc;
	void SetAutoScaleChart(bool bAuto);
	DECLARE_MESSAGE_MAP()
public:
	//int m_ScaleAuto;
	//bool m_AutoScaleChart;
	//int m_RadioAutoScale;
	afx_msg void OnRadioScaleAuto();
	afx_msg void OnRadioScaleManual();
	//double m_YAxisMax;
	//double m_YAxisMin;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELGRAPH_H__5A487580_1C17_11D3_8919_F1C93D681570__INCLUDED_)
