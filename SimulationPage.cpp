// SimulationPage.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "SimulationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimulationPage property page

IMPLEMENT_DYNCREATE(CSimulationPage, CPropertyPage)

CSimulationPage::CSimulationPage() : CPropertyPage(CSimulationPage::IDD)
{
	m_pMyPropSheet = NULL;
	m_pSelectedFilesList = NULL;
	//{{AFX_DATA_INIT(CSimulationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSimulationPage::~CSimulationPage()
{
}

void CSimulationPage::setPropSheet(CPropertySheet* theSheet) {
	m_pMyPropSheet = theSheet;
}

void CSimulationPage::setSelectedFilesList(CStringList* theList) {
	m_pSelectedFilesList = theList;
}

void CSimulationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimulationPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimulationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSimulationPage)
	ON_BN_CLICKED(IDC_SIM_HIDE_BUTTON, OnHideButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulationPage message handlers

void CSimulationPage::OnHideButton() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pMyPropSheet);
	m_pMyPropSheet->ShowWindow(SW_HIDE);
}
