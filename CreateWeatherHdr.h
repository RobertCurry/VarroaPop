#if !defined(AFX_CREATEWEATHERHDR_H__63353A00_C741_11D2_8918_44455354616F__INCLUDED_)
#define AFX_CREATEWEATHERHDR_H__63353A00_C741_11D2_8918_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateWeatherHdr.h : header file
//
#include "WeatherEvents.h"
/////////////////////////////////////////////////////////////////////////////
// CCreateWeatherHdr dialog

class CCreateWeatherHdr : public CDialog
{
// Construction
public:
	CCreateWeatherHdr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateWeatherHdr)
	enum { IDD = IDD_CREATE_HEADER };
	int		m_AveTempCol;
	int		m_Interval;
	int		m_MaxTempCol;
	int		m_MinTempCol;
	int		m_Farenheight;
	int		m_RainCol;
	CString	m_SimDate;
	CString	m_SimTime;
	CString	m_WBeginDate;
	CString	m_WBeginTime;
	CString	m_WEndDate;
	CString	m_WEndTime;
	int		m_WindspeedCol;
	int		m_Hourly;
	int		m_SolarRad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateWeatherHdr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetHeaderStrings(CStringArray& Strings, CWeatherFile* pWeather);


protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateWeatherHdr)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRadiominmax();
	afx_msg void OnRadiohourly();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEWEATHERHDR_H__63353A00_C741_11D2_8918_44455354616F__INCLUDED_)
