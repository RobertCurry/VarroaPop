// ActionPage.cpp : implementation file
//

#include "stdafx.h"
#include <afxdtctl.h>
#include "varroapop.h"
#include "MiteTreatments.h"
#include "ActionPage.h"
#include ".\actionpage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THISPAGENUMBER 2

/////////////////////////////////////////////////////////////////////////////
// CActionPage dialog


CActionPage::CActionPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CActionPage::IDD)
{
	//{{AFX_DATA_INIT(CActionPage)
	m_EggLayingDelay = 10;
	m_EnableReQueen = FALSE;
	m_Scheduled = 0;
	m_ReQueenDate = COleDateTime::GetCurrentTime();
	m_QueenStrength = 5;
	m_RQOnce = 0;
	m_TreatmentStart = COleDateTime::GetCurrentTime();
	m_TreatmentDuration = 0;
	m_VT_Enable = FALSE;
	m_TreatmentMortality = 0;
	m_Resistant = 0.0;
	//m_IEDEnable = FALSE;
	//}}AFX_DATA_INIT
	
	m_pMiteTreatments = new CMiteTreatments;


}

CActionPage::~CActionPage()
{
	m_pMiteTreatments->ClearAll();
	delete m_pMiteTreatments;
}

void CActionPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	//{{AFX_DATA_MAP(CActionPage)
	DDX_Text(pDX, IDC_EDIT_DELAY, m_EggLayingDelay);
	DDV_MinMaxUInt(pDX, m_EggLayingDelay, 0, 1000);
	DDX_Check(pDX, IDC_ENABLE, m_EnableReQueen);
	DDX_Radio(pDX, IDC_RADIO_SCHED, m_Scheduled);
	DDX_DateTimeCtrl(pDX, IDC_QUEEN_DATE, m_ReQueenDate);
	DDX_Radio(pDX, IDC_RADIO_RQ_ONCE, m_RQOnce);
	DDX_DateTimeCtrl(pDX, IDC_TREATMENT_START, m_TreatmentStart);
	DDX_Text(pDX, IDC_TREATMENT_DURATION, m_TreatmentDuration);
	DDV_MinMaxUInt(pDX, m_TreatmentDuration, 0, 52);
	DDX_Check(pDX, IDC_VT_ENABLE, m_VT_Enable);
	DDX_Text(pDX, IDC_MORTALITY, m_TreatmentMortality);
	DDV_MinMaxUInt(pDX, m_TreatmentMortality, 0, 100);
	DDX_Text(pDX, IDC_RESISTANT, m_Resistant);
	DDV_MinMaxDouble(pDX, m_Resistant, 0., 100.);
	//}}AFX_DATA_MAP


	if (pDX->m_bSaveAndValidate)
	{
		m_QueenStrength = GetQueenStrengthSlider();
	}
	else
	{
		//if (IsWindow(m_hWnd)) SetQueenStrengthSlider(m_QueenStrength);
	}


	DDX_Control(pDX, IDC_GRIDCTRL1, m_TreatmentGrid);
	//DDX_DateTimeCtrl(pDX, IDC_IED_EXPOSURE_DATE, m_IEDItem.m_IEDDate);
	//DDX_Text(pDX, IDC_EDIT_IED_M_EGG, m_IEDItem.m_MortEggs);
	//DDV_MinMaxDouble(pDX, m_IEDItem.m_MortEggs, 0.0, 1.0);
	//DDX_Text(pDX, IDC_EDIT_IED_M_LARVAE, m_IEDItem.m_MortLarvae);
	//DDV_MinMaxDouble(pDX, m_IEDItem.m_MortLarvae, 0.0, 1.0);
	//DDX_Text(pDX, IDC_EDIT_IED_M_PUPAE, m_IEDItem.m_MortBrood);
	//DDV_MinMaxDouble(pDX, m_IEDItem.m_MortBrood, 0.0, 1.0);
	//DDX_Text(pDX, IDC_EDIT_IED_M_ADULT, m_IEDItem.m_MortAdults);
	//DDV_MinMaxDouble(pDX, m_IEDItem.m_MortAdults, 0.0, 1.0);
	//DDX_Text(pDX, IDC_EDIT_IED_M_FORAGERS, m_IEDItem.m_MortForagers);
	//DDV_MinMaxDouble(pDX, m_IEDItem.m_MortForagers, 0.0, 1.0);
	//DDX_Check(pDX, IDC_CHECK1, m_IEDEnable);
}


