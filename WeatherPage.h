#pragma once
#include "MyPropSheet.h"
#include "VarroaPopDoc.h"


// CWeatherPage dialog

class CWeatherPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWeatherPage)

public:
	CWeatherPage(CVarroaPopDoc* pDoc);
	virtual ~CWeatherPage();
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}
	BOOL OnKillActive();
    BOOL CWeatherPage::OnInitDialog();
// Dialog Data
	enum { IDD = IDD_WEATHER_FORM1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateDocument();
	CMyPropSheet* m_pMyPropSheet;
	CVarroaPopDoc* m_pDoc;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSelWeather();
protected:
    CString m_WeatherFileName;

};
