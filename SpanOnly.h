#pragma once
#include "gridctrl.h"
#include "gridcelldatetime.h"
#include "gridcellcheck.h"
#include "DateRangeValues.h"


// CSpanOnly dialog

class CSpanOnly : public CDialog
{
	DECLARE_DYNAMIC(CSpanOnly)

public:
	CSpanOnly(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpanOnly();

// Dialog Data
	enum { IDD = IDD_SPAN_VALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    CGridCtrl m_DRV_GridCtrl;
    CString m_ValueTitle;
    CString m_DlgTitle;
    double m_UpperBound;
    double m_LowerBound;
    bool m_UpperBoundSet;
    bool m_LowerBoundSet;
	bool m_HasValue;  // If false, no value information is expected - just date spans
    void InitializeSpanGrid(void);
    void UpdateSpanGrid(void);
    bool Validate(double Value, bool Message=true);
    bool m_Validate;
    
public:
    CDateRangeValues* m_pDateRangeValues;
    bool GetSpanGridItem(int ItemNum, DR_ITEM& theItem);
    int GetNumSpanOnlyItems(void);
    void AddSpanOnlyItem(DR_ITEM& theItem);
    virtual BOOL OnInitDialog();
    void SetValueTitle(CString Title){m_ValueTitle = Title;} // Allows programmatic setting of the Value name
    void SetDlgTitle(CString Title) {m_DlgTitle = Title;}    // Allows programmatic setting of the dialog title (caption)
    afx_msg void OnBnClickedButtonAdd();
   // afx_msg void OnNMClickListTreatDates(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDateValueEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnBnClickedButtonDelItem();
    void SetUpperBoundVal(double UpperBound);
    void ClearUpperBoundVal() {m_UpperBoundSet = false;}
    void SetLowerBoundVal(double LowerBound);
    void ClearLowerBoundVal() {m_LowerBoundSet = false;}
    void SetValidation(bool DoValidate) {m_Validate = DoValidate;}
};