BEGIN_MESSAGE_MAP(CActionPage, CDialog)
	//{{AFX_MSG_MAP(CActionPage)
	ON_BN_CLICKED(IDC_RADIO_AUTO, OnRadioAuto)
	ON_BN_CLICKED(IDC_RADIO_SCHED, OnRadioSched)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RQSLIDERQUEEN, OnCustomdrawRqsliderqueen)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_VT_ENABLE, OnBnClickedVtEnable)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRIDCTRL1, OnTreatGridEndEdit)

ON_BN_CLICKED(IDC_VT_ENABLE, OnBnClickedVtEnable)
ON_BN_CLICKED(IDC_BUTTON_DEL, &CActionPage::OnBnClickedButtonDel)
ON_BN_CLICKED(IDC_BUTTON_ADD, &CActionPage::OnBnClickedButtonAdd)
ON_NOTIFY(NM_CLICK, IDC_LIST_TREAT_DATES, &CActionPage::OnNMClickListTreatDates)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionPage message handlers

void CActionPage::OnRadioAuto() 
{
	// Disable Date input
	EnableDate(false);
	
}

void CActionPage::OnRadioSched() 
{
	// Enable Date
	EnableDate(true);
	
}



void CActionPage::EnableDate(bool en)
{
	(CStatic*)GetDlgItem(IDC_STATICDATE)->EnableWindow(en);
	(CEdit*)GetDlgItem(IDC_QUEEN_DATE)->EnableWindow(en);
	(CButton*)GetDlgItem(IDC_RADIO_RQ_ONCE)->EnableWindow(en);
	(CButton*)GetDlgItem(IDC_RADIO_RQ_ANNUAL)->EnableWindow(en);

}


BOOL CActionPage::OnInitDialog() 
{
	// ISSUE:
	// Slider min/max set here but for some reason SetQueenStrength Slider is called before this executes so 
	// we get the wrong range.  Default range is 0::100 so the typical QS of 5 looks like 0.
	CDialog::OnInitDialog();
	CSliderCtrl* pQueenStrength = (CSliderCtrl*)GetDlgItem(IDC_RQSLIDERQUEEN);
	pQueenStrength->SetRange(1,5,TRUE);
	SetQueenStrengthSlider(m_QueenStrength);
	EnableDate(m_Scheduled==0);
			
	InitializeTreatmentGrid();
	m_pMyPropSheet->SetLastPageNumber(THISPAGENUMBER);
	return TRUE;  
}


int CActionPage::GetQueenStrengthSlider()
{
	CSliderCtrl* pQueenStrength = (CSliderCtrl*)GetDlgItem(IDC_RQSLIDERQUEEN);
	return pQueenStrength->GetPos();

}
void CActionPage::SetMiteTreatmentInfo(CMiteTreatments& theTreatments)
{
	m_pMiteTreatments->ClearAll();
	CMiteTreatmentItem theItem;
	for (int i=0; i < theTreatments.GetCount(); i++)
	{
		theTreatments.GetItem(i, theItem);
		m_pMiteTreatments->AddItem(theItem);
	}
}

void CActionPage::GetMiteTreatmentInfo(CMiteTreatments& theTreatments)
{
	theTreatments.ClearAll();
	CMiteTreatmentItem theItem;
	for (int i=0; i< m_pMiteTreatments->GetCount(); i++)
	{
		m_pMiteTreatments->GetItem(i,theItem);
		theTreatments.AddItem(theItem);
	}
	
}



void CActionPage::SetQueenStrengthSlider(int Strength)
{
	CSliderCtrl* pQueenStrength = (CSliderCtrl*)GetDlgItem(IDC_RQSLIDERQUEEN);
	if (Strength > pQueenStrength->GetRangeMax()) 
		Strength = pQueenStrength->GetRangeMax();
	else if (Strength < pQueenStrength->GetRangeMin()) 
		Strength = pQueenStrength->GetRangeMin();
	pQueenStrength->SetPos(Strength);

}

