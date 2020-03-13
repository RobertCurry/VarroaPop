// SimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "SimDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimDlg dialog


CSimDlg::CSimDlg(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CSimDlg)
	m_ImmigrationOn = -1;
	m_SimStart = _T("");
	m_SimStop = _T("");
	//}}AFX_DATA_INIT
}


void CSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimDlg)
	DDX_Radio(pDX, IDC_RADIO_IM_ON, m_ImmigrationOn);
	DDX_Text(pDX, IDC_EDIT_START, m_SimStart);
	DDX_Text(pDX, IDC_EDIT_STOP, m_SimStop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimDlg, CDialogBar)
	//{{AFX_MSG_MAP(CSimDlg)
	ON_BN_CLICKED(IDC_BUTTON_GO, OnButtonGo)
	ON_BN_CLICKED(IDC_BUTTON_IM_SETUP, OnButtonImSetup)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_START, OnDeltaposSpinStart)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STOP, OnDeltaposSpinStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimDlg message handlers

void CSimDlg::OnButtonGo() 
{
	// TODO: Add your control notification handler code here
	
}

void CSimDlg::OnButtonImSetup() 
{
	// TODO: Add your control notification handler code here
	
}

void CSimDlg::OnDeltaposSpinStart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CSimDlg::OnDeltaposSpinStop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}


void CMainFrame::OnRadioImOff() 
{
	// TODO: Add your control notification handler code here
	CButton* ImOn = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_ON);
	CButton* ImOff = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_OFF);
	ImOn->SetCheck(0);
	ImOff->SetCheck(1);
	m_EnableImmigration = false;
	
}

void CMainFrame::OnRadioImOn() 
{
	// TODO: Add your control notification handler code here
	CButton* ImOn = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_ON);
	CButton* ImOff = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_OFF);
	ImOn->SetCheck(1);
	ImOff->SetCheck(0);
	m_EnableImmigration = true;
	
}
