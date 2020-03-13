// CreateWeatherHdr.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "CreateWeatherHdr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateWeatherHdr dialog


CCreateWeatherHdr::CCreateWeatherHdr(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateWeatherHdr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateWeatherHdr)
	m_AveTempCol = 2;
	m_Interval = 60;
	m_MaxTempCol = 3;
	m_MinTempCol = 4;
	m_Farenheight = 0;
	m_RainCol = 5;
	m_SimDate = _T("5/1/1999");
	m_SimTime = _T("12:00");
	m_WBeginDate = _T("5/1/1999");
	m_WBeginTime = _T("12:00");
	m_WEndDate = _T("5/30/1999");
	m_WEndTime = _T("12:00");
	m_WindspeedCol = 6;
	m_Hourly = 1;
	m_SolarRad = 0;
	//}}AFX_DATA_INIT
}


void CCreateWeatherHdr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateWeatherHdr)
	DDX_Text(pDX, IDC_AVTEMP, m_AveTempCol);
	DDV_MinMaxInt(pDX, m_AveTempCol, 2, 32);
	DDX_Text(pDX, IDC_INTERVAL, m_Interval);
	DDV_MinMaxInt(pDX, m_Interval, 1, 1600);
	DDX_Text(pDX, IDC_MAXTEMP, m_MaxTempCol);
	DDV_MinMaxInt(pDX, m_MaxTempCol, 2, 32);
	DDX_Text(pDX, IDC_MINTEMP, m_MinTempCol);
	DDV_MinMaxInt(pDX, m_MinTempCol, 2, 32);
	DDX_Radio(pDX, IDC_RADIO_F, m_Farenheight);
	DDX_Text(pDX, IDC_RAINFALL, m_RainCol);
	DDV_MinMaxInt(pDX, m_RainCol, 2, 32);
	DDX_Text(pDX, IDC_SIM_DATE, m_SimDate);
	DDX_Text(pDX, IDC_SIM_TIME, m_SimTime);
	DDX_Text(pDX, IDC_W_BEG_DATE, m_WBeginDate);
	DDX_Text(pDX, IDC_W_BEG_TIME, m_WBeginTime);
	DDX_Text(pDX, IDC_W_END_DATE, m_WEndDate);
	DDX_Text(pDX, IDC_W_END_TIME, m_WEndTime);
	DDX_Text(pDX, IDC_WINDSPEED, m_WindspeedCol);
	DDV_MinMaxInt(pDX, m_WindspeedCol, 2, 32);
	DDX_Radio(pDX, IDC_RADIOMINMAX, m_Hourly);
	DDX_Text(pDX, IDC_SOLAR_RAD, m_SolarRad);
	DDV_MinMaxInt(pDX, m_SolarRad, 2, 32);
	//}}AFX_DATA_MAP
}



/////////////////////////////////////////////////////////////////////////////
//  CCreateWeatherHdr operations

void CCreateWeatherHdr::GetHeaderStrings(CStringArray& Strings, CWeatherFile* pWeather)
{

	CString temp;
	Strings.RemoveAll();
	Strings.Add(CString("WEATHER_FILE"));
	Strings.Add(CString("Temperature Scale . . . . . . . . .  = ")+CString(m_Farenheight?"C":"F"));
	Strings.Add(CString("Weather File Name . . . . . . . . .  = ")+pWeather->GetFileName());
	Strings.Add(CString("Format of the Weather File  . . . .  = ")+CString(m_Hourly?"HOURLY":"MINMAX"));
	if (!m_Hourly)  // MINMAX FORMAT ONLY
	{
		Strings.Add(CString("Begin Time of the Weather . . . . .  = ")+m_WBeginTime);
		Strings.Add(CString("Begin Date of the Weather . . . . .  = ")+m_WBeginDate);
		Strings.Add(CString("Ending Time of the Weather  . . . .  = ")+m_WEndTime);
		Strings.Add(CString("Ending Date of the Weather  . . . .  = ")+m_WEndDate);
		Strings.Add(CString("Start Time of the Simulation  . . .  = ")+m_SimTime);
		Strings.Add(CString("Start Date of the Simulation  . . .  = ")+m_SimDate);
	}
	temp.Format("%d",m_AveTempCol);
	Strings.Add(CString("Column # of the Temperature . . . .  = ")+temp);
	if (!m_Hourly)  // MINMAX FORMAT ONLY
	{
		temp.Format("%d",m_MaxTempCol);
		Strings.Add(CString("Column # of the Max Temperature . .  = ")+temp);
		temp.Format("%d",m_MinTempCol);
		Strings.Add(CString("Column # of the Min Temperature . .  = ")+temp);
	}
	temp.Format("%d",m_SolarRad);
	Strings.Add(CString("Column # of the Daylight . . . . . . = ")+temp);
	temp.Format("%d",m_WindspeedCol);
	Strings.Add(CString("Column # of the Wind Speed  . . . .  = ")+temp);
	temp.Format("%d",m_RainCol);
	Strings.Add(CString("Column # of the Rain  . . . . . . .  = ")+temp);
	temp.Format("%d",m_Interval);
	Strings.Add(CString("Interval Between Readings . . . . .  = ")+temp);
	Strings.Add(CString("----------begin---------"));
}



BEGIN_MESSAGE_MAP(CCreateWeatherHdr, CDialog)
	//{{AFX_MSG_MAP(CCreateWeatherHdr)
	ON_BN_CLICKED(IDC_RADIOMINMAX, OnRadiominmax)
	ON_BN_CLICKED(IDC_RADIOHOURLY, OnRadiohourly)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateWeatherHdr message handlers

void CCreateWeatherHdr::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CCreateWeatherHdr::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CCreateWeatherHdr::OnRadiominmax() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_MINTEMP)->EnableWindow(true);
	GetDlgItem(IDC_MAXTEMP)->EnableWindow(true);
	GetDlgItem(IDC_W_BEG_TIME)->EnableWindow(true);
	GetDlgItem(IDC_W_END_TIME)->EnableWindow(true);
	GetDlgItem(IDC_W_BEG_DATE)->EnableWindow(true);
	GetDlgItem(IDC_W_END_DATE)->EnableWindow(true);
	GetDlgItem(IDC_SIM_TIME)->EnableWindow(true);
	GetDlgItem(IDC_SIM_DATE)->EnableWindow(true);
	GetDlgItem(IDC_SOLAR_RAD)->EnableWindow(true);
	m_Interval = 1440;
	m_Hourly = 0;
	UpdateData(FALSE);
	
}

void CCreateWeatherHdr::OnRadiohourly() 
{
	// TODO: Add your control notification handler code here

	// Disable some dialog windows,  Enable Radiation window.
	GetDlgItem(IDC_MINTEMP)->EnableWindow(false);
	GetDlgItem(IDC_MAXTEMP)->EnableWindow(false);
	GetDlgItem(IDC_W_BEG_TIME)->EnableWindow(false);
	GetDlgItem(IDC_W_END_TIME)->EnableWindow(false);
	GetDlgItem(IDC_W_BEG_DATE)->EnableWindow(false);
	GetDlgItem(IDC_W_END_DATE)->EnableWindow(false);
	GetDlgItem(IDC_SIM_TIME)->EnableWindow(false);
	GetDlgItem(IDC_SIM_DATE)->EnableWindow(false);
	GetDlgItem(IDC_SOLAR_RAD)->EnableWindow(true);
	m_Interval = 60;
	m_Hourly = 1;
	UpdateData(FALSE);
	
}
