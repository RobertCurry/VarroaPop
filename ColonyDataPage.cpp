// ColonyDataPage.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "ColonyDataPage.h"
#include "Colony.h"
#include "VarroaPopView.h"
#include "VarroaPopDoc.h"
#include "SpanValue.h"
#include "MainFrm.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THISPAGENUMBER 0
#define FORAGER_AGE_MIN 4
#define FORAGER_AGE_MAX 16
#define QUEEN_STR_MIN 1
#define QUEEN_STR_MAX 5
/////////////////////////////////////////////////////////////////////////////
// CColonyDataPage property page

IMPLEMENT_DYNCREATE(CColonyDataPage, CPropertyPage)

CColonyDataPage::CColonyDataPage(CVarroaPopDoc* pDoc) 
	: CPropertyPage(CColonyDataPage::IDD)
{
	m_pMyPropSheet = NULL;
	m_pFile = NULL;
	m_pDoc = pDoc;
	
	ClearInitCond();

	//{{AFX_DATA_INIT(CColonyDataPage)
	droneAdultInfestField = 0.0f;
	droneBroodInfestField = 0.0f;
	droneMiteSurvivorshipField = 0.0f;
	workerAdultInfestField = 0.0f;
	workerMiteSurvivorship = 0.0f;
	droneAdultsField = 0;
	droneBroodField = 0;
	droneEggsField = 0;
	droneLarvaeField = 0;
	workerAdultsField = 0;
	workerBroodField = 0;
	workerEggsField = 0;
	workerLarvaeField = 0;
	workerBroodInfestField = 0.0f;
	droneMiteOffspringField = 0.0f;
	workerMiteOffspring = 0.0f;
	//}}AFX_DATA_INIT

}

CColonyDataPage::~CColonyDataPage()
{
	if(m_pFile) 
	{
		delete m_pFile;
	}
	
	m_IC.m_EggTransitionDRV.ClearAll();
	m_IC.m_LarvaeTransitionDRV.ClearAll();
	m_IC.m_BroodTransitionDRV.ClearAll();
	m_IC.m_AdultTransitionDRV.ClearAll();
	m_IC.m_ForagerLifespanDRV.ClearAll();	
	m_IC.m_AdultLifespanDRV.ClearAll();

}


void CColonyDataPage::ClearInitCond()
{
	m_IC.m_droneAdultInfestField = 0.0f;
	m_IC.m_workerAdultInfestField = 0.0f;
	m_IC.m_droneBroodInfestField = 0.0f;
	m_IC.m_workerBroodInfestField = 0.0f;
	m_IC.m_droneAdultsField = 0;
	m_IC.m_droneBroodField = 0;
	m_IC.m_droneEggsField = 0;
	m_IC.m_droneLarvaeField = 0;
	m_IC.m_droneMiteOffspringField = 0.0f;
	m_IC.m_droneMiteSurvivorshipField = 0.0f;
	m_IC.m_workerAdultsField = 0;
	m_IC.m_workerBroodField = 0;
	m_IC.m_workerEggsField = 0;
	m_IC.m_workerLarvaeField = 0;
	m_IC.m_workerMiteOffspring = 0.0f;
	m_IC.m_workerMiteSurvivorship = 0.0f;
	m_IC.m_QueenSperm = 0;
	m_IC.m_ForagerLifespan = 0;
	m_IC.m_MaxEggs = 0;
	m_IC.m_QueenStrength = 1;
	m_IC.m_AdultTransitionDRV.ClearAll();
	m_IC.m_BroodTransitionDRV.ClearAll();
	m_IC.m_EggTransitionDRV.ClearAll();
	m_IC.m_ForagerLifespanDRV.ClearAll();
	m_IC.m_LarvaeTransitionDRV.ClearAll();
	m_IC.m_AdultLifespanDRV.ClearAll();
}





