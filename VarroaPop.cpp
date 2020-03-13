// VarroaPop.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VarroaPop.h"

#include "MainFrm.h"
#include "VarroaPopDoc.h"
#include "VarroaPopView.h"
#include "VPCommandLineInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global object for GUI
bool gl_RunGUI;

// Global function for MessageBox that routes to AfxMessageBox or to Output Log File
int MyMessageBox( LPCTSTR lpszText, UINT nType , UINT nIDHelp  )
{
	if (gl_RunGUI) return AfxMessageBox(lpszText, nType, nIDHelp);
	else 
	{
		CVarroaPopApp* pApp = (CVarroaPopApp*)AfxGetApp();
		pApp->WriteToLogFile(lpszText);
		return IDOK;
	}

}

CString SplitPath(CString PathString, PELEMENT PathElement)
{
	CString retstg;
	char drv[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath_s(PathString.GetBuffer(255),drv,_MAX_DRIVE,dir,_MAX_DIR,fname,_MAX_FNAME,ext,_MAX_EXT);
	switch (PathElement)
	{
	case DRV: retstg = drv;
		break;
	case DIR: retstg = dir;
		break;
	case FNAME: retstg = fname;
		break;
	case EXT: retstg = ext;
		break;
	default: retstg = "";
	}
	return retstg;

}

// Object Dump Break - For Memory Leaks, set = to number in {}
int _afxBreakAlloc;
/////////////////////////////////////////////////////////////////////////////
// CVarroaPopApp

BEGIN_MESSAGE_MAP(CVarroaPopApp, CWinApp)
	//{{AFX_MSG_MAP(CVarroaPopApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_HELP_REPORTAPROBLEM, &CVarroaPopApp::OnHelpReportaproblem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopApp construction

CVarroaPopApp::CVarroaPopApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	EnableHtmlHelp();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVarroaPopApp object

CVarroaPopApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CVarroaPopApp initialization

BOOL CVarroaPopApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	//  Removed based on compiler warning message Enable3dControls();			// Call this when using MFC in a shared DLL
#endif

	//  Set the path and filename of this instance
	TCHAR buff[255];
	::GetModuleFileName(AfxGetApp()->m_hInstance, buff, 255);
	m_ModuleFileName = CString(buff);

	// Initialize OLE libraries - Required after VS2010. Templates created after 2010 autommatically add but this was a conversion from VS2008
	AfxOleInit();
	//if (!AfxOleInit())                   // ** MAKE SURE THAT YOU CALL THIS!! **
	{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);
		//return FALSE;
		int i = 1;  // A debug target if this fails.  Test Only - can be removed.
	}

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Crystal River"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	//TRACE("Opening DocTemplate\n");

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVarroaPopDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CVarroaPopView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CVPCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	gl_RunGUI = !cmdInfo.CLBackground;
	m_SessionFileName = cmdInfo.CLSessionFileName;
	m_InputFileName = cmdInfo.CLInputFileName;
	m_OutputGraphFileName = cmdInfo.CLOutputGraphFileName;
	m_OutputResultsFileName = cmdInfo.CLOutputResultsFileName;
	m_OutputLogFileName = cmdInfo.CLOutputLogFileName;


	// Dispatch commands specified on the command line
	if (!gl_RunGUI) cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	if (gl_RunGUI)	
	{
		if (m_SessionFileName.GetLength()>0) OpenDocumentFile(m_SessionFileName);

		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);

		// The one and only window has been initialized, so show and update it.
		m_pMainWnd->UpdateWindow();

		// Enable drag/drop open
		m_pMainWnd->DragAcceptFiles();
	}

	if (m_OutputLogFileName.GetLength() > 0)
	{
		OpenLogFile(m_OutputLogFileName);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_VersionString;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_VersionString = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VERSIONBOX, m_VersionString);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CVarroaPopApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	// Load Version string resource into dialog.
	// NOTE:  When version changes, update IDS_VERSIONSTRING in string table resource
	aboutDlg.m_VersionString.LoadString(IDS_VPVERSION);
	aboutDlg.DoModal();
}


int CVarroaPopApp::ExitInstance() 
{
	CloseLogFile();
	return CWinApp::ExitInstance();
}

void CVarroaPopApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CWinApp::OnFileOpen();

}

int CVarroaPopApp::Run()
{
	if (gl_RunGUI) return CWinApp::Run();
	else
	{
		RunBackground();
		CloseAllDocuments(TRUE);
		return ExitInstance();
	}

}

bool CVarroaPopApp::RunBackground()
{
	// Get Document pointer
	if (m_SessionFileName.GetLength()>0)
	{
		POSITION pos;
		pos = GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = GetNextDocTemplate(pos);
		CVarroaPopDoc* pDoc = 
			(CVarroaPopDoc*)pDocTemplate->OpenDocumentFile(m_SessionFileName,FALSE);
		if (pDoc == NULL)
		{
			//TRACE("Document Open Failed\n");
		}	
		else 
		{
			//TRACE("Opened Document %s\n",m_SessionFileName);
			pDoc->Simulate();
			//TRACE("Ran Simulation\n");
		}
	}
	return true;

}


bool CVarroaPopApp::OpenLogFile(CString LogFileName)
{
	CFileException* pError = NULL;
	COleDateTime DateTime;
	CString stg = DateTime.GetCurrentTime().Format("%b-%d, %Y  %I:%M %p");
	BOOL TF = (m_LogFile.Open(LogFileName, 
			CFile::modeCreate|CFile::modeWrite|CFile::typeText,pError ));
	bool tf = (TF==TRUE ? true : false);

	if (tf)
	{
		try
		{
			tf &= WriteToLogFile("VarroaPop Log File");
			tf &= WriteToLogFile("Created " + stg + "\n\n");

		}
		catch (CFileException* e) 
		{
			return false;
		}
	}
	return tf;			
}

bool CVarroaPopApp::CloseLogFile(bool Save)
{
	try
	{
		if (m_LogFile.m_pStream != NULL) // if it was opened, close it
		{
			m_LogFile.Close();
			if (!Save) CFile::Remove(m_LogFile.GetFilePath());
		}
		return true;
	}
	catch (CFileException* e)
	{
		return false;
	}
}


bool CVarroaPopApp::WriteToLogFile(CString Line)
{
	try
	{
		if (m_LogFile.m_pStream == NULL) return false;
		else m_LogFile.WriteString(Line + "\n");
		return true;
	}
	catch (CFileException* e)
	{
		return false;
	}


}
void CVarroaPopApp::OnHelpReportaproblem()
{
	// Plan: create a forum to identify bugs/issues found in VarroaPop.
	// Once I figure out how to do that, enable this link.
	ShellExecute(NULL,"open","http://varroapop.boards.net",NULL,NULL,SW_SHOWNORMAL); 
}
