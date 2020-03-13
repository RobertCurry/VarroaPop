#pragma once
#include "afx.h"

class DBD_ITEM 
{
    public:
    COleDateTime StartTime; 
};

class CDateBasedData : public CObject
{
public:
    CDateBasedData(void);
    ~CDateBasedData(void);
public:

    bool GetItem(int Index, DBD_ITEM& theItem);
    DBD_ITEM* GetItemPtr(int Index);
    bool GetActiveItem(COleDateTime theDate, DBD_ITEM& theItem);
    //bool GetActiveValue(COleDateTime theDate, double& theValue);
    virtual void AddItem(DBD_ITEM& theItem);
    //void AddItem(COleDateTime theStartTime, COleDateTime theEndTime, double theValue);
    void DeleteItem(int Index);
    int GetCount();
    void ClearAll();
    void Serialize(CArchive& ar) = 0;
    void Copy(CDateBasedData* pDestination);
    bool IsEnabled() {return m_Enabled;}
    void SetEnabled(bool EnableVal) {m_Enabled = EnableVal;}
    
    
protected:
    CObList* pItemList;
    bool m_Enabled;
};
