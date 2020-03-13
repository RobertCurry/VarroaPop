// VarroaPopView.cpp : implementation of the CVarroaPopView class
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "VarroaPopDoc.h"
#include "VarroaPopView.h"
#include "OlectraGraphDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PRINTMARGIN 2

int COlectraGraphDlg::m_GraphDlgCount = 0;
int COlectraGraphDlg::m_OpenGraphDlgs = 0;

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopView

IMPLEMENT_DYNCREATE(CVarroaPopView, CListView)

BEGIN_MESSAGE_MAP(CVarroaPopView, CListView)
	//{{AFX_MSG_MAP(CVarroaPopView)
	ON_COMMAND(ID_SHOW_PROPSHEET, OnShowPropsheet)
	ON_COMMAND(ID_VIEW_PARAMETERS, OnViewParameters)
	ON_COMMAND(ID_BUTTON_COLEXPAND, OnColumnExpand)
	ON_COMMAND(ID_BUTTON_COLREDUCE, OnColumnReduce)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
	ON_COMMAND(ID_APP_EXIT, &CVarroaPopView::OnAppExit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopView construction/destruction

CVarroaPopView::CVarroaPopView()
	 : CListView() {
	//{{AFX_DATA_INIT(CVarroaPopView)
	//}}AFX_DATA_INIT

	m_pPropSheet = NULL;
	m_pColonyDataPage = NULL;
	m_pWeatherPage = NULL;
	m_pSimulationPage = NULL;
	m_LastPropSheetPageNum = 0;
	m_CurrentDisplayAI = 0;
	m_NumDisplayColumns = 13;
}

CVarroaPopView::~CVarroaPopView()
{
}


BOOL CVarroaPopView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopView printing

BOOL CVarroaPopView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CVarroaPopView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{

	// Set font
	m_fontPrinter.CreatePointFont(80,_T("Courier New"),pDC);

	// Compute the width and height of a line in printer font
	TEXTMETRIC tm; 
	CFont* pOldFont = pDC->SelectObject(&m_fontPrinter);
	pDC->GetTextMetrics(&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	// Note:  head of results must be full the same length as the results data
	CString stg = GetDocument()->m_ResultsFileHeader.GetHead();
	CSize size = pDC->GetTextExtent(GetDocument()->m_ResultsFileHeader.GetHead());
	m_nCharsPerLine = GetDocument()->m_ResultsFileHeader.GetHead().GetLength();
	pDC->SelectObject(pOldFont);
	m_cxWidth = size.cx;

	// Compute the page count
	m_nLinesTotal = GetDocument()->GetDocumentLength();
	int HeaderLength = GetDocument()->m_ResultsFileHeader.GetCount();
	m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) - 
		(m_cyPrinter * (HeaderLength + (2*PRINTMARGIN)))) / m_cyPrinter;
	UINT nMaxPage = max(1, (m_nLinesTotal + (m_nLinesPerPage - 1)) / 
		m_nLinesPerPage);
	pInfo->SetMaxPage (nMaxPage);

	// Compute the horizontal offset required to center lines of output
	m_cxOffset = (pDC->GetDeviceCaps( HORZRES ) - size.cx) / 2;



}

void CVarroaPopView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_fontPrinter.DeleteObject();
}

void CVarroaPopView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CVarroaPopDoc* pDoc = GetDocument();
	PrintPageHeader(pDoc, pDC, pInfo->m_nCurPage);
	PrintPage(pDoc, pDC, pInfo->m_nCurPage);
}


