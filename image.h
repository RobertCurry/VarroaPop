#if !defined(AFX_IMAGE_H__12DF2531_6AF6_11D4_AC53_444553546170__INCLUDED_)
#define AFX_IMAGE_H__12DF2531_6AF6_11D4_AC53_444553546170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CImage wrapper class

class CImage : public COleDispatchDriver
{
public:
	CImage() {}		// Calls COleDispatchDriver default constructor
	CImage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CImage(const CImage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	CString GetFileName();
	void SetFileName(LPCTSTR);
	long GetLayout();
	void SetLayout(long);
	BOOL GetIsEmbedded();
	void SetIsEmbedded(BOOL);
	BOOL GetHasMinimumSize();
	void SetHasMinimumSize(BOOL);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE_H__12DF2531_6AF6_11D4_AC53_444553546170__INCLUDED_)
