#include "stdafx.h"
#include "VarroaPop.h"
#include "PesticideEvents.h"




bool CPesticideEvents::GetActiveValue(COleDateTime theDate, double& theValue)
{
    PE_ITEM theItem;
    bool Found = GetActiveItem(theDate,theItem); 
    //if (Found) theValue = theItem.Value;
    return Found;
}


void CPesticideEvents::AddItem(COleDateTime theStartTime, COleDateTime theEndTime, double theValue)
{
    PE_ITEM Item;
    //Item.EndTime = theEndTime;
    //Item.StartTime = theStartTime;
    //Item.Value = theValue;   
    AddItem(Item);
}


void CPesticideEvents::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        PE_ITEM* pItem;
        int itemcount = pItemList->GetCount();
        ar << itemcount;  // Store value, even if 0
        if (itemcount>0)
        {
            POSITION pos = pItemList->GetHeadPosition();
            while (pos != NULL)
            {
                pItem = (PE_ITEM*)pItemList->GetNext(pos);
                //ar << pItem->StartTime;
                //ar << pItem->EndTime;
                //ar << pItem->Value;
            }
        }
    }
    else
    {
        ClearAll();
        int itemcount;
        PE_ITEM theItem;
        ar >> itemcount;
        for (int i = 0; i < itemcount; i++)
        {
            //ar >> theItem.StartTime;
            //ar >> theItem.EndTime;
            //ar >> theItem.Value;
            AddItem(theItem);
        }
        
    }
}