void CVarroaPopView::PrintPageHeader(CVarroaPopDoc* pDoc, CDC* pDC, UINT PageNumber)
{
	// Form Header String
	CString strHeader = "Colony: "+pDoc->GetColonyFileName() + 
		"  Weather: "+pDoc->GetWeatherFileName();

	CString strPageNumber;
	strPageNumber.Format(_T("Page %d"),PageNumber);

	int nSpaces = m_nCharsPerLine-strPageNumber.GetLength() - strHeader.GetLength();
	for (int i=0;i<nSpaces;i++) strHeader += _T(' ');
	strHeader += strPageNumber;

	// Output the Text
	UINT y = m_cyPrinter * PRINTMARGIN;
	CFont* pOldFont = pDC->SelectObject(&m_fontPrinter);

	m_nLinesPerHeader = pDoc->m_ResultsFileHeader.GetCount() + 1; 

	pDC->TextOut (m_cxOffset, y, strHeader);

	POSITION pos = pDoc->m_ResultsFileHeader.GetHeadPosition();
	while (pos!=NULL)
	{
		y += m_cyPrinter;
		pDC->TextOut(m_cxOffset, y, pDoc->m_ResultsFileHeader.GetNext( pos ));
	}

	// Draw a horizontal line
	y += m_cyPrinter;
	pDC->MoveTo(m_cxOffset,y);
	pDC->LineTo(m_cxOffset + m_cxWidth, y);
	m_nLinesPerHeader += 1;


	pDC->SelectObject(pOldFont);


}

void CVarroaPopView::PrintPage(CVarroaPopDoc* pDoc, CDC* pDC, UINT PageNumber)
{
	if (m_nLinesTotal != 0)
	{
		UINT nStart = (PageNumber - 1) * m_nLinesPerPage;
		UINT nEnd = min(m_nLinesTotal - 1, nStart + m_nLinesPerPage - 1);

		CFont* pOldFont = pDC->SelectObject(&m_fontPrinter);
		for (UINT i = nStart; i<=nEnd;i++)
		{
			CString outstring = pDoc->m_ResultsText.GetAt(pDoc->m_ResultsText.FindIndex(i));
			UINT y = ((i-nStart) + PRINTMARGIN + m_nLinesPerHeader)*m_cyPrinter;
			pDC->TextOut(m_cxOffset, y, outstring);
		}
		pDC->SelectObject(pOldFont);
	}

}

void CVarroaPopView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{

	CListView::OnPrepareDC(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopView diagnostics

#ifdef _DEBUG
void CVarroaPopView::AssertValid() const
{
	CListView::AssertValid();
}

void CVarroaPopView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CVarroaPopDoc* CVarroaPopView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVarroaPopDoc)));
	return (CVarroaPopDoc*)m_pDocument;
}
#endif //_DEBUG




/////////////////////////////////////////////////////////////////////////////
// CVarroaPopView message handlers
void CVarroaPopView::UpdateColumnWidth()
{
	CListCtrl& ListCtrl = GetListCtrl();

	//  Determine the Column width in Pixels
	CRect rect;
	GetClientRect(&rect);
	int ColWidth;
	if (m_NumDisplayColumns > 0) ColWidth = rect.Width()/m_NumDisplayColumns; 
	else ColWidth = rect.Width()/13; //Default

	for (int ColumnNumber = 0; ColumnNumber < m_ColumnCount; ColumnNumber++)
	{
		ListCtrl.SetColumnWidth(ColumnNumber,ColWidth);
	}
}


