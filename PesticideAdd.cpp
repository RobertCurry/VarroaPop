// PesticideAdd.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "PesticideAdd.h"


// CPesticideAdd dialog

IMPLEMENT_DYNAMIC(CPesticideAdd, CDialog)

CPesticideAdd::CPesticideAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CPesticideAdd::IDD, pParent)
	, m_Name(_T(""))
	, m_AdultSlope(0)
	, m_AdultLD50(0)
	, m_AdultSlope_Contact(0)
	, m_AdultLD50_Contact(0)
	, m_LarvaSlope(0)
	, m_LarvaLD50(0)
	, m_KOW(0)
	, m_KOC(0)
	, m_HalfLife(0)
	, m_ContactFactor(0)
{

}

CPesticideAdd::~CPesticideAdd()
{
}

void CPesticideAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_PESTICIDE_AD_SLOPE, m_AdultSlope);
	DDV_MinMaxDouble(pDX, m_AdultSlope, 1, 20);
	DDX_Text(pDX, IDC_PESTICIDE_AD_SLOPE7, m_AdultLD50);
	DDV_MinMaxDouble(pDX, m_AdultLD50, 0, 10);
	DDX_Text(pDX, IDC_PESTICIDE_AC_SLOPE2, m_AdultSlope_Contact);
	DDV_MinMaxDouble(pDX, m_AdultSlope_Contact, 1, 20);
	DDX_Text(pDX, IDC_PESTICIDE_AC_LD50_2, m_AdultLD50_Contact);
	DDV_MinMaxDouble(pDX, m_AdultLD50_Contact, 0, 10);
	DDX_Text(pDX, IDC_PESTICIDE_LARV_SLOPE, m_LarvaSlope);
	DDV_MinMaxDouble(pDX, m_LarvaSlope, 1, 20);
	DDX_Text(pDX, IDC_PESTICIDE_LV_LD, m_LarvaLD50);
	DDV_MinMaxDouble(pDX, m_LarvaLD50, 0, 10);
	DDX_Text(pDX, IDC_PESTICIDE_KOW, m_KOW);
	DDV_MinMaxDouble(pDX, m_KOW, -.001, 10000000);
	DDX_Text(pDX, IDC_PESTICIDE_KOC, m_KOC);
	DDV_MinMaxDouble(pDX, m_KOC, .001, 10000000);
	DDX_Text(pDX, IDC_PESTICIDE_HL, m_HalfLife);
	DDV_MinMaxDouble(pDX, m_HalfLife, .01, 50);
	DDX_Text(pDX, IDC_PESTICIDE_CF, m_ContactFactor);
	DDV_MinMaxDouble(pDX, m_ContactFactor, 0, 10);
}


BEGIN_MESSAGE_MAP(CPesticideAdd, CDialog)
END_MESSAGE_MAP()


// CPesticideAdd message handlers
