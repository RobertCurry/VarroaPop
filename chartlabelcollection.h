#if !defined(AFX_CHARTLABELCOLLECTION_H__12DF254F_6AF6_11D4_AC53_444553546170__INCLUDED_)
#define AFX_CHARTLABELCOLLECTION_H__12DF254F_6AF6_11D4_AC53_444553546170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CChartLabel;

/////////////////////////////////////////////////////////////////////////////
// CChartLabelCollection wrapper class

class CChartLabelCollection : public COleDispatchDriver
{
public:
	CChartLabelCollection() {}		// Calls COleDispatchDriver default constructor
	CChartLabelCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartLabelCollection(const CChartLabelCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);

// Operations
public:
	CChartLabel GetItem(const VARIANT& Index);
	CChartLabel Add();
	void Remove(const VARIANT& Index);
	void RemoveAll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTLABELCOLLECTION_H__12DF254F_6AF6_11D4_AC53_444553546170__INCLUDED_)
