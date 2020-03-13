#include "stdafx.h"
#include "DateBasedData.h"

CDateBasedData::CDateBasedData(void)
{
    pItemList = new CObList();
    SetEnabled(true);
}

CDateBasedData::~CDateBasedData(void)
{
    ClearAll();
    delete pItemList;
}

bool CDateBasedData::GetItem(int Index, DBD_ITEM& theItem)
{
    bool Success = false;
    if ((Index >=0) && (Index < GetCount()))
    {
        DBD_ITEM* pItem = (DBD_ITEM*)pItemList->GetAt(pItemList->FindIndex(Index));
        theItem = *pItem; // Default assignment - requires ITEMs be made up of simple types.
        Success = true;
    }   
    return Success;
}

DBD_ITEM* CDateBasedData::GetItemPtr(int Index)
{
    if (Index >= pItemList->GetCount()) return NULL;
    else return (DBD_ITEM*)pItemList->GetAt(pItemList->FindIndex(Index));
}

bool CDateBasedData::GetActiveItem(COleDateTime theDate, DBD_ITEM& theItem)
{

// Needs to be rewritten - what is an active item for a Pesticide Event?  Need to define.  Do we even use this?

//  theItem is filled with the contents of the first list item where theDate is between the DR_ITEM start date and end date, 
//  and the function return value is TRUE.  If theDate does not fall between the date range in any 
//  list item, the return value is FALSE.

    bool Success = false;    
    //DR_ITEM* pListItem;
    //POSITION pos = pItemList->GetHeadPosition();
    //while (pos != NULL)
    //{   
    //    pListItem = (DR_ITEM*)pItemList->GetNext(pos);
    //            
    //    COleDateTimeSpan TimeSinceStart = (COleDateTimeSpan)(theDate - pListItem->StartTime);
    //    COleDateTimeSpan TimeTillEnd = (COleDateTimeSpan)(pListItem->EndTime - theDate);
    //    
    //        int month = theDate.GetMonth();
    //        int day = theDate.GetDay();
    //        int year = theDate.GetYear();
    //        
    //        if ((month >= 3) && (day >= 2) && (year == 1999))
    //        {
    //            int i = 0;
    //            
    //        }
    //    
    //    
    //    if ((TimeSinceStart.GetDays() >= 0) && (TimeTillEnd.GetDays() >= 0))
    //    {
    //        theItem.EndTime = pListItem->EndTime;
    //        theItem.StartTime = pListItem->StartTime;
    //        theItem.Value = pListItem->Value;
    //        Success = true;
    //        break;
    //     }
    // }
    return Success;
}



void CDateBasedData::AddItem(DBD_ITEM& theItem)
{
    DBD_ITEM* pItem = new DBD_ITEM();  
    //pItem->EndTime = theItem.EndTime;
    //pItem->StartTime = theItem.StartTime;
    //pItem->Value = theItem.Value;
    pItemList->AddTail((CObject*)pItem);
}


void CDateBasedData::DeleteItem(int Index)
{
    if (Index <0) return;
    POSITION pos = pItemList->FindIndex(Index);    
    if (pos != NULL) // Validate that Index is valid
    {
        delete(DBD_ITEM*)(pItemList->GetAt(pos)); // Deletes the item
        pItemList->RemoveAt(pos);      // Removed item pointer from list
    }
}

int CDateBasedData::GetCount()
{
    if (!pItemList) return 0;
    else return pItemList->GetCount();
}

void CDateBasedData::ClearAll()
{
    DBD_ITEM* pItem;
    if (pItemList != NULL)
    {
        while (pItemList->GetCount() > 0) 
        {
            pItem = (DBD_ITEM*)pItemList->RemoveTail();
            delete(pItem);
        }
    }
}


void CDateBasedData::Copy(CDateBasedData* pDestination)
{
    if (pDestination == NULL) return;
    pDestination->ClearAll();
    DBD_ITEM theItem;
    
    for (int i = 0; i < GetCount(); i++)
    {
        GetItem(i, theItem);
        pDestination->AddItem(theItem);
    } 
    pDestination->SetEnabled(IsEnabled());  
}

