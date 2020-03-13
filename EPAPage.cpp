// EPAPage.cpp : implementation file
//

#include "stdafx.h"
#include "afxdtctl.h"
#include "afxole.h"
#include "VarroaPop.h"
#include "VarroaPopView.h"
#include "EPAPage.h"
#include "PesticideAdd.h"

#define THISPAGENUMBER 3

// CEPAPage dialog

//IMPLEMENT_DYNAMIC(CEPAPage, CDialog)

CEPAPage::CEPAPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CEPAPage::IDD)
	, m_Pesticide_Adult_Contact_Slope(0)
	, m_Pesticide_Contact_LD50(0)
	, m_NutrientContEnabled(FALSE)
	, theFileNamer(_T(""))
	, m_SupPolEnable(FALSE)
	, m_SupNecEnable(FALSE)
	, m_SupPolInitAmount(0)
	, m_SupNecInitAmount(0)
	, m_SupPolBegin(COleDateTime::GetCurrentTime())
	, m_SupPolEnd(COleDateTime::GetCurrentTime())
	, m_SupNecBegin(COleDateTime::GetCurrentTime())
	, m_SupNecEnd(COleDateTime::GetCurrentTime())
	, m_ColonyPolInitAmount(0)
	, m_ColonyNecInitAmount(0)
	, m_ResourcesRequired(TRUE)
	, m_SuppPolAnnual(FALSE)
	, m_SuppNecAnnual(FALSE)
	, m_ColonyPolMaxAmount(0)
	, m_ColonyNecMaxAmount(0)
{
	m_pParent = (CVarroaPopView*)pParent;
}

CEPAPage::~CEPAPage()
{
}

void CEPAPage::Initialize()
{
	// Load Combo Box
	POSITION pos = m_EPAData.m_AIItemList.GetHeadPosition();
	AIItem* pItem;
	m_AI_CB_Name.ResetContent();
	if (pos == NULL) // nothing in AI list so don't allow the user to update anything until a list item is added
	{
		SetAIParametersEditable(false);
		return;
	}
	else
	{
		SetAIParametersEditable(true);
	}
	while (pos != NULL)
	{
		pItem = (AIItem*)m_EPAData.m_AIItemList.GetNext(pos);
		m_AI_CB_Name.AddString(pItem->m_AI_Name);
	}

	// Point to last selected item in AI list
	if (m_AI_CB_Name.SetCurSel(m_pParent->m_CurrentDisplayAI) != CB_ERR)
	{
		CString AIName;
		m_AI_CB_Name.GetLBText(m_pParent->m_CurrentDisplayAI,AIName);
		SelectPesticide(AIName);
	}

	m_DispContaminantFileName = m_NutrientCT.GetFileName();
}

void CEPAPage::SetAIParametersEditable(bool bEdit)
{
	GetDlgItem(IDC_PESTICIDE_NAME)->EnableWindow(bEdit);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_AD_SLOPE)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_AD_LD50)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_LARV_SLOPE)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_LARV_LD50)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_KOW)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_KOC)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_HALFLIFE)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_CONFACT)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_AC_SLOPE)->EnableWindow(bEdit);
	GetDlgItem(IDC_PESTICIDE_AC_LD50)->EnableWindow(bEdit);
}

void CEPAPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// Pesticide Name control
	DDX_Control(pDX, IDC_PESTICIDE_NAME, m_AI_CB_Name);

	// Active Ingredient

	// TODO:  EPA requests we stop validation during early testing stages
	// All DDV_routines should be re-enabled when we decide to turn validation back on
	DDX_Text(pDX, IDC_PESTICIDE_AD_SLOPE, m_EPAData.m_AI_AdultSlope);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_AdultSlope, 1, 20);
	DDX_Text(pDX, IDC_PESTICIDE_AD_LD50, m_EPAData.m_AI_AdultLD50);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_AdultLD50, 0, 10);
	DDX_Text(pDX, IDC_PESTICIDE_AC_SLOPE, m_EPAData.m_AI_AdultSlope_Contact);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_AdultSlope_Contact, 1, 20);
	DDX_Text(pDX, IDC_PESTICIDE_AC_LD50, m_EPAData.m_AI_AdultLD50_Contact);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_AdultLD50_Contact, 0, 10);
	DDX_Text(pDX, IDC_PESTICIDE_LARV_SLOPE, m_EPAData.m_AI_LarvaSlope);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_LarvaSlope, 1, 20);
	DDX_Text(pDX, IDC_PESTICIDE_LARV_LD50, m_EPAData.m_AI_LarvaLD50);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_LarvaLD50, 0, 10);
	DDX_Text(pDX, IDC_PESTICIDE_KOW, m_EPAData.m_AI_KOW);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_KOW, 0.001, 100000000);
	DDX_Text(pDX, IDC_PESTICIDE_KOC, m_EPAData.m_AI_KOC);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_KOC, 0.001, 100000000);
	DDX_Text(pDX, IDC_PESTICIDE_HALFLIFE, m_EPAData.m_AI_HalfLife);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_HalfLife, 0.01, 50);
	DDX_Text(pDX, IDC_EDIT5, m_EPAData.m_AI_ContactFactor);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_AI_ContactFactor, 0, 10);

	// Consumption Data
	DDX_Text(pDX, IDC_PESTICIDE_C_P1, m_EPAData.m_C_L4_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_L4_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N1, m_EPAData.m_C_L4_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_L4_Nectar, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_P2, m_EPAData.m_C_L5_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_L5_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N2, m_EPAData.m_C_L5_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_L5_Nectar, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_P3, m_EPAData.m_C_LD_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_L5_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N3, m_EPAData.m_C_LD_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_L5_Nectar, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_P4, m_EPAData.m_C_A13_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_A13_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N4, m_EPAData.m_C_A13_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_A13_Nectar, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_P5, m_EPAData.m_C_A410_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_A410_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N5, m_EPAData.m_C_A410_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_A410_Nectar, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_P6, m_EPAData.m_C_A1120_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_A1120_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N6, m_EPAData.m_C_A1120_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_A1120_Nectar, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_P7, m_EPAData.m_C_Forager_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_Forager_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N7, m_EPAData.m_C_Forager_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_Forager_Nectar, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_P8, m_EPAData.m_C_AD_Pollen);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_AD_Pollen, 0,300);
	DDX_Text(pDX, IDC_PESTICIDE_C_N8, m_EPAData.m_C_AD_Nectar);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_C_AD_Nectar, 0,300);

	// Exposure Data
	DDX_Check(pDX, IDC_CHECK1, m_EPAData.m_FoliarEnabled);
	DDX_Check(pDX, IDC_CHECK3, m_EPAData.m_SoilEnabled);
	DDX_Check(pDX, IDC_CHECK2, m_EPAData.m_SeedEnabled);
	DDX_Text(pDX, IDC_EDIT1, m_EPAData.m_E_AppRate);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_E_AppRate, 0, 1000);
	DDX_Text(pDX, IDC_EDIT2, m_EPAData.m_E_SoilTheta);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_E_SoilTheta, -10, 10);
	DDX_Text(pDX, IDC_EDIT8, m_EPAData.m_E_SoilP);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_E_SoilP, -10, 10);
	DDX_Text(pDX, IDC_EDIT7, m_EPAData.m_E_SoilFoc);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_E_SoilFoc, -10, 10);
	DDX_Text(pDX, IDC_EDIT3, m_EPAData.m_E_SeedConcentration);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_E_SeedConcentration, 0, 10);

	DDX_DateTimeCtrl(pDX, IDC_FOLIAR_APP_DATE, m_EPAData.m_FoliarAppDate);
	DDX_DateTimeCtrl(pDX, IDC_FOL_FORAGEBEGIN, m_EPAData.m_FoliarForageBegin);
	DDX_DateTimeCtrl(pDX, IDC_FOL_FORAGEEND, m_EPAData.m_FoliarForageEnd);
	DDX_DateTimeCtrl(pDX, IDC_SOIL_FORAGEBEGIN, m_EPAData.m_SoilForageBegin);
	DDX_DateTimeCtrl(pDX, IDC_SOIL_FORAGEEND, m_EPAData.m_SoilForageEnd);
	DDX_DateTimeCtrl(pDX, IDC_SD_FORAGEBEGIN, m_EPAData.m_SeedForageBegin);
	DDX_DateTimeCtrl(pDX, IDC_SD_FORAGEEND, m_EPAData.m_SeedForageEnd);
	DDX_Text(pDX, IDC_EDIT4, m_EPAData.m_E_SoilConcentration);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_E_SoilConcentration, 0, 10);
	DDX_Text(pDX, IDC_EDIT6, m_EPAData.m_I_PollenTrips);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_I_PollenTrips, 0, 100);
	DDX_Text(pDX, IDC_EDIT9, m_EPAData.m_I_NectarTrips);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_I_NectarTrips, 0, 100);
	DDX_Text(pDX, IDC_EDIT10, m_EPAData.m_I_PollenLoad);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_I_PollenLoad, 0, 1000);
	DDX_Text(pDX, IDC_EDIT11, m_EPAData.m_I_NectarLoad);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_I_NectarLoad, 0, 1000);
	//DDX_Text(pDX, IDC_EDIT12, m_EPAData.m_I_PercentNectarForagers);
	//DDV_MinMaxDouble(pDX, m_EPAData.m_I_PercentNectarForagers, 0, 100);
	DDX_Check(pDX, IDC_CHECK_NPCont, m_NutrientCT.m_NutrientContEnabled);
	//DDX_Text(pDX, IDC_STATIC_CONTFILENAME, m_NutrientCT.m_ContaminantFileName);
	DDX_Text(pDX, IDC_STATIC_CONTFILENAME, m_DispContaminantFileName);
	DDX_Check(pDX, IDC_CHECK_SUP_POP_EN, m_SupPolEnable);
	DDX_Check(pDX, IDC_CHECK_SUP_NEC_EN, m_SupNecEnable);
	DDX_Text(pDX, IDC_EDIT_SUP_POL_AMT, m_SupPolInitAmount);
	DDX_Text(pDX, IDC_EDIT_SUP_NEC_AMT, m_SupNecInitAmount);
	DDX_DateTimeCtrl(pDX, IDC_SUP_POL_BEGIN, m_SupPolBegin);
	DDX_DateTimeCtrl(pDX, IDC_SUP_POL_END, m_SupPolEnd);
	DDX_DateTimeCtrl(pDX, IDC_SUP_NEC_BEGIN, m_SupNecBegin);
	DDX_DateTimeCtrl(pDX, IDC_SUP_NEC_END, m_SupNecEnd);
	DDX_Text(pDX, IDC_EDIT_INIT_POLLEN, m_ColonyPolInitAmount);
	DDX_Text(pDX, IDC_EDIT_INIT_NECTAR, m_ColonyNecInitAmount);
	DDX_Check(pDX, IDC_CHECK_RESOURCES, m_ResourcesRequired);
	DDX_Check(pDX, IDC_CHECK_POL_ANN, m_SuppPolAnnual);
	DDX_Check(pDX, IDC_CHECK_NEC_AN, m_SuppNecAnnual);
	DDX_Text(pDX, IDC_EDIT_MAX_POLLEN, m_ColonyPolMaxAmount);
	DDX_Text(pDX, IDC_EDIT_MAX_NECTAR, m_ColonyNecMaxAmount);
}