void CVarroaPopView::OnUpdate(CView* pSender, LPARAM lHint, 
							  CObject* pHint) 
{
	//TRACE("In OnUpdate\n");

	// Build Header Information
	CVarroaPopDoc* pDoc = GetDocument();
	CListCtrl& ListCtrl = GetListCtrl();
	ListCtrl.DeleteAllItems();


	// Turn this back on later.  It works but need to be able to save column order between
	// VP executions.  Use registry.  See MyListCtrl.h, .cpp for hints on how to use registry for this.
	ListCtrl.SetExtendedStyle( LVS_EX_HEADERDRAGDROP );	


	//ListCtrl.SetExtendedStyle( LVS_EX_GRIDLINES );
	
	// Set ListCtrl style to LVS_REPORT
	DWORD dwStyle = ::GetWindowLong(ListCtrl.m_hWnd,GWL_STYLE);
	dwStyle |= LVS_REPORT;
	::SetWindowLong(ListCtrl.m_hWnd,GWL_STYLE,dwStyle);

	//  Determine the Column width in Pixels
	CRect rect;
	GetClientRect(&rect);
	int ColWidth;
	if (m_NumDisplayColumns > 0) ColWidth = rect.Width()/m_NumDisplayColumns; 
	else ColWidth = rect.Width()/13; //Default


	//  Now insert the columns
	CString HdrLabel;

	if (!pDoc->m_ResultsHeader.IsEmpty())
	{
		m_ColumnCount = 1;
		while (!pDoc->m_ResultsHeader.IsEmpty())
		{
			HdrLabel = pDoc->m_ResultsHeader.RemoveHead();
			ListCtrl.InsertColumn(	m_ColumnCount,
									LPCTSTR(HdrLabel),
									LVCFMT_CENTER,
									ColWidth,
									m_ColumnCount-1);
			m_ColumnCount++;
		}
	}
	else 
	for (int ColumnNumber = 0; ColumnNumber < m_ColumnCount; ColumnNumber++)
	{
		ListCtrl.SetColumnWidth(ColumnNumber,ColWidth);
	}

	// Now display the items
	POSITION pos = pDoc->m_ResultsText.GetHeadPosition();
	CString tempstg;
	char delims[] = " ,\t"; // Possible Item Delimiters
	LPTSTR buffer;
	char* colvalue;
	char* NextToken = NULL;
	int itemnumber=0;
	while (pos != NULL)
	{
		tempstg = pDoc->m_ResultsText.GetNext(pos);
		buffer = tempstg.GetBuffer(0);
		colvalue = strtok_s(buffer,delims,&NextToken);
		ListCtrl.InsertItem(itemnumber,colvalue);
		int subitemnumber = 1;
		while (colvalue != NULL)
		{
			colvalue = strtok_s(NULL,delims, &NextToken);
			ListCtrl.SetItem(itemnumber,subitemnumber++,LVIF_TEXT,LPCTSTR(colvalue),0,0,0,0);
		}
		itemnumber++;
		tempstg.ReleaseBuffer();
	}



	bool test = pDoc->IsSimulationComplete();
	if (pDoc->IsSimulationComplete()) // Display new graph
	{
		ChartData(pDoc);
	}

	UpdateEventLog();

	CListView::OnUpdate(pSender, lHint, pHint); 

}

// Reads the ColonyEventLog and places the strings in the log ListBox on the Dialog Bar
// Note - apparently there is no copy constructor for a string list so have to use pointers 
void CVarroaPopView::UpdateEventLog()
{
	((CMainFrame*)AfxGetApp()->GetMainWnd())->ClearLogEvents();
	CVarroaPopDoc* pDoc = GetDocument();
	if (!pDoc->GetColony()->m_ColonyEventList.IsEmpty())
	{
		CString EventString = "";
		int Count = 0;
		POSITION pos = pDoc->GetColony()->m_ColonyEventList.GetHeadPosition();
		while (pos != NULL)
		{
			EventString += pDoc->GetColony()->m_ColonyEventList.GetNext(pos) + "\r\n";
		}
		((CMainFrame*)AfxGetApp()->GetMainWnd())->AddLogEvents(EventString);
	}
}


void CVarroaPopView::SaveColumnInfo()
{

}

void CVarroaPopView::GetColumnInfo()
{

}


void CVarroaPopView::OnDraw(CDC* pDC)
 {
	//TRACE("In OnDraw\n");

	
	// Display the simulation results table
	CVarroaPopDoc* pDoc = GetDocument();
	POSITION pos = pDoc->m_ResultsText.GetHeadPosition();

	// Set font
	CFont theFont;
	CFont* pOldFont;
	theFont.CreatePointFont(80,"Courier New",pDC);
	pOldFont = pDC->SelectObject(&theFont);


	TEXTMETRIC Metrics;
	if (!pDC->GetTextMetrics(&Metrics))
	{
		//TRACE("FAILED TO GET TEXTMETRICS\n");
	}
	else 
	{
		int linenum = 0;
		int row = 0;
		int rowstep = Metrics.tmHeight;
		while (pos != NULL)
		{
			row += rowstep;
			pDC->TextOut(10,row,pDoc->m_ResultsText.GetNext(pos));
		}
	}
	ReleaseDC(pDC);

}

