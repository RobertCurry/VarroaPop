// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "varroapop.h"
#include "vcdatagrid.h"
#include "vccategoryscale.h"
#include "vcplot.h"
#include "vcaxis.h"
#include "GraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphDlg dialog


CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGraphDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphDlg)
	DDX_Control(pDX, IDC_MSCHART1, m_Chart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	//{{AFX_MSG_MAP(CGraphDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDlg message handlers

BOOL CGraphDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DoChart();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGraphDlg::DoChart()
{
	int rows = m_pData->Row(), columns = m_pData->Col();
	int i,j;
	m_Chart.SetColumnCount((columns-1)*2);
	m_Chart.SetRowCount(rows);
	m_Chart.GetPlot().SetUniformAxis(false);
	CVcCategoryScale scale = 
		m_Chart.GetPlot().GetAxis(1,COleVariant((short)1)).GetCategoryScale();
	scale.SetAuto(true);


	CVcDataGrid MyGrid = m_Chart.GetDataGrid();
	
	/*
	The required format for the CVcDataGrid object is:

		Column 1 = Day Number
		Column 2 = Data Value for Series 1
		Column 3 = Day Number
		Column 4 = Data Value for Series 2
		  etc.
	*/
	double DataPoint;
	for (i=0;i<(columns-1)*2;i+=2)
	{
		for (j=0;j<rows;j++)
		{
			MyGrid.SetData(j+1,i+1,m_pData->Get(0,j),0);
			DataPoint = m_pData->Get((i/2)+1,j);
			MyGrid.SetData(j+1,i+2,DataPoint,0);
		}
		if (!m_pLabels->IsEmpty())  // Set the series labels
		{
			m_Chart.SetColumn(i+1);
			m_Chart.SetColumnLabel(m_pLabels->RemoveHead());
		}
	}
	/*  STILL WORKING ON GETTING X_AXIS LABELS TO BE DATES
	short k = 1;
	CString outstg;
	CString tempstg;
	CString Gridlabel;
	m_Chart.SetColumn(1);
	m_Chart.SetRowLabelCount(m_Chart.GetRowCount());
	m_Chart.SetRowLabelIndex(2);
	while ((!m_pAxisLabels->IsEmpty())&&(k<=m_Chart.GetRowCount()))
	{
		Gridlabel = MyGrid.GetRowLabel(k,2);
		m_Chart.SetRow(k);
		tempstg=m_Chart.GetRowLabel();
		outstg = m_pAxisLabels->RemoveHead();
		m_Chart.SetRowLabel(LPCTSTR(outstg));
		MyGrid.SetRowLabel(k,2,outstg);
		tempstg=m_Chart.GetRowLabel();
		k++;
	}
	*/
}
