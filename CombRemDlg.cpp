// CombRemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "CombRemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCombRemDlg dialog


CCombRemDlg::CCombRemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCombRemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCombRemDlg)
	m_RemovalDate = COleDateTime();
	m_IsEnabled = FALSE;
	m_Percent = 0.0;
	//}}AFX_DATA_INIT
}


void CCombRemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCombRemDlg)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_RemovalDate);
	DDX_Check(pDX, IDC_REMCOMBEN, m_IsEnabled);
	DDX_Text(pDX, IDC_PERCENT, m_Percent);
	DDV_MinMaxDouble(pDX, m_Percent, 0., 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCombRemDlg, CDialog)
	//{{AFX_MSG_MAP(CCombRemDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCombRemDlg message handlers
