#pragma once
#include "MyPropSheet.h"


// CPesticidePage dialog

class CPesticidePage : public CPropertyPage
{
	//DECLARE_DYNAMIC(CPesticidePage)

public:
	CPesticidePage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPesticidePage();
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}

protected:
		CMyPropSheet* m_pMyPropSheet;


// Dialog Data
	enum { IDD = IDD_PESTICIDE_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddPesticide();
	afx_msg void OnBnClickedButtonUpdatePesticide();
	// Active Ingredients
	double m_AI_AdultLD50;
	double m_AI_LarvaSlope;
	double m_AI_LarvaLD50;
	double m_AI_KOW;
	double m_AI_KOC;

	// Consumption
	double m_C_Larv4_Pollen;
	double m_C_Larv4_Nectar;
	double m_C_Larva5_Pollen;
};
