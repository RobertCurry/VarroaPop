// ImmigrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include <afxdtctl.h>
#include "varroapop.h"
#include "VarroaPopDoc.h"
#include "ImmigrationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImmigrationDlg dialog



CImmigrationDlg::CImmigrationDlg(CString ImType, CWnd* pParent /*=NULL*/)
	: CDialog(CImmigrationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImmigrationDlg)
	m_MiteNum = 0;
	m_ImmigrationEnd = COleDateTime::GetCurrentTime();
	m_ImmigrationStart = COleDateTime::GetCurrentTime();
	m_MitePctResistant = 0.0;
	//}}AFX_DATA_INIT
	m_ImmigrationType = ImType;
	m_pDoc = NULL;
}


void CImmigrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImmigrationDlg)
	DDX_Text(pDX, IDC_MITE_NUM, m_MiteNum);
	DDX_DateTimeCtrl(pDX, IDC_IM_END, m_ImmigrationEnd);
	DDX_DateTimeCtrl(pDX, IDC_IM_START, m_ImmigrationStart);
	DDX_Text(pDX, IDC_MITE_PCT_RESISTANT, m_MitePctResistant);
	DDV_MinMaxDouble(pDX, m_MitePctResistant, 0., 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImmigrationDlg, CDialog)
	//{{AFX_MSG_MAP(CImmigrationDlg)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_IM_START, OnDatetimechangeImStart)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_IM_END, OnDatetimechangeImEnd)
	ON_BN_CLICKED(IDC_RADIOCOS, OnGraphcos)
	ON_BN_CLICKED(IDC_RADIOEXP, OnGraphexp)
	ON_BN_CLICKED(IDC_RADIOLOG, OnGraphlog)
	ON_BN_CLICKED(IDC_RADIOPOLY, OnGraphpoly)
	ON_BN_CLICKED(IDC_RADIOSIN, OnGraphsin)
	ON_BN_CLICKED(IDC_RADIOTAN, OnGraphtan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





void CImmigrationDlg::GetDataFromDoc()
{
	ASSERT(m_pDoc);
	m_ImmigrationType = m_pDoc->GetImmigrationType();
	m_MiteNum = m_pDoc->GetNumImmigrationMites();
	m_MitePctResistant = m_pDoc->GetImmPctResistant();
	m_ImmigrationEnd = m_pDoc->GetImmigrationEnd();
	m_ImmigrationStart = m_pDoc->GetImmigrationStart();
	m_Enabled = m_pDoc->IsImmigrationEnabled();
}

void CImmigrationDlg::SendDataToDoc()
{
	// Move all dialog data to the CVarroaPop Document.  Check each
	// Piece of data and if it has changed, note the document has changed
	ASSERT(m_pDoc);
	if (m_pDoc->GetImmigrationType() != GetImmigrationType()) 
	{
		m_pDoc->SetModifiedFlag();
		m_pDoc->SetImmigrationType(GetImmigrationType());
	}
	if (m_pDoc->GetNumImmigrationMites()!=GetImmigrationMiteNum())
	{
		m_pDoc->SetModifiedFlag();
		m_pDoc->SetNumImmigrationMites(GetImmigrationMiteNum());
	}
	if (m_pDoc->GetImmPctResistant()!=int(m_MitePctResistant))
	{
		m_pDoc->SetModifiedFlag();
		m_pDoc->SetImmPctResistant(m_MitePctResistant);
	}
	CString stg1 = m_pDoc->GetImmigrationEnd().Format("%m/%d/%Y");
	CString stg2 = GetImmigrationStop().Format("%m/%d/%Y");
	if (m_pDoc->GetImmigrationEnd().Format("%m/%d/%Y")!=
		GetImmigrationStop().Format("%m/%d/%Y"))
	{
		m_pDoc->SetModifiedFlag();
		m_pDoc->SetImmigrationEnd(GetImmigrationStop());
	}
	if (m_pDoc->GetImmigrationStart().Format("%m/%d/%Y")!= 
		GetImmigrationStart().Format("%m/%d/%Y"))
	{
		m_pDoc->SetModifiedFlag();
		m_pDoc->SetImmigrationStart(GetImmigrationStart());
	}
	if (m_Enabled != m_pDoc->IsImmigrationEnabled())
	{
		m_pDoc->SetModifiedFlag();
		m_pDoc->SetImmigrationEnabled(m_Enabled);
	}

}
/////////////////////////////////////////////////////////////////////////////
// CImmigrationDlg message handlers
BOOL  CImmigrationDlg::OnInitDialog()
{

	CBitmapButton* pButtonSin = (CBitmapButton*)GetDlgItem(IDC_RADIOSIN);
	pButtonSin->SetIcon(AfxGetApp()->LoadIcon(IDI_SIN));

	CBitmapButton* pButtonCos = (CBitmapButton*)GetDlgItem(IDC_RADIOCOS);
	pButtonCos->SetIcon(AfxGetApp()->LoadIcon(IDI_COS));

	CBitmapButton* pButtonTan = (CBitmapButton*)GetDlgItem(IDC_RADIOTAN);
	pButtonTan->SetIcon(AfxGetApp()->LoadIcon(IDI_TAN));

	CBitmapButton* pButtonPoly = (CBitmapButton*)GetDlgItem(IDC_RADIOPOLY);
	pButtonPoly->SetIcon(AfxGetApp()->LoadIcon(IDI_POLY));

	CBitmapButton* pButtonExp = (CBitmapButton*)GetDlgItem(IDC_RADIOEXP);
	pButtonExp->SetIcon(AfxGetApp()->LoadIcon(IDI_EXP));

	CBitmapButton* pButtonLog = (CBitmapButton*)GetDlgItem(IDC_RADIOLOG);
	pButtonLog->SetIcon(AfxGetApp()->LoadIcon(IDI_LOG));

	if (m_ImmigrationType.MakeUpper() == "COSINE") pButtonCos->SetCheck(1); 
	else if (m_ImmigrationType.MakeUpper() == "SINE") pButtonSin->SetCheck(1); 
	else if (m_ImmigrationType.MakeUpper() == "TANGENT") pButtonTan->SetCheck(1); 
	else if (m_ImmigrationType.MakeUpper() == "POLYNOMIAL") pButtonPoly->SetCheck(1); 
	else if (m_ImmigrationType.MakeUpper() == "EXPONENTIAL") pButtonExp->SetCheck(1); 
	else if (m_ImmigrationType.MakeUpper() == "LOGARITHMIC") pButtonLog->SetCheck(1);


	CStatic* pWin = (CStatic*)GetDlgItem(IDC_IM_SELECTED);
	CDateTimeCtrl* StartCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_IM_START);
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_IM_END);
//	StartCtrl->SetRange(&m_pDoc->GetSimStart(),&m_pDoc->GetSimEnd());
//	EndCtrl->SetRange(&m_pDoc->GetSimStart(),&m_pDoc->GetSimEnd());
	StartCtrl->SetRange(&m_pDoc->GetWeather()->GetBeginningTime(),
		&m_pDoc->GetWeather()->GetEndingTime());
	EndCtrl->SetRange(&m_pDoc->GetWeather()->GetBeginningTime(),
		&m_pDoc->GetWeather()->GetEndingTime());
	StartCtrl->SetTime(m_pDoc->GetWeather()->GetBeginningTime());
	EndCtrl->SetTime(m_pDoc->GetWeather()->GetEndingTime());
	pWin->SetWindowText(m_ImmigrationType);
	UpdateData(false);
	return true;

}



void CImmigrationDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


void CImmigrationDlg::OnGraphcos() 
{
	// TODO: Add your control notification handler code here
	m_ImmigrationType.MakeUpper() = "COSINE";
	CStatic* pWin = (CStatic*)GetDlgItem(IDC_IM_SELECTED);
	pWin->SetWindowText(m_ImmigrationType);
	
	
}

void CImmigrationDlg::OnGraphexp() 
{
	// TODO: Add your control notification handler code here
	m_ImmigrationType.MakeUpper() = "EXPONENTIAL";
	CStatic* pWin = (CStatic*)GetDlgItem(IDC_IM_SELECTED);
	pWin->SetWindowText(m_ImmigrationType);
	
	
}

void CImmigrationDlg::OnGraphlog() 
{
	// TODO: Add your control notification handler code here
	m_ImmigrationType.MakeUpper() = "LOGARITHMIC";
	CStatic* pWin = (CStatic*)GetDlgItem(IDC_IM_SELECTED);
	pWin->SetWindowText(m_ImmigrationType);
	
	
}

void CImmigrationDlg::OnGraphpoly() 
{
	// TODO: Add your control notification handler code here
	m_ImmigrationType.MakeUpper() = "POLYNOMIAL";
	CStatic* pWin = (CStatic*)GetDlgItem(IDC_IM_SELECTED);
	pWin->SetWindowText(m_ImmigrationType);
	
	
}

void CImmigrationDlg::OnGraphsin() 
{
	// TODO: Add your control notification handler code here
	m_ImmigrationType.MakeUpper() = "SINE";
	CStatic* pWin = (CStatic*)GetDlgItem(IDC_IM_SELECTED);
	pWin->SetWindowText(m_ImmigrationType);
	
	
}

void CImmigrationDlg::OnGraphtan() 
{
	// TODO: Add your control notification handler code here
	m_ImmigrationType.MakeUpper() = "TANGENT";
	CStatic* pWin = (CStatic*)GetDlgItem(IDC_IM_SELECTED);
	pWin->SetWindowText(m_ImmigrationType);
	
	
}

void CImmigrationDlg::OnDatetimechangeImStart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_IM_END);
	CDateTimeCtrl* StartCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_IM_START);
	COleDateTime StartTime;
	COleDateTime EndTime;
	StartCtrl->GetTime(StartTime);
	EndCtrl->GetTime(EndTime);
	if (StartTime>EndTime) EndCtrl->SetTime(StartTime);
	*pResult = 0;
}

void CImmigrationDlg::OnDatetimechangeImEnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDateTimeCtrl* EndCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_IM_END);
	CDateTimeCtrl* StartCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_IM_START);
	COleDateTime EndTime;
	COleDateTime StartTime;
	EndCtrl->GetTime(EndTime);
	StartCtrl->GetTime(StartTime);
	if (EndTime < StartTime) StartCtrl->SetTime(EndTime);
	*pResult = 0;
}
