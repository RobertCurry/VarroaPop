#pragma once
#include "afx.h"

class CMiteTreatmentItem :
    public CObject
{
public:

    COleDateTime theStartTime;
    UINT Duration;
    double PctMortality; // NOTE: Need to change logic in rest of program to treat this like a double (percentage)
    double PctResistant;
	void Serialize(CArchive &ar);
	bool IsValid();

    CMiteTreatmentItem(void);
    ~CMiteTreatmentItem(void);
};
