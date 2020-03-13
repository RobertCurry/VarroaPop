// SimDlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "SimDlgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimDlgBar dialog


CSimDlgBar::CSimDlgBar(CWnd* pParent /*=NULL*/)
	: CDialog(CSimDlgBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimDlgBar)
	m_FAge = 0;
	m_Sperm = 0;
	m_SimStart = _T("");
	m_SImStop = _T("");
	//}}AFX_DATA_INIT
}


void CSimDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimDlgBar)
	DDX_Text(pDX, IDC_EDIT_F_AGE, m_FAge);
	DDV_MinMaxInt(pDX, m_FAge, 4, 16);
	DDX_Text(pDX, IDC_EDIT_SPERM, m_Sperm);
	DDX_Text(pDX, IDC_EDIT_START, m_SimStart);
	DDX_Text(pDX, IDC_EDIT_STOP, m_SImStop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimDlgBar, CDialog)
	//{{AFX_MSG_MAP(CSimDlgBar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimDlgBar message handlers
