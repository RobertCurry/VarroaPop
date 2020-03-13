// SpanValue.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "SpanValue.h"


// CDlgSpanValue dialog

IMPLEMENT_DYNAMIC(CDlgSpanValue, CDialog)

CDlgSpanValue::CDlgSpanValue(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSpanValue::IDD, pParent)
	, m_pDateRangeValues(NULL)
{
	m_pDateRangeValues = new CDateRangeValues();
	SetValueTitle("");
	SetDlgTitle("Date Ranges and Values");
	
	m_Validate = true;
}

CDlgSpanValue::~CDlgSpanValue()
{
	// NOTE:  If I do this, I must move the data out of the CDateRangeValue class
	// before this CDlgSpanValue object is deleted.
	m_pDateRangeValues->ClearAll();
	//delete m_pDateRangeValues;  //NOTE:  This was causing an invalid memory access - still need to fix
}

void CDlgSpanValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_DATE_VAL, m_DRV_GridCtrl);
	DDX_Control(pDX, IDC_DRV_Ctrl2, m_DRV_GridCtrl);
}


BEGIN_MESSAGE_MAP(CDlgSpanValue, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSpanValue::OnBnClickedButtonAdd)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_DRV_Ctrl2, OnDateValueEndEdit)
//    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMESTART, &CDlgSpanValue::OnDtnDatetimechangeDatetimestart)
//    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEEND, &CDlgSpanValue::OnDtnDatetimechangeDatetimeend)
ON_BN_CLICKED(IDC_BUTTON_DEL_ITEM, &CDlgSpanValue::OnBnClickedButtonDelItem)
END_MESSAGE_MAP()


// CDlgSpanValue message handlers

void CDlgSpanValue::InitializeSpanGrid(void)
{
	CWnd* pDRVWindow = GetDlgItem(IDC_DRV_Ctrl2);    
	CRect theRect;
	pDRVWindow->GetClientRect(theRect);
	int WidthCtrl = theRect.Width();

	m_DRV_GridCtrl.SetColumnCount(4);
	m_DRV_GridCtrl.SetRowCount(1);
	m_DRV_GridCtrl.SetFixedRowCount(1);
	m_DRV_GridCtrl.SetFixedColumnCount(1);
	m_DRV_GridCtrl.SetAutoSizeStyle();
	m_DRV_GridCtrl.SetSingleRowSelection(TRUE);
	
	// Set Column Widths
	int WidthNumCol = 25;
	int WidthStartCol = 85;
	int WidthEndCol = 85;
	int WidthCheckCol = 20;
	int WidthValCol = WidthCtrl - WidthNumCol - WidthStartCol - WidthEndCol;
	if (WidthValCol < 0) WidthValCol = 10;  
	
	// Write Header
	GV_ITEM Item;
	
	Item.nFormat = DT_CENTER;
			
	Item.mask = GVIF_TEXT;
	Item.row = 0;
	
	Item.col = 0;
	Item.strText = "#";
	m_DRV_GridCtrl.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_DRV_GridCtrl.SetItem(&Item);
	m_DRV_GridCtrl.SetColumnWidth(0,WidthNumCol);
	
	Item.col = 1;
	Item.strText = "Start";
	m_DRV_GridCtrl.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_DRV_GridCtrl.SetItem(&Item);
	m_DRV_GridCtrl.SetColumnWidth(1,WidthStartCol);
	
	Item.col = 2;
	Item.strText = "End";
	m_DRV_GridCtrl.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_DRV_GridCtrl.SetItem(&Item);
	m_DRV_GridCtrl.SetColumnWidth(2,WidthEndCol);

	Item.col = 3;
	Item.strText = m_ValueTitle;
	m_DRV_GridCtrl.SetItemFormat(Item.row, Item.col, DT_CENTER);
	m_DRV_GridCtrl.SetItem(&Item);
	m_DRV_GridCtrl.SetColumnWidth(3,WidthValCol);
	   
	UpdateSpanGrid();

}


void CDlgSpanValue::UpdateSpanGrid(void)
{
	//m_pDateRangeValues->TestFill();

	GV_ITEM GridItem;
	DR_ITEM DateRangeValueItem;
	m_DRV_GridCtrl.DeleteNonFixedRows();
	m_DRV_GridCtrl.SetRowCount(m_pDateRangeValues->GetCount()+1);  // Add one for the header row
	for (int i = 0; i < m_pDateRangeValues->GetCount(); i++)
	{
		m_pDateRangeValues->GetItem(i, DateRangeValueItem);
		GridItem.row = i+1;
		GridItem.mask = GVIF_TEXT;
		
		GridItem.col = 0;
		GridItem.strText.Format("%d",GridItem.row);
		m_DRV_GridCtrl.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_DRV_GridCtrl.SetItem(&GridItem);

		GridItem.col = 1;
		GridItem.strText = DateRangeValueItem.StartTime.Format("%m/%d/%Y");
		m_DRV_GridCtrl.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_DRV_GridCtrl.SetCellType(GridItem.row,GridItem.col,RUNTIME_CLASS(CGridCellDateTime));
		m_DRV_GridCtrl.SetItem(&GridItem);
		
		GridItem.col = 2;
		GridItem.strText.Format(DateRangeValueItem.EndTime.Format("%m/%d/%Y"));
		m_DRV_GridCtrl.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_DRV_GridCtrl.SetCellType(GridItem.row,GridItem.col, RUNTIME_CLASS(CGridCellDateTime));
		m_DRV_GridCtrl.SetItem(&GridItem);
		
		GridItem.col = 3;
		GridItem.strText.Format("%4.1f",DateRangeValueItem.Value);
		m_DRV_GridCtrl.SetItemFormat(GridItem.row, GridItem.col, DT_CENTER);
		m_DRV_GridCtrl.SetItem(&GridItem);
	}
}

