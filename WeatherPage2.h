#pragma once
#include "MyPropsheet.h"

/////////////////////////////////////////////////////////////////////////////
// CWeatherPage2 dialog

class CWeatherPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CWeatherPage2)

protected:
    CMyPropSheet* m_pMyPropSheet;
    CString m_WeatherFileName;


public:
	CWeatherPage2();   // standard constructor
	virtual ~CWeatherPage2();
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}
	void SetWeatherFileName(CString fname) {m_WeatherFileName = fname;}

// Dialog Data
	enum { IDD = IDD_WEATHER_FORM2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSelWeather();

};
