#pragma once
#include "atlcomtime.h"
#include "gridctrl.h"
#include "gridcelldatetime.h"
#include "mypropsheet.h"
#include "afxwin.h"

// CResourcePage dialog

class CResourcePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CResourcePage)

public:
	CResourcePage(CWnd* pParent = NULL);
	virtual ~CResourcePage();

// Dialog Data
	enum { IDD = IDD_RESOURCE_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_PolFeedingDatesCtrl;
};
