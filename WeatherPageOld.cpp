// WeatherPage.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "VarroaPopDoc.h"
#include "VarroaPopView.h"
#include "WeatherPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeatherPage property page

IMPLEMENT_DYNCREATE(CWeatherPage, CPropertyPage)

CWeatherPage::CWeatherPage() : CPropertyPage(CWeatherPage::IDD)
{
	m_pMyPropSheet = NULL;
	m_WeatherFilesList.RemoveAll();
	m_WeatherFileName = "";
	m_Dir = "";
	//{{AFX_DATA_INIT(CWeatherPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWeatherPage::~CWeatherPage()
{
}



void CWeatherPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeatherPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeatherPage, CPropertyPage)
	//{{AFX_MSG_MAP(CWeatherPage)
	ON_BN_CLICKED(IDC_WTH_HIDE_BUTTON, OnHideButton)
	ON_CBN_SELCHANGE(IDC_WEATHER_FILE_LIST, OnSelchangeWeatherFileList)
	ON_BN_CLICKED(IDC_BUTTONSELECT, OnButtonselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeatherPage message handlers

void CWeatherPage::OnHideButton() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pMyPropSheet);
	m_pMyPropSheet->ShowWindow(SW_HIDE);
}

void CWeatherPage::rebuildFileList() {
	// Find all weather description files and build the WeatherFilesList
	// List consists of just filename - no .wth extension or directory info
	CVarroaPopView* pView = m_pMyPropSheet->GetView();
	CVarroaPopDoc* pDoc = pView->GetDocument();
	m_Dir = pDoc->GetDefaultPathName();

	CWnd* PathWnd = GetDlgItem(IDC_STATIC_PATH);
	PathWnd->SetWindowText(m_Dir);

	m_WeatherFilesList.RemoveAll();
	CFileFind finder;
	CString name;
	int ok = finder.FindFile(m_Dir+"*.wth");  // weather files
	while(ok) 
	{
		ok = finder.FindNextFile();
		name = finder.GetFileName();
		m_WeatherFilesList.AddTail(SplitPath(name,FNAME));
	}
}

BOOL CWeatherPage::OnSetActive() 
{
	// Execute when this property page becomes active
	rebuildFileList();
	prepComboBox();
	return CPropertyPage::OnSetActive();
}

void CWeatherPage::prepComboBox() 
{
	// Builds the combo box for the weather file dialog
	CComboBox* files = 
		(CComboBox*) GetDlgItem(IDC_WEATHER_FILE_LIST);
	ASSERT(files);

	files->ResetContent();  // clear combo box
 

	CVarroaPopView* pView = m_pMyPropSheet->GetView();
	CVarroaPopDoc* pDoc = pView->GetDocument();
	if (pDoc->GetWeather()->IsInitialized())
	{
		m_WeatherFileName = pDoc->GetWeatherFileName();
	}
	else m_WeatherFileName = "";

   // Transfer weather filenames to ComboBox
	if(!m_WeatherFilesList.IsEmpty()) 
	{
		POSITION pos = m_WeatherFilesList.GetHeadPosition();
		while(pos) 
		{
			files->AddString(
					m_WeatherFilesList.GetNext(pos));
		}
		files->SetWindowText(SplitPath(m_WeatherFileName,FNAME));
	}
}

void CWeatherPage::OnSelchangeWeatherFileList() 
{
	// User selected an weather file from the list.
	// Update the WeatherFileName accordingly.
	CComboBox* files = 
		(CComboBox*) GetDlgItem(IDC_WEATHER_FILE_LIST);
	ASSERT(files);

	CString fname;
	int index = files->GetCurSel();
	if(index != CB_ERR) 
	{
		files->GetLBText(index, fname);
		m_WeatherFileName = m_Dir + fname + ".wth";
	}
}

void CWeatherPage::OnButtonselect() 
{
	// TODO: Add your control notification handler code here
	CVarroaPopView* pView = m_pMyPropSheet->GetView();
	CVarroaPopDoc* pDoc = pView->GetDocument();
	BeginWaitCursor();
	if (!pDoc->GetWeather()->LoadWeatherFile(m_WeatherFileName))
	{
		MyMessageBox("Error Loading Weather File!");
		return;
	}
	else
	{
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_WeatherFileName = m_WeatherFileName;
		pDoc->SetModifiedFlag();
	}
	pDoc->SetSimEnd(pDoc->GetWeather()->GetEndingTime());
	pDoc->SetSimStart(pDoc->GetWeather()->GetBeginningTime());
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->EnableDialogBar(pDoc->IsImmigrationEnabled());
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_WeatherFileName = 
					pDoc->GetWeather()->GetFileName();
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls(); 
	EndWaitCursor();
	
}
