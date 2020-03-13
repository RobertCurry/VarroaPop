#if !defined(AFX_COLONYSELECTIONPAGE_H__AF303DC2_8E83_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_COLONYSELECTIONPAGE_H__AF303DC2_8E83_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColonySelectionPage.h : header file
//
#include "MyPropSheet.h"
/////////////////////////////////////////////////////////////////////////////
// CColonySelectionPage dialog

class CColonySelectionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CColonySelectionPage)

// Construction
public:
	CColonySelectionPage();
	~CColonySelectionPage();

// Dialog Data
	//{{AFX_DATA(CColonySelectionPage)
	enum { IDD = IDD_COLONY_SELECTION_FORM };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColonySelectionPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPropertySheet* m_pMyPropSheet;
	CPropertyPage* m_pColonyDataPage;
	CStringList* m_pAvailableFilesList;
	CStringList* m_pSelectedFilesList;
	int* m_pSelectedFileIndex;

public:
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}
	void setColonyDataPage(CPropertyPage* thePage);
	void setSelectedFileIndex(int* theLoc);
	void prepListBoxes();
	void rebuildFileLists();

protected:
	// Generated message map functions
	//{{AFX_MSG(CColonySelectionPage)
	afx_msg void OnHideButton();
	afx_msg void OnDeselectColony();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkColoniesAvailable();
	afx_msg void OnDblclkColoniesToSimulate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLONYSELECTIONPAGE_H__AF303DC2_8E83_11D2_8D9A_0020AF233A70__INCLUDED_)