BOOL CVarroaPopView::Create(LPCTSTR lpszClassName, 
							LPCTSTR lpszWindowName, 
							DWORD dwStyle, const RECT& rect, 
							CWnd* pParentWnd, UINT nID, 
							CCreateContext* pContext) 
{


	int result = CListView::Create(lpszClassName, lpszWindowName, 
							 dwStyle, rect, pParentWnd, nID, pContext);

	ASSERT(pParentWnd != NULL);
	m_pMyParentWindow = pParentWnd;

	return result;
}

void CVarroaPopView::OnShowPropsheet() 
{

	m_pPropSheet = new CMyPropSheet("   Model Parameters");
	m_pPropSheet->m_psh.dwFlags |= PSH_NOAPPLYNOW;

	
	m_pPropSheet->SetView(this);

	m_pColonyDataPage = new CColonyDataPage(GetDocument());
	m_pWeatherPage    = new CWeatherPage(GetDocument());
	m_pResourcePage  = new CResourcePage();
	m_pActionPage	  = new CActionPage;
	m_pEPAPage  = new CEPAPage(this);
	if (GetDocument()->IsWeatherLoaded())
	{
		m_pEPAPage->SetEarliestDate(GetDocument()->GetWeather()->GetBeginningTime());
		m_pEPAPage->SetLatestDate(GetDocument()->GetWeather()->GetEndingTime());
	}

	m_pPropSheet->AddPage(m_pColonyDataPage);
	m_pPropSheet->AddPage(m_pWeatherPage);
	m_pPropSheet->AddPage(m_pActionPage);
	//m_pPropSheet->AddPage(m_pResourcePage);
	m_pPropSheet->AddPage(m_pEPAPage);

	// Give each property page a pointer to the property sheet
	m_pColonyDataPage->setPropSheet(m_pPropSheet);
	m_pWeatherPage->setPropSheet(m_pPropSheet);
	m_pActionPage->setPropSheet(m_pPropSheet);
	m_pEPAPage->setPropSheet(m_pPropSheet);

	m_pColonyDataPage->m_MaxPropForagers = GetDocument()->GetColony()->foragers.GetPropActualForagers();

	// Set Action Page variables to CVarroaPop variables
	m_pActionPage->m_EggLayingDelay = GetDocument()->m_RQEggLayingDelay;
	m_pActionPage->m_EnableReQueen = GetDocument()->m_RQEnableReQueen;
	m_pActionPage->m_Scheduled = GetDocument()->m_RQScheduled;
	m_pActionPage->m_QueenStrength = (int)(GetDocument()->m_RQQueenStrength + 0.5);  // Round up for integer conversion
	m_pActionPage->m_ReQueenDate = GetDocument()->m_RQReQueenDate;
	m_pActionPage->m_RQOnce = GetDocument()->m_RQOnce;
	m_pActionPage->m_TreatmentStart = GetDocument()->m_VTTreatmentStart;
	m_pActionPage->m_TreatmentDuration = GetDocument()->m_VTTreatmentDuration;
	m_pActionPage->m_VT_Enable = GetDocument()->m_VTEnable;
	m_pActionPage->m_TreatmentMortality = GetDocument()->m_VTMortality;
	m_pActionPage->m_Resistant = GetDocument()->m_InitMitePctResistant;
	m_pActionPage->SetMiteTreatmentInfo(GetDocument()->m_MiteTreatments);

	// Set EPAPage variables from the CColony
	m_pEPAPage->m_EPAData = GetDocument()->GetColony()->m_EPAData;
	m_pEPAPage->m_NutrientCT = GetDocument()->GetColony()->m_NutrientCT;
	m_pEPAPage->m_SupNecBegin = GetDocument()->GetColony()->m_SuppNectar.m_BeginDate;
	m_pEPAPage->m_SupNecEnd = GetDocument()->GetColony()->m_SuppNectar.m_EndDate;
	m_pEPAPage->m_SupNecInitAmount = GetDocument()->GetColony()->m_SuppNectar.m_StartingAmount;
	m_pEPAPage->m_SupNecEnable = GetDocument()->GetColony()->m_SuppNectarEnabled;
	m_pEPAPage->m_SuppNecAnnual = GetDocument()->GetColony()->m_SuppNectarAnnual;
	m_pEPAPage->m_SupPolBegin = GetDocument()->GetColony()->m_SuppPollen.m_BeginDate;
	m_pEPAPage->m_SupPolEnd = GetDocument()->GetColony()->m_SuppPollen.m_EndDate;
	m_pEPAPage->m_SupPolInitAmount = GetDocument()->GetColony()->m_SuppPollen.m_StartingAmount;
	m_pEPAPage->m_SupPolEnable = GetDocument()->GetColony()->m_SuppPollenEnabled;
	m_pEPAPage->m_SuppPolAnnual = GetDocument()->GetColony()->m_SuppPollenAnnual;
	m_pEPAPage->m_ColonyPolInitAmount = GetDocument()->GetColony()->m_ColonyPolInitAmount;
	m_pEPAPage->m_ColonyNecInitAmount = GetDocument()->GetColony()->m_ColonyNecInitAmount;
	m_pEPAPage->m_ColonyPolMaxAmount = GetDocument()->GetColony()->m_ColonyPolMaxAmount;
	m_pEPAPage->m_ColonyNecMaxAmount = GetDocument()->GetColony()->m_ColonyNecMaxAmount;
	m_pEPAPage->m_ResourcesRequired = GetDocument()->GetColony()->m_NoResourceKillsColony;


	m_pPropSheet->SetActivePage(m_LastPropSheetPageNum);


	if (m_pPropSheet->DoModal() == IDOK)
	{
		double qs = GetDocument()->GetColony()->queen.GetQueenStrength();
		double iqs = GetDocument()->GetColony()->m_InitCond.m_QueenStrength;
		GetDocument()->m_RQEggLayingDelay = m_pActionPage->m_EggLayingDelay;
		GetDocument()->m_RQEnableReQueen = m_pActionPage->m_EnableReQueen;
		GetDocument()->m_RQScheduled = m_pActionPage->m_Scheduled;
		GetDocument()->m_RQQueenStrength = m_pActionPage->m_QueenStrength;
		GetDocument()->m_RQReQueenDate = m_pActionPage->m_ReQueenDate;
		GetDocument()->m_RQOnce = m_pActionPage->m_RQOnce;
		GetDocument()->m_VTTreatmentStart = m_pActionPage->m_TreatmentStart;
		GetDocument()->m_VTTreatmentDuration = m_pActionPage->m_TreatmentDuration;
		GetDocument()->m_VTEnable = m_pActionPage->m_VT_Enable;
		GetDocument()->m_VTMortality = m_pActionPage->m_TreatmentMortality;
		GetDocument()->m_InitMitePctResistant = m_pActionPage->m_Resistant;
		GetDocument()->GetColony()->foragers.SetPropActualForagers(m_pColonyDataPage->m_MaxPropForagers);
		m_pActionPage->GetMiteTreatmentInfo(GetDocument()->m_MiteTreatments);

		
		// Get EPAPage variables from the CColony
		GetDocument()->GetColony()->m_EPAData = m_pEPAPage->m_EPAData;
		GetDocument()->GetColony()->m_NutrientCT = m_pEPAPage->m_NutrientCT;

		GetDocument()->GetColony()->m_SuppNectar.m_BeginDate = m_pEPAPage->m_SupNecBegin;
		GetDocument()->GetColony()->m_SuppNectar.m_EndDate = m_pEPAPage->m_SupNecEnd;
		GetDocument()->GetColony()->m_SuppNectar.m_StartingAmount = m_pEPAPage->m_SupNecInitAmount;
		GetDocument()->GetColony()->m_SuppNectarEnabled = m_pEPAPage->m_SupNecEnable;
		GetDocument()->GetColony()->m_SuppNectarAnnual = m_pEPAPage->m_SuppNecAnnual;
		GetDocument()->GetColony()->m_SuppPollen.m_BeginDate = m_pEPAPage->m_SupPolBegin;
		GetDocument()->GetColony()->m_SuppPollen.m_EndDate = m_pEPAPage->m_SupPolEnd;
		GetDocument()->GetColony()->m_SuppPollen.m_StartingAmount = m_pEPAPage->m_SupPolInitAmount;
		GetDocument()->GetColony()->m_SuppPollenEnabled = m_pEPAPage->m_SupPolEnable;
		GetDocument()->GetColony()->m_SuppPollenAnnual = m_pEPAPage->m_SuppPolAnnual;
		GetDocument()->GetColony()->m_ColonyPolInitAmount = m_pEPAPage->m_ColonyPolInitAmount;
		GetDocument()->GetColony()->m_ColonyNecInitAmount = m_pEPAPage->m_ColonyNecInitAmount;
		GetDocument()->GetColony()->m_ColonyPolMaxAmount = m_pEPAPage->m_ColonyPolMaxAmount;
		GetDocument()->GetColony()->m_ColonyNecMaxAmount = m_pEPAPage->m_ColonyNecMaxAmount;
		GetDocument()->GetColony()->m_NoResourceKillsColony = m_pEPAPage->m_ResourcesRequired;

		m_LastPropSheetPageNum = m_pPropSheet->GetLastPageNumber(); // Remember last page number and come back next time
	}

	delete m_pPropSheet;
	delete m_pColonyDataPage;
	delete m_pWeatherPage;
	delete m_pActionPage;


	
}