BEGIN_MESSAGE_MAP(CEPAPage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CEPAPage::OnBnClickedButtonAddPesticide)
	ON_BN_CLICKED(IDC_BUTTON2, &CEPAPage::OnBnClickedButtonUpdatePesticide)
	ON_CBN_SELCHANGE(IDC_PESTICIDE_NAME, &CEPAPage::OnCbnSelchangePesticideName)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FOL_FORAGEBEGIN, &CEPAPage::OnDtnDatetimechangeFolForagebegin)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FOL_FORAGEEND, &CEPAPage::OnDtnDatetimechangeFolForageend)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SOIL_FORAGEBEGIN, &CEPAPage::OnDtnDatetimechangeSoilForagebegin)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SOIL_FORAGEEND, &CEPAPage::OnDtnDatetimechangeSoilForageend)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SD_FORAGEBEGIN, &CEPAPage::OnDtnDatetimechangeSdForagebegin)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SD_FORAGEEND, &CEPAPage::OnDtnDatetimechangeSdForageend)
	ON_BN_CLICKED(IDC_BUTTON3, &CEPAPage::OnBnClickedRemoveAI)
	ON_BN_CLICKED(IDC_BUTTON_SELCONTFILE, &CEPAPage::OnBnClickedButtonSelcontfile)