void CColonyDataPage::DoDataExchange(CDataExchange* pDX)
{

	// Stupid code because Class Wizard can't deal with structures
	droneAdultInfestField=m_IC.m_droneAdultInfestField;
	droneBroodInfestField=m_IC.m_droneBroodInfestField;
	droneMiteOffspringField=m_IC.m_droneMiteOffspringField;
	droneMiteSurvivorshipField=m_IC.m_droneMiteSurvivorshipField;
	workerAdultInfestField=m_IC.m_workerAdultInfestField;
	workerBroodInfestField=m_IC.m_workerBroodInfestField;
	workerMiteOffspring=m_IC.m_workerMiteOffspring;
	workerMiteSurvivorship=m_IC.m_workerMiteSurvivorship;
	droneAdultsField=m_IC.m_droneAdultsField;
	droneBroodField=m_IC.m_droneBroodField;
	droneEggsField=m_IC.m_droneEggsField;
	droneLarvaeField=m_IC.m_droneLarvaeField;
	workerAdultsField=m_IC.m_workerAdultsField;
	workerBroodField=m_IC.m_workerBroodField;
	workerEggsField=m_IC.m_workerEggsField;
	workerLarvaeField=m_IC.m_workerLarvaeField;
	// End of Stupid code


	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColonyDataPage)
	DDX_Text(pDX, IDC_ADULT_DRONE_INFEST, droneAdultInfestField);
	DDV_MinMaxFloat(pDX, droneAdultInfestField, 0.f, 1000.f);
	DDX_Text(pDX, IDC_BROOD_DRONE_INFEST, droneBroodInfestField);
	DDV_MinMaxFloat(pDX, droneBroodInfestField, 0.f, 1000.f);
	DDX_Text(pDX, IDC_DRONE_MITE_SURVIVORSHIP, droneMiteSurvivorshipField);
	DDV_MinMaxFloat(pDX, droneMiteSurvivorshipField, 0.f, 100.f);
	DDX_Text(pDX, IDC_ADULT_WORKER_INFEST, workerAdultInfestField);
	DDV_MinMaxFloat(pDX, workerAdultInfestField, 0.f, 1000.f);
	DDX_Text(pDX, IDC_WORKER_MITE_SURVIVORSHIP, workerMiteSurvivorship);
	DDV_MinMaxFloat(pDX, workerMiteSurvivorship, 0.f, 100.f);
	DDX_Text(pDX, IDC_DRONE_ADULTS, droneAdultsField);
	DDV_MinMaxInt(pDX, droneAdultsField, 0, 1000000);
	DDX_Text(pDX, IDC_DRONE_BROOD, droneBroodField);
	DDV_MinMaxInt(pDX, droneBroodField, 0, 1000000);
	DDX_Text(pDX, IDC_DRONE_EGGS, droneEggsField);
	DDV_MinMaxInt(pDX, droneEggsField, 0, 1000000);
	DDX_Text(pDX, IDC_DRONE_LARVAE, droneLarvaeField);
	DDV_MinMaxInt(pDX, droneLarvaeField, 0, 1000000);
	DDX_Text(pDX, IDC_WORKER_ADULTS, workerAdultsField);
	DDV_MinMaxInt(pDX, workerAdultsField, 0, 1000000);
	DDX_Text(pDX, IDC_WORKER_BROOD, workerBroodField);
	DDV_MinMaxInt(pDX, workerBroodField, 0, 1000000);
	DDX_Text(pDX, IDC_WORKER_EGGS, workerEggsField);
	DDV_MinMaxInt(pDX, workerEggsField, 0, 1000000);
	DDX_Text(pDX, IDC_WORKER_LARVAE, workerLarvaeField);
	DDV_MinMaxInt(pDX, workerLarvaeField, 0, 1000000);
	DDX_Text(pDX, IDC_BROOD_WORKER_INFEST, workerBroodInfestField);
	DDV_MinMaxFloat(pDX, workerBroodInfestField, 0.f, 1000.f);
	DDX_Text(pDX, IDC_DRONE_MITE_OFFSPRING, droneMiteOffspringField);
	DDV_MinMaxFloat(pDX, droneMiteOffspringField, 0.f, 5.f);
	DDX_Text(pDX, IDC_WORKER_MITE_OFFSPRING, workerMiteOffspring);
	DDV_MinMaxFloat(pDX, workerMiteOffspring, 0.f, 5.f);
	DDX_Text(pDX, IDC_EDIT_MAXPFORAGERS, m_MaxPropForagers);
	DDV_MinMaxDouble(pDX, m_MaxPropForagers, 0.0, 1.0);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		m_IC.m_ForagerLifespan = GetForagerAgeSlider();
		m_IC.m_QueenStrength = GetQueenStrengthSlider();
		m_pDoc->GetColony()->queen.SetStrength(GetQueenStrengthSlider());
		//SetQueenVariables(GetQueenStrengthSlider(),
		//	&m_IC.m_MaxEggs,&m_IC.m_QueenSperm);

	}
	else
	{
		SetForagerAgeSlider(m_IC.m_ForagerLifespan);
		//int str = GetQueenStrengthValue(m_IC.m_MaxEggs,m_IC.m_QueenSperm);
		//double str = m_pDoc->GetColony()->queen.GetStrengthFromMaxEggs(m_IC.m_MaxEggs);
		double str = m_pDoc->GetColony()->queen.GetQueenStrength();
		SetQueenStrengthSlider(m_pDoc->GetColony()->queen.GetQueenStrength()); 

	}


	// Stupid code because Class Wizard can't deal with structures
	m_IC.m_droneAdultInfestField=droneAdultInfestField;
	m_IC.m_droneBroodInfestField=droneBroodInfestField;
	m_IC.m_droneMiteOffspringField=droneMiteOffspringField;
	m_IC.m_droneMiteSurvivorshipField=droneMiteSurvivorshipField;
	m_IC.m_workerAdultInfestField=workerAdultInfestField;
	m_IC.m_workerBroodInfestField=workerBroodInfestField;
	m_IC.m_workerMiteOffspring=workerMiteOffspring;
	m_IC.m_workerMiteSurvivorship=workerMiteSurvivorship;
	m_IC.m_droneAdultsField=droneAdultsField;
	m_IC.m_droneBroodField=droneBroodField;
	m_IC.m_droneEggsField=droneEggsField;
	m_IC.m_droneLarvaeField=droneLarvaeField;
	m_IC.m_workerAdultsField=workerAdultsField;
	m_IC.m_workerBroodField=workerBroodField;
	m_IC.m_workerEggsField=workerEggsField;
	m_IC.m_workerLarvaeField=workerLarvaeField;
	// End of Stupid code

}


