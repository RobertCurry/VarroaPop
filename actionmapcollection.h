#if !defined(AFX_ACTIONMAPCOLLECTION_H__12DF2547_6AF6_11D4_AC53_444553546170__INCLUDED_)
#define AFX_ACTIONMAPCOLLECTION_H__12DF2547_6AF6_11D4_AC53_444553546170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CActionMap;

/////////////////////////////////////////////////////////////////////////////
// CActionMapCollection wrapper class

class CActionMapCollection : public COleDispatchDriver
{
public:
	CActionMapCollection() {}		// Calls COleDispatchDriver default constructor
	CActionMapCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CActionMapCollection(const CActionMapCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	LPDISPATCH GetParent();

// Operations
public:
	CActionMap Add(short Message, short Modifier, short Keycode, long Action);
	void Remove(short Message, short Modifier, short Keycode);
	CActionMap GetItem(short Message, short Modifier, short Keycode);
	void RemoveAll();
	void Reset();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONMAPCOLLECTION_H__12DF2547_6AF6_11D4_AC53_444553546170__INCLUDED_)