bool CDlgSpanValue::GetSpanGridItem(int ItemNum, DR_ITEM& theItem)
{
	return false;
}

int CDlgSpanValue::GetNumSpanValueItems(void)
{
	return 0;
}

void CDlgSpanValue::AddSpanValueItem(DR_ITEM& theItem)
{
	m_pDateRangeValues->AddItem(theItem);
	UpdateSpanGrid();
}

BOOL CDlgSpanValue::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ((m_UpperBoundSet) && (m_LowerBoundSet)) // Automatically append range to ValueTitle
	{
		m_ValueTitle.Format("%s [%2.0f ... %2.0f]", m_ValueTitle, m_LowerBound, m_UpperBound);
	}
	((CStatic*)GetDlgItem(IDC_VAL_LABEL))->SetWindowTextA(m_ValueTitle);
	SetWindowTextA(m_DlgTitle); // Set caption for window

	InitializeSpanGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSpanValue::OnBnClickedButtonAdd()
{
	// First Validate start and stop dates then add to list or display error textbox
	// Adds a treatment date and associated info to the list of treatment dates
	CDateTimeCtrl* theStartDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMESTART);
	CDateTimeCtrl* theEndDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEEND);
	CEdit* theValue = (CEdit*)GetDlgItem(IDC_EDIT_VAL);

	// Format date and put in list control
	COleDateTime StartDateVal;
	COleDateTime EndDateVal;
	CString ValueText;
	theStartDate->GetTime(StartDateVal);
	theEndDate->GetTime(EndDateVal);
	theValue->GetWindowText(ValueText);
	
	//Validation
	bool InputValid = true;
	CString ErrorString;
	
	// Dates make sense?
	int i = (int)(((COleDateTimeSpan)(StartDateVal - EndDateVal)).GetTotalDays());
	if (((COleDateTimeSpan)(EndDateVal - StartDateVal)).GetTotalDays() < 0) 
	{
		InputValid = false;
		ErrorString = "Start Date is not before End Date";
		MyMessageBox(ErrorString);
	}
	
	
	if (InputValid) 
	{
		double val = atof(ValueText);
		InputValid = Validate(val);  // Validate the value if the dates appeared correct
	}
	
	if (InputValid)
	{
		// Add item 
		m_pDateRangeValues->AddItem(StartDateVal, EndDateVal, atof(ValueText));	
		UpdateSpanGrid();
	}

}
void CDlgSpanValue::OnDateValueEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	
	try
	{
		CString stgResults = m_DRV_GridCtrl.GetItemText(pItem->iRow,pItem->iColumn);
		if (pItem->iColumn == 1) // Start Date
		{
			COleDateTime theTime;
			theTime.ParseDateTime(stgResults);
			m_pDateRangeValues->GetItemPtr(pItem->iRow -1)->StartTime = theTime;
		}
			
		else if (pItem->iColumn == 2) // End Date
		{
			COleDateTime theTime;
			theTime.ParseDateTime(stgResults);
			m_pDateRangeValues->GetItemPtr(pItem->iRow -1)->EndTime = theTime;
		}
		else if (pItem->iColumn == 3) // Value
		{
			double Val = atof(stgResults);
			// Validate
			if (Validate(Val)) 
				m_pDateRangeValues->GetItemPtr(pItem->iRow-1)->Value = Val;
		   //FIX else m_DRV_GridCtrl.SetItem(m_pDateRangeValues->GetItemPtr(pItem->iRow));

		}
		*pResult = 0;
	}
	catch(CException* e)
	{
		*pResult = -1;
	}
	
  
}

void CDlgSpanValue::OnBnClickedButtonDelItem()
{
	for (int row = 1; row <= m_pDateRangeValues->GetCount(); row++)
	{
		if ((m_DRV_GridCtrl.IsCellSelected(row,1)) && (m_DRV_GridCtrl.IsCellSelected(row,2)) && (m_DRV_GridCtrl.IsCellSelected(row,3)))
		{
			m_pDateRangeValues->DeleteItem(row-1);
			UpdateSpanGrid();
			break;  // Currently only allow deletion of one row at a time
		}
	}
}

void CDlgSpanValue::SetUpperBoundVal(double UpperBound)
{
	m_UpperBound = UpperBound;
	m_UpperBoundSet = true;
}

void CDlgSpanValue::SetLowerBoundVal(double LowerBound)
{
	m_LowerBound = LowerBound;
	m_LowerBoundSet = true;
}


bool CDlgSpanValue::Validate(double Value, bool Message)
{
	bool InputValid = true;
	CString ErrorString;
	if (m_Validate) // Validate if upper and lower bounds are valid
	{   
		if ((Value  < m_LowerBound) || ( Value > m_UpperBound)) InputValid = false;
	}
	
	if ((!InputValid)  && (Message)) 
	{
		ErrorString.Format("%s value not between %4.1f and %4.1f", m_ValueTitle, m_LowerBound, m_UpperBound);
		MyMessageBox(ErrorString);
	}
	return InputValid;
}