BEGIN_MESSAGE_MAP(CColonyDataPage, CPropertyPage)
	//{{AFX_MSG_MAP(CColonyDataPage)
	ON_BN_CLICKED(IDC_COL_HIDE_BUTTON, OnHideButton)
	ON_BN_CLICKED(IDC_SAVE_COLONY_BUTTON, OnSaveColonyButton)
	ON_CBN_SELCHANGE(IDC_COLONY_LIST, OnSelchangeColonyList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERFORAGER, OnCustomdrawSliderforager)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_DISMISS, OnHideButton)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERQUEEN, OnCustomdrawSliderqueen)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_EGG_SPAN, &CColonyDataPage::OnBnClickedButtonEggSpan)
	ON_BN_CLICKED(IDC_BUTTON_LARVAE_SPAN, &CColonyDataPage::OnBnClickedButtonLarvaeSpan)
	ON_BN_CLICKED(IDC_BUTTON_BROOD_SPAN, &CColonyDataPage::OnBnClickedButtonBroodSpan)
	ON_BN_CLICKED(IDC_BUTTON_ADULT_SPAN, &CColonyDataPage::OnBnClickedButtonAdultSpan)
	ON_BN_CLICKED(IDC_CHECK_EN_EGG, &CColonyDataPage::OnBnClickedCheckEnEgg)
	ON_BN_CLICKED(IDC_CHECK_EN_LARV, &CColonyDataPage::OnBnClickedCheckEnLarv)
	ON_BN_CLICKED(IDC_CHECK_EN_BROOD, &CColonyDataPage::OnBnClickedCheckEnBrood)
	ON_BN_CLICKED(IDC_CHECK_EN_ADULT, &CColonyDataPage::OnBnClickedCheckEnAdult)
	ON_BN_CLICKED(IDC_BUTTON_FORAGER_SPAN2, &CColonyDataPage::OnBnClickedButtonForagerSpan)
	ON_BN_CLICKED(IDC_CHECK_EN_FORAGER, &CColonyDataPage::OnBnClickedCheckEnForager)
	ON_BN_CLICKED(IDC_BUTTON_ADULT_SPAN2, &CColonyDataPage::OnBnClickedButtonAdultToForager)
	ON_BN_CLICKED(IDC_CHECK_EN_ADULTSP, &CColonyDataPage::OnBnClickedCheckEnAdultsp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColonyDataPage message handlers

void CColonyDataPage::OnHideButton() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pMyPropSheet);
	m_pMyPropSheet->ShowWindow(SW_HIDE);
}

