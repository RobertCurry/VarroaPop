// OlectraGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "actionmapcollection.h"
#include "actionmap.h"
#include "axis.h"
#include "axiscollection.h"
#include "chartarea.h"
#include "chartgroupcollection.h"
#include "chartgroup.h"
#include "chart2ddata.h"
#include "deriveddouble.h"
#include "timescale.h"
#include "labelcollection.h"
#include "label.h"
#include "2dconst.h"
//#include "c:\olectra\include\2dconst.h"
#include "OlectraGraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Originally was 4000.  Changed to 10000, then changed to 100000 9/7/2019
#define MAXPOINTS 100000
/////////////////////////////////////////////////////////////////////////////
// COlectraGraphDlg dialog


COlectraGraphDlg::COlectraGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COlectraGraphDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COlectraGraphDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_GraphDlgCount++;
	m_OpenGraphDlgs++;
	m_Dragging = false;
	m_pMouseLabel = NULL;
	//SetYAxisMinMax(true);

}


void COlectraGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COlectraGraphDlg)
	DDX_Control(pDX, IDC_OLECTRACHART, m_Graph);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COlectraGraphDlg, CDialog)
	//{{AFX_MSG_MAP(COlectraGraphDlg)
	ON_COMMAND(IDR_PRINTER, OnPrinter)
	ON_COMMAND(IDR_FILE, OnFile)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &COlectraGraphDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COlectraGraphDlg message handlers

BOOL COlectraGraphDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString theTitle;
	theTitle.Format("%s %d",m_Title,m_GraphDlgCount);
	SetWindowText(theTitle);

	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



void COlectraGraphDlg::SetYAxisMinMax(bool Auto, double Min, double Max)
{
	m_AutoScaleYAxis = Auto;
	m_YAxisMin = Min;
	m_YAxisMax = Max;
}


void COlectraGraphDlg::DoChart(CString OutFileName)
{
	double da[MAXPOINTS];
	int i,j;
	int rows = m_pData->Row(), columns = m_pData->Col();

	// initialize a local SafeArray structure easily.
	SAFEARRAY sa = {
		1,					// number of dimensions
		FADF_FIXEDSIZE,		// type of data
		sizeof(double),		// size of array element
		0,					// number of locks (don't care)
		(void*)0,			// data pointer:  pvData
		{rows, 0}};			// number of elements: rgsabound[0].cElements, lower bound

	m_safeArray = sa;		// copy the values to the dialog variable.


	CChart2DData theData = m_Graph.GetChartGroups().GetItem(COleVariant(short(1))).GetData();
	// Set User Action Events
	m_Graph.GetActionMaps().RemoveAll();
	CActionMap theMap = m_Graph.GetActionMaps().Add(WM_RBUTTONUP,0,0,oc2dActionModifyEnd);
	

	// Set size of graph data
	theData.SetNumSeries(columns-1);
	for (i=1;i<columns;i++)
	{
		theData.SetNumPoints(i,rows);
	}

	// Set Axis Formats
	CAxis XAxis(m_Graph.GetChartArea().GetAxes().GetItem(COleVariant("X")));
	XAxis.GetTimeScale().SetBase(DATE(m_StartDate)); //ERROR - this is throwing a COleDispatchException for dates prior to 1970 - Olectra Chart issue
	XAxis.GetTimeScale().SetUnit(4);

	CAxis YAxis(m_Graph.GetChartArea().GetAxes().GetItem(COleVariant("Y")));

	YAxis.GetTitle().SetText("Quantity");


	// Set the Y-Axis scaling if user selected manual scaling
	// 
	if (!m_AutoScaleYAxis)
	{
		YAxis.GetMax().SetValue(m_YAxisMax);
		YAxis.GetMin().SetValue(m_YAxisMin);
	}


	// set up a variant to pass in the SafeArray.
	// note that we do NOT want to VariantClear() because
	// we are not allocating the variant as is usual practice.
	// all of our variables are on the stack or in CVectorDlg.

	VARIANT var;
	VariantInit(&var);
	V_VT(&var) = VT_ARRAY | VT_R8;		// a safeArray of R8
	V_ARRAY(&var) = &m_safeArray;		// set the variant address pointer.

	m_safeArray.pvData = da;			// just set the SafeArray pointer to the array
	theData.SetIsBatched(TRUE);


	for(i=0; i <rows; i++)
	{
		da[i] = m_pData->Get(0,i);
	}
	theData.CopyXVectorIn(1,var);

	for (i = 1; i<columns; i++) //Series
	{
		for (j = 0; j<rows; j++) // Datapoint
		{
			da[j] = m_pData->Get(i,j);
		}
		theData.CopyYVectorIn(i,var);
	}


	// Set Legends
	POSITION pos = m_pLabels->GetHeadPosition();
	CString LegendLabel;
	for (i = 1; i<columns; i++)
	{
		if (pos != NULL) 
		{
			LegendLabel = m_pLabels->GetNext(pos);
			m_Graph.GetChartGroups().GetItem(COleVariant(short(1))).GetSeriesLabels().
				Add(LPCTSTR(LegendLabel),COleVariant(long(i)));
		}
	}
	

	theData.SetIsBatched(FALSE);

	// Write bitmap file to filename specified on command line
	if (OutFileName.GetLength() > 0) 
	{
		BOOL success = false;
		CString ext = 
//			OutFileName.Right(OutFileName.GetLength() - OutFileName.Find('.') - 1);
			OutFileName.Right(OutFileName.GetLength() - OutFileName.ReverseFind('.')-1);
		ext.MakeLower();
		if (ext == "bmp") 
			success = m_Graph.DrawToFile(OutFileName,oc2dFormatBitmap);
		else if ((ext == "jpg") || (ext == "jpeg"))
			success = m_Graph.SaveImageAsJpeg(OutFileName,90,FALSE,TRUE,TRUE);

		MyMessageBox("Extension is "+ext);
		if (!success)
			{
				CString ErrStg;
				ErrStg.Format("Error writing output graph file '%s' specified on command line\n",OutFileName);
				MyMessageBox(ErrStg);
			}
	}

}