void CVarroaPopView::OnViewParameters() 
{
	// TODO: Add your command handler code here
	OnShowPropsheet();
}

void CVarroaPopView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	//UseSavedClmInfo("ListClms");

}


void CVarroaPopView::ChartData(CVarroaPopDoc *pDoc, bool Display)
{
	try
	{
	//  Graph resource is initially not visible.  If the intent is to
	//  display the graph, the ShowWindow function is called explicitly
		COlectraGraphDlg* theGraph = new COlectraGraphDlg;
		theGraph->SetTitle(pDoc->GetTitle());
		theGraph->SetData(&(pDoc->m_SimResults));
		theGraph->SetLabels(&(pDoc->m_SimLabels));
		COleDateTime theTime = pDoc->GetSimStart();
		theGraph->SetYAxisMinMax((pDoc->m_AutoScaleChart == 0), pDoc->m_YAxisMin, pDoc->m_YAxisMax);
		theGraph->SetStartDate(COleDateTime(theTime.GetYear(),theTime.GetMonth(),
			theTime.GetDay(),theTime.GetHour(),theTime.GetMinute(),theTime.GetSecond()));
		theGraph->Create(IDD_OLECTRAGRAPHDLG);
		if (Display) 
		TRY
		{
			theGraph->ShowWindow(SW_SHOW);
		}
		CATCH (CException, e)
		{
			MyMessageBox("Cannot Open Any More Graphs - Try Closing Some");
		}
		END_CATCH
		CVarroaPopApp* pApp = (CVarroaPopApp*)AfxGetApp();
		theGraph->DoChart(pApp->m_OutputGraphFileName);
	}
	catch (COleDispatchException *e)
	{
		MyMessageBox("OLE Exception in VPView::ChartData: "+e->m_strDescription);
	}

}

