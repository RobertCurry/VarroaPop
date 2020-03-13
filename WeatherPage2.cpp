// WeatherPage2.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "WeatherPage2.h"


// CWeatherPage2 dialog

IMPLEMENT_DYNAMIC(CWeatherPage2, CDialog)

CWeatherPage2::CWeatherPage2() : CPropertyPage(CWeatherPage2::IDD)
, m_WeatherFileName(_T(""))
{

}

CWeatherPage2::~CWeatherPage2()
{
}

void CWeatherPage2::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_WFNAME, m_WeatherFileName);
}


BEGIN_MESSAGE_MAP(CWeatherPage2, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_SEL_WEATHER, &CWeatherPage2::OnBnClickedButtonSelWeather)
END_MESSAGE_MAP()


// CWeatherPage2 message handlers

void CWeatherPage2::OnBnClickedButtonSelWeather()
{
    CFileDialog theFileSelection(TRUE,
					".wth",
					NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("Weather Files (*.wth)|*.wth|All Files|*.*||"));
    
    
    if (theFileSelection.DoModal() == IDOK)
    {
    
    }
}