END_MESSAGE_MAP()


// CEPAPage message handlers

void CEPAPage::OnBnClickedButtonAddPesticide()
{
	CPesticideAdd PesticideAddDialog;

	if (PesticideAddDialog.DoModal()== IDOK)
	{
		AIItem* pItem = new AIItem;
		pItem->m_AI_Name = PesticideAddDialog.m_Name;
		pItem->m_AI_AdultSlope = PesticideAddDialog.m_AdultSlope;
		pItem->m_AI_AdultLD50 = PesticideAddDialog.m_AdultLD50;
		pItem->m_AI_AdultSlope_Contact = PesticideAddDialog.m_AdultSlope_Contact;
		pItem->m_AI_AdultLD50_Contact = PesticideAddDialog.m_AdultLD50_Contact;
		pItem->m_AI_LarvaSlope = PesticideAddDialog.m_LarvaSlope;
		pItem->m_AI_LarvaLD50 = PesticideAddDialog.m_LarvaLD50;
		pItem->m_AI_KOW = PesticideAddDialog.m_KOW;
		pItem->m_AI_KOC = PesticideAddDialog.m_KOC;
		pItem->m_AI_HalfLife = PesticideAddDialog.m_HalfLife;
		pItem->m_AI_ContactFactor = PesticideAddDialog.m_ContactFactor;
		m_EPAData.AddAIItem(pItem);
		m_AI_CB_Name.AddString(pItem->m_AI_Name);

		SelectPesticide(pItem->m_AI_Name);
		SetAIParametersEditable(true);

	}
}

void CEPAPage::OnBnClickedButtonUpdatePesticide()
{
	UpdateData(1);
	CString CurName;
	// Find the name in the AIList and then update the values based on the dialog contents.
	m_AI_CB_Name.GetLBText(m_AI_CB_Name.GetCurSel(),CurName);
	AIItem* pItem = m_EPAData.GetAIItemPtr(CurName);
	if (pItem != NULL)
	{
		pItem->m_AI_AdultLD50 = m_EPAData.m_AI_AdultLD50;
		pItem->m_AI_AdultSlope = m_EPAData.m_AI_AdultSlope;
		pItem->m_AI_AdultLD50_Contact = m_EPAData.m_AI_AdultLD50_Contact;
		pItem->m_AI_AdultSlope_Contact = m_EPAData.m_AI_AdultSlope_Contact;
		pItem->m_AI_LarvaLD50 = m_EPAData.m_AI_LarvaLD50;
		pItem->m_AI_LarvaSlope = m_EPAData.m_AI_LarvaSlope;
		pItem->m_AI_KOC = m_EPAData.m_AI_KOC;
		pItem->m_AI_KOW = m_EPAData.m_AI_KOW;
		pItem->m_AI_HalfLife = m_EPAData.m_AI_HalfLife;
		pItem->m_AI_ContactFactor = m_EPAData.m_AI_ContactFactor;
	}
}