void CVarroaPopView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("In OnHScroll\n");
	
	CListView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVarroaPopView::OnColumnExpand()
{
	if (m_NumDisplayColumns > 4) 
	{
		m_NumDisplayColumns--;
		UpdateColumnWidth();
	}
}

void CVarroaPopView::OnColumnReduce()
{
	if (m_NumDisplayColumns < m_ColumnCount ) 
	{
		m_NumDisplayColumns++;
		UpdateColumnWidth();
	}
}


/////////////////////////////////////////////////////////////////////////////
// Save and restore user prefs on column positions and widths
// to the registry key for this program
//
void CVarroaPopView::SaveClmInfo( LPCSTR szKey ) 
{
	HDITEM rHdr;

	int nClmCnt= GetListCtrl().GetHeaderCtrl()->GetItemCount();

	int anClmIdxs[   MaxListClms ];
	int anClmWidths[ MaxListClms ];

	for (int j=0; j< nClmCnt; j++ ) {
		rHdr.mask= HDI_WIDTH | HDI_ORDER;
		GetListCtrl().GetHeaderCtrl()->GetItem(j, &rHdr);	
		anClmIdxs[   j ] = rHdr.iOrder;
		anClmWidths[ j ] = rHdr.cxy;
	}
	SaveClmHdrInfo( szKey, nClmCnt, anClmIdxs, anClmWidths );
}

