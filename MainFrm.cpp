// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "VarroaPopDoc.h"
#include "ImmigrationDlg.h"

#include <afxdtctl.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_GO, OnButtonGo)
	ON_BN_CLICKED(IDC_BUTTON_IM_SETUP, OnButtonImSetup)
	ON_BN_CLICKED(IDC_RADIO_IM_OFF, OnRadioImOff)
	ON_BN_CLICKED(IDC_RADIO_IM_ON, OnRadioImOn)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_RESULTS, OnUpdateFileSaveResults)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_SESSION, OnUpdateFileSaveSession)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_WEATHER_EDITCURRENTWEATHERFILE, OnUpdateEditcurrentweather)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, &CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &CFrameWnd::OnHelpFinder)

	ON_UPDATE_COMMAND_UI(ID_INDICATOR_WEATHER, OnUpdateWeather)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_START, OnUpdateWeatherStart)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_STOP, OnUpdateWeatherStop)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SESSION, OnUpdateSession)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_RES_FREQ, OnUpdateResFreq)
	ON_UPDATE_COMMAND_UI(IDC_IM_TYPE, OnUpdateImType)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_SESSION, OnUpdateFileSaveSession)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_RESULTS, OnUpdateFileSaveResults)


	ON_COMMAND(ID_HELP, &CMainFrame::OnHelp)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_SESSION,
	ID_INDICATOR_WEATHER,
	ID_INDICATOR_RES_FREQ,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	updatecnt = 0;
}

CMainFrame::~CMainFrame()
{
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_SimulationBar.Create(this,IDD_SIMULATION_BAR,CBRS_RIGHT,IDD_SIMULATION_BAR);

	EnableDialogBar(FALSE);
	OnRadioImOff();

	CStatic* MyType = (CStatic*)m_SimulationBar.GetDlgItem(IDC_IM_TYPE);
	MyType->SetWindowText("None");


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	int ScreenX = GetSystemMetrics(SM_CXSCREEN);
	int ScreenY = GetSystemMetrics(SM_CYSCREEN);

	cs.cx = int(ScreenX*0.75);
	cs.cy = int(ScreenY*0.75);

	cs.x = ScreenX/2 - cs.cx/2;
	cs.y = ScreenY/2 - cs.cy/2;

	if (!gl_RunGUI) cs.style &= ~WS_VISIBLE;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::EnableDialogBar(bool bEnable)
{
	(m_SimulationBar.GetDlgItem(IDC_EDIT_START))->EnableWindow(bEnable);
	(m_SimulationBar.GetDlgItem(IDC_EDIT_STOP))->EnableWindow(bEnable);
	(m_SimulationBar.GetDlgItem(IDC_BUTTON_GO))->EnableWindow(bEnable);
	(m_SimulationBar.GetDlgItem(IDC_BUTTON_IM_SETUP))->EnableWindow(bEnable);
	(m_SimulationBar.GetDlgItem(IDC_RADIO_IM_ON))->EnableWindow(bEnable);
	(m_SimulationBar.GetDlgItem(IDC_RADIO_IM_OFF))->EnableWindow(bEnable);
	
}
void CMainFrame::ClearLogEvents()
{
	((CListCtrl*)m_SimulationBar.GetDlgItem(IDC_LIST_EVENT_LOG))->DeleteAllItems();
}

void CMainFrame::AddLogEvents(CString EventString)
{
	CEdit* pLB = (CEdit*)m_SimulationBar.GetDlgItem(IDC_LIST_EVENT_LOG);
	//TRACE("Added String:  "+EventString);
	pLB->SetWindowTextA(EventString);
}


void CMainFrame::OnButtonGo() 
{
	// TODO: Add your control notification handler code here
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	
	//  Get Initial Conditions from the Dialog Bar
	m_SimulationBar.GetDlgItemText(IDC_EDIT_START,
		pDoc->GetColony()->m_InitCond.m_SimStart);
	m_SimulationBar.GetDlgItemText(IDC_EDIT_STOP,
		pDoc->GetColony()->m_InitCond.m_SimEnd);

	AddLogEvents(""); // Clear Notification Log 

	//  Run the Simulation
	pDoc->Simulate();
	
}


void CMainFrame::OnButtonImSetup() 
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	if (pDoc->ReadyToSimulate())
	{
		CImmigrationDlg MyImDlg(pDoc->GetImmigrationType());
		MyImDlg.SetDocument(pDoc);
		MyImDlg.GetDataFromDoc();
		if (MyImDlg.DoModal()==IDOK)
		{
			CButton* ImOn = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_ON);
			MyImDlg.SetEnabled(ImOn->GetCheck()==1);
			MyImDlg.SendDataToDoc();
		}
	}
}

