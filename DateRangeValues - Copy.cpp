// DateRangeValues.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "DateRangeValues.h"


// CDateRangeValue
/*
    The data in this class represent a list of date ranges and an associated value to apply in that range.  
    The public accessor functions allow retrieval of the value associated with a date range.  If 
    the date ranges from two separate DateRangeValue items overlap, the accessor returns the value of the first one.
*/

CDateRangeValues::CDateRangeValues()
{
    pItemList = new CObList();
    SetEnabled(true);
}

CDateRangeValues::~CDateRangeValues()
{
    ClearAll();
    delete pItemList;
}

CDateRangeValues& CDateRangeValues::operator = (CDateRangeValues& DRV)
{
    DRV.Copy(this);
    return *this;
}

void CDateRangeValues::Copy(CDateRangeValues* pDestination)
{
    if (pDestination == NULL) return;
    pDestination->ClearAll();
    DR_ITEM theItem;
    
    for (int i = 0; i < GetCount(); i++)
    {
        GetItem(i, theItem);
        pDestination->AddItem(theItem);
    } 
    pDestination->SetEnabled(IsEnabled());  
}

int CDateRangeValues::GetCount()
{
    if (!pItemList) return 0;
    else return pItemList->GetCount();
}


bool CDateRangeValues::GetItem(int Index, DR_ITEM& theItem)
{
    bool Success = false;
    if ((Index >=0) && (Index < GetCount()))
    {
        DR_ITEM* pItem = (DR_ITEM*)pItemList->GetAt(pItemList->FindIndex(Index));
        theItem.EndTime = pItem->EndTime;
        theItem.StartTime = pItem->StartTime;
        theItem.Value = pItem->Value;
        Success = true;
    }   
    return Success;
}

DR_ITEM* CDateRangeValues::GetItemPtr(int Index)
{
    if (Index >= pItemList->GetCount()) return NULL;
    else return (DR_ITEM*)pItemList->GetAt(pItemList->FindIndex(Index));
}


bool CDateRangeValues::GetActiveItem(COleDateTime theDate, DR_ITEM& theItem)
{
//  theItem is filled with the contents of the first list item where theDate is between the DR_ITEM start date and end date, 
//  and the function return value is TRUE.  If theDate does not fall between the date range in any 
//  list item, the return value is FALSE.

    bool Success = false;    
    DR_ITEM* pListItem;
    POSITION pos = pItemList->GetHeadPosition();
    while (pos != NULL)
    {   
        pListItem = (DR_ITEM*)pItemList->GetNext(pos);
                
        COleDateTimeSpan TimeSinceStart = (COleDateTimeSpan)(theDate - pListItem->StartTime);
        COleDateTimeSpan TimeTillEnd = (COleDateTimeSpan)(pListItem->EndTime - theDate);
        
            int month = theDate.GetMonth();
            int day = theDate.GetDay();
            int year = theDate.GetYear();
            
            if ((month >= 3) && (day >= 2) && (year == 1999))
            {
                int i = 0;
                
            }
        
        
        if ((TimeSinceStart.GetDays() >= 0) && (TimeTillEnd.GetDays() >= 0))
        {
            theItem.EndTime = pListItem->EndTime;
            theItem.StartTime = pListItem->StartTime;
            theItem.Value = pListItem->Value;
            Success = true;
            break;
         }
     }
    return Success;
}

bool CDateRangeValues::GetActiveValue(COleDateTime theDate, double& theValue)
{
    DR_ITEM theItem;
    bool Found = GetActiveItem(theDate,theItem); 
    if (Found) theValue = theItem.Value;
    return Found;
}


void CDateRangeValues::AddItem(DR_ITEM& theItem)
{
    DR_ITEM* pItem = new DR_ITEM();  
    pItem->EndTime = theItem.EndTime;
    pItem->StartTime = theItem.StartTime;
    pItem->Value = theItem.Value;
    pItemList->AddTail((CObject*)pItem);
}

void CDateRangeValues::AddItem(COleDateTime theStartTime, COleDateTime theEndTime, double theValue)
{
    DR_ITEM Item;
    Item.EndTime = theEndTime;
    Item.StartTime = theStartTime;
    Item.Value = theValue;   
    AddItem(Item);
}

void CDateRangeValues::DeleteItem(int Index)
{
    if (Index <0) return;
    POSITION pos = pItemList->FindIndex(Index);    
    if (pos != NULL) // Validate that Index is valid
    {
        delete(DR_ITEM*)(pItemList->GetAt(pos)); // Deletes the item
        pItemList->RemoveAt(pos);      // Removed item pointer from list
    }
}

void CDateRangeValues::ClearAll()
{
    DR_ITEM* pItem;
    if (pItemList != NULL)
    {
        while (pItemList->GetCount() > 0) 
        {
            pItem = (DR_ITEM*)pItemList->RemoveTail();
            delete(pItem);
        }
    }
}

void CDateRangeValues::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        DR_ITEM* pItem;
        int itemcount = pItemList->GetCount();
        ar << itemcount;  // Store value, even if 0
        if (itemcount>0)
        {
            POSITION pos = pItemList->GetHeadPosition();
            while (pos != NULL)
            {
                pItem = (DR_ITEM*)pItemList->GetNext(pos);
                ar << pItem->StartTime;
                ar << pItem->EndTime;
                ar << pItem->Value;
            }
        }
    }
    else
    {
        ClearAll();
        int itemcount;
        DR_ITEM theItem;
        ar >> itemcount;
        for (int i = 0; i < itemcount; i++)
        {
            ar >> theItem.StartTime;
            ar >> theItem.EndTime;
            ar >> theItem.Value;
            AddItem(theItem);
        }
        
    }
    
}



// CDateRangeValue member functions