void COlectraGraphDlg::OnPrinter() 
{
	m_Graph.PrintChart(oc2dFormatBitmap, oc2dScaleToFit, 0,0,0,0);
	
}

void COlectraGraphDlg::OnFile() 
{
	CFileDialog MyFileDlg(FALSE,
							".bmp",
							NULL,
							OFN_OVERWRITEPROMPT,
							_T("Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|All Files|*.*||"));

	if (MyFileDlg.DoModal() == IDOK)
	{
		CString ext = MyFileDlg.GetFileExt();
		ext.MakeLower();
		if (ext == "bmp") 
			m_Graph.DrawToFile(MyFileDlg.GetPathName(),oc2dFormatBitmap);
		else if (ext == "jpg")
			m_Graph.SaveImageAsJpeg(MyFileDlg.GetPathName(),85,FALSE,TRUE,TRUE);

	}
	
}


void COlectraGraphDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}


void COlectraGraphDlg::OnOK() 
{
	//  When there are no more open graph dialogs, we reset the m_GraphDlgCount
	//  variable to zero.  This causes the dialog title bar numbering to restart
	//  at 1.
	m_OpenGraphDlgs--;
	if (m_OpenGraphDlgs <= 0)
	{
		m_GraphDlgCount = 0;
		m_OpenGraphDlgs = 0;
	}
	
	CDialog::OnOK();
}


void COlectraGraphDlg::OnClose() 
{
	//  When there are no more open graph dialogs, we reset the m_GraphDlgCount
	//  variable to zero.  This causes the dialog title bar numbering to restart
	//  at 1.
	m_OpenGraphDlgs--;
	if (m_OpenGraphDlgs <= 0)
	{
		m_GraphDlgCount = 0;
		m_OpenGraphDlgs = 0;
	}
	
	CDialog::OnClose();
}

