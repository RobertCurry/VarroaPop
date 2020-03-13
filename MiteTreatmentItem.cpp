#include "stdafx.h"
#include "MiteTreatmentItem.h"

CMiteTreatmentItem::CMiteTreatmentItem(void)
{
}

CMiteTreatmentItem::~CMiteTreatmentItem(void)
{
}

void CMiteTreatmentItem::Serialize(CArchive& ar) 
{
	CTime theTime;
	if (ar.IsStoring())
	{	// storing code
		//  TODO: this is a temp fix to help work thru converting from COleDateTime to CTime everywhere
		SYSTEMTIME st;
		theStartTime.GetAsSystemTime(st);
		theTime = CTime(st);
		//ar << theStartTime;
		ar << theTime;
		ar << Duration;
		ar << PctMortality;
		ar << PctResistant;
	}
	else
	{	// loading code
		//  TODO: this is a temp fix to help work thru converting from CTime to COleDateTime everywhere
		ar >> theTime;
		theStartTime = theTime.GetTime();
		//ar >> theStartTime;
		ar >> Duration;
		ar >> PctMortality;
		ar >> PctResistant;
	}
}

bool CMiteTreatmentItem::IsValid()
{
	bool valid = false;
	valid = (theStartTime.GetStatus() == COleDateTime::valid);
	

	return valid;
}
