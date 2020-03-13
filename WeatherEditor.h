#if !defined(AFX_WEATHEREDITOR_H__802B8844_FE00_11D6_B486_00A024E290BA__INCLUDED_)
#define AFX_WEATHEREDITOR_H__802B8844_FE00_11D6_B486_00A024E290BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeatherEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWeatherEditor dialog
//
// Responsibilities:
// 
// Loads the dialog with data from a CWeatherEvents object.  Updates the object
// as instructed in the control.  Sets the m_Modified flag to true if any changes
// are made
//
// Calling function handles storage of changed object
//
/////////////////////////////////////////////////////////////////////////////

class CWeatherEditor : public CDialog
{
// Construction
public:
	~CWeatherEditor();
	CWeatherEvents* m_pWeather;
	void SetWeather(CWeatherEvents* pWeather);
	CWeatherEvents* GetWeather();
	bool IsModified();
	CWeatherEditor(CWnd* pParent = NULL);   // standard constructor
	CWeatherEditor(CWeatherEvents* pWeather, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CWeatherEditor)
	enum { IDD = IDD_WEATHEREDIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeatherEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_Modified;

	// Generated message map functions
	//{{AFX_MSG(CWeatherEditor)
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEATHEREDITOR_H__802B8844_FE00_11D6_B486_00A024E290BA__INCLUDED_)