BEGIN_EVENTSINK_MAP(COlectraGraphDlg, CDialog)
	//{{AFX_EVENTSINK_MAP(COlectraGraphDlg)
	ON_EVENT(COlectraGraphDlg, IDC_OLECTRACHART, 1 /* ModifyEnd */, OnModifyEndOlectrachart, VTS_NONE)
	ON_EVENT(COlectraGraphDlg, IDC_OLECTRACHART, -605 /* MouseDown */, OnMouseDownOlectrachart, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(COlectraGraphDlg, IDC_OLECTRACHART, -606 /* MouseMove */, OnMouseMoveOlectrachart, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void COlectraGraphDlg::OnModifyEndOlectrachart() 
{
	m_Dragging = false;
	if (m_pMouseLabel != NULL)
	{
		m_pMouseLabel->DestroyWindow();
		delete m_pMouseLabel;
		m_pMouseLabel = NULL;
	}
}

void COlectraGraphDlg::OnMouseDownOlectrachart(short Button, short Shift, long X, long Y) 
{
	m_Dragging = true;
	double datax, datay;
	int Region = m_Graph.GetChartGroups().GetItem(COleVariant(long(1))).CoordToDataCoord(X,Y,&datax, &datay);
	if (Region == oc2dRegionInChartArea)
	{
		CString CoordStg;
		COleDateTime theDate = m_StartDate + COleDateTimeSpan(long(datax+0.5),0,0,0);
		if (datay > 100.0) CoordStg.Format(_T("%s, %4.0f"),theDate.Format("%m/%d/%Y"),datay);
		else if (datay > 10.0) CoordStg.Format(_T("%s, %4.1f"),theDate.Format("%m/%d/%Y"),datay);
		else if (datay > 1.0) CoordStg.Format(_T("%s, %4.2f"),theDate.Format("%m/%d/%Y"),datay);
		else if (datay > 0.1) CoordStg.Format(_T("%s, %4.3f"),theDate.Format("%m/%d/%Y"),datay);
		else  CoordStg.Format(_T("%s, %5.4f"),theDate.Format("%m/%d/%Y"),datay);
		MouseLabel(CoordStg,X,Y);
	}
}

void COlectraGraphDlg::OnMouseMoveOlectrachart(short Button, short Shift, long X, long Y) 
{
	if (m_Dragging)
	{
		double datax, datay;
		int Region = m_Graph.GetChartGroups().GetItem(COleVariant(long(1))).CoordToDataCoord(X,Y,&datax, &datay);
		if (Region == oc2dRegionInChartArea)
		{
			CString CoordStg;
			COleDateTime theDate = m_StartDate + COleDateTimeSpan(long(datax+0.5),0,0,0);
			if (datay > 100.0) CoordStg.Format(_T("%s, %4.0f"),theDate.Format("%m/%d/%Y"),datay);
			else if (datay > 10.0) CoordStg.Format(_T("%s, %4.1f"),theDate.Format("%m/%d/%Y"),datay);
			else if (datay > 1.0) CoordStg.Format(_T("%s, %4.2f"),theDate.Format("%m/%d/%Y"),datay);
			else if (datay > 0.1) CoordStg.Format(_T("%s, %4.3f"),theDate.Format("%m/%d/%Y"),datay);
			else  CoordStg.Format(_T("%s, %5.4f"),theDate.Format("%m/%d/%Y"),datay);
			MouseLabel(CoordStg,X,Y);
		}
	}
	
}

void COlectraGraphDlg::MouseLabel(CString LabelStg, double x, double y)
{
	CRect LabelRect(int(x-50),int(y-30),int(x),int(y));
	if (m_pMouseLabel == NULL) // Create label window if it doesn't exist
	{
		m_pMouseLabel = new CWnd;
		m_pMouseLabel->Create(	NULL,
								"Coords", 
								WS_VISIBLE|WS_CHILD,
								LabelRect,
								this,
								ID_MOUSELABEL );

			
		CFont theFont;
		LOGFONT LogFont;
		GetFont()->GetLogFont(&LogFont);
		theFont.CreatePointFont(80,"");
		m_pMouseLabel->GetDC()->SelectObject(&theFont);
	}

	CSize sz = m_pMouseLabel->GetDC()->GetTextExtent(LabelStg);
	CRect rect((int)x-sz.cx/2,(int)y-sz.cy/2,(int)x+sz.cx/2,(int)y+sz.cy/2);
	m_pMouseLabel->MoveWindow(rect);
	m_pMouseLabel->GetDC()->TextOut(0,0,LabelStg);
	ReleaseDC(m_pMouseLabel->GetDC());
}

void COlectraGraphDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (::IsWindow(m_Graph.GetSafeHwnd()))
	{
	CRect theRect;
	m_Graph.GetWindowRect(theRect);
	theRect.right = theRect.left + cx;
	theRect.bottom = theRect.top + cy;
//	m_Graph.MoveWindow(theRect);
	m_Graph.MoveWindow(0,0,cx,cy);
	}

	int i=0;
}

void COlectraGraphDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
