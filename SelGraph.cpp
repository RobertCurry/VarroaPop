// SelGraph.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "SelGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelGraph dialog


CSelGraph::CSelGraph(CVarroaPopDoc* theDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CSelGraph::IDD, pParent)
{
	pDoc = theDoc;
	//{{AFX_DATA_INIT(CSelGraph)
	m_TM = FALSE;
	m_AD = TRUE;
	m_AW = TRUE;
	m_CS = TRUE;
	m_DB = FALSE;
	m_DE = FALSE;
	m_DL = FALSE;
	m_F = FALSE;
	m_MDB = FALSE;
	m_MWB = FALSE;
	m_PDB = FALSE;
	m_PWB = FALSE;
	m_PRM = FALSE;
	m_RM = FALSE;
	m_WB = FALSE;
	m_WE = FALSE;
	m_WL = FALSE;
	m_MD = FALSE;
	m_PMD = FALSE;
	m_NS = FALSE;
	m_PS = FALSE;
	m_NPC = FALSE;
	m_PPC = FALSE;
	//pDoc->m_AutoScaleChart = 0;
	////pDoc->m_RadioAutoScale = 0;
	//pDoc->m_YAxisMax = 0;
	//pDoc->m_YAxisMin = 0;
	//}}AFX_DATA_INIT
}


void CSelGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelGraph)
	DDX_Check(pDX, IDC_CHECK_TM, pDoc->m_TM);
	DDX_Check(pDX, IDC_CHECK_AD, pDoc->m_AD);
	DDX_Check(pDX, IDC_CHECK_AW, pDoc->m_AW);
	DDX_Check(pDX, IDC_CHECK_CS, pDoc->m_CS);
	DDX_Check(pDX, IDC_CHECK_DB, pDoc->m_DB);
	DDX_Check(pDX, IDC_CHECK_DE, pDoc->m_DE);
	DDX_Check(pDX, IDC_CHECK_DL, pDoc->m_DL);
	DDX_Check(pDX, IDC_CHECK_F, pDoc->m_F);
	DDX_Check(pDX, IDC_CHECK_MDB, pDoc->m_MDB);
	DDX_Check(pDX, IDC_CHECK_MWB, pDoc->m_MWB);
	DDX_Check(pDX, IDC_CHECK_PDB, pDoc->m_PDB);
	DDX_Check(pDX, IDC_CHECK_PWB, pDoc->m_PWB);
	DDX_Check(pDX, IDC_CHECK_PRM, pDoc->m_PRM);
	DDX_Check(pDX, IDC_CHECK_RM, pDoc->m_RM);
	DDX_Check(pDX, IDC_CHECK_WB, pDoc->m_WB);
	DDX_Check(pDX, IDC_CHECK_WE, pDoc->m_WE);
	DDX_Check(pDX, IDC_CHECK_WL, pDoc->m_WL);
	DDX_Check(pDX, IDC_CHECK_IM, pDoc->m_IM);
	DDX_Check(pDX, IDC_CHECK_MD, pDoc->m_MD);
	DDX_Check(pDX, IDC_CHECK_PMD, pDoc->m_PMD);
	DDX_Check(pDX, IDC_CHECK_NS, pDoc->m_NS);
	DDX_Check(pDX, IDC_CHECK_PS, pDoc->m_PS);
	DDX_Check(pDX, IDC_CHECK_NCONC, pDoc->m_NPC);
	DDX_Check(pDX, IDC_CHECK_PCONC, pDoc->m_PPC);
	DDX_Check(pDX, IDC_CHECK_DDL, pDoc->m_DDL);
	DDX_Check(pDX, IDC_CHECK_DWL, pDoc->m_DWL);
	DDX_Check(pDX, IDC_CHECK_DDA, pDoc->m_DDA);
	DDX_Check(pDX, IDC_CHECK_DWA, pDoc->m_DWA);
	DDX_Check(pDX, IDC_CHECK_DFG, pDoc->m_DFG);
	DDX_Text(pDX, IDC_EDIT_MAX, pDoc->m_YAxisMax);
	DDX_Text(pDX, IDC_EDIT_MIN, pDoc->m_YAxisMin);
	DDX_Radio(pDX, IDC_RADIO_AUTO, pDoc->m_AutoScaleChart);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CSelGraph, CDialog)
	//{{AFX_MSG_MAP(CSelGraph)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_AUTO, &CSelGraph::OnRadioScaleAuto)
	ON_BN_CLICKED(IDC_RADIO_MANUAL, &CSelGraph::OnRadioScaleManual)
	ON_BN_CLICKED(IDOK, &CSelGraph::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


void CSelGraph::OnRadioScaleAuto()
{
	SetAutoScaleChart(true);
}

void CSelGraph::OnRadioScaleManual()
{
	SetAutoScaleChart(false);
}

void CSelGraph::SetAutoScaleChart(bool bAuto)
{
	pDoc->m_AutoScaleChart = bAuto?0:1;

	(CStatic*)GetDlgItem(IDC_STATICMAX)->EnableWindow(!pDoc->m_AutoScaleChart==0);
	(CStatic*)GetDlgItem(IDC_STATICMIN)->EnableWindow(!pDoc->m_AutoScaleChart==0);
	(CEdit*)GetDlgItem(IDC_EDIT_MAX)->EnableWindow(!pDoc->m_AutoScaleChart==0);
	(CEdit*)GetDlgItem(IDC_EDIT_MIN)->EnableWindow(!pDoc->m_AutoScaleChart==0);

}


BOOL CSelGraph::OnInitDialog()
{
	CDialog::OnInitDialog();

	//UpdateData(false);  // Load dialog box with variables

	SetAutoScaleChart(pDoc->m_AutoScaleChart == 0); //If AutoScale Radio button m_AutoScaleChart == 0.


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelGraph::OnBnClickedOk()
{
	UpdateData(true);
	OnOK();
}

void CSelGraph::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
