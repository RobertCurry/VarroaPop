// Options.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptions dialog


COptions::COptions(CVarroaPopDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(COptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptions)
	m_ColTitles = FALSE;
	m_InitConds = FALSE;
	m_Version = FALSE;
	m_WeatherColony = FALSE;
	m_FieldDelimiter = -1;
	m_DispFrequency = -1;
	//}}AFX_DATA_INIT
	m_pDoc = pDoc;
}


void COptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptions)
	DDX_Check(pDX, IDC_CHECKCOLTITLES, m_ColTitles);
	DDX_Check(pDX, IDC_CHECKINITCONDS, m_InitConds);
	DDX_Check(pDX, IDC_CHECKVERSION, m_Version);
	DDX_Check(pDX, IDC_CHECKWTHCOL, m_WeatherColony);
	DDX_Radio(pDX, IDC_RADIOFIELDFIXED, m_FieldDelimiter);
	DDX_Radio(pDX, IDC_RADIOWEEKLY, m_DispFrequency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
	//{{AFX_MSG_MAP(COptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptions message handlers

BOOL COptions::OnInitDialog() 
{
	
	m_ColTitles = m_pDoc->m_ColTitles;
	m_InitConds = m_pDoc->m_InitConds;
	m_Version = m_pDoc->m_Version;
	m_WeatherColony = m_pDoc->m_WeatherColony;
	m_FieldDelimiter = m_pDoc->m_FieldDelimiter;
	m_DispFrequency = m_pDoc->m_DispWeeklyData?0:1;
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptions::OnOK() 
{
	CDialog::OnOK();
	m_pDoc->m_ColTitles = m_ColTitles;
	m_pDoc->m_InitConds = m_InitConds;
	m_pDoc->m_Version = m_Version;
	m_pDoc->m_WeatherColony = m_WeatherColony;
	m_pDoc->m_FieldDelimiter = m_FieldDelimiter;
	m_pDoc->m_DispWeeklyData = (m_DispFrequency==0);
	
}