void CActionPage::OnCustomdrawRqsliderqueen(NMHDR* pNMHDR, LRESULT* pResult) 
{

	m_QueenStrength = GetQueenStrengthSlider();
	*pResult = 0;
}

BOOL CActionPage::OnSetActive()
{
	SetQueenStrengthSlider(m_QueenStrength);
	return CPropertyPage::OnSetActive();
}

BOOL CActionPage::OnKillActive()
{
	return CPropertyPage::OnKillActive();
}




void CActionPage::OnBnClickedVtEnable()
{
	m_VT_Enable = !m_VT_Enable;
	// Miticide treatment or Spore treatment but not both
	UpdateData(FALSE);
}



void CActionPage::OnBnClickedButtonDel()
{
	for (int row = 1; row <= m_pMiteTreatments->GetCount(); row++)
	{
		if ((m_TreatmentGrid.IsCellSelected(row,1)) && (m_TreatmentGrid.IsCellSelected(row,2)) && (m_TreatmentGrid.IsCellSelected(row,3)))
		{
			m_pMiteTreatments->DeleteItem(row-1);
			UpdateTreatmentGrid();
			break;  // Currently only allow deletion of one row at a time
		}
	}
}

void CActionPage::OnBnClickedButtonAdd()
{
	// Adds a treatment date and associated info to the list of treatment dates
	CDateTimeCtrl* theDate = (CDateTimeCtrl*)GetDlgItem(IDC_TREATMENT_START);
	CEdit* theDuration = (CEdit*)GetDlgItem(IDC_TREATMENT_DURATION);
	CEdit* theMort = (CEdit*)GetDlgItem(IDC_MORTALITY);
	CEdit* theRes = (CEdit*)GetDlgItem(IDC_RESISTANT);

	// Format date and put in list control
	COleDateTime DateVal;
	CString DurText;
	CString MortText;
	CString ResText;
	theDate->GetTime(DateVal);
	theDuration->GetWindowText(DurText);
	theMort->GetWindowText(MortText);
	theRes->GetWindowText(ResText);
	
	// Add item to CMiteTreatments class
	m_pMiteTreatments->AddItem(DateVal, atoi(DurText), atof(MortText), atof(ResText));
	
	UpdateTreatmentGrid();

}


void CActionPage::OnNMClickListTreatDates(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CButton* pDelButton = (CButton*)GetDlgItem(IDC_BUTTON_DEL);    
	//CListCtrl* theList = (CListCtrl*)GetDlgItem(IDC_LIST_TREAT_DATES);
	//pDelButton->EnableWindow(theList->GetSelectedCount() > 0);  //Enable delete button if something is selected


	*pResult = 0;
}

void CActionPage::OnTreatGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	
	// AcceptChange is a fictional routine that should return TRUE
	// if you want to accept the new value for the cell.
	//BOOL bAcceptChange = AcceptChange(pItem->iRow, pItem->iColumn);
	try
	{
		CString stgResults = m_TreatmentGrid.GetItemText(pItem->iRow,pItem->iColumn);
		if (pItem->iColumn == 1) // Date
		{
			COleDateTime theTime;
			theTime.ParseDateTime(stgResults);
			m_pMiteTreatments->GetItemPtr(pItem->iRow-1)->theStartTime = (__time64_t)theTime;
		}
			
		else if (pItem->iColumn == 2) // Weeks
		{
			m_pMiteTreatments->GetItemPtr(pItem->iRow-1)->Duration = atoi(stgResults);
		}
		else if (pItem->iColumn == 3) // %Mort
		{
			double Mort = atof(stgResults);
			if ((Mort >= 0.0) && (Mort <= 100.0))
					m_pMiteTreatments->GetItemPtr(pItem->iRow-1)->PctMortality = Mort;
		}
		//*pResult = (bAcceptChange)? 0 : -1;
		*pResult = 0;
	}
	catch(CException* e)
	{
		CString errmsg = "Exception Found in Action Page: ";
		TCHAR msgbuf[255];
		e->GetErrorMessage(msgbuf,255);
		errmsg = errmsg + msgbuf;
		TRACE("Exception found: %s", errmsg);
		*pResult = -1;
	}
	
  
}

