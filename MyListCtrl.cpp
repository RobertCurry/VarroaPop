#include "stdafx.h"
#include "MyListCtrl.h"


// MyListCtrl.cpp : implementation file
//
IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

//----------------------------------- nothing special for the ctor or dtor
CMyListCtrl::CMyListCtrl(){}
CMyListCtrl::~CMyListCtrl(){}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Save and restore user prefs on column positions and widths
// to the registry key for this program
//
void CMyListCtrl::SaveClmInfo( LPCSTR szKey ) 
{
	HDITEM rHdr;

	int nClmCnt= GetHeaderCtrl()->GetItemCount();

	int anClmIdxs[   CNUM_MaxListClms ];
	int anClmWidths[ CNUM_MaxListClms ];

	for (int j=0; j< nClmCnt; j++ ) {
		rHdr.mask= HDI_WIDTH | HDI_ORDER;
		GetHeaderCtrl()->GetItem(j, &rHdr);	
		anClmIdxs[   j ] = rHdr.iOrder;
		anClmWidths[ j ] = rHdr.cxy;
	}
	SaveClmHdrInfo( szKey, nClmCnt, anClmIdxs, anClmWidths );
}

void CMyListCtrl::UseSavedClmInfo( LPCSTR szKey ) 
{
	HDITEM rHdr;

	int nClmCnt= GetHeaderCtrl()->GetItemCount();

	int anClmIdxs[   CNUM_MaxListClms ];
	int anClmWidths[ CNUM_MaxListClms ];

	if ( ReadClmHdrInfo( szKey, nClmCnt, anClmIdxs, anClmWidths ) ) {
		for (int j=0; j< nClmCnt; j++ ) {
			if ( anClmWidths[j] == 0 ) anClmWidths[j]= 5;

			rHdr.mask= HDI_WIDTH | HDI_ORDER;
			rHdr.iOrder= anClmIdxs[   j ];
			rHdr.cxy   = anClmWidths[ j ];
			GetHeaderCtrl()->SetItem(j, &rHdr);	
		}
	}
}
//-------------------------------------------
// private functions used by the above to get data from registry
//
void CMyListCtrl::SaveClmHdrInfo( LPCSTR szKey, int nClms, LPINT paiIndexes, LPINT paiWidths )
{
	CString sAll="";
	CString sOne;
	if (szKey==0) szKey= "ListClms";
	
	for (int j=0; j< nClms; j++ ) {
		sOne.Format("%d,", *paiIndexes++ );
		sAll += sOne;
		sOne.Format("%d, ", *paiWidths++ );
		sAll += sOne;
	}
	AfxGetApp()->WriteProfileString("Prefs", szKey, sAll);
}
BOOL CMyListCtrl::ReadClmHdrInfo( LPCSTR szKey, int nClms, LPINT paiIndexes, LPINT paiWidths )
{
	if (szKey==0) szKey= "ListClms";

	CString sAll= AfxGetApp()->GetProfileString("Prefs", szKey );
	if (sAll.IsEmpty()) {
		return FALSE;
	}
	sAll += "                                ";
	int nCurPos=0;
	for (int j=0; j<nClms; j++ ) {

		*paiIndexes++ = atoi( sAll.Mid( nCurPos ) );
		nCurPos= sAll.Find( ',', nCurPos ) +1; // get past the comma
		if ( nCurPos <= 0) return( FALSE );

		*paiWidths++  = atoi( sAll.Mid( nCurPos ) );
		nCurPos= sAll.Find( ',', nCurPos ) +1; // get past the comma
		if ( nCurPos <= 0) return( FALSE );
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Generic utilities simplify use of ListView control

//------------------------------------------------------------------	
// Set up the columns -- given title, order and widths from a setup array
//
void CMyListCtrl::SetupClms( ListClmInfo* parLCI, BOOL fSetFromSaved /*=FALSE*/, LPCSTR szKey /*=0*/ )
{
	LV_COLUMN  rLVC;

	EraseList();  // delete all columns

	while (parLCI->nId != -1 ) {
		rLVC.mask=     LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		rLVC.cx=       parLCI->nWidth;     // in pixels
		rLVC.iSubItem= parLCI->nId;        // 
		rLVC.pszText=  parLCI->pszName;
		rLVC.fmt=      LVCFMT_LEFT;

		int nRet= InsertColumn( parLCI->nClmNum, &rLVC );
		parLCI++;
	}
	if ( fSetFromSaved ) {
		UseSavedClmInfo( szKey );
	}
}

void CMyListCtrl::EraseList()
{
	DeleteAllItems();
	while( DeleteColumn(0) )
		;
	UpdateWindow();
}
//--------------------------------------
// Used in populating the list -- overloads given for an int or a string value
// Use fAddRow=TRUE when populating the first column in the new row
//
BOOL CMyListCtrl::PutItem(int nItem,int nSubItem, int nVal, BOOL fAddRow /*=FALSE*/ )
{
	CString str;  str.Format("%d",nVal);
	return( PutItem(nItem, nSubItem, str) );
}

BOOL CMyListCtrl::PutItem(int nItem,int nSubItem, LPCTSTR sItem, BOOL fAddRow /*=FALSE*/)
{
	LV_ITEM         lvItm;
	lvItm.mask=     LVIF_TEXT;
	lvItm.iItem=    nItem;
	lvItm.iSubItem= nSubItem;
	lvItm.pszText=  (LPSTR)sItem;
	if( nSubItem == 0 && fAddRow ) {
		return InsertItem( &lvItm );
	}
	return SetItem( &lvItm );
}