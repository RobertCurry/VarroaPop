#pragma once
#include "MyPropSheet.h"
#include "afxwin.h"
#include "EPAData.h"
#include "NutrientContaminationTable.h"



// CEPAPage dialog

class CEPAPage : public CPropertyPage
{
	//DECLARE_DYNAMIC(CEPAPage)

public:
	CEPAPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEPAPage();
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}
	void Initialize();
	void ClearAISection();

protected:
		CMyPropSheet* m_pMyPropSheet;
		CVarroaPopView* m_pParent;

// Dialog Data
	enum { IDD = IDD_PESTICIDE_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SelectPesticide(CString PesticideName);
	void SetDateControlRanges();

public:
	void SetEarliestDate(COleDateTime Earliest);
	void SetLatestDate(COleDateTime Latest);
	void SetAIParametersEditable(bool bEdit = true);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddPesticide();
	afx_msg void OnBnClickedButtonUpdatePesticide();

	CEPAData m_EPAData;

protected:
	AIItem m_Current_AIItem;

public:
	virtual BOOL OnInitDialog();
	CComboBox m_AI_CB_Name;
	afx_msg void OnCbnSelchangePesticideName();
	double m_AI_HalfLife;
	double m_PTrips;
	double m_NTrips;
	double m_PollenLoad;
	double m_NectarLoad;
	double m_pctNectarForagers;
	CNutrientContaminationTable m_NutrientCT;
	CString m_DispContaminantFileName;

	COleDateTime m_EarliestDate;
	COleDateTime m_LatestDate;

	afx_msg void OnDtnDatetimechangeFolForagebegin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeFolForageend(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeSoilForagebegin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeSoilForageend(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeSdForagebegin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeSdForageend(NMHDR *pNMHDR, LRESULT *pResult);
	double m_Pesticide_Adult_Contact_Slope;
	double m_Pesticide_Contact_LD50;
	afx_msg void OnBnClickedRemoveAI();
	afx_msg void OnBnClickedButtonSelcontfile();
	BOOL m_NutrientContEnabled;
protected:
	CString theFileNamer;
public:
	BOOL m_SupPolEnable;
	BOOL m_SupNecEnable;
	BOOL m_SuppPolAnnual;
	BOOL m_SuppNecAnnual;
	BOOL m_ResourcesRequired;
	double m_SupPolInitAmount;
	double m_SupNecInitAmount;
	COleDateTime m_SupPolBegin;
	COleDateTime m_SupPolEnd;
	COleDateTime m_SupNecBegin;
	COleDateTime m_SupNecEnd;
	double m_ColonyPolInitAmount;
	double m_ColonyNecInitAmount;
	double m_ColonyPolMaxAmount;
	double m_ColonyNecMaxAmount;
};
