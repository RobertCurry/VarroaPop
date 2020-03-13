// WeatherEditor.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "WeatherEvents.h"
#include "WeatherEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeatherEditor dialog


CWeatherEditor::CWeatherEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CWeatherEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWeatherEditor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Modified = false;
	m_pWeather = new CWeatherEvents;
}

CWeatherEditor::CWeatherEditor(CWeatherEvents* pWeather, CWnd* pParent)
	: CDialog(CWeatherEditor::IDD, pParent)
{
	m_Modified = false;
	m_pWeather = pWeather;
}


CWeatherEditor::~CWeatherEditor()
{
}

void CWeatherEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeatherEditor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeatherEditor, CDialog)
	//{{AFX_MSG_MAP(CWeatherEditor)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeatherEditor message handlers

void CWeatherEditor::OnViewZoomin() 
{
	AfxMessageBox("Zooming IN");
	
}

void CWeatherEditor::OnViewZoomout() 
{
	AfxMessageBox("Zooming OUT");
	
}


bool CWeatherEditor::IsModified()
{
	return m_Modified;
}

CWeatherEvents* CWeatherEditor::GetWeather()
{
	return m_pWeather;
}

void CWeatherEditor::SetWeather(CWeatherEvents *pWeather)
{
	m_pWeather = pWeather;
}

BOOL CWeatherEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