void CMainFrame::OnRadioImOff() 
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	if (pDoc!=NULL) pDoc->SetImmigrationEnabled(false);
	if (gl_RunGUI)
	{
		CButton* ImOn = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_ON);
		CButton* ImOff = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_OFF);
		ImOn->SetCheck(0);
		ImOff->SetCheck(1);
	}
	
}

void CMainFrame::OnRadioImOn() 
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	if (pDoc!=NULL) pDoc->SetImmigrationEnabled(true);
	if (gl_RunGUI)
	{
		CButton* ImOn = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_ON);
		CButton* ImOff = (CButton*)m_SimulationBar.GetDlgItem(IDC_RADIO_IM_OFF);
		ImOn->SetCheck(1);
		ImOff->SetCheck(0);
	}
	
}

void CMainFrame::OnUpdateWeather(CCmdUI *pCmdUI)
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	m_WeatherFileName = pDoc->GetWeatherFileName();

	// Set Pane Width
	UINT nID;
	UINT nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo(2,nID, nStyle, cxWidth);
	CDC* pDC = GetDC();
	cxWidth = (pDC->GetTextExtent("Weather: "+m_WeatherFileName)).cx;
	m_wndStatusBar.SetPaneInfo(2,nID, nStyle, cxWidth);

	pCmdUI->Enable();
	pCmdUI->SetText("Weather: "+m_WeatherFileName );
	ReleaseDC(pDC);
}

void CMainFrame::OnUpdateSession(CCmdUI *pCmdUI)
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	m_SessionFileName = pDoc->GetPathName();

	// Set Pane Width
	UINT nID;
	UINT nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo(1,nID, nStyle, cxWidth);
	CDC* pDC = GetDC();
	cxWidth = (pDC->GetTextExtent( "Session: "+m_SessionFileName )).cx;
	m_wndStatusBar.SetPaneInfo(1,nID, nStyle, cxWidth);

	pCmdUI->Enable();
	pCmdUI->SetText( "Session: "+m_SessionFileName );
	ReleaseDC(pDC);
}

void CMainFrame::OnUpdateResFreq(CCmdUI *pCmdUI)
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	CString DispType = pDoc->m_DispWeeklyData?"Weekly":" Daily";
 
	// Set Pane Width
	UINT nID;
	UINT nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo(3,nID, nStyle, cxWidth);
	CDC* pDC = GetDC();
	cxWidth = (pDC->GetTextExtent( DispType )).cx;
	m_wndStatusBar.SetPaneInfo(3,nID, nStyle, cxWidth);

	
	pCmdUI->Enable();
	pCmdUI->SetText(DispType);
	ReleaseDC(pDC);

}


void CMainFrame::InitializeDateCtrls()
{
	// Forces the range of the date controls to remain inside the limits of the 
	// weather file and sets their initial values to the beginning and end of the
	// weather data
	CDateTimeCtrl* StartCtrl = (CDateTimeCtrl*)m_SimulationBar.GetDlgItem(IDC_EDIT_START);
	CDateTimeCtrl* StopCtrl = (CDateTimeCtrl*)m_SimulationBar.GetDlgItem(IDC_EDIT_STOP);
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();


	CString begintime = pDoc->GetWeather()->GetBeginningTime().Format("%m/%d/%Y");
	CString endtime = pDoc->GetWeather()->GetEndingTime().Format("%m/%d/%Y");

	StartCtrl->SetRange(&(pDoc->GetWeather()->GetBeginningTime()),
		&(pDoc->GetWeather()->GetEndingTime()));
	
	StartCtrl->SetTime((pDoc->GetSimStart()));
	
	StopCtrl->SetRange(&(pDoc->GetWeather()->GetBeginningTime()),
		&(pDoc->GetWeather()->GetEndingTime()));
	
	StopCtrl->SetTime((pDoc->GetSimEnd()));
	
}