BOOL CEPAPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pMyPropSheet->SetLastPageNumber(THISPAGENUMBER);

	Initialize();
	SetDateControlRanges();
	UpdateData(0);

	// Select the most recently selected AI or first in list if this is the first time after program start.
	if (m_AI_CB_Name.GetCount() > 0)
	{
		CString SelectedName;
		if (m_pParent->m_CurrentDisplayAI > m_AI_CB_Name.GetCount() - 1) m_pParent->m_CurrentDisplayAI = m_AI_CB_Name.GetCount() - 1;  // Trap overflows
		m_AI_CB_Name.GetLBText(m_pParent->m_CurrentDisplayAI, SelectedName);
		SelectPesticide(SelectedName);
	}

	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CEPAPage::OnCbnSelchangePesticideName()
{
	CString SelectedName;
	m_AI_CB_Name.GetLBText(m_AI_CB_Name.GetCurSel(),SelectedName);
	AIItem Item;
	BOOL Found = m_EPAData.GetAIItem(SelectedName,&Item);
	
	if (Found)
	{
		m_EPAData.SetCurrentAIItem(&Item);
		m_pParent->m_CurrentDisplayAI = m_AI_CB_Name.GetCurSel();
		//m_Current_AIItem = Item;
		UpdateData(0);
	}
}

void CEPAPage::SelectPesticide(CString PesticideName)
{
	int count = m_AI_CB_Name.GetCount();
	int LBIndex = -1;
	CString TestString;
	// Search for the PesticideName
	for (int i=0; i<count; i++)
	{
		m_AI_CB_Name.GetLBText(i,TestString);
		if (TestString == PesticideName) 
		{
			LBIndex = i; // Found the match - save the index
			break;
		}
	}
	AIItem Item;
	BOOL Found = m_EPAData.GetAIItem(PesticideName,&Item);
	
	if (Found && (LBIndex >=0))
	{
		m_EPAData.SetCurrentAIItem(&Item);
		//m_Current_AIItem = Item;
		m_AI_CB_Name.SetCurSel(LBIndex);
		UpdateData(0);
	}

}

void CEPAPage::OnBnClickedRemoveAI()
{
	// This function deletes the currently selected AI from the dialog box and from m_EPAData.
	// Checks to ensure user really wants to do this.
	// Moves the next lower item into the currently selected position unless this is the last
	// item in which case the "new" last item is selected.
	// Remove from m_EPAData.m_AIItemList;

	if (m_AI_CB_Name.GetCount() == 0) return;

	// Get the currently selected name
	CString CurrentName = m_EPAData.m_AI_Name;

	if (MyMessageBox("Are you sure you want to delete "+CurrentName+" ?", MB_YESNO) == IDYES)
	{
		// First remove the data
		if (m_EPAData.RemoveAIItem(CurrentName))
		{
			// if this item was found and successfully removed, now remove the name from the list and reposition
			int count = m_AI_CB_Name.GetCount();
			int position = m_AI_CB_Name.FindString(-1, CurrentName);
			ASSERT(position >= 0); // position < 0 means the string wasn't found - would indicate confusion
			int newcount = m_AI_CB_Name.DeleteString((UINT)position);

			if (position < newcount) // select the next item in the list box
			{
				m_AI_CB_Name.GetLBText(position, CurrentName);
				SelectPesticide(CurrentName);
			}
			else if (newcount > 0) // select the last item in the list box.
			{
				m_AI_CB_Name.GetLBText(newcount - 1, CurrentName);
				SelectPesticide(CurrentName);
			}
			else // Nothing left in list.  Clear the AI boxes
			{
				ClearAISection();
				SetAIParametersEditable(false);
			}

		}
	}
}

void CEPAPage::ClearAISection()
{
	m_AI_CB_Name.Clear();
	m_AI_CB_Name.SetWindowText("");
	m_EPAData.m_AI_AdultLD50 = 0.0;
	m_EPAData.m_AI_AdultLD50_Contact = 0.0;
	m_EPAData.m_AI_AdultSlope = 0.0;
	m_EPAData.m_AI_AdultSlope_Contact = 0.0;
	m_EPAData.m_AI_ContactFactor = 0.0;
	m_EPAData.m_AI_HalfLife = 0.0;
	m_EPAData.m_AI_KOC = 0.0;
	m_EPAData.m_AI_KOW = 0.0;
	m_EPAData.m_AI_LarvaLD50 = 0.0;
	m_EPAData.m_AI_LarvaSlope = 0.0;
	m_EPAData.m_AI_Name = "";
	UpdateData(0);
}

