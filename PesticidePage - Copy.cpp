// PesticidePage.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "PesticidePage.h"
#include "PesticideAdd.h"


// CPesticidePage dialog

//IMPLEMENT_DYNAMIC(CPesticidePage, CDialog)

CPesticidePage::CPesticidePage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CPesticidePage::IDD)
	, m_AI_AdultLD50(0)
	, m_AI_LarvaSlope(0)
	, m_AI_LarvaLD50(0)
	, m_AI_KOW(0)
	, m_AI_KOC(0)
	, m_C_Larv4_Pollen(0)
	, m_C_Larv4_Nectar(0)
	, m_C_Larva5_Pollen(0)
{

}

CPesticidePage::~CPesticidePage()
{
}

void CPesticidePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPesticidePage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPesticidePage::OnBnClickedButtonAddPesticide)
	ON_BN_CLICKED(IDC_BUTTON2, &CPesticidePage::OnBnClickedButtonUpdatePesticide)
END_MESSAGE_MAP()


// CPesticidePage message handlers

void CPesticidePage::OnBnClickedButtonAddPesticide()
{
	CPesticideAdd PesticideAddDialog;

	if (PesticideAddDialog.DoModal()== IDOK)
	{
		int i = 0;
	}
}

void CPesticidePage::OnBnClickedButtonUpdatePesticide()
{
	this->UpdateData(1);
}
