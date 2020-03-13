#pragma once


// CPesticideAdd dialog

class CPesticideAdd : public CDialog
{
	DECLARE_DYNAMIC(CPesticideAdd)

public:
	CPesticideAdd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPesticideAdd();

// Dialog Data
	enum { IDD = IDD_PESTICIDE_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	double m_AdultSlope;
	double m_AdultLD50;
	double m_AdultSlope_Contact;
	double m_AdultLD50_Contact;
	double m_LarvaSlope;
	double m_LarvaLD50;
	double m_KOW;
	double m_KOC;
	double m_HalfLife;
	double m_ContactFactor;
};
