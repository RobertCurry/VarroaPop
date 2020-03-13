#if !defined(AFX_COLONYDATAPAGE_H__5A5C6F41_8072_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_COLONYDATAPAGE_H__5A5C6F41_8072_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColonyDataPage.h : header file
//

#include "Colony.h"
#include "VarroaPopDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CColonyDataPage dialog

class CColonyDataPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CColonyDataPage)

// Construction
public:
	CColonyDataPage()  : m_MaxPropForagers(0)
	{}
	CColonyDataPage(CVarroaPopDoc* pDoc);
	~CColonyDataPage();

// Dialog Data
	//{{AFX_DATA(CColonyDataPage)
	enum { IDD = IDD_COLONY_DATA_FORM };
	ColonyInitCond m_IC;
	float	droneAdultInfestField;
	float	droneBroodInfestField;
	float	droneMiteSurvivorshipField;
	float	workerAdultInfestField;
	float	workerMiteSurvivorship;
	int		droneAdultsField;
	int		droneBroodField;
	int		droneEggsField;
	int		droneLarvaeField;
	int		workerAdultsField;
	int		workerBroodField;
	int		workerEggsField;
	int		workerLarvaeField;
	float	workerBroodInfestField;
	float	droneMiteOffspringField;
	float	workerMiteOffspring;
	double m_MaxPropForagers;
	//}}AFX_DATA



// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColonyDataPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMyPropSheet* m_pMyPropSheet;
	//CDateRangeValues m_EggTransitionDRV;
	//CDateRangeValues m_LarvaeTransitionDRV;
	//CDateRangeValues m_BroodTransitionDRV;
	//CDateRangeValues m_AdultTransitionDRV;
	//CDateRangeValues m_ForagerLifespanDRV;

private:	
	CFile* m_pFile;
	CVarroaPopDoc* m_pDoc;
	void readColonyFile(CString theFileName);
	void ClearInitCond();
	int GetForagerAgeSlider();
	void SetForagerAgeSlider(int theAge);
	int GetQueenStrengthSlider();
	void SetQueenStrengthSlider(double theStrength);
	void SetSpanEnableChecks();

public:
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}
	void prepComboBoxes();
	//int GetQueenStrengthValue(int MaxEggs, int Sperm);
//	void SetQueenVariables(double Strength, double* MaxEggs, double* Sperm);

protected:
	// Generated message map functions
	//{{AFX_MSG(CColonyDataPage)
	afx_msg void OnHideButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnSaveColonyButton();
	afx_msg void OnSelchangeColonyList();
	afx_msg void OnCustomdrawSliderforager(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCustomdrawSliderqueen(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonEggSpan();
    afx_msg void OnBnClickedButtonLarvaeSpan();
    afx_msg void OnBnClickedButtonBroodSpan();
    afx_msg void OnBnClickedButtonAdultSpan();
    afx_msg void OnBnClickedCheckEnEgg();
    afx_msg void OnBnClickedCheckEnLarv();
    afx_msg void OnBnClickedCheckEnBrood();
    afx_msg void OnBnClickedCheckEnAdult();
    afx_msg void OnBnClickedButtonForagerSpan();
    afx_msg void OnBnClickedCheckEnForager();
	afx_msg void OnBnClickedButtonAdultToForager();
	afx_msg void OnBnClickedCheckEnAdultsp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLONYDATAPAGE_H__5A5C6F41_8072_11D2_8D9A_0020AF233A70__INCLUDED_)
