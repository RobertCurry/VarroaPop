// SplashScreen.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "SplashScreen.h"
#include <afxctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen dialog


CSplashScreen::CSplashScreen(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashScreen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashScreen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CSplashScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashScreen)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashScreen, CDialog)
	//{{AFX_MSG_MAP(CSplashScreen)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen message handlers

void CSplashScreen::Display()
{
	Create(IDD);
	m_startTime = CTime::GetCurrentTime();

	/*
	CBitmap theBitmap;
	theBitmap.LoadBitmap(IDB_SPLASH);
	BITMAP BMData;
	BOOL Success = theBitmap.GetBitmap(&BMData);
	//  bmWidth and bmHeight attributes of pBMData 

	MoveWindow(0,0,BMData.bmWidth,BMData.bmHeight);
	CPictureHolder* theDlgBM = (CPictureHolder*)GetDlgItem(IDC_SPLASH_CONT);

	theDlgBM->CreateFromBitmap(IDB_SPLASH);
	ShowWindow(SW_SHOW);
//	theDlgBM->ShowWindow(SW_SHOW);

  */



}

void CSplashScreen::Kill()
{
	delete this;
}

void CSplashScreen::KillAfter(int Seconds)
{
	m_endTime = CTime::GetCurrentTime();
	m_elapsedTime = m_endTime - m_startTime;
	while (m_elapsedTime.GetSeconds() < Seconds) 
	{
		m_endTime = CTime::GetCurrentTime();
		m_elapsedTime = m_endTime - m_startTime;
	}
	Kill();
}

