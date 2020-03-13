// ResourcePage.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "ResourcePage.h"

#define THISPAGENUMBER 4

// CResourcePage dialog

IMPLEMENT_DYNAMIC(CResourcePage, CPropertyPage)

CResourcePage::CResourcePage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CResourcePage::IDD)
{

}

CResourcePage::~CResourcePage()
{
}

void CResourcePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_PolFeedingDatesCtrl);
}


BEGIN_MESSAGE_MAP(CResourcePage, CPropertyPage)
END_MESSAGE_MAP()


// CResourcePage message handlers