BOOL CColonyDataPage::OnInitDialog()  // called only once
{
	CPropertyPage::OnInitDialog();
		
	// Set Slider Ranges
	CSliderCtrl* pForagerAge = (CSliderCtrl*)GetDlgItem(IDC_SLIDERFORAGER);
	pForagerAge->SetRange(FORAGER_AGE_MIN,FORAGER_AGE_MAX);

	CSliderCtrl* pQueenStrength = (CSliderCtrl*)GetDlgItem(IDC_SLIDERQUEEN);
	pQueenStrength->SetRange(QUEEN_STR_MIN,QUEEN_STR_MAX, TRUE);

	SetForagerAgeSlider(FORAGER_AGE_MIN);
	double qs = m_pDoc->GetColony()->queen.GetQueenStrength();
	SetQueenStrengthSlider(m_pDoc->GetColony()->queen.GetQueenStrength());
	SetSpanEnableChecks();
	
	m_pMyPropSheet->SetLastPageNumber(THISPAGENUMBER);


	return TRUE;  
}

void CColonyDataPage::prepComboBoxes() 
{

	if (m_pDoc->GetColony()->IsInitialized()) 
	{
		m_IC = m_pDoc->GetColony()->m_InitCond;
		SetSpanEnableChecks();
	}
	else ClearInitCond();
	UpdateData(FALSE);
}

BOOL CColonyDataPage::OnSetActive()
{
	prepComboBoxes();
	return CPropertyPage::OnSetActive();
}

BOOL CColonyDataPage::OnKillActive() 
{
	OnSaveColonyButton();
	return CPropertyPage::OnKillActive();
}


void CColonyDataPage::OnSaveColonyButton() 
{
	UpdateData(TRUE);  // transfer field entries to vars
	OnBnClickedCheckEnAdult();
	OnBnClickedCheckEnBrood();
	OnBnClickedCheckEnLarv();
	OnBnClickedCheckEnEgg();
	OnBnClickedCheckEnForager();
	OnBnClickedCheckEnAdultsp();
	m_pDoc->GetColony()->m_InitCond = m_IC;  // Update Document
	m_pDoc->SetModifiedFlag();
}



void CColonyDataPage::readColonyFile(CString theName) 
{
	m_IC = m_pDoc->GetColony()->m_InitCond;
	m_pDoc->SetModifiedFlag();
	UpdateData(FALSE);   // Update the Dialog with the variable values
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->EnableDialogBar(m_pDoc->IsImmigrationEnabled());
}


void CColonyDataPage::OnSelchangeColonyList() 
{
	// TODO: Add your control notification handler code here
	// User selected an item from the list.
	// Open the file and import the data.
	CComboBox* selectedFiles = 
		(CComboBox*) GetDlgItem(IDC_COLONY_LIST);
	ASSERT(selectedFiles);

	int index = selectedFiles->GetCurSel();
	if(index != LB_ERR) 
	{
		CString theName;
		selectedFiles->GetLBText(index, theName);
		readColonyFile(theName+".col");
	}
}



void CColonyDataPage::OnCustomdrawSliderforager(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	m_IC.m_ForagerLifespan = GetForagerAgeSlider();
}

int CColonyDataPage::GetForagerAgeSlider()
{
	CSliderCtrl* pForagerAge = (CSliderCtrl*)GetDlgItem(IDC_SLIDERFORAGER);
	CStatic* pForagerVal = (CStatic*)GetDlgItem(IDC_FORAGERVAL);
	CString AgeStg;
	int theAge = pForagerAge->GetPos();
	AgeStg.Format("%d",theAge);
	pForagerVal->SetWindowText(AgeStg);
	return theAge;
}

