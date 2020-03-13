#if !defined(AFX_SIMULATIONPAGE_H__7CBB5421_83B0_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_SIMULATIONPAGE_H__7CBB5421_83B0_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SimulationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimulationPage dialog

class CSimulationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSimulationPage)

// Construction
public:
	CSimulationPage();
	~CSimulationPage();

// Dialog Data
	//{{AFX_DATA(CSimulationPage)
	enum { IDD = IDD_SIMULATION_FORM };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSimulationPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPropertySheet* m_pMyPropSheet;
	CStringList* m_pSelectedFilesList;

public:
	void setPropSheet(CPropertySheet* theSheet);
	void setSelectedFilesList(CStringList* theList);
	void prepComboBoxes();

protected:
	// Generated message map functions
	//{{AFX_MSG(CSimulationPage)
	afx_msg void OnHideButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULATIONPAGE_H__7CBB5421_83B0_11D2_8D9A_0020AF233A70__INCLUDED_)