void CVarroaPopView::UseSavedClmInfo( LPCSTR szKey ) 
{
	HDITEM rHdr;

	int nClmCnt= GetListCtrl().GetHeaderCtrl()->GetItemCount();

	int anClmIdxs[   MaxListClms ];
	int anClmWidths[ MaxListClms ];

	if ( ReadClmHdrInfo( szKey, nClmCnt, anClmIdxs, anClmWidths ) ) {
		for (int j=0; j< nClmCnt; j++ ) {
			if ( anClmWidths[j] == 0 ) anClmWidths[j]= 5;

			rHdr.mask= HDI_WIDTH | HDI_ORDER;
			rHdr.iOrder= anClmIdxs[   j ];
			rHdr.cxy   = anClmWidths[ j ];
			GetListCtrl().GetHeaderCtrl()->SetItem(j, &rHdr);	
		}
	}
}

void CVarroaPopView::SaveClmHdrInfo( LPCSTR szKey, int nClms, LPINT paiIndexes, LPINT paiWidths )
{
	CString sAll="";
	CString sOne;
	if (szKey==0) szKey= "ListClms";
	
	for (int j=0; j< nClms; j++ ) {
		sOne.Format("%d,", *paiIndexes++ );
		sAll += sOne;
		sOne.Format("%d, ", *paiWidths++ );
		sAll += sOne;
	}
	AfxGetApp()->WriteProfileString("Prefs", szKey, sAll);
}
BOOL CVarroaPopView::ReadClmHdrInfo( LPCSTR szKey, int nClms, LPINT paiIndexes, LPINT paiWidths )
{
	if (szKey==0) szKey= "ListClms";

	CString sAll= AfxGetApp()->GetProfileString("Prefs", szKey );
	if (sAll.IsEmpty()) {
		return FALSE;
	}
	sAll += "                                ";
	int nCurPos=0;
	for (int j=0; j<nClms; j++ ) {

		*paiIndexes++ = atoi( sAll.Mid( nCurPos ) );
		nCurPos= sAll.Find( ',', nCurPos ) +1; // get past the comma
		if ( nCurPos <= 0) return( FALSE );

		*paiWidths++  = atoi( sAll.Mid( nCurPos ) );
		nCurPos= sAll.Find( ',', nCurPos ) +1; // get past the comma
		if ( nCurPos <= 0) return( FALSE );
	}
	return TRUE;
}

void CVarroaPopView::OnAppExit()
{
	//SaveClmInfo("ListClms");
	exit(0);
	
}