void CColonyDataPage::SetForagerAgeSlider(int theAge)
{
	CSliderCtrl* pForagerAge = (CSliderCtrl*)GetDlgItem(IDC_SLIDERFORAGER);
	CStatic* pForagerVal = (CStatic*)GetDlgItem(IDC_FORAGERVAL);
	CString AgeStg;

	// Verify value in allowed range
	if (theAge > pForagerAge->GetRangeMax()) 
		theAge = pForagerAge->GetRangeMax();
	if (theAge < pForagerAge->GetRangeMin())
		theAge = pForagerAge->GetRangeMin();
	pForagerAge->SetPos(theAge);

	AgeStg.Format("%d",theAge);
	pForagerVal->SetWindowText(AgeStg);
}

void CColonyDataPage::OnCustomdrawSliderqueen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
	m_IC.m_QueenStrength = GetQueenStrengthSlider();
	m_pDoc->GetColony()->queen.SetStrength(m_IC.m_QueenStrength);
}

int CColonyDataPage::GetQueenStrengthSlider()
{
	CSliderCtrl* pQueenStrength = (CSliderCtrl*)GetDlgItem(IDC_SLIDERQUEEN);
	int theStrength = pQueenStrength->GetPos();
	return theStrength;
}

void CColonyDataPage::SetQueenStrengthSlider(double theStrength)
{
	CSliderCtrl* pQueenStrength = (CSliderCtrl*)GetDlgItem(IDC_SLIDERQUEEN);
	if (theStrength > pQueenStrength->GetRangeMax()) 
		theStrength = pQueenStrength->GetRangeMax();
	if (theStrength < pQueenStrength->GetRangeMin())
		theStrength = pQueenStrength->GetRangeMin();
	pQueenStrength->SetPos(int(theStrength +0.5));  // Rounded integer
}


void CColonyDataPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow) prepComboBoxes();
	
}



void CColonyDataPage::OnBnClickedButtonEggSpan()
{
	// Test logic
	CDlgSpanValue dlgSpanValue;
	dlgSpanValue.SetDlgTitle("Percentages of Eggs Transitioning to Larvae by Date Ranges");
	dlgSpanValue.SetValueTitle("%Trans");
	dlgSpanValue.SetUpperBoundVal(100);
	dlgSpanValue.SetLowerBoundVal(0);
	*dlgSpanValue.m_pDateRangeValues = m_IC.m_EggTransitionDRV;
	if (dlgSpanValue.DoModal() == IDOK)
	{
		m_IC.m_EggTransitionDRV = *dlgSpanValue.m_pDateRangeValues;
	}
}

void CColonyDataPage::OnBnClickedButtonLarvaeSpan()
{
	CDlgSpanValue dlgSpanValue;
	dlgSpanValue.SetDlgTitle("Percentages of Larvae Transitioning to Capped Brood by Date Ranges");
	dlgSpanValue.SetValueTitle("%Trans");
	dlgSpanValue.SetUpperBoundVal(100);
	dlgSpanValue.SetLowerBoundVal(0);
	*dlgSpanValue.m_pDateRangeValues = m_IC.m_LarvaeTransitionDRV;
	if (dlgSpanValue.DoModal() == IDOK)
	{
		m_IC.m_LarvaeTransitionDRV = *dlgSpanValue.m_pDateRangeValues;
	}
}

void CColonyDataPage::OnBnClickedButtonBroodSpan()
{
	CDlgSpanValue dlgSpanValue;
	dlgSpanValue.SetDlgTitle("Percentages of Capped Brood Transitioning to Adults by Date Ranges");
	dlgSpanValue.SetValueTitle("%Trans");
	dlgSpanValue.SetUpperBoundVal(100);
	dlgSpanValue.SetLowerBoundVal(0);
	*dlgSpanValue.m_pDateRangeValues = m_IC.m_BroodTransitionDRV;
	if (dlgSpanValue.DoModal() == IDOK)
	{
		m_IC.m_BroodTransitionDRV = *dlgSpanValue.m_pDateRangeValues;
	}
}

void CColonyDataPage::OnBnClickedButtonAdultSpan()
{
	CDlgSpanValue dlgSpanValue;
	dlgSpanValue.SetDlgTitle("Percentages of Adults Transitioning to Foragers by Date Ranges");
	dlgSpanValue.SetValueTitle("%Trans");
	dlgSpanValue.SetUpperBoundVal(100);
	dlgSpanValue.SetLowerBoundVal(0);
	*dlgSpanValue.m_pDateRangeValues = m_IC.m_AdultTransitionDRV;
	if (dlgSpanValue.DoModal() == IDOK)
	{
		m_IC.m_AdultTransitionDRV = *dlgSpanValue.m_pDateRangeValues;
	}
}