/////////////////////////////////////////////////////////////////////////////////
// OnDateTimeChange handlers
//
// These ensure the beginning date is prior to the ending date.  Uses the 
// CDateTimeCtrl ranges to implement.
void CEPAPage::OnDtnDatetimechangeFolForagebegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDateTimeCtrl* BeginCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_FOL_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_FOL_FORAGEEND);
	
	COleDateTime BeginCtrlMax, BeginCtrlMin;
	COleDateTime EndCtrlMax, EndCtrlMin;

	COleDateTime BeginTime;
	COleDateTime EndTime;

	BeginCtrl->GetTime(BeginTime);
	BeginCtrl->GetRange(&BeginCtrlMin, &BeginCtrlMax);

	EndCtrl->GetTime(EndTime);
	EndCtrl->GetRange(&EndCtrlMin, &EndCtrlMax);

	if (BeginTime < EndTime)
	{
		BeginCtrl->SetRange(&BeginCtrlMin, &EndTime);
		EndCtrl->SetRange(&BeginTime, &EndCtrlMax);
	}
	*pResult = 0;
}

void CEPAPage::OnDtnDatetimechangeFolForageend(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDateTimeCtrl* BeginCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_FOL_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_FOL_FORAGEEND);
	
	COleDateTime BeginCtrlMax, BeginCtrlMin;
	COleDateTime EndCtrlMax, EndCtrlMin;

	COleDateTime BeginTime;
	COleDateTime EndTime;

	BeginCtrl->GetTime(BeginTime);
	BeginCtrl->GetRange(&BeginCtrlMin, &BeginCtrlMax);

	EndCtrl->GetTime(EndTime);
	EndCtrl->GetRange(&EndCtrlMin, &EndCtrlMax);

	if (EndTime > BeginTime)
	{
		BeginCtrl->SetRange(&BeginCtrlMin, &EndTime);
		EndCtrl->SetRange(&BeginTime, &EndCtrlMax);
	}
	*pResult = 0;
}

void CEPAPage::OnDtnDatetimechangeSoilForagebegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDateTimeCtrl* BeginCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SOIL_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SOIL_FORAGEEND);
	
	COleDateTime BeginCtrlMax, BeginCtrlMin;
	COleDateTime EndCtrlMax, EndCtrlMin;

	COleDateTime BeginTime;
	COleDateTime EndTime;

	BeginCtrl->GetTime(BeginTime);
	BeginCtrl->GetRange(&BeginCtrlMin, &BeginCtrlMax);

	EndCtrl->GetTime(EndTime);
	EndCtrl->GetRange(&EndCtrlMin, &EndCtrlMax);

	if (BeginTime < EndTime)
	{
		BeginCtrl->SetRange(&BeginCtrlMin, &EndTime);
		EndCtrl->SetRange(&BeginTime, &EndCtrlMax);
	}
	*pResult = 0;
}

void CEPAPage::OnDtnDatetimechangeSoilForageend(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDateTimeCtrl* BeginCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SOIL_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SOIL_FORAGEEND);
	
	COleDateTime BeginCtrlMax, BeginCtrlMin;
	COleDateTime EndCtrlMax, EndCtrlMin;

	COleDateTime BeginTime;
	COleDateTime EndTime;

	BeginCtrl->GetTime(BeginTime);
	BeginCtrl->GetRange(&BeginCtrlMin, &BeginCtrlMax);

	EndCtrl->GetTime(EndTime);
	EndCtrl->GetRange(&EndCtrlMin, &EndCtrlMax);

	if (EndTime > BeginTime)
	{
		BeginCtrl->SetRange(&BeginCtrlMin, &EndTime);
		EndCtrl->SetRange(&BeginTime, &EndCtrlMax);
	}
	*pResult = 0;
}







