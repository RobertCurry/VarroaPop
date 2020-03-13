// FlyList.cpp: implementation of the CFlyList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "whitefly.h"
#include "FlyList.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


double CFlyList::GetQuantity( double MinAge)
{
	// Returns the number of flys in the list that are older than
	// MinAge.  Note the default value for MinAge is -1 which returns
	// the quantity of all flys in the list

	double TotalCount = 0.0;
	POSITION pos = GetHeadPosition();
	while (pos != NULL)
	{
		CInsect* theFly = (CInsect*)GetNext(pos);
		if (theFly->GetAge()>= MinAge) TotalCount += theFly->GetQuantity();
	}
	return TotalCount;
}


CInsect* CFlyList::RemoveMatures()
{
	// Removes the tail and returns it if the age of that boxcar is >= Maximum Age
	// If the age is not not >= Maximum, NULL is returned
	if (IsEmpty()) return NULL;
	else
	{
		CInsect* theFly = (CInsect*)GetTail();
		if (theFly->GetAge() >= m_MaxAgeDD)
		{
			theFly = (CInsect*)RemoveTail();
		}
		else theFly = NULL;
		return theFly;
	}
}

