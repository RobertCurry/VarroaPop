#include "atlcomtime.h"
#include "gridctrl.h"
#include "gridcelldatetime.h"
//#include "IEDItem.h"
#include "mypropsheet.h"
#include "afxwin.h"
#if !defined(AFX_ACTIONPAGE_H__0F212FE0_BC91_11D4_8D9A_00A024E290BA__INCLUDED_)
#define AFX_ACTIONPAGE_H__0F212FE0_BC91_11D4_8D9A_00A024E290BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActionPage dialog

class CActionPage : public CPropertyPage
{
// Construction
public:
	CActionPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CActionPage();

// Dialog Data
	//{{AFX_DATA(CActionPage)
	enum { IDD = IDD_ACTION_FORM };
	UINT	m_EggLayingDelay;
	BOOL	m_EnableReQueen;
	int		m_Scheduled;
	COleDateTime	m_ReQueenDate;
	int		m_QueenStrength;
	int		m_RQOnce;
	COleDateTime	m_TreatmentStart;
	UINT	m_TreatmentDuration;
	BOOL	m_VT_Enable;
	UINT	m_TreatmentMortality;
	double	m_Resistant;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL OnKillActive();
	BOOL OnSetActive();
	void EnableDate(bool en = true);
	void SetQueenStrengthSlider(int Strength);
	int GetQueenStrengthSlider();
	CMiteTreatments* m_pMiteTreatments;
	void InitializeTreatmentGrid();


	// Generated message map functions
	//{{AFX_MSG(CActionPage)
	afx_msg void OnRadioAuto();
	afx_msg void OnRadioSched();
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawRqsliderqueen(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



public:
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}
	void GetMiteTreatmentInfo(CMiteTreatments& theTreatments);
	void SetMiteTreatmentInfo(CMiteTreatments& theTreatments);
	//void SetIEDItem(CIEDItem& theIEDItem);
	//void GetIEDItem(CIEDItem& theIEDItem);
	void PopulateMiteTreatmentInfo();
	
public:
	afx_msg void OnBnClickedVtEnable();
    afx_msg void OnBnClickedButtonDel();
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnNMClickListTreatDates(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTreatGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
protected:
    CGridCtrl m_TreatmentGrid;
	CMyPropSheet* m_pMyPropSheet;
public:
    void AddTreatmentItem(CMiteTreatmentItem Item);
    bool GetTreatmentItem(int ItemNum, CMiteTreatmentItem &theItem);
    int GetNumTreatmentItems(void);
    void UpdateTreatmentGrid(void);
    
// These are attributes added for IED Method
public:
    //CIEDItem m_IEDItem;    
    //BOOL m_IEDEnable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONPAGE_H__0F212FE0_BC91_11D4_8D9A_00A024E290BA__INCLUDED_)
