// InsectList.cpp: implementation of the CInsectList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "whitefly.h"
#include "InsectList.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInsectList::CInsectList()
{

}


CInsectList::~CInsectList()
{
	Clear();
}

CInsectList::CInsectList(int Age)
{
	m_MaxAgeDD = Age;
}


void CInsectList::Clear()
{
	while (!IsEmpty()) delete (RemoveHead());
	RemoveAll();
}


void CInsectList::Populate(int Boxcars, double Quantity)
//  Creates initial list with "Boxcars" elements. each element given
//  an equal portion of "Quantity" flys
{
	ASSERT(Boxcars>0); 
	Clear();
	int count;
	CInsect* theInsect;
	double AgeIncrement = GetMaxAge()/Boxcars;
	double QuantityIncrement = Quantity/Boxcars;
	for (count = 0; count < Boxcars; count++)
	{
		theInsect = new CInsect;
		theInsect->SetAge(AgeIncrement*count);
		theInsect->SetQuantity(QuantityIncrement);
		AddTail(theInsect);
	}
}


double CInsectList::GetQuantity()
{
	double TotalCount = 0.0;
	POSITION pos = GetHeadPosition();
	while (pos != NULL)
	{
		CInsect* theInsect = (CInsect*)GetNext(pos);
		TotalCount += theInsect->GetQuantity();
	}
	return TotalCount;
}

void CInsectList::IncrementAge(double DegreeDays)
{
	POSITION pos = GetHeadPosition();
	while (pos != NULL)
	{
		CInsect* theInsect = (CInsect*)GetNext(pos);
		theInsect->SetAge(theInsect->GetAge() + DegreeDays);
	}
}


void CInsectList::ChangeQuantity( double DeltQuant)
{
	if (IsEmpty()) return;
	int boxcars = GetCount();
	POSITION pos = GetHeadPosition();
	CInsect* theInsect;
	while (pos != NULL)
	{
		theInsect = (CInsect*)GetNext( pos );
		theInsect->SetQuantity(theInsect->GetQuantity()+DeltQuant/boxcars);
	}
}