void CActionPage::InitializeTreatmentGrid()
{
	m_TreatmentGrid.SetColumnCount(4);
	m_TreatmentGrid.SetRowCount(1);
	m_TreatmentGrid.SetFixedRowCount(1);
	m_TreatmentGrid.SetFixedColumnCount(1);
	m_TreatmentGrid.SetAutoSizeStyle();
	
	// Write Header
	GV_ITEM Item;
	
	Item.nFormat = DT_CENTER;
			
	Item.mask = GVIF_TEXT;
	Item.row = 0;
	
	Item.col = 0;
	Item.strText = "#";
	m_TreatmentGrid.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_TreatmentGrid.SetItem(&Item);
	m_TreatmentGrid.SetColumnWidth(0,25);
	
	Item.col = 1;
	Item.strText = "Start";
	m_TreatmentGrid.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_TreatmentGrid.SetItem(&Item);
	m_TreatmentGrid.SetColumnWidth(1,85);
	
	Item.col = 2;
	Item.strText = "Wks";
	m_TreatmentGrid.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_TreatmentGrid.SetItem(&Item);
	m_TreatmentGrid.SetColumnWidth(2,35);

	Item.col = 3;
	Item.strText = "%Mort";
	m_TreatmentGrid.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_TreatmentGrid.SetItem(&Item);
	m_TreatmentGrid.SetColumnWidth(3,45);
		
	UpdateTreatmentGrid();
}


void CActionPage::AddTreatmentItem(CMiteTreatmentItem Item)
{
	m_pMiteTreatments->AddItem(Item);
	// Now update the grid
	UpdateTreatmentGrid();
}

bool CActionPage::GetTreatmentItem(int ItemNum, CMiteTreatmentItem &theItem)
{
	bool Success = false;
	if ((ItemNum >= 0) && (ItemNum < GetNumTreatmentItems())) 
	{
		Success = m_pMiteTreatments->GetItem(ItemNum, theItem);
	}
	return Success;
}

int CActionPage::GetNumTreatmentItems(void)
{
	int Number = 0;
	
	if (m_pMiteTreatments != NULL) Number = m_pMiteTreatments->GetCount();
	return Number;
}

void CActionPage::UpdateTreatmentGrid(void)
// Clears the grid and refills it with the current contents of m_pMiteTreatmentItems
{
	GV_ITEM GridItem;
	CMiteTreatmentItem TreatmentItem;
	m_TreatmentGrid.DeleteNonFixedRows();
	m_TreatmentGrid.SetRowCount(m_pMiteTreatments->GetCount()+1);  // Add one for the header
	for (int i = 0; i < m_pMiteTreatments->GetCount(); i++)
	{
		m_pMiteTreatments->GetItem(i,TreatmentItem);
		GridItem.row = i+1;
		GridItem.mask = GVIF_TEXT;
		
		GridItem.col = 0;
		GridItem.strText.Format("%d",GridItem.row);
		m_TreatmentGrid.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_TreatmentGrid.SetItem(&GridItem);

		GridItem.col = 1;
		GridItem.strText = TreatmentItem.theStartTime.Format("%m/%d/%Y");
		m_TreatmentGrid.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_TreatmentGrid.SetCellType(GridItem.row,GridItem.col,RUNTIME_CLASS(CGridCellDateTime));
		m_TreatmentGrid.SetItem(&GridItem);
		
		GridItem.col = 2;
		GridItem.strText.Format("%d",TreatmentItem.Duration);
		m_TreatmentGrid.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_TreatmentGrid.SetItem(&GridItem);
		
		GridItem.col = 3;
		GridItem.strText.Format("%4.1f",TreatmentItem.PctMortality);
		m_TreatmentGrid.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_TreatmentGrid.SetItem(&GridItem);
	}
}