void CEPAPage::OnDtnDatetimechangeSdForagebegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDateTimeCtrl* BeginCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SD_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SD_FORAGEEND);
	
	COleDateTime BeginCtrlMax, BeginCtrlMin;
	COleDateTime EndCtrlMax, EndCtrlMin;

	COleDateTime BeginTime;
	COleDateTime EndTime;

	BeginCtrl->GetTime(BeginTime);
	BeginCtrl->GetRange(&BeginCtrlMin, &BeginCtrlMax);

	EndCtrl->GetTime(EndTime);
	EndCtrl->GetRange(&EndCtrlMin, &EndCtrlMax);

	if (BeginTime < EndTime)
	{
		BeginCtrl->SetRange(&BeginCtrlMin, &EndTime);
		EndCtrl->SetRange(&BeginTime, &EndCtrlMax);
	}
	*pResult = 0;
}

void CEPAPage::OnDtnDatetimechangeSdForageend(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDateTimeCtrl* BeginCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SD_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_SD_FORAGEEND);
	
	COleDateTime BeginCtrlMax, BeginCtrlMin;
	COleDateTime EndCtrlMax, EndCtrlMin;

	COleDateTime BeginTime;
	COleDateTime EndTime;

	BeginCtrl->GetTime(BeginTime);
	BeginCtrl->GetRange(&BeginCtrlMin, &BeginCtrlMax);

	EndCtrl->GetTime(EndTime);
	EndCtrl->GetRange(&EndCtrlMin, &EndCtrlMax);

	if (EndTime > BeginTime)
	{
		BeginCtrl->SetRange(&BeginCtrlMin, &EndTime);
		EndCtrl->SetRange(&BeginTime, &EndCtrlMax);
	}
	*pResult = 0;
}


// Sets the Date Control Ranges based on the current values of the Begin and End dates.
// Start range is earliest weather to current value of Begin.  End range is from current value of Begin to latest weather
void CEPAPage::SetDateControlRanges()
{
	/*
	// TEMPORARILY TURN OFF RANGE SETTING
	// Get all the controls
	CDateTimeCtrl* BeginCtrl_Foliar = (CDateTimeCtrl*)GetDlgItem(IDC_FOL_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl_Foliar = (CDateTimeCtrl*)GetDlgItem(IDC_FOL_FORAGEEND);
	CDateTimeCtrl* BeginCtrl_Soil = (CDateTimeCtrl*)GetDlgItem(IDC_SOIL_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl_Soil = (CDateTimeCtrl*)GetDlgItem(IDC_SOIL_FORAGEEND);
	CDateTimeCtrl* BeginCtrl_Seed = (CDateTimeCtrl*)GetDlgItem(IDC_SD_FORAGEBEGIN);
	CDateTimeCtrl* EndCtrl_Seed = (CDateTimeCtrl*)GetDlgItem(IDC_SD_FORAGEEND);

	// Foliar Forage Begin and End
	BeginCtrl_Foliar->SetRange(&m_EarliestDate, &(m_EPAData.m_FoliarForageBegin));
	EndCtrl_Foliar->SetRange(&(m_EPAData.m_FoliarForageBegin), &m_LatestDate);
	
	// Soil Forage Begin and End
	BeginCtrl_Soil->SetRange(&m_EarliestDate, &(m_EPAData.m_SoilForageBegin));
	EndCtrl_Soil->SetRange(&(m_EPAData.m_SoilForageBegin), &m_LatestDate);
	
	// Seed Forage Begin and End
	BeginCtrl_Seed->SetRange(&m_EarliestDate, &(m_EPAData.m_SeedForageBegin));
	EndCtrl_Seed->SetRange(&(m_EPAData.m_SeedForageBegin), &m_LatestDate);

	*/

}

void CEPAPage::SetEarliestDate(COleDateTime date)
{
	m_EarliestDate = date;
}

void CEPAPage::SetLatestDate(COleDateTime date)
{
	m_LatestDate = date;
}


void CEPAPage::OnBnClickedButtonSelcontfile()
{
	CFileDialog fd(	TRUE,
					".npc",
					NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("N/P Conc. (*.npc, *.csv)|*.npc; *.csv|All Files|*.*||"));

	if (fd.DoModal() == IDOK)
	{
		m_NutrientCT.m_ContaminantFileName =  fd.GetPathName();
		CWnd* pwnd = GetDlgItem(IDC_STATIC_CONTFILENAME);
		pwnd->SetWindowTextA("File: "+fd.GetPathName());
	}
}


