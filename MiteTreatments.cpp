// MiteTreatments.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "MiteTreatments.h"


// CMiteTreatments

CMiteTreatments::CMiteTreatments()
{
    pTreatmentItemList = new CObList();
}

CMiteTreatments::~CMiteTreatments()
{
    // Clean up pointers
    ClearAll();
    delete pTreatmentItemList;
}

void CMiteTreatments::Serialize(CArchive &ar)
{
    int ItemQuan;
    if (ar.IsLoading())
    {
        ar >> ItemQuan;
        ClearAll();
        for (int i = 0; i < ItemQuan; i++)
        {
            CMiteTreatmentItem* ptheItem = new CMiteTreatmentItem;
            ptheItem->Serialize(ar);
            pTreatmentItemList->AddTail(ptheItem);
        }
    }
    else
    {
        ItemQuan = (int)pTreatmentItemList->GetCount();
        ar << ItemQuan;
        CMiteTreatmentItem* ptheItem;
        POSITION pos = pTreatmentItemList->GetHeadPosition();
        for (int i = 0; i < ItemQuan; i++)
        {
            ptheItem = (CMiteTreatmentItem*)pTreatmentItemList->GetNext(pos);
            ptheItem->Serialize(ar);
        }    
    }
    


}

bool CMiteTreatments::GetItem(int Index, CMiteTreatmentItem& theItem)
{
    POSITION pos;
    if ((pos = pTreatmentItemList->FindIndex(Index)) == NULL) return false;
    else
    {
        CMiteTreatmentItem* pListItem = (CMiteTreatmentItem*)pTreatmentItemList->GetAt(pos);
        theItem.Duration = pListItem->Duration;
        theItem.PctMortality = pListItem->PctMortality;
        theItem.PctResistant = pListItem->PctResistant;
        theItem.theStartTime = pListItem->theStartTime;
        return true;
    }
}


bool CMiteTreatments::GetItem(COleDateTime theDate, CMiteTreatmentItem& theItem)
{
    bool Result = false;
    POSITION pos;
    CMiteTreatmentItem* pListItem;
    if ((pos = pTreatmentItemList->GetHeadPosition())!= NULL)
    {
        while (pos != NULL)
        {
            pListItem = (CMiteTreatmentItem*)pTreatmentItemList->GetNext(pos);
            if ((pListItem->theStartTime.GetDay() == theDate.GetDay()) &&
                (pListItem->theStartTime.GetMonth() == theDate.GetMonth()) &&
                 (pListItem->theStartTime.GetYear() == theDate.GetYear()))
            {
                theItem.Duration = pListItem->Duration;
                theItem.PctMortality = pListItem->PctMortality;
                theItem.PctResistant = pListItem->PctResistant;
                theItem.theStartTime = pListItem->theStartTime;
                Result = true;
             }
         }
    }
    return Result;
}

bool CMiteTreatments::GetActiveItem(COleDateTime theDate, CMiteTreatmentItem& theItem)
{
    // If theDate is between theStartTime and theStartTime + Duration weeks, the function fills theItem with the corresponding 
    // MiteTreatmentItem and returns TRUE.  If theDate falls outside of all ranges, the return value if FALSE.  If there are overlapping
    // treatment periods, the first one encountered is used.
    bool Result = false;
    CMiteTreatmentItem* pListItem;
    POSITION pos = pTreatmentItemList->GetHeadPosition();
	if (!(theDate.GetStatus() == COleDateTime::valid) || (!theItem.IsValid())) return false;
    while (pos != NULL)
    {   
        pListItem = (CMiteTreatmentItem*)pTreatmentItemList->GetNext(pos);
		if (pListItem->IsValid())
		{
			COleDateTime theEndTime = pListItem->theStartTime + COleDateTimeSpan(pListItem->Duration*7,0,0,0);
	                
			COleDateTimeSpan TimeSinceStart = theDate - pListItem->theStartTime;
			COleDateTimeSpan TimeTillEnd = theEndTime - theDate;
	        
			if ((TimeSinceStart.GetDays() >= 0) && (TimeTillEnd.GetDays() > 0))
			{
				theItem.Duration = pListItem->Duration;
				theItem.PctMortality = pListItem->PctMortality;
				theItem.PctResistant = pListItem->PctResistant;
				theItem.theStartTime = pListItem->theStartTime;
				Result = true;
				break;
			}
		}
     }
    return Result;
}

CMiteTreatmentItem* CMiteTreatments::GetItemPtr(int Index)
{
    if (Index < 0) return NULL;  // Prevent exception being generated
    POSITION pos = pTreatmentItemList->FindIndex(Index);
    return (CMiteTreatmentItem*)pTreatmentItemList->GetAt(pos);
}

void CMiteTreatments::AddItem(CMiteTreatmentItem& theItem)
{
    CMiteTreatmentItem* pNewItem = new CMiteTreatmentItem;
    pNewItem->theStartTime = theItem.theStartTime;
    pNewItem->Duration = theItem.Duration;
    pNewItem->PctMortality = theItem.PctMortality;
    pNewItem->PctResistant = theItem.PctResistant;
    pTreatmentItemList->AddTail((CObject*)pNewItem);
    
}

void CMiteTreatments::AddItem(COleDateTime theTime, UINT theDuration, double thePctMortality, double thePctResistant)
{
    CMiteTreatmentItem* pNewItem = new CMiteTreatmentItem;
    pNewItem->theStartTime = theTime;
    pNewItem->Duration = theDuration;
    pNewItem->PctMortality = thePctMortality;
    pNewItem->PctResistant = thePctResistant;
    pTreatmentItemList->AddTail((CObject*)pNewItem);
}

void CMiteTreatments::DeleteItem(int Index)
{
    POSITION pos, pos2;
    CMiteTreatmentItem* pItem;
    pos = pTreatmentItemList->GetHeadPosition();
    for (int i = 0; pos != NULL; i++)
    {
        pos2 = pos;
        pItem = (CMiteTreatmentItem*)pTreatmentItemList->GetNext(pos);    
        if (i == Index)
        {
            if (pos == NULL) // Deleting the last item in the list
                pItem = (CMiteTreatmentItem*)pTreatmentItemList->RemoveTail();
            else
                pTreatmentItemList->RemoveAt(pos2);
            if (pItem != NULL) delete pItem;
            break;
        }
    }
}


void CMiteTreatments::ClearAll()
{
    while (pTreatmentItemList->GetCount() > 0)
    {
        delete(pTreatmentItemList->RemoveTail());
    }
}

bool CMiteTreatments::IsNewTreatmentStarting(COleDateTime theDate)
{
    bool Results = false;
    CMiteTreatmentItem* pItem;
    for (POSITION pos = pTreatmentItemList->GetHeadPosition(); pos != NULL;)
    {
        pItem = (CMiteTreatmentItem*)pTreatmentItemList->GetNext(pos);
        if ((pItem->theStartTime.GetDay() == theDate.GetDay()) ||
            (pItem->theStartTime.GetMonth() == theDate.GetMonth()) ||
            (pItem->theStartTime.GetYear() == theDate.GetYear()))
        {
            Results = true;
            break;
        }
    }
    return Results;
}
// CMiteTreatments member functions