void CMainFrame::OnUpdateWeatherStart(CCmdUI *pCmdUI)
{
	CDateTimeCtrl* StartCtrl = (CDateTimeCtrl*)m_SimulationBar.GetDlgItem(IDC_EDIT_START);
	CDateTimeCtrl* StopCtrl = (CDateTimeCtrl*)m_SimulationBar.GetDlgItem(IDC_EDIT_STOP);
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	if (pDoc->GetWeather()->IsInitialized())
	{
		pCmdUI->Enable();
		COleDateTime theTime;
		StartCtrl->GetTime(theTime);
		if (theTime!=pDoc->GetSimStart())
		{
			pDoc->SetSimStart(theTime);
			// TEMPORARILY Stop Range Setting
			//StopCtrl->SetRange(&theTime,&(pDoc->GetWeather()->GetEndingTime()));
/*
			// If Sim Start is after Immigration Start, update Immigration Start
			if (COleDateTime(pDoc->GetSimStart().GetTime())>pDoc->GetImmigrationStart())
				pDoc->SetImmigrationStart(COleDateTime(theTime.GetTime()));
*/
		}
	}
}

void CMainFrame::OnUpdateWeatherStop(CCmdUI *pCmdUI)
{
	CDateTimeCtrl* StartCtrl = (CDateTimeCtrl*)m_SimulationBar.GetDlgItem(IDC_EDIT_START);
	CDateTimeCtrl* StopCtrl = (CDateTimeCtrl*)m_SimulationBar.GetDlgItem(IDC_EDIT_STOP);
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	if (pDoc->GetWeather()->IsInitialized())
	{
		pCmdUI->Enable();
		COleDateTime theTime;
		StopCtrl->GetTime(theTime);
		if (theTime!=pDoc->GetSimEnd())
		{
			pDoc->SetSimEnd(theTime);
			//  TEMPORARILY STOP RANGE SETTING
			//StartCtrl->SetRange(&(pDoc->GetWeather()->GetBeginningTime()),&theTime);

/*
			// If Sim End is before Immigration End, update Immigration End
			if (COleDateTime(pDoc->GetSimEnd().GetTime())<pDoc->GetImmigrationEnd())
				pDoc->SetImmigrationEnd(COleDateTime(theTime.GetTime()));
*/
		}
	}
}

void CMainFrame::OnUpdateImType(CCmdUI *pCmdUI)
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	CStatic* MyType = (CStatic*)m_SimulationBar.GetDlgItem(IDC_IM_TYPE);
	MyType->SetWindowText(pDoc->GetImmigrationType());
}


void CMainFrame::OnUpdateFileSaveResults(CCmdUI* pCmdUI) 
{
		CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
		if (pDoc->AreResultsReady()) pCmdUI->Enable();
		else pCmdUI->Enable(false);
	
}

void CMainFrame::OnUpdateFileSaveSession(CCmdUI* pCmdUI) 
{
		CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
		if (pDoc->ReadyToSimulate()) pCmdUI->Enable();
		else pCmdUI->Enable(false);
	
}

void CMainFrame::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
		if (pDoc->AreResultsReady()) pCmdUI->Enable();
		else pCmdUI->Enable(false);
	
	
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
		if (pDoc->AreResultsReady()) pCmdUI->Enable();
		else pCmdUI->Enable(false);
	
}


void CMainFrame::OnUpdateEditcurrentweather(CCmdUI* pCmdUI) 
{
	CVarroaPopDoc* pDoc = (CVarroaPopDoc*)GetActiveDocument();
	pCmdUI->Enable(pDoc->IsSessionLoaded());
	
}

void CMainFrame::OnHelp()
{
}
