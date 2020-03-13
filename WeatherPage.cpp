// WeatherPage.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "WeatherPage.h"
#include "MainFrm.h"


#define THISPAGENUMBER 1
// CWeatherPage dialog

IMPLEMENT_DYNAMIC(CWeatherPage, CPropertyPage)

CWeatherPage::CWeatherPage(CVarroaPopDoc* pDoc)
	: CPropertyPage(CWeatherPage::IDD)
	, m_WeatherFileName(_T(""))
{
	m_pDoc = pDoc;
}

CWeatherPage::~CWeatherPage()
{
}

BOOL CWeatherPage::OnInitDialog()
{
	m_WeatherFileName = m_pDoc->GetWeatherFileName();
	m_pMyPropSheet->SetLastPageNumber(THISPAGENUMBER);
	CDialog::OnInitDialog();
	return true;
}

void CWeatherPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_WF_NAME, m_WeatherFileName);
}


BEGIN_MESSAGE_MAP(CWeatherPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SEL_WEATHER, &CWeatherPage::OnBnClickedButtonSelWeather)
END_MESSAGE_MAP()


// CWeatherPage message handlers

void CWeatherPage::OnBnClickedButtonSelWeather()
{
	CFileDialog fd(TRUE,
					NULL,
					NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("All Weather Files (*.wth, *.dvf, *.wea)|*.wth;*.dvf;*.wea|EPA Weather Files (*.dvf, *.wea)|*.dvf; *.wea|All Files|*.*||"));
					
	if (fd.DoModal() == IDOK)
	{
		m_WeatherFileName = fd.GetPathName();
		UpdateData(FALSE);
		m_pDoc->LoadWeatherFile(m_WeatherFileName);
		m_pDoc->SetSimStart(m_pDoc->GetWeather()->GetBeginningTime());
		m_pDoc->SetSimEnd(m_pDoc->GetWeather()->GetEndingTime());
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls(); 
	}
}

void CWeatherPage::UpdateDocument()
{
	m_pDoc->SetWeatherFileName(m_WeatherFileName);
}

BOOL CWeatherPage::OnKillActive()
{
	UpdateDocument();
	return (CPropertyPage::OnKillActive());
}




