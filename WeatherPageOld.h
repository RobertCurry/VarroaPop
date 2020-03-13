#if !defined(AFX_WEATHERPAGE_H__9A39CD01_891A_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_WEATHERPAGE_H__9A39CD01_891A_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WeatherPage.h : header file
//

#include "MyPropSheet.h"


/////////////////////////////////////////////////////////////////////////////
// CWeatherPage dialog

class CWeatherPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CWeatherPage)

// Construction
public:
	CWeatherPage();
	~CWeatherPage();

// Dialog Data
	//{{AFX_DATA(CWeatherPage)
	enum { IDD = IDD_WEATHER_FORM };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWeatherPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMyPropSheet* m_pMyPropSheet;
	CStringList m_WeatherFilesList;
	CString m_WeatherFileName;
	CString m_Dir;
	void rebuildFileList();

public:
	void setPropSheet(CMyPropSheet* theSheet) {m_pMyPropSheet = theSheet;}
	void prepComboBox();
	void SetWeatherFileName(CString fname) {m_WeatherFileName = fname;}
	CString GetWeatherFileName() {return m_WeatherFileName;}

protected:
	// Generated message map functions
	//{{AFX_MSG(CWeatherPage)
	afx_msg void OnHideButton();
	afx_msg void OnSelchangeWeatherFileList();
	afx_msg void OnButtonselect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEATHERPAGE_H__9A39CD01_891A_11D2_8D9A_0020AF233A70__INCLUDED_)
