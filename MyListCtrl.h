#pragma once

//--------------- these two structs are used to define the default column layout
typedef enum {
	SORT_None = 0,
	SORT_AZ   = 1,
	SORT_ZA   = -1,
} SortOrder;

typedef struct {
	int       nId;
	int       nClmNum;
	int       nWidth;       // pixels
	SortOrder eSortOrder;
	LPSTR     pszName;
} ListClmInfo;

#define CNUM_MaxListClms 20
//------------------------------------------- the class object extends CListCtrl
class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();

	void SetupClms( ListClmInfo* parLCI, BOOL fSetFromSaved=FALSE, LPCSTR szKey=0 );

	void SaveClmInfo(     LPCSTR sKey ); 
	void UseSavedClmInfo( LPCSTR sKey ); 

	void EraseList();
	BOOL PutItem(int nItem,int nSubItem, int nVal,      BOOL fAddRow=FALSE );
	BOOL PutItem(int nItem,int nSubItem, LPCTSTR sItem, BOOL fAddRow=FALSE );

private:
	void SaveClmHdrInfo( LPCSTR sKey, int nClms, LPINT paiIndexes, LPINT paiWidths );
	BOOL ReadClmHdrInfo( LPCSTR sKey, int nClms, LPINT paiIndexes, LPINT paiWidths );

protected:
	DECLARE_MESSAGE_MAP()
};