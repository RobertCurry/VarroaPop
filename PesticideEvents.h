#pragma once
#include "DateBasedData.h"

class PE_ITEM : public DBD_ITEM
{
    public:
    COleDateTime StartTime; 
    double Value1; 
    double Value2;
    double Value3;
    double Value4;
};

class CPesticideEvents :
    public CDateBasedData
{
    
public:

    bool GetActiveValue(COleDateTime theDate, double& theValue);
    void AddItem(COleDateTime theStartTime, COleDateTime theEndTime, double theValue);
    void Serialize(CArchive& ar);
    void Copy(CPesticideEvents* pDestination);
    using CDateBasedData::AddItem;
    
    
	
};