void CColonyDataPage::OnBnClickedButtonForagerSpan()
{
	CDlgSpanValue dlgSpanValue;
	dlgSpanValue.SetDlgTitle("Forager Lifespan in Days by Date Ranges");
	dlgSpanValue.SetValueTitle("Days"); // arbitrary value range - be sure this is correct
	dlgSpanValue.SetUpperBoundVal(20);
	dlgSpanValue.SetLowerBoundVal(0);
	dlgSpanValue.SetValidation(true);
	*dlgSpanValue.m_pDateRangeValues = m_IC.m_ForagerLifespanDRV;
	if (dlgSpanValue.DoModal() == IDOK)
	{
		m_IC.m_ForagerLifespanDRV = *dlgSpanValue.m_pDateRangeValues;
	}
}


void CColonyDataPage::OnBnClickedButtonAdultToForager()
{
	CDlgSpanValue dlgSpanValue;
	dlgSpanValue.SetDlgTitle("Age at which Adult become Foragers by Date Ranges");
	dlgSpanValue.SetValueTitle("Days");
	dlgSpanValue.SetUpperBoundVal(21);
	dlgSpanValue.SetLowerBoundVal(7);
	*dlgSpanValue.m_pDateRangeValues = m_IC.m_AdultLifespanDRV;
	if (dlgSpanValue.DoModal() == IDOK)
	{
		m_IC.m_AdultLifespanDRV = *dlgSpanValue.m_pDateRangeValues;
	}

}
void CColonyDataPage::OnBnClickedCheckEnEgg()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_EN_EGG);
	if (pCheck->GetCheck() == BST_CHECKED) m_IC.m_EggTransitionDRV.SetEnabled(true);
	else m_IC.m_EggTransitionDRV.SetEnabled(false);
}

void CColonyDataPage::OnBnClickedCheckEnLarv()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_EN_LARV);
	if (pCheck->GetCheck() == BST_CHECKED) m_IC.m_LarvaeTransitionDRV.SetEnabled(true);
	else m_IC.m_LarvaeTransitionDRV.SetEnabled(false);
}

void CColonyDataPage::OnBnClickedCheckEnBrood()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_EN_BROOD);
	if (pCheck->GetCheck() == BST_CHECKED) m_IC.m_BroodTransitionDRV.SetEnabled(true);
	else m_IC.m_BroodTransitionDRV.SetEnabled(false);
}

void CColonyDataPage::OnBnClickedCheckEnAdult()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_EN_ADULT);
	if (pCheck->GetCheck() == BST_CHECKED) m_IC.m_AdultTransitionDRV.SetEnabled(true);
	else m_IC.m_AdultTransitionDRV.SetEnabled(false);
}



void CColonyDataPage::OnBnClickedCheckEnForager()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_EN_FORAGER);
	if (pCheck->GetCheck() == BST_CHECKED) m_IC.m_ForagerLifespanDRV.SetEnabled(true);
	else m_IC.m_ForagerLifespanDRV.SetEnabled(false);
}

void CColonyDataPage::OnBnClickedCheckEnAdultsp()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_EN_ADULTSP);
	if (pCheck->GetCheck() == BST_CHECKED) m_IC.m_AdultLifespanDRV.SetEnabled(true);
	else m_IC.m_AdultLifespanDRV.SetEnabled(false);
}

void CColonyDataPage::SetSpanEnableChecks()
{
	CButton* pButton;
	
	pButton = (CButton*)GetDlgItem(IDC_CHECK_EN_EGG);
	if (m_IC.m_EggTransitionDRV.IsEnabled()) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_EN_LARV);
	if (m_IC.m_LarvaeTransitionDRV.IsEnabled()) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_EN_BROOD);
	if (m_IC.m_BroodTransitionDRV.IsEnabled()) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_EN_ADULT);
	if (m_IC.m_AdultTransitionDRV.IsEnabled()) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_EN_FORAGER);
	if (m_IC.m_ForagerLifespanDRV.IsEnabled()) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_EN_ADULTSP);
	if (m_IC.m_AdultLifespanDRV.IsEnabled()) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
}


