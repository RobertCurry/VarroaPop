#pragma once
#include "MiteTreatmentItem.h"

// CMiteTreatments command target

class CMiteTreatments : public CObject
{
protected:
    CObList* pTreatmentItemList;

public:
	CMiteTreatments();
	virtual ~CMiteTreatments();
	
    
    bool GetItem(int Index, CMiteTreatmentItem& theItem);
    bool GetItem(COleDateTime theDate, CMiteTreatmentItem& theItem);
    CMiteTreatmentItem* GetItemPtr(int Index);
    bool GetActiveItem(COleDateTime theDate, CMiteTreatmentItem& theItem);
    bool IsNewTreatmentStarting(COleDateTime theDate);
    void AddItem(CMiteTreatmentItem& theItem);
    void AddItem(COleDateTime theTime, UINT theDuration, double thePctMortality, double thePctResistant);
    void DeleteItem(int Index);
    int GetCount() {return pTreatmentItemList->GetCount();}
    void ClearAll();
    void Serialize(CArchive& ar);
};


