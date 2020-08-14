// Colony.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "Colony.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// List Class implementations

/////////////////////////////////////////////////////////////////////////////
//
// CBeelist - this is the base class for all bee list classes.  Contains the functions and attributes common to all
//
int CBeelist::DroneCount = 0;
int CBeelist::ForagerCount = 0;
int CBeelist::WorkerCount = 0;



CBeelist::~CBeelist()
{
	while (!IsEmpty()) delete (RemoveHead());
	RemoveAll();
}

int CBeelist::GetQuantity()
{
	int TotalCount=0;

	POSITION pos = GetHeadPosition();

	const BOOL mt = IsEmpty();
	const int count = GetCount();

	while (pos != NULL) 
	{
		CBee* theBee = (CBee*)GetNext(pos);
		if (theBee->IsAlive()) TotalCount += theBee->number;
	}
	return TotalCount;
}
void CBeelist::KillAll()
{
	POSITION pos = GetHeadPosition();

	while (pos != NULL)
	{
		CBee* theBee = (CBee*)GetNext(pos);
		theBee->Alive=false;
		theBee->SetNumber(0);
	}
}

// Returns the quantity of bees in this boxcar.  Note this is zero-based so the first boxcar is index = 0
int CBeelist::GetQuantityAt(int index)
{
	int Count = 0;
	if ((index < GetCount()) && (index >= 0))
	{
		CBee* theBee = (CBee*)GetAt(FindIndex(index));
		Count = theBee->GetNumber();
	}

	return Count;
}

void CBeelist::SetQuantityAt(int index, int Quan)
{
	if ((index < GetCount()) && (index >= 0))
	{
		CBee* theBee = (CBee*)GetAt(FindIndex(index));
		theBee->SetNumber(Quan);
	}

}


// Returns the sum of quantity of bees in the "from" boxcar thru the "to" boxcar, inclusive.  If "to" is greater than the length
// of the list, the sum stops at the end of the list (last boxcar).  If from == to, the quantity at that point is returned.
int CBeelist::GetQuantityAt(int from, int to)
{
	int Count = 0;
	const int ListLength = GetCount();

	if ((from >= 0) && (from < ListLength))
	{
		for (int i = from; i <= to; i++)
		{
			if (i >= ListLength) break;  // Exceeded the length of the list
			Count += GetQuantityAt(i);
		}
	}
	return Count;
}

// Evenly divides Quan bees between boxcars from -> to inclusive
void CBeelist::SetQuantityAt(int from, int to, int Quan)
{
	ASSERT(from <= to);
	const int ListLength = GetCount();
	if (to > ListLength - 1) to = ListLength - 1;
	const int QuanPerBoxcar = Quan/(1 + (to-from));  // divide by number of boxcars

	if ((from >= 0) && (from <= to))
	{
		for (int i = from; i <= to; i++) SetQuantityAt(i, QuanPerBoxcar);
	}

}
// Sets the quantity of bees in boxcars between from -> to = Number*Proportion.
void CBeelist::SetQuantityAtProportional(int from, int to, double Proportion)
{
	ASSERT(from <= to);
	const int ListLength = GetCount();
	if (to > ListLength - 1) to = ListLength - 1;

	if ((from >= 0) && (from <= to))
	{
		int BoxcarQuant = 0;
		for (int i = from; i <= to; i++) 
		{
			BoxcarQuant = GetQuantityAt(i);
			SetQuantityAt(i, (int)(BoxcarQuant * Proportion));
		}
	}

}


// Factor quantity increases or decreases the number of bees 
// in each age group of the list such that New Quantity = Old Quantity * factor
void CBeelist::FactorQuantity(double factor)
{
	POSITION pos = GetHeadPosition();
	CBee* pBee;
	while (pos != NULL)
	{
		pBee = (CBee*)GetNext(pos);
		pBee->number = int(pBee->number*factor);
	}

}

CString CBeelist::Status()
{
	POSITION pos = GetHeadPosition();
	CString StatStg, InterStg;
	int count, boxcar = 1;
	StatStg.Format("Tot BC: %d, ",m_ListLength);

	while (pos!=NULL)
	{
		count = ((CBee*)GetNext(pos))->number;
		InterStg.Format("BC%d: %d, ",boxcar, count);
		StatStg += InterStg;
		boxcar++;
	}
	return StatStg;
}




/////////////////////////////////////////////////////////////////////////////
//
// CForagerlist - this is a type of CAdultList implementing Forager behaviors
//
CForagerlist::CForagerlist()
{
	PendingForagers.RemoveAll();
	CAdultlist::CAdultlist();
	m_PropActualForagers = .8;
}

CForagerlist::~CForagerlist()
{
	while(!PendingForagers.IsEmpty()) delete(PendingForagers.RemoveHead());
	PendingForagers.RemoveAll();
}


void CForagerlist::ClearPendingForagers()
{
	while (!PendingForagers.IsEmpty())
	{
		CAdult* temp = (CAdult*)PendingForagers.RemoveHead();
		ASSERT(temp);
		delete(temp);
	}
}

int CForagerlist::GetQuantity()
{
	int quan = CBeelist::GetQuantity();
	POSITION pos = PendingForagers.GetHeadPosition();
	CAdult* temp;
	while (pos != NULL)
	{
		temp = (CAdult*)PendingForagers.GetNext(pos);
		quan += temp->GetNumber();
	}
	quan += m_UnemployedForagers.GetNumber();
	return quan;
}

int CForagerlist::GetActiveQuantity()
{
	return (GetQuantity() - m_UnemployedForagers.GetNumber());
}

int CForagerlist::GetUnemployedQuantity()
{
	return m_UnemployedForagers.GetNumber();
}

void CForagerlist::KillAll()
{
	CBeelist::KillAll();
	SetUnemployedForagerQuantity(0);
	//Caboose->SetNumber(0);
}

void CForagerlist::Update(CAdult* theAdult, CEvent* theDay)
// The Adult Workers coming in are added to the Forager list.
// If the day is a foraging day, the new adult is pushed onto 
// the Pending Forager list and the list is aged one forage increment(.25, .5, .75, or 1.0)
// Then the Pending Forager list is scanned and any elements that have accumulated
// at least one foraging day are moved to the Forager list.  If the day is not a foraging
// day, the number of number of bees coming in are just added to the number of 
// bees in the first boxcar.  Also, a check is
// done on the foragers to see if they have had their lifetime reduced
// due to varroa infestation.
//
// March 2017:  Update to limit active forager population to a proportion of total colony size.
//
//	Reference Gloria DeGrandi-Hoffman "Establishing the foraging population in VARROAPOP"  3/23/2017
//
// Incoming foragers are all added to the UnemployedForager bucket.  Then a quantity of foragers = PropActiveForagers * ColonySize
// is moved from this bucket and added to the first forager boxcar.  If not enough foragers are available in UnemployedForager then
// move all the foragers in UnemployedForager and be done.

{
	// We will either add adults to the existing first boxcar or we will add a new boxcar
	bool AddToFirstBoxcar = true;

	// 10% of foragers die over the winter 11/1 - 4/1
	if (theDay->GetTime() > COleDateTime(2000,4,15,0,0,0))
	{
		int i=0;
	}
#define WINTER_MORTALITY_PER_DAY 0.10/152 // Reduce 10% over the 152 days of winter
	ASSERT(theAdult);
	// Change lifespan from that of Worker to that of forager
	WorkerCount--;
	ForagerCount++;
	theAdult->SetLifespan(GetColony()->m_CurrentForagerLifespan); 

	if (theDay->IsForageDay())							 
	{
		/****  Bypass the Pending Forager logic - too complicated for not much value.  All foraging days age one day
		PendingForagers.AddHead(theAdult);

		POSITION pos = PendingForagers.GetHeadPosition();
		CAdult* pendingAdult;
		while (pos != NULL) // Increment the forageIncrement for Pending List
		{
			pendingAdult = (CAdult*)PendingForagers.GetNext(pos);
			pendingAdult->SetForageInc(theAdult->GetForageInc()+theDay->GetForageInc());
		}
		pos = PendingForagers.GetTailPosition();
		POSITION oldpos;
		while (pos != NULL)
		{
			oldpos = pos;  // Save old position for possible deletion
			pendingAdult = (CAdult*)PendingForagers.GetPrev(pos);
			if (pendingAdult->GetForageInc() >= 1.0) // Ready to move some forward now
			{
				pendingAdult->SetForageInc(0.0);
				//AddHead(pendingAdult); // Add the appropriate incoming adults to the first boxcar in forager list.
				m_UnemployedForagers.SetNumber(m_UnemployedForagers.GetNumber() + pendingAdult->GetNumber());
				AddToFirstBoxcar = false;

				PendingForagers.RemoveAt(oldpos);
				if (GetCount() >= m_ListLength+1) 
				{
					Caboose = (CAdult*)RemoveTail();
					delete Caboose;
					ForagerCount--;
					Caboose = NULL;
				}
				else Caboose = NULL;
			}
		}
		**********  End of Pending Forager logic  */
		m_UnemployedForagers.SetNumber(m_UnemployedForagers.GetNumber() + theAdult->GetNumber());
		AddToFirstBoxcar = false;

	}
	else  //Non-forage day
	{
		if (IsEmpty()) 
		{
		   if (GetLength() > 0) // In special case of 0 lifespan, don't add head
		   {
			   //AddHead(theAdult); 
			   m_UnemployedForagers.SetNumber(m_UnemployedForagers.GetNumber() + theAdult->GetNumber());
			   AddToFirstBoxcar = false;
		   }
		}
		else
		{
			m_UnemployedForagers.SetNumber(m_UnemployedForagers.GetNumber() + theAdult->GetNumber());
			AddToFirstBoxcar = true;
		}
	}

	// Calculate how many new foragers we have.  Total DesiredForager count is ColonySize*m_PropActualForagers.  If 
	// The NewForagerNumber is the difference between the current active forager quantity and the Desired Forager quantity
	int NewForagerNumber = (int)(m_pColony->GetColonySize() * m_PropActualForagers) - GetActiveQuantity();
	if (NewForagerNumber < 0) NewForagerNumber = 0;
	if (NewForagerNumber > m_UnemployedForagers.GetNumber()) NewForagerNumber = m_UnemployedForagers.GetNumber(); //Limit to available potential foragers
	const int uf = m_UnemployedForagers.GetNumber() - NewForagerNumber;
	m_UnemployedForagers.SetNumber(uf);

	if (m_UnemployedForagers.GetNumber() > 0)//test
	{
		int i = 1;
	}

	if (AddToFirstBoxcar)
	{
		CAdult* ForagerHead = (CAdult*)GetHead();
		ForagerHead->SetNumber(ForagerHead->GetNumber() + NewForagerNumber);

	}
	else
	{
		CAdult* NewBoxcar = new CAdult(NewForagerNumber);
		AddHead(NewBoxcar);
		delete theAdult;
		Caboose = (CAdult*)RemoveTail();
		delete Caboose;
		Caboose = NULL;
	}
	
	//TRACE("Unemployed Foragers %s --- %d\n", theDay->GetDateStg(), m_UnemployedForagers.GetNumber());

	// Check for lifespan reduction
	if (true) // Turn on or off
	{
		CAdult* ListAdult;
		double PropRedux;
		POSITION pos = GetHeadPosition();
		int day = WADLLIFE + 1;
		while (pos!=NULL)
		{
			ListAdult = (CAdult*)GetNext(pos);
			if (ListAdult->IsAlive()) // If already killed, don't kill again
			{
				if (ListAdult->GetNumber()<=0) PropRedux = m_pColony->LongRedux[0];
				else PropRedux = m_pColony->LongRedux[int(ListAdult->GetMites()/ListAdult->GetNumber())];
				if (day>(1-PropRedux)*(WADLLIFE+ GetColony()->m_CurrentForagerLifespan)) 
				{
					ListAdult->Kill();
				}
			}
			day++;
		}
	}

	//  Is this a winter mortality day?
	if ((theDay->GetTime().GetMonth() >= 11) || (theDay->GetTime().GetMonth() < 4))
	{
		POSITION pos = GetHeadPosition();
		CAdult* theForager;
		int Number;
		while (pos != NULL)
		{
			theForager = (CAdult*)GetNext(pos);
			Number = theForager->GetNumber();
			Number = int(Number*(1-WINTER_MORTALITY_PER_DAY));
			theForager->SetNumber(Number);
		}
		// Mortality for Unemployed Foragers
		Number = m_UnemployedForagers.GetNumber();
		Number = (int)(Number * (1.0-WINTER_MORTALITY_PER_DAY));
		m_UnemployedForagers.SetNumber(Number);
	}


}

void CForagerlist::SetLength(int len)
{
	// Because forager lifespan can change as a function of Date/Time spans, SetLength is different than all the other life stages
	// If the desired length is greater than the current length, new, empty boxcars are added to the list.  If the desired length
	// is shorter than the current length, the excess tail boxcars are removed.
	CAdult* pForager;
	if (len < GetCount())
	{
		while((GetCount() > len) && (GetCount() > 0)) // Delete all boxcars over new age value
		{
			pForager = (CAdult*)RemoveTail();
			delete pForager;      
		}
	}
	else if (len > GetCount())
	{
		while(GetCount() < len)
		{
			pForager = new CAdult(0);            
			AddTail(pForager);
		}
	}
	m_ListLength = len;
}


/////////////////////////////////////////////////////////////////////////////
//
// CForagerlistA - An update from CForagerlist (created in version 3.2.8.0) to implement a new data structure logic for foragers.
// The design intent is to maintain the same interface as CForagerlist but implement to the new data structure which is a longer CAdultlist.
//
CForagerlistA::CForagerlistA()
{
	PendingForagers.RemoveAll();
	CAdultlist::CAdultlist();
	m_PropActualForagers = .3;
}

CForagerlistA::~CForagerlistA()
{
	while (!PendingForagers.IsEmpty()) delete(PendingForagers.RemoveHead());
	PendingForagers.RemoveAll();
}


void CForagerlistA::ClearPendingForagers()
{
	while (!PendingForagers.IsEmpty())
	{
		CAdult* temp = (CAdult*)PendingForagers.RemoveHead();
		ASSERT(temp);
		delete(temp);
	}
}

int CForagerlistA::GetQuantity()
{
	int quan = CBeelist::GetQuantity();
	POSITION pos = PendingForagers.GetHeadPosition();
	CAdult* temp;
	while (pos != NULL)
	{
		temp = (CAdult*)PendingForagers.GetNext(pos);
		quan += temp->GetNumber();
	}
	return quan;
}

int CForagerlistA::GetActiveQuantity()
{
	int Quan = GetQuantity();
	if (Quan > 0)
	{
		if (Quan > m_pColony->GetColonySize()*m_PropActualForagers) Quan = (int)(m_pColony->GetColonySize()*m_PropActualForagers);
	}
	return Quan;
}

int CForagerlistA::GetUnemployedQuantity()
{
	return (GetQuantity() - GetActiveQuantity());
}

void CForagerlistA::KillAll()
{
	CBeelist::KillAll();
	//Caboose->SetNumber(0);
}

void CForagerlistA::Update(CAdult* theAdult, CEvent* theDay)
// The Adult Workers coming in are added to the Forager list.
// If the day is a foraging day, the new adult is pushed onto 
// the Pending Forager list and the list is aged one forage increment(.25, .5, .75, or 1.0)
// Then the Pending Forager list is scanned and any elements that have accumulated
// at least one foraging day are moved to the Forager list.  If the day is not a foraging
// day, the number of number of bees coming in are just added to the number of 
// bees in the first boxcar.  Also, a check is
// done on the foragers to see if they have had their lifetime reduced
// due to varroa infestation.
//
// March 2017:  Update to limit active forager population to a proportion of total colony size.
//
//	Reference Gloria DeGrandi-Hoffman "Establishing the foraging population in VARROAPOP"  3/23/2017
//
// Incoming foragers are all added to the UnemployedForager bucket.  Then a quantity of foragers = PropActiveForagers * ColonySize
// is moved from this bucket and added to the first forager boxcar.  If not enough foragers are available in UnemployedForager then
// move all the foragers in UnemployedForager and be done.
//
// June 2017:  Eliminating the UnemployedForager bucket - reverting back to original data structure.  Will keep track of
// Active forager population as not to exceed the specified percentage of colony population size

{
	// 10% of foragers die over the winter 11/1 - 4/1
#define WINTER_MORTALITY_PER_DAY 0.10/152 // Reduce 10% over the 152 days of winter
	ASSERT(theAdult);
	// Change lifespan from that of Worker to that of forager
	WorkerCount--;
	ForagerCount++;
	theAdult->SetLifespan(GetColony()->m_CurrentForagerLifespan);

	if (theDay->IsForageDay())
	{
		PendingForagers.AddHead(theAdult);

		POSITION pos = PendingForagers.GetHeadPosition();
		CAdult* pendingAdult;
		while (pos != NULL) // Increment the forageIncrement for Pending List
		{
			pendingAdult = (CAdult*)PendingForagers.GetNext(pos);
			pendingAdult->SetForageInc(theAdult->GetForageInc() + theDay->GetForageInc());
		}
		pos = PendingForagers.GetTailPosition();
		POSITION oldpos;
		while (pos != NULL)
		{
			oldpos = pos;  // Save old position for possible deletion
			pendingAdult = (CAdult*)PendingForagers.GetPrev(pos);
			if (pendingAdult->GetForageInc() >= 1.0)
			{
				pendingAdult->SetForageInc(0.0);
				AddHead(pendingAdult);
				PendingForagers.RemoveAt(oldpos);
				if (GetCount() == m_ListLength + 1)
				{
					Caboose = (CAdult*)RemoveTail();
					delete Caboose;
					ForagerCount--;
					Caboose = NULL;
				}
				else Caboose = NULL;
			}
		}
	}
	else
	{
		if (IsEmpty())
		{
			if (GetLength() > 0) AddHead(theAdult); // In special case of 0 lifespan, don't add head
		}
		else
		{
			CAdult* ForagerHead = (CAdult*)GetHead();
			ForagerHead->SetNumber(ForagerHead->GetNumber() + theAdult->GetNumber());
			delete theAdult;
			ForagerCount--;
		}
	}


	// Check for lifespan reduction
	if (true) // Turn on or off
	{
		CAdult* ListAdult;
		double PropRedux;
		POSITION pos = GetHeadPosition();
		int day = WADLLIFE + 1;
		while (pos != NULL)
		{
			ListAdult = (CAdult*)GetNext(pos);
			if (ListAdult->IsAlive()) // If already killed, don't kill again
			{
				if (ListAdult->GetNumber() <= 0) PropRedux = m_pColony->LongRedux[0];
				else PropRedux = m_pColony->LongRedux[int(ListAdult->GetMites() / ListAdult->GetNumber())];
				if (day>(1 - PropRedux)*(WADLLIFE + GetColony()->m_CurrentForagerLifespan))
				{
					ListAdult->Kill();
				}
			}
			day++;
		}
	}

	//  Is this a winter mortality day?
	if ((theDay->GetTime().GetMonth() >= 11) || (theDay->GetTime().GetMonth() < 4))
	{
		POSITION pos = GetHeadPosition();
		CAdult* theForager;
		int Number;
		while (pos != NULL)
		{
			theForager = (CAdult*)GetNext(pos);
			Number = theForager->GetNumber();
			Number = int(Number*(1 - WINTER_MORTALITY_PER_DAY));
			theForager->SetNumber(Number);
		}
	}


}


void CForagerlistA::SetLength(int len)
{
	// Because forager lifespan can change as a function of Date/Time spans, SetLength is different than all the other life stages
	// If the desired length is greater than the current length, new, empty boxcars are added to the list.  If the desired length
	// is shorter than the current length, the excess tail boxcars are removed.
	CAdult* pForager;
	if (len < GetCount())
	{
		while ((GetCount() > len) && (GetCount() > 0)) // Delete all boxcars over new age value
		{
			pForager = (CAdult*)RemoveTail();
			delete pForager;
		}
	}
	else if (len > GetCount())
	{
		while (GetCount() < len)
		{
			pForager = new CAdult(0);
			AddTail(pForager);
		}
	}
	m_ListLength = len;
}


/////////////////////////////////////////////////////////////////////////////
//
// CAdultlist - Drones and Workers
//
void CAdultlist::Update(CBrood* theBrood, CColony* theColony, CEvent* theEvent, bool bWorker)
// The Capped Brood coming in are converted to Adults and pushed onto the list.
// If the list is now greater than the specified number of days, the
// bottom of the list is removed and assigned to the Caboose for Workers or the 
// bottom of the list is deleted for drones.  We retain the number of mites from
// the Brood so they can be retrieved and released back into the colony.  We also
// establish the maximum lifetime for the new adult worker and see if any other
// workers in the list have extended beyone their lifetime as reduced by varroa infestation
//
// TODO:
//
// When the following are all true (non-foraging day, no brood, no larvae), do not age the adults
{
	ASSERT(theBrood);
	//int NumberOfNonAdults = theColony->Wlarv.GetQuantity() +  theColony->Dlarv.GetQuantity() + 
	//	theColony->CapDrn.GetQuantity() + theColony->CapWkr.GetQuantity() + theBrood->GetNumber();
	//if (Caboose != NULL) Caboose->SetNumber(0); // Initialize the Caboose to zero - will pass 0 Adults to foragers unless aging occurs
	//if (( theBrood->GetNumber() > 0) || (NumberOfNonAdults > 0) || (theEvent->IsForageDay())) // Age if any of these are true
	{
		CAdult* theAdult = new CAdult(theBrood->GetNumber());
		if (bWorker) WorkerCount++;
		else DroneCount++;
		theAdult->SetMites(theBrood->m_Mites);
		theAdult->SetPropVirgins(theBrood->m_PropVirgins);
		theAdult->SetLifespan(WADLLIFE);
		delete theBrood;  // These brood are now  gone
		AddHead(theAdult);
		const int count = GetCount();
		if (GetCount() == m_ListLength+1) // All Boxcars are full - put workers in caboose, drones die off
		{
			Caboose = (CAdult*)RemoveTail();
			Caboose->number *= GetPropTransition();
			if (!bWorker) 
			{
				delete Caboose;
				DroneCount--;
				Caboose = NULL;
			}
		}
		else 
		{
			Caboose = NULL;
		}

		// Check for age beyond reduced lifespan in workers
		if ((true) && bWorker) // Turn on or off
		{
			double PropRedux;
			POSITION pos = GetHeadPosition();
			int day = 1;
			int index;
			int NumMites;
			while (pos!=NULL)
			{
				theAdult = (CAdult*)GetNext(pos);
				NumMites = theAdult->GetMites();
				if ((theAdult->IsAlive()) && (NumMites > 0) && (theAdult->GetNumber() > 0)) 
				// If already killed, don't kill again, if no mites, ignore
				{
					index = NumMites/theAdult->GetNumber();
					PropRedux = m_pColony->LongRedux[int(index)];
					if (day>(1-PropRedux)*(theAdult->GetLifespan() + GetColony()->m_CurrentForagerLifespan)) theAdult->Kill();
				}
				day++;
			}
		}
	}
}

void CAdultlist::KillAll()
{
	CBeelist::KillAll();
	//Caboose->SetNumber(0);
}


void CAdultlist::UpdateLength(int len, bool bWorker)
{
	// Because adult house bee lifespan can change as a function of Date/Time spans, SetLength is different than the other life stages
	// If the desired length is greater than the current length, new, empty boxcars are added to the list.  If the desired length
	// is shorter than the current length, the excess bees are added to the caboose so they will become foragers at the next update.
	//
	// SetLength should be called before Update in order to properly move the caboose bees.

	if (bWorker)
	{
		CAdult* pAdult;
		const int count = GetCount();
		if (len < GetCount())
		{
			int AdultsToForagers = 0;
			while((GetCount() > len) && (GetCount() > 0)) // Delete all boxcars over new age value
			{
				pAdult = (CAdult*)RemoveTail();
				AdultsToForagers+= pAdult->GetNumber();
				delete pAdult;      
			}
			if (Caboose != NULL) // just in case the reduction happens on the very first day of the simulation, Caboose not defined
			{
				Caboose->number += AdultsToForagers;
			}
		}
		else if (len > GetCount()) //Add empty boxcars
		{
			while(GetCount() < len)
			{
				pAdult = new CAdult;            
				AddTail(pAdult);
			}
		}
	}
	m_ListLength = len;
}


// MoveToEnd
// Moves the oldest QuantityToMove bees to the last boxcar.  Does not move any adults younger than MinAge
int CAdultlist::MoveToEnd(int QuantityToMove, int MinAge)
{
	if ((MinAge < 0) || (QuantityToMove <= 0)) return 0;  
	int TotalMoved = 0;
	int CurrentlyMoving = 0;
	const int EndIndex = GetLength() - 1;  // The last boxcar in the list
	int index = GetLength() - 2; // Initially points to first boxcar to move - the one just before the last one
	if (QuantityToMove > 0)
	{
		int i = 0;
	}
	while ((TotalMoved < QuantityToMove) && (index >= MinAge))
	{
		CurrentlyMoving = GetQuantityAt(index);
		if (CurrentlyMoving > QuantityToMove - TotalMoved)// In this case, don't move all bees in this boxcar
		{
			CurrentlyMoving = QuantityToMove - TotalMoved;
		}
		SetQuantityAt(EndIndex, CurrentlyMoving + GetQuantityAt(EndIndex));  // Add CurrentlyMoving to the end boxcar
		SetQuantityAt(index, GetQuantityAt(index) -  CurrentlyMoving);  // Remove them from the current boxcar
		 if (CurrentlyMoving > 0) //TRACE("Moving Adults to End: %d\n", CurrentlyMoving);
		TotalMoved += CurrentlyMoving;
		index --;
	}
	//TRACE("In MoveToEnd %s\n",Status());
	return TotalMoved;
}


void CAdultlist::Serialize(CArchive &ar)
{
	int count,i;
	POSITION pos;
	if (ar.IsStoring())
	{	// storing code
		count = GetCount();
		ar << count;
		pos = GetHeadPosition();
		CAdult* pAdult;
		while (pos != NULL)
		{
			pAdult = (CAdult*)GetNext(pos);
			pAdult->Serialize(ar);
		}
	}
	else
	{	// loading code
		ar >> count;
		for (i=0; i<count; i++)
		{
			CAdult* temp = new CAdult;
			temp->Serialize(ar);
			pos = AddTail(temp);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// CBroodlist - capped brood
//
void CBroodlist::Update(CLarva* theLarva)
// The Larva coming in are converted to Capped Brood and pushed onto the list.
// If the list is now greater than the specified number of days, the
// bottom of the list is removed and assigned to the Caboose.
{
	ASSERT(theLarva);
	CBrood* theBrood = new CBrood((int)theLarva->GetNumber());
	delete theLarva;  // These larvae are now  gone
	AddHead(theBrood);
	if (GetCount() == m_ListLength+1)
	{
		Caboose = (CBrood*)RemoveTail();
		Caboose->number *= GetPropTransition();
	}
	else Caboose = NULL;

}

void CBroodlist::KillAll()
{
	CBeelist::KillAll();
	//Caboose->SetNumber(0);
}

void CBroodlist::Serialize(CArchive &ar)
{
	int count,i;
	POSITION pos;
	if (ar.IsStoring())
	{	// storing code
		count = GetCount();
		ar << count;
		pos = GetHeadPosition();
		CBrood* pBrood;
		while (pos != NULL)
		{
			pBrood = (CBrood*)GetNext(pos);
			pBrood->Serialize(ar);
		}
	}
	else
	{	// loading code
		ar >> count;
		for (i=0; i<count; i++)
		{
			CBrood* temp = new CBrood;
			temp->Serialize(ar);
			pos = AddTail(temp);
		}
	}
}

int CBroodlist::GetMiteCount()
{
	int TotalCount=0;

	POSITION pos = GetHeadPosition();
	while (pos != NULL) 
	{
		CBrood* theBrood = (CBrood*)GetNext(pos);
		TotalCount += theBrood->m_Mites.GetTotal();
	}
	return TotalCount;
}


float CBroodlist::GetPropInfest()
{
	int TotalUninfested=0, Uninfested = 0;
	int TotalCells = 0;
	float PropInfest;

	POSITION pos = GetHeadPosition();
	while (pos != NULL) 
	{
		CBrood* theBrood = (CBrood*)GetNext(pos);
		Uninfested = theBrood->number - theBrood->m_Mites.GetTotal();
		if (Uninfested < 0) Uninfested = 0;
		TotalCells += theBrood->number;
		TotalUninfested += Uninfested;
	}
	if (TotalCells > 0) PropInfest = 1-(float(TotalUninfested)/float(TotalCells));
	else PropInfest = 0;
	return PropInfest;
}


double CBroodlist::GetMitesPerCell()
{
	return (GetQuantity()>0) ? double(GetMiteCount())/double(GetQuantity()):0.0;

}


void CBroodlist::DistributeMites(CMite theMites)
{
	// Scan thru all the brood boxcars and set mite number
	const int Len = GetLength();
	if (Len <= 0) return;
	const int MitesPerBoxcar = theMites.GetTotal()/Len;
	const double PercentRes = theMites.GetPctResistant();
	POSITION pos;
	CBrood* pBrood;
	pos = GetHeadPosition();
	while (pos != NULL)
	{
		pBrood = (CBrood*)GetNext(pos);
		pBrood->m_Mites = MitesPerBoxcar;
		pBrood->m_Mites.SetPctResistant(PercentRes);
		pBrood->m_PropVirgins = 1.0;
	}
}



/////////////////////////////////////////////////////////////////////////////
//
// CLarvalist
//
void CLarvalist::Update(CEgg* theEggs)
// The eggs coming in are converted to Larvae and pushed onto the list.
// If the list is now greater than the specified number of days, the
// bottom of the list is removed and assigned to the Caboose.
{
	ASSERT(theEggs);
	CLarva* theLarva = new CLarva((int)theEggs->GetNumber());
	delete theEggs;  // These eggs are now  gone
	AddHead(theLarva);
	if (GetCount() == m_ListLength+1) 
	{
		Caboose = (CLarva*)RemoveTail();
		Caboose->number *= GetPropTransition();
	}
	else Caboose = NULL;

}

void CLarvalist::KillAll()
{
	CBeelist::KillAll();
	//Caboose->SetNumber(0);
}

void CLarvalist::Serialize(CArchive &ar)
{
	int count,i;
	POSITION pos;
	if (ar.IsStoring())
	{	// storing code
		count = GetCount();
		ar << count;
		pos = GetHeadPosition();
		CLarva* pLarva;
		while (pos != NULL)
		{
			pLarva = (CLarva*)GetNext(pos);
			pLarva->Serialize(ar);
		}
	}
	else
	{	// loading code
		ar >> count;
		for (i=0; i<count; i++)
		{
			CLarva* temp = new CLarva;
			temp->Serialize(ar);
			pos = AddTail(temp);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// CEgglist
//
void CEgglist::Update(CEgg* theEggs)
// The eggs laid by the queen on this day are pushed onto the list.
// If the list is now greater than the specified number of days, the
// bottom of the list is removed and assigned to the Caboose.
{
	AddHead(theEggs);
	if (GetCount() == m_ListLength+1)
	{
		Caboose = (CEgg*)RemoveTail();
		Caboose->number *= GetPropTransition();
	}
	else Caboose = NULL;

}
void CEgglist::KillAll()
{
	CBeelist::KillAll();
	//Caboose->SetNumber(0);
}


void CEgglist::Serialize(CArchive &ar)
{
	int count,i;
	POSITION pos;
	if (ar.IsStoring())
	{	// storing code
		count = GetCount();
		ar << count;
		pos = GetHeadPosition();
		CEgg* pEgg;
		while (pos != NULL)
		{
			pEgg = (CEgg*)GetNext(pos);
			pEgg->Serialize(ar);
		}
	}
	else
	{	// loading code
		ar >> count;
		for (i=0; i<count; i++)
		{
			CEgg* temp = new CEgg;
			temp->Serialize(ar);
			pos = AddTail(temp);
		}
	}
}




/////////////////////////////////////////////////////////////////////////////
// CColony

IMPLEMENT_DYNCREATE(CColony, CCmdTarget)

CColony::CColony()
{
	name = "";
	HasBeenInitialized = false;
	PropRMVirgins = 1.0;

	//  Define table for longevity reduction.  This table is indexed by the
	//  number of mites/cell a given capped brood boxcar.  The reduction is a
	//  proportion based on that infestation rate


	LongRedux[0] = 0.0;
	LongRedux[1] = 0.02;
	LongRedux[2] = 0.1;
	LongRedux[3] = 0.2;
	LongRedux[4] = 0.4;			// Old Redux Table
	LongRedux[5] = 0.8;
	LongRedux[6] = 0.9;
	LongRedux[7] = 0.9;


	m_VTTreatmentActive = false;
	m_VTEnable = false;

	m_SuppPollen.m_CurrentAmount = 0;
	m_SuppPollen.m_StartingAmount = 0;
	m_SuppPollen.m_BeginDate = COleDateTime::GetCurrentTime();
	m_SuppPollen.m_EndDate = COleDateTime::GetCurrentTime();
	m_SuppNectar.m_CurrentAmount = 0;
	m_SuppNectar.m_StartingAmount = 0;
	m_SuppNectar.m_BeginDate = COleDateTime::GetCurrentTime();
	m_SuppNectar.m_EndDate = COleDateTime::GetCurrentTime();

	m_ColonyNecMaxAmount = 0;
	m_ColonyPolMaxAmount = 0;
	m_ColonyNecInitAmount = 0;
	m_ColonyPolInitAmount = 0;

	m_NoResourceKillsColony = false;

}



CColony::~CColony()
{
	Clear();
	if (!m_EventMap.IsEmpty())
	{
		POSITION pos = m_EventMap.GetStartPosition();
		CUIntArray* pArray;
		CString datestg;
		while (pos != NULL)
		{
			m_EventMap.GetNextAssoc(pos,datestg,(CObject *&)pArray);
			pArray->RemoveAll();
			delete pArray;
			m_EventMap.RemoveKey(datestg);
		}
	}
}


CColony CColony::operator = (CColony col)
{
	CColony temp;
	temp.HasBeenInitialized = col.HasBeenInitialized;
	temp.m_InitCond = col.m_InitCond;
	temp.name = col.name;
	temp.queen=col.queen;

	POSITION pos = col.Dadl.GetHeadPosition();
	while (pos!=NULL) temp.Dadl.AddTail(col.Dadl.GetNext(pos));

	pos = col.foragers.GetHeadPosition();
	while (pos!=NULL) temp.foragers.AddTail(col.foragers.GetNext(pos));

	pos = col.Wlarv.GetHeadPosition();
	while (pos!=NULL) temp.Wlarv.AddTail(col.Wlarv.GetNext(pos));

	pos = col.Dlarv.GetHeadPosition();
	while (pos!=NULL) temp.Dlarv.AddTail(col.Dlarv.GetNext(pos));

	pos = col.Wadl.GetHeadPosition();
	while (pos!=NULL) temp.Wadl.AddTail(col.Wadl.GetNext(pos));

	return temp;
}

CColony:: CColony(CColony& col)  // Copy Constructor
{
	HasBeenInitialized = col.HasBeenInitialized;
	m_InitCond = col.m_InitCond;
	name = col.name;
	queen=col.queen;

	POSITION pos = col.Wadl.GetHeadPosition();
	while (pos!=NULL) Wadl.AddTail(col.Wadl.GetNext(pos));

	pos = col.Dadl.GetHeadPosition();
	while (pos!=NULL) Dadl.AddTail(col.Dadl.GetNext(pos));

	pos = col.foragers.GetHeadPosition();
	while (pos!=NULL) foragers.AddTail(col.foragers.GetNext(pos));

	pos = col.Wlarv.GetHeadPosition();
	while (pos!=NULL) Wlarv.AddTail(col.Wlarv.GetNext(pos));

	pos = col.Dlarv.GetHeadPosition();
	while (pos!=NULL) Dlarv.AddTail(col.Dlarv.GetNext(pos));

	pos = col.Weggs.GetHeadPosition();
	while (pos!=NULL) Weggs.AddTail(col.Weggs.GetNext(pos));

	pos = col.Dlarv.GetHeadPosition();
	while (pos!=NULL) Deggs.AddTail(col.Deggs.GetNext(pos));


}




BEGIN_MESSAGE_MAP(CColony, CCmdTarget)
	//{{AFX_MSG_MAP(CColony)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColony 

void CColony::InitializeColony()
{
	InitializeBees();
	InitializeMites();


	// Set pesticide Dose rate to 0
	m_EPAData.m_D_L4 = 0;
	m_EPAData.m_D_L5 = 0;
	m_EPAData.m_D_LD = 0;
	m_EPAData.m_D_A13 = 0;
	m_EPAData.m_D_A410 = 0;
	m_EPAData.m_D_A1120 = 0;
	m_EPAData.m_D_AD = 0;
	m_EPAData.m_D_C_Foragers = 0;
	m_EPAData.m_D_D_Foragers = 0;

	// And set max doses to 0 also
	m_EPAData.m_D_L4_Max = 0;
	m_EPAData.m_D_L5_Max = 0;
	m_EPAData.m_D_LD_Max = 0;
	m_EPAData.m_D_A13_Max = 0;
	m_EPAData.m_D_A410_Max = 0;
	m_EPAData.m_D_A1120_Max = 0;
	m_EPAData.m_D_AD_Max = 0;
	m_EPAData.m_D_C_Foragers_Max = 0;
	m_EPAData.m_D_D_Foragers_Max = 0;

	// Set resources 
	m_Resources.Initialize(m_ColonyPolInitAmount, m_ColonyNecInitAmount);
	m_SuppPollen.m_CurrentAmount = m_SuppPollen.m_StartingAmount;
	m_SuppNectar.m_CurrentAmount = m_SuppNectar.m_StartingAmount;

	// Set pesticide mortality trackers to zero
	m_DeadWorkerLarvaePesticide = 0;
	m_DeadDroneLarvaePesticide = 0;
	m_DeadWorkerAdultsPesticide = 0;
	m_DeadDroneAdultsPesticide = 0;
	m_DeadForagersPesticide = 0;

	m_ColonyEventList.RemoveAll();
	//m_NutrientCT.RemoveAll();
	if (m_NutrientCT.IsEnabled()) m_NutrientCT.LoadTable(m_NutrientCT.GetFileName());

}

void CColony::AddEventNotification(CString DateStg, CString Msg)
{
	CString EventString = DateStg + ": " + Msg;
	m_ColonyEventList.AddTail(EventString);
}



void CColony::Serialize(CArchive& ar, int FileFormatVersion) 
{
	if (ar.IsStoring())
	{	// storing code
		//  First store the Colony initial condition data
		ar << name;
		ar << m_InitCond.m_droneAdultInfestField;
		ar << m_InitCond.m_droneBroodInfestField;
		ar << m_InitCond.m_droneMiteOffspringField;
		ar << m_InitCond.m_droneMiteSurvivorshipField;
		ar << m_InitCond.m_workerAdultInfestField;
		ar << m_InitCond.m_workerBroodInfestField;
		ar << m_InitCond.m_workerMiteOffspring;
		ar << m_InitCond.m_workerMiteSurvivorship;
		ar << m_InitCond.m_droneAdultsField;
		ar << m_InitCond.m_droneBroodField;
		ar << m_InitCond.m_droneEggsField;
		ar << m_InitCond.m_droneLarvaeField;
		ar << m_InitCond.m_workerAdultsField;
		ar << m_InitCond.m_workerBroodField;
		ar << m_InitCond.m_workerEggsField;
		ar << m_InitCond.m_workerLarvaeField;
		if (FileFormatVersion <= 14) // Eliminated internal queen variables - now just double QueenStrength
		{
			ar << (int)m_InitCond.m_QueenSperm; // FIX - THIS NEEDS TO CONVERT TO DOUBLE
		}
		ar << m_InitCond.m_ForagerLifespan;
		ar << m_InitCond.m_SimStart;
		ar << m_InitCond.m_SimEnd;
		if (FileFormatVersion <= 14) // Eliminated internal queen variables - now just double QueenStrength
		{
			ar << (int)m_InitCond.m_MaxEggs;
		}
		if (FileFormatVersion >= 15) // Eliminated internal queen variables - now just double QueenStrength
		{
			ar << m_InitCond.m_QueenStrength;
		}
		if (FileFormatVersion >= 16) // Added Supplemental Feeding variables
		{
			ar << m_SuppPollen.m_StartingAmount;
			ar << m_SuppPollen.m_BeginDate;
			ar << m_SuppPollen.m_EndDate;
			ar << (m_SuppPollenEnabled ? 1:0);
			ar << m_SuppNectar.m_StartingAmount;
			ar << m_SuppNectar.m_BeginDate;
			ar << m_SuppNectar.m_EndDate;
			ar << (m_SuppNectarEnabled ? 1:0);
			ar << m_ColonyPolInitAmount;
			ar << m_ColonyNecInitAmount;
		}
		if (FileFormatVersion >= 17) // Added Proportion of Actual Foragers and some spares
		{
			ar << foragers.GetPropActualForagers();
			ar << m_ColonyPolMaxAmount;  // Added 3.2.8.8 - decremented rannge for dTemp below
			ar << m_ColonyNecMaxAmount;   //

			// Store some integers, some doubles, and dates as place holders
			// As we need to store/retrieve new actual variables, reduce these placeholders appropriately
			const int iTemp = 0;
			const double dTemp = 0.0;
			const COleDateTime dateTemp = COleDateTime(2001,9,11,0,0,0);
			int i = 0;
			for (i=0;i<8;i++) ar << iTemp;
			for (i=0;i<8;i++) ar << dTemp;
			for (i=0;i<10;i++) ar << dateTemp;
		}

		if (FileFormatVersion >= 19)
		{
			ar << (m_NoResourceKillsColony ? 1 : 0);
			ar << (m_SuppPollenAnnual ? 1 : 0);
			ar << (m_SuppNectarAnnual ? 1 : 0);
		}

	}
	else
	{	// loading code
		int TEMP;  //FIX
		ar >> name;
		ar >> m_InitCond.m_droneAdultInfestField;
		ar >> m_InitCond.m_droneBroodInfestField;
		ar >> m_InitCond.m_droneMiteOffspringField;
		ar >> m_InitCond.m_droneMiteSurvivorshipField;
		ar >> m_InitCond.m_workerAdultInfestField;
		ar >> m_InitCond.m_workerBroodInfestField;
		ar >> m_InitCond.m_workerMiteOffspring;
		ar >> m_InitCond.m_workerMiteSurvivorship;
		ar >> m_InitCond.m_droneAdultsField;
		ar >> m_InitCond.m_droneBroodField;
		ar >> m_InitCond.m_droneEggsField;
		ar >> m_InitCond.m_droneLarvaeField;
		ar >> m_InitCond.m_workerAdultsField;
		ar >> m_InitCond.m_workerBroodField;
		ar >> m_InitCond.m_workerEggsField;
		ar >> m_InitCond.m_workerLarvaeField;
		if (FileFormatVersion <= 14) // Eliminated internal queen variables - now just double QueenStrength
		{
			ar >> TEMP;
			m_InitCond.m_QueenSperm = TEMP;
		}
		ar >> m_InitCond.m_ForagerLifespan;
		ar >> m_InitCond.m_SimStart;
		ar >> m_InitCond.m_SimEnd;
		if (FileFormatVersion <= 14) // Eliminated internal queen variables - now just double QueenStrength
		{
		ar >> TEMP;
		m_InitCond.m_MaxEggs = TEMP;
		}
		if (FileFormatVersion >= 15)
		{
			ar >> m_InitCond.m_QueenStrength;
			queen.SetStrength(m_InitCond.m_QueenStrength);
		}
		if (FileFormatVersion >= 16) // Added Supplemental Feeding variables
		{
			int bval;
			ar >> m_SuppPollen.m_StartingAmount;
			ar >> m_SuppPollen.m_BeginDate;
			ar >> m_SuppPollen.m_EndDate;
			ar >> bval;
			m_SuppPollenEnabled = (bval == 1);
			ar >> m_SuppNectar.m_StartingAmount;
			ar >> m_SuppNectar.m_BeginDate;
			ar >> m_SuppNectar.m_EndDate;
			ar >> bval;
			m_SuppNectarEnabled = (bval == 1);
			ar >> m_ColonyPolInitAmount;
			ar >> m_ColonyNecInitAmount;
		}

		if (FileFormatVersion >= 17) // Added Proportion of Actual Foragers and some spares
		{
			double paf;
			ar >> paf;
			foragers.SetPropActualForagers(paf);
			ar >> m_ColonyPolMaxAmount;  // Added in 3.2.8.8  Decremented range in dTemp below
			ar >> m_ColonyNecMaxAmount;  //

			// Load some integers, some doubles, and dates as place holders
			// As we need to store/retrieve new actual variables, reduce these placeholders appropriately
			int iTemp = 0;
			double dTemp = 0.0;
			COleDateTime dateTemp = COleDateTime(2001,9,11,0,0,0);
			int i = 0;
			for (i=0;i<8;i++) ar >> iTemp;
			for (i=0;i<8;i++) ar >> dTemp;
			for (i=0;i<10;i++) ar >> dateTemp;
		}

		if (FileFormatVersion >= 19)
		{
			int bval;
			ar >> bval;
			m_NoResourceKillsColony = (bval == 1);
			
			// Added in version 3.2.8.4
			ar >> bval;
			m_SuppPollenAnnual = (bval == 1);
			ar >> bval;
			m_SuppNectarAnnual = (bval == 1);
		}


		Clear();


	}

	// Now serialize the lists

	Wadl.Serialize(ar);
	Dadl.Serialize(ar);
	foragers.Serialize(ar);
	Wlarv.Serialize(ar);
	Dlarv.Serialize(ar);
	Weggs.Serialize(ar);
	Deggs.Serialize(ar);
	queen.Serialize(ar);
	
	if (FileFormatVersion >= 6)
	{
		m_InitCond.m_EggTransitionDRV.Serialize(ar, FileFormatVersion);
		m_InitCond.m_LarvaeTransitionDRV.Serialize(ar, FileFormatVersion);
		m_InitCond.m_BroodTransitionDRV.Serialize(ar, FileFormatVersion);
		m_InitCond.m_AdultTransitionDRV.Serialize(ar, FileFormatVersion);	
		m_InitCond.m_ForagerLifespanDRV.Serialize(ar, FileFormatVersion);	
	}	

	if (FileFormatVersion >= 12)
	{
		m_InitCond.m_AdultLifespanDRV.Serialize(ar, FileFormatVersion);	
	}	

	if (FileFormatVersion >= 8)
	{
		m_EPAData.Serialize(ar, FileFormatVersion);
	}

	if (FileFormatVersion >= 14)
	{
		m_NutrientCT.Serialize(ar);
	}

	SetInitialized(true);
}

COleDateTime* CColony::GetDayNumDate(int DayNum)
{
	COleDateTime temptime;
	COleDateTime* pReturnDate = NULL;
	const COleDateTimeSpan ts(DayNum -1,0,0,0); // First day is 0 span
	if( temptime.ParseDateTime(m_InitCond.m_SimStart) )
	//if( temptime.ParseDateTime(Get) )
	{
		pReturnDate = new COleDateTime(temptime.GetYear(), temptime.GetMonth(), temptime.GetDay(), 0,0,0);
		*pReturnDate = *pReturnDate + ts;
	}
	
	return pReturnDate;
}

void CColony::KillColony()
{


	// Initialize Queen
	queen.SetStrength(1);

	//  Set lengths of the various lists
	Deggs.KillAll();
	Weggs.KillAll();
	Dlarv.KillAll();
	Wlarv.KillAll();
	CapDrn.KillAll();
	CapWkr.KillAll();
	Dadl.KillAll();
	Wadl.KillAll();
	foragers.KillAll();
	foragers.ClearPendingForagers();
}

void CColony::Clear()
{

	//  Empty Wadl list
	while (!Wadl.IsEmpty())
	{
		CAdult* temp = (CAdult*)Wadl.RemoveHead();
		ASSERT(temp);
		delete temp;
		Wadl.ClearCaboose();
	}

	//  Empty Dadl list
	while (!Dadl.IsEmpty())
	{
		CAdult* temp = (CAdult*)Dadl.RemoveHead();
		ASSERT(temp);
		delete temp;
		Dadl.ClearCaboose();
	}

	//  Empty foragers list
	while (!foragers.IsEmpty())
	{
		foragers.ClearPendingForagers();
		CAdult* temp = (CAdult*)foragers.RemoveHead();
		ASSERT(temp);
		delete temp;
		foragers.ClearCaboose();
	}


	//  Empty worker larvae list
	while (!Wlarv.IsEmpty())
	{
		CLarva* temp = (CLarva*)Wlarv.RemoveHead();
		ASSERT(temp);
		delete temp;
		Wlarv.ClearCaboose();
	}

	//  Empty drone larvae list
	while (!Dlarv.IsEmpty())
	{
		CLarva* temp = (CLarva*)Dlarv.RemoveHead();
		ASSERT(temp);
		delete temp;
		Dlarv.ClearCaboose();
	}

	//  Empty drone Brood list
	while (!CapDrn.IsEmpty())
	{
		CBrood* temp = (CBrood*)CapDrn.RemoveHead();
		ASSERT(temp);
		delete temp;
		CapDrn.ClearCaboose();
	}

	//  Empty worker Brood list
	while (!CapWkr.IsEmpty())
	{
		CBrood* temp = (CBrood*)CapWkr.RemoveHead();
		ASSERT(temp);
		delete temp;
		CapWkr.ClearCaboose();
	}


	//  Empty worker egg list
	while (!Weggs.IsEmpty())
	{
		CEgg* temp = (CEgg*)Weggs.RemoveHead();
		ASSERT(temp);
		delete temp;
		Weggs.ClearCaboose();
	}

	//  Empty drone egg list
	while (!Deggs.IsEmpty())
	{
		CEgg* temp = (CEgg*)Deggs.RemoveHead();
		ASSERT(temp);
		delete temp;
		Deggs.ClearCaboose();
	}
	
	// Empty the MiteTreatment list
	m_MiteTreatmentInfo.ClearAll();
	m_RQQueenStrengthArray.RemoveAll();
	queen.SetStrength(m_InitCond.m_QueenStrength);  // Reset this in case requeening happened, reset to original initial condition
}

void CColony::InitializeBees()
{
	// Set current forager lifespan to the initial condition - could be changed during sim run
	m_CurrentForagerLifespan = m_InitCond.m_ForagerLifespan;

	// Initialize Queen
	queen.SetStrength(m_InitCond.m_QueenStrength);

	//  Set lengths of the various lists
	Deggs.SetLength(EGGLIFE);
	Deggs.SetPropTransition(1.0);
	Weggs.SetLength(EGGLIFE);
	Weggs.SetPropTransition(1.0);
	Dlarv.SetLength(DLARVLIFE);
	Dlarv.SetPropTransition(1.0);
	Wlarv.SetLength(WLARVLIFE);
	Wlarv.SetPropTransition(1.0);
	CapDrn.SetLength(DBROODLIFE);
	CapDrn.SetPropTransition(1.0);
	CapWkr.SetLength(WBROODLIFE);
	CapWkr.SetPropTransition(1.0);
	Dadl.SetLength(DADLLIFE);
	Dadl.SetPropTransition(1.0);
	Wadl.SetLength(WADLLIFE);
	Wadl.SetPropTransition(1.0);
	Wadl.SetColony(this);
	foragers.SetLength(m_CurrentForagerLifespan);
	foragers.SetColony(this);
	//foragers.SetUnemployedForagerQuantity(0);
	
	// Distribute bees from initial conditions into age groupings
	// Any rounding error goes into the last boxcar
	int i,e,del;
	
	// Eggs
	e = m_InitCond.m_droneEggsField/Deggs.GetLength();
	del  = m_InitCond.m_droneEggsField - e*Deggs.GetLength();
	for (i=0;i<Deggs.GetLength();i++)
	{
		CEgg* theEggs;
		if (i<(Deggs.GetLength()-1)) theEggs = new CEgg(e);
		else  theEggs = new CEgg(e+del);
		Deggs.AddHead(theEggs);
	}

	e = m_InitCond.m_workerEggsField/Weggs.GetLength();
	del  = m_InitCond.m_workerEggsField - e*Weggs.GetLength();
	for (i=0;i<Weggs.GetLength();i++)
	{
		CEgg* theEggs;
		if (i<(Weggs.GetLength()-1)) theEggs = new CEgg(e);
		else  theEggs = new CEgg(e+del);
		Weggs.AddHead(theEggs);
	}

	// Larvae
	e = m_InitCond.m_droneLarvaeField/Dlarv.GetLength();
	del  = m_InitCond.m_droneLarvaeField - e*Dlarv.GetLength();
	for (i=0;i<Dlarv.GetLength();i++)
	{
		CLarva* theLarva;
		if (i<(Dlarv.GetLength()-1)) theLarva = new CLarva(e);
		else theLarva = new CLarva(e+del);
		Dlarv.AddHead(theLarva);
	}

	e = m_InitCond.m_workerLarvaeField/Wlarv.GetLength();
	del  = m_InitCond.m_workerLarvaeField - e*Wlarv.GetLength();
	for (i=0;i<Wlarv.GetLength();i++)
	{
		CLarva* theLarva;
		if (i<(Wlarv.GetLength()-1)) theLarva = new CLarva(e);
		else theLarva = new CLarva(e+del);
		Wlarv.AddHead(theLarva);
	}

	// Capped Brood
	e = m_InitCond.m_droneBroodField/CapDrn.GetLength();
	del  = m_InitCond.m_droneBroodField - e*CapDrn.GetLength();
	for (i=0;i<CapDrn.GetLength();i++)
	{
		CBrood* theBrood;
		if (i<(CapDrn.GetLength()-1)) theBrood = new CBrood(e);
		else theBrood = new CBrood(e+del);
		CapDrn.AddHead(theBrood);
	}

	e = m_InitCond.m_workerBroodField/CapWkr.GetLength();
	del  = m_InitCond.m_workerBroodField - e*CapWkr.GetLength();
	for (i=0;i<CapWkr.GetLength();i++)
	{
		CBrood* theBrood;
		if (i<(CapWkr.GetLength()-1)) theBrood = new CBrood(e);
		else theBrood = new CBrood(e+del);
		CapWkr.AddHead(theBrood);
	}

	// Drone Adults
	e = m_InitCond.m_droneAdultsField/Dadl.GetLength();
	del  = m_InitCond.m_droneAdultsField - e*Dadl.GetLength();
	for (i=0;i<Dadl.GetLength();i++)
	{
		CAdult* theDrone;
		if (i<(Dadl.GetLength()-1)) theDrone = new CAdult(e);
		else theDrone = new CAdult(e+del);
		theDrone->SetLifespan(DADLLIFE);
		Dadl.AddHead(theDrone);
	}

	// Distribute Worker Adults over Workers and Foragers
	e = m_InitCond.m_workerAdultsField/
	(Wadl.GetLength() + foragers.GetLength());
	del  = m_InitCond.m_workerAdultsField - 
		e*(Wadl.GetLength()+foragers.GetLength());
	for (i=0;i<Wadl.GetLength();i++)  // Distribute into Worker Boxcars
	{
		CAdult* theWorker;
		theWorker = new CAdult(e);
		theWorker->SetLifespan(WADLLIFE);
		Wadl.AddHead(theWorker);
	}
	for (i=0;i<foragers.GetLength();i++) // Distribute remaining into Foragers
	{
		CAdult* theForager;
		if (i<(foragers.GetLength()-1)) theForager = new CAdult(e);
		else theForager = new CAdult(e+del);
		theForager->SetLifespan(foragers.GetLength());
		foragers.AddHead(theForager);
	}



	queen.SetDayOne(1);
	queen.SetEggLayingDelay(0);
}

int CColony::GetColonySize()
{
	return(Dadl.GetQuantity() + Wadl.GetQuantity() + foragers.GetQuantity());
}


void CColony::UpdateBees(CEvent* pEvent, int DayNum)
{

	const float LarvPerBee = float(Wlarv.GetQuantity() + Dlarv.GetQuantity())/
					(Wadl.GetQuantity() + Dadl.GetQuantity() + foragers.GetQuantity());
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Apply Date Range values
	///////////////////////////////////////////////////////////////////////////////////////////////////
   CString dateStg = pEvent->GetDateStg("%m/%d/%Y");
   COleDateTime theDate;
   if (theDate.ParseDateTime(dateStg)) // The date string was valid
   {
		double PropTransition;
		
		// Eggs Transition Rate
		if ((m_InitCond.m_EggTransitionDRV.GetActiveValue(theDate,PropTransition))  && (m_InitCond.m_EggTransitionDRV.IsEnabled()))
		{
			Deggs.SetPropTransition(PropTransition/100);
			Weggs.SetPropTransition(PropTransition/100);
		}
		else
		{
			Deggs.SetPropTransition(1.0);
			Weggs.SetPropTransition(1.0);
		}
		// Larvae Transition Rate
		if ((m_InitCond.m_LarvaeTransitionDRV.GetActiveValue(theDate,PropTransition)) && (m_InitCond.m_LarvaeTransitionDRV.IsEnabled()))
		{
			Dlarv.SetPropTransition(PropTransition/100);
			Wlarv.SetPropTransition(PropTransition/100);
		}
		else
		{
			Dlarv.SetPropTransition(1.0);
			Wlarv.SetPropTransition(1.0);
		}
		// Brood Transition Rate
		if ((m_InitCond.m_BroodTransitionDRV.GetActiveValue(theDate,PropTransition)) && (m_InitCond.m_BroodTransitionDRV.IsEnabled()))
		{
			CapDrn.SetPropTransition(PropTransition/100);
			CapWkr.SetPropTransition(PropTransition/100);
		}
		else
		{
			CapDrn.SetPropTransition(1.0);
			CapWkr.SetPropTransition(1.0);
		}
		// Adults Transition Rate
		if ((m_InitCond.m_AdultTransitionDRV.GetActiveValue(theDate,PropTransition)) && (m_InitCond.m_AdultTransitionDRV.IsEnabled()))
		{
			Dadl.SetPropTransition(PropTransition/100);
			Wadl.SetPropTransition(PropTransition/100);
		}
		else
		{
			Dadl.SetPropTransition(1.0);
			Wadl.SetPropTransition(1.0);
		}
		// Adults Lifespan Change
		// A reduction moves Adults to caboose in boxcars > new max limit
		// An increase adds empty boxcars up to the new max limit
		double AdultAgeLimit;
		if ((m_InitCond.m_AdultLifespanDRV.GetActiveValue(theDate,AdultAgeLimit)) && (m_InitCond.m_AdultLifespanDRV.IsEnabled()))
		{
			if (Wadl.GetLength() != (int)AdultAgeLimit) // Update if new AdultAgeLimit
			{
				Wadl.UpdateLength((int)AdultAgeLimit);
			}
		}
		else
		{
			if (Wadl.GetLength() != WADLLIFE) // Update if return to default
			{
				Wadl.UpdateLength(WADLLIFE);
			}
		}



		// Foragers Lifespan Change
		// A reduction kills all foragers in boxcars > new max limit
		// An increase adds empty boxcars to the new max limit
		double ForagerLifespan;
		if ((m_InitCond.m_ForagerLifespanDRV.GetActiveValue(theDate,ForagerLifespan)) && (m_InitCond.m_ForagerLifespanDRV.IsEnabled()))
		{
			m_CurrentForagerLifespan = (int)ForagerLifespan;
		}
		else
		{
			m_CurrentForagerLifespan = m_InitCond.m_ForagerLifespan;
		}
		foragers.SetLength(m_CurrentForagerLifespan);
		
		
   }
   else   // Invalid date string - reset to default
   {
		Deggs.SetPropTransition(1.0);
		Weggs.SetPropTransition(1.0);
		Dlarv.SetPropTransition(1.0);
		Wlarv.SetPropTransition(1.0);
		CapDrn.SetPropTransition(1.0);
		CapWkr.SetPropTransition(1.0);
		Dadl.SetPropTransition(1.0);
		Wadl.SetPropTransition(1.0);
   }
	
	queen.LayEggs(DayNum,pEvent->GetTemp(),pEvent->GetDaylightHours(),
		foragers.GetQuantity(), LarvPerBee);
	Deggs.Update(queen.GetDeggs());
	Weggs.Update(queen.GetWeggs());
	Dlarv.Update((CEgg*)Deggs.GetCaboose());
	Wlarv.Update((CEgg*)Weggs.GetCaboose());
	CapDrn.Update((CLarva*)Dlarv.GetCaboose());
	CapWkr.Update((CLarva*)Wlarv.GetCaboose());
	const int NumberOfNonAdults = Wlarv.GetQuantity() +  Dlarv.GetQuantity() + CapDrn.GetQuantity() + CapWkr.GetQuantity();
	if ((NumberOfNonAdults > 0) || (pEvent->IsForageDay()))
	{
		// Foragers killed due to pesticide.  Recruit precocious Adult Workers to be foragers - Add them to the last Adult Boxcar
		// The last boxcar will be moved to the Caboose when Wadl.Update is called a little later.  The Wadl Caboose will be moved to the 
		// forager list when forager.Update is called.
		//
		int ForagersToBeKilled = QuantityPesticideToKill(&foragers,m_EPAData.m_D_C_Foragers,0,m_EPAData.m_AI_AdultLD50_Contact,m_EPAData.m_AI_AdultSlope_Contact); //Contact Mortality
		ForagersToBeKilled +=    QuantityPesticideToKill(&foragers, m_EPAData.m_D_D_Foragers, 0, m_EPAData.m_AI_AdultLD50, m_EPAData.m_AI_AdultSlope); //Diet Mortality
		const int MinAgeToForager = 14;
		Wadl.MoveToEnd(ForagersToBeKilled, MinAgeToForager);
		if (ForagersToBeKilled > 0)
		{
			CString notification;
			notification.Format("%d Foragers killed by pesticide - recruiting workers", ForagersToBeKilled);
			AddEventNotification(pEvent->GetDateStg("%m/%d/%Y"), notification);
		}
		// End Forgers killed due to pesticide

		//TRACE("Date: %s\n",pEvent->GetDateStg());
		Dadl.Update((CBrood*)CapDrn.GetCaboose(),this, pEvent, false);
		//TRACE("HB Before Update:%s\n",Wadl.Status());
		Wadl.Update((CBrood*)CapWkr.GetCaboose(), this, pEvent, true);
		//TRACE(" HB After Update:%s\n",Wadl.Status());
		//TRACE("    Worker Caboose Quan: %d\n", Wadl.GetCaboose()->number);
		foragers.Update((CAdult*)Wadl.GetCaboose(), pEvent);
		//TRACE("Updated Foragers:%s\n",foragers.Status());
	}

   ///////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////
   //
   // Apply the Pesticide Mortality Impacts Here
   //
   ///////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////
	//TRACE("AIAdultSlope = %f4.2, AIAdultLD50 = %f4.2\n", m_EPAData.m_AI_AdultSlope, m_EPAData.m_AI_AdultLD50);
   ConsumeFood(pEvent, DayNum);
   DetermineFoliarDose(DayNum);
   ApplyPesticideMortality();
   //
   ///////////////////////////////////////////////////////////////////////////////////////////////

}

void CColony::AddMites(CMite NewMites)
{
	//Assume new mites are "virgins"
	CMite virgins = RunMite*int(PropRMVirgins) + NewMites;
	RunMite += NewMites;
	if (RunMite.GetTotal() <=0) PropRMVirgins = 1.0;
	else PropRMVirgins = virgins.GetTotal()/RunMite.GetTotal();
}


void CColony::InitializeMites()
{
	// Initial condition infestation of capped brood
	WMites = CMite(0,int((CapWkr.GetQuantity()*m_InitCond.m_workerBroodInfestField)/float(100)));
	DMites = CMite(0,int((CapDrn.GetQuantity()*m_InitCond.m_droneBroodInfestField)/float(100)));
	CapWkr.DistributeMites(WMites);
	CapDrn.DistributeMites(DMites);

	// Initial condition mites on adult bees i.e. Running Mites
	RunMiteW = CMite(0,int((Wadl.GetQuantity()*m_InitCond.m_workerAdultInfestField+
							foragers.GetQuantity()*m_InitCond.m_workerAdultInfestField)/float(100)));
	RunMiteD = CMite(0,int((Dadl.GetQuantity()*m_InitCond.m_droneAdultInfestField)/float(100)));
	
	RunMite = RunMiteD + RunMiteW;

	PrevEmergMite = CMite(0,0);
	PropRMVirgins = 1.0;

	m_MitesDyingToday = 0;
	m_MitesDyingThisPeriod = 0;

}


void CColony::UpdateMites(CEvent* pEvent, int DayNum)
{
	/*  Assume UpdateMites is called after UpdateBees.  This means the 
		last Larva boxcar has been moved to the first Brood boxcar and the last
		Brood boxcar has been moved to the first Adult boxcar.  Therefore, we infest
		the first Brood boxcar with mites and we have mites emerging from the
		first boxcar in the appropriate Adult list.
	*/

	/*  
		Initial step is to infest the larval cells with the free mites just
		before they are capped.  

	*/
	#define PROPINFSTW 0.08
	#define PROPINFSTD 0.92
	#define MAXMITES_PER_DRONE_CELL 3
	#define MAXMITES_PER_WORKER_CELL 4

	m_MitesDyingToday = 0;

	CBrood* WkrBrood = ((CBrood*)CapWkr.GetHead()); // The cells being infested
	CBrood* DrnBrood = ((CBrood*)CapDrn.GetHead()); // in this time cycle

	//  Calculate proportion of RunMites that can invade cells (per Calis)
	double I;
	const double B = GetColonySize()*0.125; // Weight in grams of colony
	if (B > 0.0)
	{
		const double rD = 6.49*DrnBrood->GetNumber()/B;
		const double rW = 0.56*WkrBrood->GetNumber()/B;
		I = (1-exp(-(rD+rW)));
		if (I<0.0)	I = 0.0;
	}
	else 
	{
		I = 0.0;
	}


	WMites = RunMite * (I * PROPINFSTW); // Mites going into worker brood cells

	// Modify potential Drone Mite infestation rate by factoring in liklihood of finding a drone target cell in all the cells
	double Likelihood = 1.0;
	if (WkrBrood->GetNumber() > 0) // Prevent divide by zero
	{
		Likelihood = (double)DrnBrood->GetNumber()/(double)WkrBrood->GetNumber();
		if (Likelihood > 1.0) Likelihood = 1.0;
	}

	CMite OverflowMax;
	CMite OverflowLikelihood;
	DMites = RunMite * (I * PROPINFSTD * Likelihood); // Mites going into drone brood cells

	// Capture the number of mites targeted to drone brood but filtered out due to liklihood function
	OverflowLikelihood = RunMite * (I * PROPINFSTD * (1.0 - Likelihood));
	OverflowLikelihood.SetPctResistant(DMites.GetPctResistant());

	//  Determine if too many mites/drone cell.  If so send excess to worker cells
	if (DMites.GetTotal() > MAXMITES_PER_DRONE_CELL*DrnBrood->GetNumber())
	{
		OverflowMax = DMites.GetTotal() - (MAXMITES_PER_DRONE_CELL*DrnBrood->GetNumber());
		OverflowMax.SetPctResistant(DMites.GetPctResistant());
		DMites -= OverflowMax;
	}

	// Add all the overflow mites to those available to infest worker brood
	WMites += OverflowMax + OverflowLikelihood;

	//  Determine if too many mites/worker cell.  If so, limit
	if (WMites.GetTotal() > MAXMITES_PER_WORKER_CELL*WkrBrood->GetNumber()) 
	{
		const double pr = WMites.GetPctResistant();
		WMites = MAXMITES_PER_WORKER_CELL*WkrBrood->GetNumber();
		WMites.SetPctResistant(pr);
	}
	RunMite = RunMite - WMites - DMites;
	if (RunMite.GetTotal()<0) RunMite = 0;

	WkrBrood->m_Mites = WMites;
	WkrBrood->m_PropVirgins = PropRMVirgins;
	DrnBrood->m_Mites = DMites;
	DrnBrood->m_PropVirgins = PropRMVirgins;


	// Calculate proportion of infestation for worker brood cells
	PropInfstW = CapWkr.GetPropInfest();

	// Calculate proportion of infestation for drone brood cells
	PropInfstD = (float)CapDrn.GetPropInfest();

	/*
		Now we determine the emerging mites from the first boxcar in
		the adult lists

	*/

	// Set the correct values for numbers of cells and numbers of mites
	// emerging this time
	CBrood WkrEmerge;
	CBrood DrnEmerge;
	WkrEmerge.number = ((CAdult*)Wadl.GetHead())->GetNumber();
	WkrEmerge.m_Mites = ((CAdult*)Wadl.GetHead())->GetMites();
	WkrEmerge.m_PropVirgins = ((CAdult*)Wadl.GetHead())->GetPropVirgins();
	DrnEmerge.number = ((CAdult*)Dadl.GetHead())->GetNumber();
	DrnEmerge.m_Mites = ((CAdult*)Dadl.GetHead())->GetMites();
	DrnEmerge.m_PropVirgins = ((CAdult*)Dadl.GetHead())->GetPropVirgins();



	double MitesPerCellW;
	double MitesPerCellD;
	if (WkrEmerge.GetNumber() == 0) MitesPerCellW = 0;
	else MitesPerCellW = (double)WkrEmerge.m_Mites.GetTotal()/WkrEmerge.GetNumber();
	if (DrnEmerge.GetNumber() == 0) MitesPerCellD = 0;
	else MitesPerCellD = (double)DrnEmerge.m_Mites.GetTotal()/DrnEmerge.GetNumber();

	// Calculate survivorship
	const double PropSurviveMiteW = m_InitCond.m_workerMiteSurvivorship/100;  
	const double PropSurviveMiteD = m_InitCond.m_droneMiteSurvivorshipField/100;  
	
	// Calculate reproduction rates per mite per cell
	double ReproMitePerCellD;
	double ReproMitePerCellW;


	if (MitesPerCellW <= 1.0) ReproMitePerCellW = 
		m_InitCond.m_workerMiteOffspring;
	else ReproMitePerCellW = 
			double((1.15*MitesPerCellW) - (0.233*MitesPerCellW*MitesPerCellW));
	if (ReproMitePerCellW < 0) ReproMitePerCellW = 0;


	if (MitesPerCellD <= 1.0) ReproMitePerCellD = 
		m_InitCond.m_droneMiteOffspringField;
	else ReproMitePerCellD = 
			double(1.11 + (0.151*MitesPerCellD) - (0.3*MitesPerCellD*MitesPerCellD));
	if (ReproMitePerCellD < 0) ReproMitePerCellD = 0;
 
	// Calculate the number of new running mites at the end of this cycle
	#define PROPRUNMITE2 0.6

	CMite SurviveMitesW = WkrEmerge.m_Mites * PropSurviveMiteW;
	CMite SurviveMitesD = DrnEmerge.m_Mites * PropSurviveMiteD;

	const int NumEmergingMites = SurviveMitesW.GetTotal() + SurviveMitesD.GetTotal();
	
	CMite NewMitesW = SurviveMitesW * ReproMitePerCellW;
	CMite NewMitesD = SurviveMitesD * ReproMitePerCellD;

	SurviveMitesW = SurviveMitesW * WkrEmerge.m_PropVirgins;
	SurviveMitesD = SurviveMitesD * DrnEmerge.m_PropVirgins;

	const int NumVirgins = SurviveMitesW.GetTotal() + SurviveMitesD.GetTotal();

	m_MitesDyingToday = int(NumEmergingMites - NumVirgins + NumVirgins*(1-PROPRUNMITE2));
	m_MitesDyingToday = (m_MitesDyingToday >= 0) ? m_MitesDyingToday : 0; // Constrain positive 

	CMite RunMiteVirgins = RunMite*PropRMVirgins;
	RunMite = RunMite + NewMitesW + NewMitesD +
		(SurviveMitesW + SurviveMitesD)*PROPRUNMITE2;
	if (RunMite.GetTotal()<=0) PropRMVirgins = 1.0;
	else PropRMVirgins = double(	RunMiteVirgins.GetTotal() + 
							NewMitesW.GetTotal() + 
							NewMitesD.GetTotal()	)/double(RunMite.GetTotal());

	// Kill NonResistant Running Mites if Treatment Enabled
	/*
	if ((m_VTEnable) && 
		(DayNum >= m_VTStart) && 
		(DayNum <= int(m_VTStart + (7*m_VTDuration))))
		*/
	if (m_VTEnable)
	{
		CMiteTreatmentItem theItem;
		COleDateTime* theDate = GetDayNumDate(DayNum);
		if (m_MiteTreatmentInfo.GetActiveItem(*theDate,theItem))
		{
			const int Quan = RunMite.GetTotal();
			RunMite.SetNonResistant(int(double(RunMite.GetNonResistant())*
												(100.0 - theItem.PctMortality)/100.0));
			m_MitesDyingToday += (Quan - RunMite.GetTotal());
			//TRACE("Mite Treatment Made on %s\n",theDate->Format("%m/%d/%Y"));
		}

		m_MitesDyingThisPeriod += m_MitesDyingToday;
		delete theDate;
	}
}
	

void CColony::
ReQueenIfNeeded(
			int		SimDayNum,
			CEvent* theEvent,
			UINT	EggLayingDelay,
			double	WkrDrnRatio,
			BOOL	EnableReQueen,
			int		Scheduled,	// Scheduled == 0,  Automatic != 0
			double		QueenStrength,
			int		RQOnce,		// Once == 0, Annually != 0
			COleDateTime	ReQueenDate)
{
	/* 
	
	There are 2 types of re-queening, 1) Scheduled or 2) Automatic

	For an Automatic re-queening, the re-queening is triggered when the 
	proportion of non-fertilized eggs falls below a certain level but only 
	between April and September of the year, inclusive.

	For Scheduled re-queening, the event is initially triggered on the ReQueenDate.  
	Subsequent re-queenings occur annually if RQOnce is != 0.  Initial re-queening
	occurs when the Year, Month, and Day of the ReQueenDate and the current simulation 
	date are equal.  Subsequent annual re-queening occurs when the Month and Day of 
	the ReQeenDate and the current simulation date are equal.

	Once re-queening happens, the new queen has the specified QueenStrength but egg-
	laying is delayed EggLayingDelay days beyond the date of the re-queening.

	No re-queening occurs if EnableReQueen is false

	*/

	double AppliedStrength = QueenStrength;
	if (!EnableReQueen) return;
	if (Scheduled==0)
	{
		if (  // Initial re-queening
			((ReQueenDate.GetYear()==theEvent->GetTime().GetYear()) &&
			(ReQueenDate.GetMonth()==theEvent->GetTime().GetMonth()) &&
			(ReQueenDate.GetDay()==theEvent->GetTime().GetDay())) ||

			  // Subsequent re-queening
			((ReQueenDate.GetYear()<theEvent->GetTime().GetYear()) &&
			(ReQueenDate.GetMonth()==theEvent->GetTime().GetMonth()) &&
			(ReQueenDate.GetDay()==theEvent->GetTime().GetDay()) &&
			(RQOnce != 0)))
		{
			if (!m_RQQueenStrengthArray.IsEmpty()) // Pop the next strength off the array
			{
				AppliedStrength = m_RQQueenStrengthArray[0];
				m_RQQueenStrengthArray.RemoveAt(0);
			}
			CString notification;
			notification.Format("Scheduled Requeening Occurred, Strength %5.1f",AppliedStrength);
			AddEventNotification(theEvent->GetDateStg("%m/%d/%Y"),notification);
			queen.ReQueen(EggLayingDelay, AppliedStrength, SimDayNum);
		}
	}
	else  // Automatic re-queening
	{
		if ((queen.GetPropDroneEggs() > 0.15 ) &&
			(theEvent->GetTime().GetMonth()>3) && 
			(theEvent->GetTime().GetMonth()<10))
		{
			if (!m_RQQueenStrengthArray.IsEmpty()) // Pop the next strength off the array
			{
				AppliedStrength = m_RQQueenStrengthArray[0];
				m_RQQueenStrengthArray.RemoveAt(0);
			}
			CString notification;
			notification.Format("Automatic Requeening Occurred, Strength %5.1f",AppliedStrength);
			AddEventNotification(theEvent->GetDateStg("%m/%d/%Y"),notification);
			queen.ReQueen(EggLayingDelay, AppliedStrength, SimDayNum);
		}
	}



}

void CColony::SetMiticideTreatment(int StartDayNum, UINT Duration, UINT Mortality, BOOL Enable)
{
	if (Enable)
	{
		m_VTStart = StartDayNum; 
		m_VTDuration = Duration; 
		m_VTMortality = Mortality;
		m_VTEnable = true;
	}
	else m_VTEnable = false;
	m_VTTreatmentActive = false;
}

void CColony::SetMiticideTreatment(CMiteTreatments& theTreatments, BOOL Enable)
{
	if (Enable)
	{
		m_VTEnable = true;
		CMiteTreatmentItem DestItem;
		CMiteTreatmentItem SourceItem;
		m_MiteTreatmentInfo.ClearAll();
		for (int i = 0; i < theTreatments.GetCount(); i++)
		{
			theTreatments.GetItem(i, SourceItem);
			DestItem.theStartTime = SourceItem.theStartTime;
			DestItem.Duration = SourceItem.Duration;
			DestItem.PctMortality = SourceItem.PctMortality;
			DestItem.PctResistant = SourceItem.PctResistant;
			m_MiteTreatmentInfo.AddItem(DestItem);
		}	    
	}
	else m_VTEnable = false;
	m_VTTreatmentActive = false;    
}


void CColony::SetSporeTreatment(int StartDayNum, BOOL Enable)
{
	if (Enable)
	{
		m_SPStart = StartDayNum; 
		m_SPEnable = true;
	}
	else m_SPEnable = false;
	m_SPTreatmentActive = false;
}

void CColony::RemoveDroneComb(double pct)
{
	// Simulates the removal of drone comb.  The variable pct is the amount to 
	// be removed

	if (pct > 100) pct = 100.0;
	if (pct < 0) pct = 0.0;

	POSITION pos;

	pos = Deggs.GetHeadPosition();

	CEgg* theEgg;
	while(pos != NULL)
	{
		theEgg = (CEgg*)Deggs.GetNext(pos);
		theEgg->number *= (int)(100.0-pct);
	}

	pos = Dlarv.GetHeadPosition();
	CLarva* theLarv;
	while(pos != NULL)
	{
		theLarv = (CLarva*)Dlarv.GetNext(pos);
		theLarv->number *= (int)(100.0-pct);
	}

	pos = CapDrn.GetHeadPosition();

	CBrood* theBrood;
	while(pos != NULL)
	{
		theBrood = (CBrood*)CapDrn.GetNext(pos);
		theBrood->number *= (int)(100.0-pct);
		theBrood->m_Mites = theBrood->m_Mites * (100.0 - pct);
		theBrood->m_PropVirgins = 0;
	}
}


void CColony::AddDiscreteEvent(CString datestg, UINT EventID)
{
	CUIntArray* pEventArray;
	if (m_EventMap.Lookup(datestg,(CObject*&)pEventArray))
	{
		// Date already exists, add a new event to the array
		pEventArray->Add(EventID);
	}
	else
	{
		// Create new map element
		pEventArray = new CUIntArray();
		pEventArray->Add(EventID);
		m_EventMap.SetAt(datestg, pEventArray);
	}
}

void CColony::RemoveDiscreteEvent(CString datestg, UINT EventID)
{
	CUIntArray* pEventArray;
	if (m_EventMap.Lookup(datestg,(CObject*&)pEventArray))
	{
		// Date exists
		for (int i = 0; i < pEventArray->GetSize(); i++)
		{
			if (pEventArray->GetAt(i) == EventID) pEventArray->RemoveAt(i--);
		}
		if (pEventArray->GetSize() == 0)
		{
			delete pEventArray;
			m_EventMap.RemoveKey(datestg);
		}
	}

}


BOOL CColony::GetDiscreteEvents(CString key, CUIntArray*& theArray)
{
	return m_EventMap.Lookup(key,(CObject*&)theArray);
}

// DoPendingEvents is used when running WebBeePop.  The predefined events from a legacy program are
// mapped into VarroaPop parameters and this is executed as part of the main simulation loop.  A much 
// simplified set of features for use by elementary school students.
void CColony::DoPendingEvents(CEvent* pWeatherEvent, int CurrentSimDay)
{
	CUIntArray* pEventArray = NULL;

	if (!GetDiscreteEvents(pWeatherEvent->GetDateStg("%m/%d/%Y"),pEventArray)) return;
	for (int i=0; i<pEventArray->GetSize(); i++)
	{
		//TRACE("A Discrete Event on %s\n",pWeatherEvent->GetDateStg("%m/%d/%Y"));
		MyMessageBox("Looking For a Discrete Event\n");
		const int EggLayDelay = 17;
		const int Strength = 5;

		switch(pEventArray->GetAt(i))
		{
		case DE_SWARM:	// Swarm
			MyMessageBox("Detected SWARM Discrete Event\n");
			AddEventNotification(pWeatherEvent->GetDateStg("%m/%d/%Y"), "Detected SWARM Discrete Event");
			foragers.FactorQuantity(0.75);
			Wadl.FactorQuantity(0.75);
			Dadl.FactorQuantity(0.75);
			break;

		case DE_CHALKBROOD:	// Chalk Brood
			//  All Larvae Die
			AddEventNotification(pWeatherEvent->GetDateStg("%m/%d/%Y"), "Detected CHALKBROOD Discrete Event");
			Dlarv.FactorQuantity(0.0);
			Wlarv.FactorQuantity(0.0);
			break;

		case DE_RESOURCEDEP:	// Resource Depletion
			//  Forager Lifespan = minimum
			AddEventNotification(pWeatherEvent->GetDateStg("%m/%d/%Y"), "Detected RESOURCEDEPLETION Discrete Event");
			m_InitCond.m_ForagerLifespan = 4;
			break;


		case DE_SUPERCEDURE:	// Supercedure of Queen
			//  New queen = 17 days before egg laying starts
			AddEventNotification(pWeatherEvent->GetDateStg("%m/%d/%Y"), "Detected SUPERCEDURE Discrete Event");
			queen.ReQueen(EggLayDelay,Strength,CurrentSimDay);
			break;

		case DE_PESTICIDE:	// Death of foragers by pesticide
			//  25% of foragers die
			AddEventNotification(pWeatherEvent->GetDateStg("%m/%d/%Y"), "Detected PESTICIDE Discrete Event");
			foragers.FactorQuantity(0.75);
			break;

		default:;
		}
	}
}




int CColony::GetMitesDyingToday()
{
	return m_MitesDyingToday;
}

int CColony::GetNurseBees()
// Number of nurse bees is defined as # larvae/2.  Implication is that a nurse bee is needed for each two larvae
{
	const int TotalLarvae = Wlarv.GetQuantity() + Dlarv.GetQuantity();
	return TotalLarvae/2;
}

int CColony::GetTotalMiteCount()
{
	return ( RunMite.GetTotal() + CapDrn.GetMiteCount() + CapWkr.GetMiteCount() );

}

void CColony::SetStartSamplePeriod()
{
	// Notifies CColony that it is the beginning of a sample period.  Since we gather either weekly or 
	// daily data this is used to reset accumulators.
	m_MitesDyingThisPeriod = 0;
}

int CColony::GetMitesDyingThisPeriod()
{
	return m_MitesDyingThisPeriod;
}


////////////////////////////////////////////////////////////////////////////////
// Pesticide Dose and Mortality calculations
//
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ApplyPesticideMortality
//
// Called after ConsumeFood and DetermineFoliarDose.  At that point, the m_EPAData.m_D_ variables
// contain the current exposure of each bee life stage in grams/bee.  This function performs the appropriate reductions
// of bee quantities based on the doses and the appropriate Slope/LD50 for the bee type and current pesticide.
//
// Constraint:  Bee quantities are not reduced unless the current pesticide dose is > previous maximum dose.  But, 
//              for bees just getting into Larva4 or Adult1, this is the first time they have had a dose.
//
//
void CColony::ApplyPesticideMortality()
{
	// Worker Larvae 4
	//if (m_EPAData.m_D_L4 > m_EPAData.m_D_L4_Max) // IED - only reduce if current dose greater than previous maximum dose
	{
		m_DeadWorkerLarvaePesticide = ApplyPesticideToBees(&Wlarv,3,3,m_EPAData.m_D_L4,0,m_EPAData.m_AI_LarvaLD50,m_EPAData.m_AI_LarvaSlope);
	//	if (m_EPAData.m_D_L4 > 0.0) TRACE("Pesticide to Larva 4: %4.2f\n",m_EPAData.m_D_L4);
		if (m_EPAData.m_D_L4 > m_EPAData.m_D_L4_Max) m_EPAData.m_D_L4_Max = m_EPAData.m_D_L4;
	}

	// Worker Larvae 5
	if (m_EPAData.m_D_L5 > m_EPAData.m_D_L5_Max) 
	{
		m_DeadWorkerLarvaePesticide += ApplyPesticideToBees(&Wlarv,4,4,m_EPAData.m_D_L5,m_EPAData.m_D_L5_Max,m_EPAData.m_AI_LarvaLD50,m_EPAData.m_AI_LarvaSlope);
		m_EPAData.m_D_L5_Max = m_EPAData.m_D_L5;
	}

	// Drone Larvae
	m_DeadDroneLarvaePesticide = ApplyPesticideToBees(&Dlarv,3,3,m_EPAData.m_D_LD,0,m_EPAData.m_AI_LarvaLD50,m_EPAData.m_AI_LarvaSlope); // New L4 drones
	if (m_EPAData.m_D_LD > m_EPAData.m_D_LD_Max)
	{
		m_DeadDroneLarvaePesticide += ApplyPesticideToBees(&Dlarv,4,DLARVLIFE-1,m_EPAData.m_D_LD,m_EPAData.m_D_LD_Max,m_EPAData.m_AI_LarvaLD50,m_EPAData.m_AI_LarvaSlope);
		m_EPAData.m_D_LD_Max = m_EPAData.m_D_LD;

	}


	// Worker Adults 1-3
	m_DeadWorkerAdultsPesticide = ApplyPesticideToBees(&Wadl,0,0,m_EPAData.m_D_A13,0,m_EPAData.m_AI_AdultLD50,m_EPAData.m_AI_AdultSlope); // New adults
	if (m_EPAData.m_D_A13 > m_EPAData.m_D_A13_Max) 
	{
		m_DeadWorkerAdultsPesticide += ApplyPesticideToBees(&Wadl,1,2,m_EPAData.m_D_A13,m_EPAData.m_D_A13_Max,m_EPAData.m_AI_AdultLD50,m_EPAData.m_AI_AdultSlope);
		m_EPAData.m_D_A13_Max = m_EPAData.m_D_A13;
	}


	// Worker Adults 4-10
	if (m_EPAData.m_D_A410 > m_EPAData.m_D_A410_Max) 
	{
		m_DeadWorkerAdultsPesticide += ApplyPesticideToBees(&Wadl,3,9,m_EPAData.m_D_A410,m_EPAData.m_D_A410_Max,m_EPAData.m_AI_AdultLD50,m_EPAData.m_AI_AdultSlope);
		m_EPAData.m_D_A410_Max = m_EPAData.m_D_A410;
	}

	// Worker Adults 11-20
	if (m_EPAData.m_D_A1120 > m_EPAData.m_D_A1120_Max) 
	{
		m_DeadWorkerAdultsPesticide += ApplyPesticideToBees(&Wadl,10,WADLLIFE-1,m_EPAData.m_D_A1120,m_EPAData.m_D_A1120_Max,m_EPAData.m_AI_AdultLD50,m_EPAData.m_AI_AdultSlope);
		m_EPAData.m_D_A1120_Max = m_EPAData.m_D_A1120;
	}

	// Worker Drones
	m_DeadDroneAdultsPesticide = ApplyPesticideToBees(&Dadl,0,0,m_EPAData.m_D_AD,0,m_EPAData.m_AI_AdultLD50,m_EPAData.m_AI_AdultSlope);
	if (m_EPAData.m_D_AD > m_EPAData.m_D_AD_Max) 
	{
		m_DeadDroneAdultsPesticide += ApplyPesticideToBees(&Dadl,1,DADLLIFE-1,m_EPAData.m_D_AD,m_EPAData.m_D_AD_Max,m_EPAData.m_AI_AdultLD50,m_EPAData.m_AI_AdultSlope);
		m_EPAData.m_D_AD_Max = m_EPAData.m_D_AD;
	}

	// Foragers - Contact Mortality
	m_DeadForagersPesticide = ApplyPesticideToBees(&foragers,0,0,m_EPAData.m_D_C_Foragers,0,m_EPAData.m_AI_AdultLD50_Contact,m_EPAData.m_AI_AdultSlope_Contact);
	if (m_EPAData.m_D_C_Foragers > m_EPAData.m_D_C_Foragers_Max) 
	{
		m_DeadForagersPesticide += ApplyPesticideToBees(&foragers,1,foragers.GetLength() - 1,m_EPAData.m_D_C_Foragers,m_EPAData.m_D_C_Foragers_Max,m_EPAData.m_AI_AdultLD50_Contact,m_EPAData.m_AI_AdultSlope_Contact);
		m_EPAData.m_D_C_Foragers_Max = m_EPAData.m_D_C_Foragers;
	}

	// Foragers - Diet Mortality
	m_DeadForagersPesticide += ApplyPesticideToBees(&foragers, 0, 0, m_EPAData.m_D_D_Foragers, 0, m_EPAData.m_AI_AdultLD50, m_EPAData.m_AI_AdultSlope);
	if (m_EPAData.m_D_D_Foragers > m_EPAData.m_D_D_Foragers_Max)
	{
		m_DeadForagersPesticide += ApplyPesticideToBees(&foragers, 1, foragers.GetLength() - 1, m_EPAData.m_D_D_Foragers, m_EPAData.m_D_D_Foragers_Max, m_EPAData.m_AI_AdultLD50, m_EPAData.m_AI_AdultSlope);
		m_EPAData.m_D_D_Foragers_Max = m_EPAData.m_D_D_Foragers;
	}
	if (m_DeadForagersPesticide > 0)
	{
		int i = 0;
	}
	// Reset the current doses to zero after mortality is applied.
	m_EPAData.m_D_L4 = 0;
	m_EPAData.m_D_L5 = 0;
	m_EPAData.m_D_LD = 0;
	m_EPAData.m_D_A13 = 0;
	m_EPAData.m_D_A410 = 0;
	m_EPAData.m_D_A1120 = 0;
	m_EPAData.m_D_AD = 0;
	m_EPAData.m_D_C_Foragers = 0;
	m_EPAData.m_D_D_Foragers = 0;
}

// QuantityPesticideToKill
// This just calculates the number of bees in the list that would be killed by the pesticide and dose
int CColony::QuantityPesticideToKill(CBeelist* pList, double CurrentDose, double MaxDose, double LD50, double Slope)
{
		int BeeQuant;
		int NewBeeQuant;
		BeeQuant = pList->GetQuantity();
		const double reduxcurrent = m_EPAData.DoseResponse(CurrentDose, LD50, Slope);
		const double reduxmax = m_EPAData.DoseResponse(MaxDose, LD50, Slope);
		if (reduxcurrent <= reduxmax) return 0;  // Less than max already seen
		NewBeeQuant = (int)(BeeQuant * ( 1 - (reduxcurrent - reduxmax)));
		//pList->SetQuantityAt(from,to,NewBeeQuant);
		return BeeQuant - NewBeeQuant;  // This is the number killed by pesticide
}

// ApplyPesticideToBees
// This calculates the number of bees to kill then reduces that number from all age groups between "from" and "to" in the list.
int CColony::ApplyPesticideToBees(CBeelist* pList, int from, int to, double CurrentDose, double MaxDose, double LD50, double Slope)
{
		int BeeQuant;
		int NewBeeQuant;
		BeeQuant = pList->GetQuantityAt(from,to);
		if (BeeQuant <= 0) return 0;
		const double reduxcurrent = m_EPAData.DoseResponse(CurrentDose, LD50, Slope);
		const double reduxmax = m_EPAData.DoseResponse(MaxDose, LD50, Slope);
		if (reduxcurrent <= reduxmax) return 0;  // Less than max already seen
		NewBeeQuant = (int)(BeeQuant * ( 1 - (reduxcurrent - reduxmax)));
		const double PropRedux = (double)NewBeeQuant/(double)BeeQuant;
		//pList->SetQuantityAt(from,to,NewBeeQuant);
		pList->SetQuantityAtProportional(from,to,PropRedux);
		return BeeQuant - NewBeeQuant;  // This is the number killed by pesticide
}



///////////////////////////////////////////////////////////////////////////////////////
// DetermineFoliarDose
//
// If we are in a date range with Dose, this routing adds to the Dose rate variables.
// 
void CColony::DetermineFoliarDose(int DayNum)
{
	// Jump out if Foliar is not enabled
	if (!m_EPAData.m_FoliarEnabled) return;

	COleDateTime* pDate = GetDayNumDate(DayNum);
	const COleDateTime CurDate(pDate->GetYear(), pDate->GetMonth(), pDate->GetDay(), 0,0,0);
	delete pDate;
	
	// In order to expose, must be after the application date and inside the forage window
	if ((CurDate >= m_EPAData.m_FoliarAppDate) && (CurDate >= m_EPAData.m_FoliarForageBegin) && (CurDate < m_EPAData.m_FoliarForageEnd))
	{
		const LONG DaysSinceApplication = (CurDate - m_EPAData.m_FoliarAppDate).GetDays();

		// Foliar Dose is related to AI application rate and Contact Exposure factor (See
		// Kris Garber's EFED Training Insect Exposure.pptx for a summary); 

		double Dose = m_EPAData.m_E_AppRate * m_EPAData.m_AI_ContactFactor/1000000.0;  // convert to Grams AI/bee
		//TRACE("Incoming Foliar Pesticide on %s\n",CurDate.Format("%m/%d/%Y"));

		// Dose reduced due to active ingredient half-life
		if (m_EPAData.m_AI_HalfLife > 0)
		{
			const double k = log(2.0)/m_EPAData.m_AI_HalfLife;
			Dose *= exp(-k*DaysSinceApplication);
		}
		m_EPAData.m_D_C_Foragers += Dose;  // Adds to any diet-based exposure.  Only foragers impacted.
	}

}
/////////////////////////////////////////////////////////////////////////////////
// ConsumeFood 
//
// Determines colony needs, incoming nutrient availability and stored resource availability
// and adjusts the values accordingly.  When this function is complete, the diet-based pesticide Dose (per bee)
// for each lifestage is known and can be used in the mortality calculations.
void CColony::ConsumeFood(CEvent* pEvent, int DayNum)
{
	if (DayNum == 1) return;


	if (IsPollenFeedingDay(pEvent))
	{
		//TRACE("Pollen Feeding Day on %s\n",pEvent->GetDateStg("%m/%d/%Y"));
	}
	if (IsNectarFeedingDay(pEvent))
	{
		//TRACE("Nectar Feeding Day on %s\n",pEvent->GetDateStg("%m/%d/%Y"));

	}

	double NeedP = GetPollenNeeds(pEvent);	// The amount of pollen the colony needs today
	double NeedN = GetNectarNeeds(pEvent);	// The amount of nectar the colony needs today
	double InP = 0;							// The amount of pollen coming into the colony today
	double InN = 0;							// The amount of nectar coming into the colony today
	double C_AI_P = 0;						// The incoming pollen concentration today in Grams AI/Gram
	double C_AI_N = 0;						// The incoming nectar concentration today in Grams AI/Gram
	double C_Actual_P = 0;					// The actual concentration of exposure the bees receive this day in Grams AI/Gram
	double C_Actual_N = 0;					// The actual concentration of exposure the bees receive this day in Grams AI/Gram

	SResourceItem theResource;

	if (pEvent->IsForageDay())  
	{
		// Pollen
		if (IsPollenFeedingDay(pEvent))
		{
			InP = GetIncomingPollenQuant();
			m_SuppPollen.m_CurrentAmount -= NeedP;  // Decrement current supplemental pollen
			//TRACE("Current Supp Pollen = %6.3f\n",m_SuppPollen.m_CurrentAmount);
			NeedP = 0; // All pollen needs are met by paddies and all incoming pollen will be stored - no incoming needed by colony
			C_AI_P = 0;
		}
		else 
		{
			InP = GetIncomingPollenQuant();
			C_AI_P = GetIncomingPollenPesticideConcentration(DayNum);
		}

		// Nectar
		if (IsNectarFeedingDay(pEvent))
		{
			// For nectar feeding day, move supplemental nectar to stores then process same as when no supplemental
			theResource.m_ResourseQuantity = m_SuppNectar.m_StartingAmount / ((m_SuppNectar.m_EndDate - m_SuppNectar.m_BeginDate).GetDays());
			theResource.m_PesticideQuantity = 0;
			AddNectarToResources(theResource);
			//m_Resources.AddNectar(theResource);
			m_SuppNectar.m_CurrentAmount -= theResource.m_ResourseQuantity;  // Decrement current supplemental nectar
			//TRACE("Current Supp Nectar = %6.3f\n", m_SuppNectar.m_CurrentAmount);

			InN = GetIncomingNectarQuant();
			C_AI_N = 0;
		}
		else
		{
			InN = GetIncomingNectarQuant();
			C_AI_N = GetIncomingNectarPesticideConcentration(DayNum);
		}
	}
	else // Non foraging day
	{
		InP = 0;
		if (IsPollenFeedingDay(pEvent))
		{
			m_SuppPollen.m_CurrentAmount -= NeedP;  // Decrement current supplemental pollen
			NeedP = 0;  // Pollen needs met by paddies
		}
		InN = 0;
		if (IsNectarFeedingDay(pEvent))
		{
			// For nectar feeding day, move supplemental nectar to stores then process same as when no supplemental
			theResource.m_ResourseQuantity = m_SuppNectar.m_StartingAmount / ((m_SuppNectar.m_EndDate - m_SuppNectar.m_BeginDate).GetDays());
			theResource.m_PesticideQuantity = 0;
			AddNectarToResources(theResource);
		}
		C_AI_P = 0;
		C_AI_N = 0;
	}

	// Check to see if we need to use stored pollen
	if (InP >= NeedP) // Enough incoming pollen to meet need - don't use stores 
	{
		C_Actual_P = C_AI_P;
		theResource.m_ResourseQuantity = InP - NeedP;
		theResource.m_PesticideQuantity = (InP - NeedP) * C_Actual_P;
		AddPollenToResources(theResource);
	}
	else // Less incoming pollen than needed
	{
		double Shortfall = NeedP - InP;
		if (Shortfall < 0) Shortfall = 0;  

		// Calculate resultant concentration [C1*Q1 + C2*Q2]/[Q1 + Q2]
		C_Actual_P = ((C_AI_P * InP) + (m_Resources.GetPollenPesticideConcentration() * Shortfall))/(InP + Shortfall);

		/// TEST - Disable Killing Colony
		//if (false)
		if (m_Resources.GetPollenQuantity() < Shortfall) //&& pEvent->IsWinterDay()) 
		{
			if (m_NoResourceKillsColony)
			{			
				KillColony();
				AddEventNotification(pEvent->GetDateStg("%m/%d/%Y"), "Colony Died - Lack of Pollen Stores");
			}
		}
		//else 
		m_Resources.RemovePollen(Shortfall);
	}

	// Check to see if we need to use stored nectar
	if (InN >= NeedN) // Enough incoming nectar
	{
		C_Actual_N = C_AI_N;
		theResource.m_ResourseQuantity = InN - NeedN;
		theResource.m_PesticideQuantity = (InN - NeedN) * C_Actual_N;
		AddNectarToResources(theResource);
	}
	else // Less incoming nectar than need
	{
		double Shortfall = NeedN - InN;
		if (Shortfall < 0) Shortfall = 0; 

		// Calculate resultant concentration [C1*Q1 + C2*Q2]/[Q1 + Q2]
		C_Actual_N = ((C_AI_N * InN) + (m_Resources.GetNectarPesticideConcentration() * Shortfall))/(InN + Shortfall);

		if (m_Resources.GetNectarQuantity() < Shortfall) // && pEvent->IsWinterDay()) 
		{
			if (m_NoResourceKillsColony)
			{
				KillColony();
				//TRACE("Killed Colony due to lack of Nectar stores on %s\n", pEvent->GetDateStg("%m/%d/%Y"));
				AddEventNotification(pEvent->GetDateStg("%m/%d/%Y"), "Colony Died - Lack of Nectar Stores");
			}
		}
		//else 
		m_Resources.RemoveNectar(Shortfall);
		if (m_Resources.GetNectarQuantity() < 0)
		{
			int i = 1;
		}
	}

	m_EPAData.m_D_L4 = C_Actual_P * m_EPAData.m_C_L4_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_L4_Nectar/1000.0;
	m_EPAData.m_D_L5 = C_Actual_P * m_EPAData.m_C_L5_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_L5_Nectar/1000.0;
	m_EPAData.m_D_LD = C_Actual_P * m_EPAData.m_C_LD_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_LD_Nectar/1000.0;
	m_EPAData.m_D_A13 = C_Actual_P * m_EPAData.m_C_A13_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_A13_Nectar/1000.0;
	m_EPAData.m_D_A410 = C_Actual_P * m_EPAData.m_C_A410_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_A410_Nectar/1000.0;
	m_EPAData.m_D_A1120 = C_Actual_P * m_EPAData.m_C_A1120_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_A1120_Nectar/1000.0;
	m_EPAData.m_D_AD = C_Actual_P * m_EPAData.m_C_AD_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_AD_Nectar/1000.0;
	m_EPAData.m_D_D_Foragers = C_Actual_P * m_EPAData.m_C_Forager_Pollen/1000.0 + C_Actual_N * m_EPAData.m_C_Forager_Nectar/1000.0;
}

void CColony::AddPollenToResources(SResourceItem theResource)
{
	// Add pollen but don't exceed the maximum amount
	// If CurrentPollen/MaxPollen > 0.9, Add theResources * (1 - CurrentPollen/MaxPollen)
	if (m_ColonyPolMaxAmount <= 0) 
	{
		MyMessageBox("Maximum Colony Pollen is <= 0.  Forcing to 5000g");
		m_ColonyPolMaxAmount = 5000;
	}
	const double PropFull = m_Resources.GetPollenQuantity()/m_ColonyPolMaxAmount;
	const double Reduction = 1 - m_Resources.GetPollenQuantity()/m_ColonyPolMaxAmount;
	if (PropFull > 0.9)
	{
		theResource.m_ResourseQuantity *= Reduction;
		theResource.m_PesticideQuantity *= Reduction;
	}

	m_Resources.AddPollen(theResource);
}

void CColony::AddNectarToResources(SResourceItem theResource)
{
	// Add nectar but don't exceed the maximum amount
	if (m_ColonyNecMaxAmount <= 0) 
	{
		MyMessageBox("Maximum Colony Nectar is <= 0.  Forcing to 5000g");
		m_ColonyNecMaxAmount = 5000;
	}
	const double PropFull = m_Resources.GetNectarQuantity()/m_ColonyNecMaxAmount;

	double Reduction = 1 - m_Resources.GetNectarQuantity()/m_ColonyNecMaxAmount;
	if (Reduction < 0)
	{
		// Don't exceed Max Value
		Reduction = 0;
	}
	if (PropFull > 0.9)
	{
		theResource.m_ResourseQuantity *= Reduction;
		theResource.m_PesticideQuantity *= Reduction;
	}

	m_Resources.AddNectar(theResource);
}


// Supplemental feeding days are days when:
//   There is some supplemental feed available (current amount > 0),
//   The current day is in the window of Begin Date and End Date
//	 The colony is not dead
//
// Note:  Still need to confirm this.  May eliminate end date logic if 
// the decision is made to just consume the resources and quit.
bool CColony::IsPollenFeedingDay(CEvent* pEvent)
{
	bool FeedingDay = false;

	if ((m_SuppPollenEnabled) && (GetColonySize() > 100))  // Supplemental enabled and colony still alive
	{
		if (m_SuppPollenAnnual)
		{
			const COleDateTime TestBeginTime(pEvent->GetTime().GetYear(), m_SuppPollen.m_BeginDate.GetMonth(), m_SuppPollen.m_BeginDate.GetDay(),0,0,0);
			const COleDateTime TestEndTime(pEvent->GetTime().GetYear(), m_SuppPollen.m_EndDate.GetMonth(), m_SuppPollen.m_EndDate.GetDay(),0,0,0);

			FeedingDay = ((m_SuppPollen.m_CurrentAmount > 0.0) &&
				(TestBeginTime < pEvent->GetTime()) &&
				(TestEndTime >= pEvent->GetTime()));
			
		}
		else
		{
			FeedingDay = ((m_SuppPollen.m_CurrentAmount > 0.0) &&
				(m_SuppPollen.m_BeginDate < pEvent->GetTime()) &&
				(m_SuppPollen.m_EndDate >= pEvent->GetTime()));
		}
	}
	return FeedingDay;
}

bool CColony::IsNectarFeedingDay(CEvent* pEvent)
{
	bool FeedingDay = false;
	if ((m_SuppNectarEnabled) && (GetColonySize() > 100))  // Supplemental enabled and colony still alive
	{
		if (m_SuppNectarAnnual)
		{
			const COleDateTime TestBeginTime(pEvent->GetTime().GetYear(), m_SuppNectar.m_BeginDate.GetMonth(), m_SuppNectar.m_BeginDate.GetDay(),0,0,0);
			const COleDateTime TestEndTime(pEvent->GetTime().GetYear(), m_SuppNectar.m_EndDate.GetMonth(), m_SuppNectar.m_EndDate.GetDay(),0,0,0);

			FeedingDay = ((m_SuppNectar.m_CurrentAmount > 0.0) &&
				(TestBeginTime < pEvent->GetTime()) &&
				(TestEndTime >= pEvent->GetTime()));		}
		else
		{
			FeedingDay = ((m_SuppNectar.m_CurrentAmount > 0.0) &&
				(m_SuppNectar.m_BeginDate < pEvent->GetTime()) &&
				(m_SuppNectar.m_EndDate >= pEvent->GetTime()));
		}
	}
	return FeedingDay;
}

void CColony::InitializeColonyResources(void)
{
	////////////////////////////////////////////////
	// TODO:
	// This should ultimately be pre-settable at the beginning of a simulation.
	// For now, initialize everything to 0.0.
	m_Resources.SetPollenQuantity(0);
	m_Resources.SetNectarQuantity(0);
	m_Resources.SetPollenPesticideQuantity(0);
	m_Resources.SetNectarPesticideQuantity(0);
}

// returns value in grams.  EPA data for consumption in mg.
double CColony::GetPollenNeeds(CEvent* pEvent)
{
	double Need = 0;

	if (pEvent->IsWinterDay())
	{
		//Need = GetColonySize()*.002; // 2 mg per day per bee
		// Nurse bees have different consumption rates in winter than other adults so need to calculate separately
		// Nurse bees are the youngest of the age groups so determine how many of the youngest bees are nurse bees and
		// compute need based on that
		int WadlAG[3];  // Worker Adult Age Groups for consumption rates
		WadlAG[0]= Wadl.GetQuantityAt(0, 2);
		WadlAG[1] = Wadl.GetQuantityAt(3, 9);
		WadlAG[2] = Wadl.GetQuantityAt(10, 19);
		double Consumption[3];  // Consumption rates for each of the Adult Age Groups in grams
		Consumption[0] = m_EPAData.m_C_A13_Pollen/1000.0;
		Consumption[1] = m_EPAData.m_C_A410_Pollen/1000.0;
		Consumption[2] = m_EPAData.m_C_A1120_Pollen/1000.0;

		const int NurseBeeQuantity = GetNurseBees();
		int MovedNurseBees = 0;

		for (int i = 0; i < 3; i++)
		{
			if (WadlAG[i] <= NurseBeeQuantity - MovedNurseBees)  // This age group has fewer bees than required to fill nurse bee quantity
			{
				MovedNurseBees += WadlAG[i];
				Need += WadlAG[i] * Consumption[i];
			}
			else// This age group has more bees than required to fill nurse bee quantity
			{
				MovedNurseBees += (NurseBeeQuantity - MovedNurseBees);
				Need += (NurseBeeQuantity - MovedNurseBees) * Consumption[i];
			}
			if (MovedNurseBees >= NurseBeeQuantity) 
			{
				break;  // All nurse bees accounted for.
			}
		}
		const int NonNurseBees = GetColonySize() - MovedNurseBees;
		Need += NonNurseBees*.002;  // 2 mg per day consumption for non nurse bees

		// Add Forager need
		double ForagerNeed = 0;
		if (pEvent->IsForageDay())
		{
			ForagerNeed = foragers.GetActiveQuantity()*m_EPAData.m_C_Forager_Pollen/1000; //grams
			ForagerNeed += (foragers.GetQuantity() - foragers.GetActiveQuantity())*m_EPAData.m_C_A1120_Pollen/1000; // Unemployed consume like adults
		}
		else
		{
			ForagerNeed = foragers.GetQuantity()*.002;
		}
		Need += ForagerNeed;
	}
	else // non-winter day
	{
		// Larvae needs
		const double LNeeds = Wlarv.GetQuantityAt(3)*m_EPAData.m_C_L4_Pollen + Wlarv.GetQuantityAt(4)*m_EPAData.m_C_L5_Pollen + 
			Dlarv.GetQuantity()*m_EPAData.m_C_LD_Pollen;

		// Adult needs
		double ANeeds;
		if (pEvent->IsForageDay())
		{
			ANeeds = Wadl.GetQuantityAt(0,2)*m_EPAData.m_C_A13_Pollen + Wadl.GetQuantityAt(3,9)*m_EPAData.m_C_A410_Pollen +
				Wadl.GetQuantityAt(10,19)*m_EPAData.m_C_A1120_Pollen + Dadl.GetQuantity()*m_EPAData.m_C_AD_Pollen + 
				foragers.GetActiveQuantity()*m_EPAData.m_C_Forager_Pollen + foragers.GetUnemployedQuantity()*m_EPAData.m_C_A1120_Pollen;
				// Unemployed foragers consume like most mature adult house bees
		}
		else // All foragers consume the same quantity as the most mature adult house bees on non-forage days
		{
			ANeeds = Wadl.GetQuantityAt(0,2)*m_EPAData.m_C_A13_Pollen + Wadl.GetQuantityAt(3,9)*m_EPAData.m_C_A410_Pollen +
				(Wadl.GetQuantityAt(10,19) + foragers.GetQuantity())*m_EPAData.m_C_A1120_Pollen + Dadl.GetQuantity()*m_EPAData.m_C_AD_Pollen;
		}

		Need = (LNeeds + ANeeds)/1000.0;  // Convert to grams
	}

	
	if (Need > 1000)
	{
		CString stg = pEvent->GetDateStg("%m/%d/%Y");
		int i = 1;
	}
	return Need;
}

// returns value in grams
double CColony::GetNectarNeeds(CEvent* pEvent)
{
	double Need = 0;

	if (pEvent->IsWinterDay())
	{
		if (GetColonySize() > 0)
		{
			if (pEvent->GetTemp() <= 8.5) // See K. Garber's Winter Failure logic
			{
				Need = 0.3121*GetColonySize()*pow(0.128*GetColonySize(), -0.48); //grams
			}
			else  //  8.5 < AveTemp < 18.0
			{
				if (pEvent->IsForageDay())  // In this case, foragers need normal forager nutrition
				{
					const double NonForagers = GetColonySize() - foragers.GetActiveQuantity();
					Need = (foragers.GetActiveQuantity()*m_EPAData.m_C_Forager_Nectar)/1000 + 0.05419*NonForagers*pow(0.128*NonForagers, -0.27); //grams
				}
				else  // Otherwise, all bees consume at winter rates
				{
					Need = 0.05419*GetColonySize()*pow(0.128*GetColonySize(), -0.27); //grams
				}
			}
		}
	}
	else // Summer Day
	{

		// Larvae needs
		const double LNeeds = Wlarv.GetQuantityAt(3)*m_EPAData.m_C_L4_Nectar + Wlarv.GetQuantityAt(4)*m_EPAData.m_C_L5_Nectar +
			Dlarv.GetQuantity()*m_EPAData.m_C_LD_Nectar;

		// Adult needs
		double ANeeds;
		if (pEvent->IsForageDay())
		{
			ANeeds = Wadl.GetQuantityAt(0,2)*m_EPAData.m_C_A13_Nectar + Wadl.GetQuantityAt(3,9)*m_EPAData.m_C_A410_Nectar +
				Wadl.GetQuantityAt(10,19)*m_EPAData.m_C_A1120_Nectar + foragers.GetUnemployedQuantity()*m_EPAData.m_C_A1120_Nectar + 
				foragers.GetActiveQuantity()*m_EPAData.m_C_Forager_Nectar + Dadl.GetQuantity()*m_EPAData.m_C_AD_Nectar;
		}
		else // Foragers consume the same quantity as the most mature adult house bees
		{
			ANeeds = Wadl.GetQuantityAt(0,2)*m_EPAData.m_C_A13_Nectar + Wadl.GetQuantityAt(3,9)*m_EPAData.m_C_A410_Nectar +
				(Wadl.GetQuantityAt(10,19) + foragers.GetQuantity())*m_EPAData.m_C_A1120_Nectar + Dadl.GetQuantity()*m_EPAData.m_C_AD_Nectar;
		}

		Need = (LNeeds + ANeeds)/1000.0;  //Convert to grams
	}

	return Need;
}

// returns value in grams.  m_I_NectarLoad is in mg/bee
double CColony::GetIncomingNectarQuant(void)
{
	double Nectar = 0;
	Nectar =  foragers.GetActiveQuantity()  * m_EPAData.m_I_NectarTrips * m_EPAData.m_I_NectarLoad/1000.0;

	// If there are no larvae, all pollen foraging trups will become nectar foraging trips
	if ((Wlarv.GetQuantity() + Dlarv.GetQuantity()) <= 0)
	{
		Nectar +=  foragers.GetActiveQuantity()  * m_EPAData.m_I_PollenTrips * m_EPAData.m_I_NectarLoad/1000.0;	
	}
	return Nectar;
}

// returns value in grams  m_I_PollenLoad is in mg/bee
double CColony::GetIncomingPollenQuant(void)
{
	double Pollen;
	// If there are no larvae, no pollen will be brought in to the colony
	if ((Wlarv.GetQuantity() + Dlarv.GetQuantity()) > 0)
	{
		Pollen = foragers.GetActiveQuantity()  * m_EPAData.m_I_PollenTrips * m_EPAData.m_I_PollenLoad/1000.0;
	}
	else Pollen = 0;
	return Pollen;
}

/////////////////////////////////////////////////////////////////////////////////////////
// The Incoming Pesticide Concentration functions are responsible to determine
// if we are in a window where pesticide present as well as calculating any decay of 
// effectiveness (effectiveness decay is represented by a reduced "effective" concentration
//
// Returns Gram AI/Gram
double CColony::GetIncomingNectarPesticideConcentration(int DayNum)
{
	double IncomingConcentration = 0;
	COleDateTime* pDate = GetDayNumDate(DayNum);
	const COleDateTime CurDate(pDate->GetYear(), pDate->GetMonth(), pDate->GetDay(), 0,0,0);
	delete pDate;

	// Check to see if we're using only pesticide concentration table
	// If so, query this date and set Nectar pesticide concentration as indicated in the file
	//
	//
	// if not, continue
	if (m_NutrientCT.IsEnabled())
	{
		double NectConc, PolConc;
		m_NutrientCT.GetContaminantConc(CurDate,NectConc, PolConc);
		if (NectConc > 0.0)
		{
			int i = 0;
		}
		IncomingConcentration = NectConc;
	}
	else  // Normal process
	{
		if ((m_EPAData.m_FoliarEnabled) && (CurDate >= m_EPAData.m_FoliarAppDate) && (CurDate >= m_EPAData.m_FoliarForageBegin) && (CurDate < m_EPAData.m_FoliarForageEnd))
		{
			IncomingConcentration = 110.0 * m_EPAData.m_E_AppRate/1000000.0;  // Incoming AI Grams/Grams Nectar due to foliar spray
			// Dose reduced due to active ingredient half-life
			const LONG DaysSinceApplication = (CurDate - m_EPAData.m_FoliarAppDate).GetDays();
			if (m_EPAData.m_AI_HalfLife > 0)
			{
				const double k = log(2.0)/m_EPAData.m_AI_HalfLife;
				IncomingConcentration *= exp(-k*DaysSinceApplication);
			}
			//TRACE("Incoming Foliar Spray Nectar Pesticide on %s\n",CurDate.Format("%m/%d/%Y"));
			AddEventNotification(CurDate.Format("%m/%d/%Y"), "Incoming Foliar Spray Nectar Pesticide");
		}
		if ((CurDate >= m_EPAData.m_SeedForageBegin) && (CurDate < m_EPAData.m_SeedForageEnd) && (m_EPAData.m_SeedEnabled))
		{
			IncomingConcentration += m_EPAData.m_E_SeedConcentration/1000000.0; // Add Incoming Grams AI/Grams Bectar due to seed concentration
			//TRACE("Incoming Seed Nectar Pesticide on %s\n",CurDate.Format("%m/%d/%Y"));
			AddEventNotification(CurDate.Format("%m/%d/%Y"), "Incoming Seed Nectar Pesticide");
		}
		if ((CurDate >= m_EPAData.m_SoilForageBegin) && (CurDate < m_EPAData.m_SoilForageEnd) && (m_EPAData.m_SoilEnabled)) // In soil forage range
		{
			if ((m_EPAData.m_AI_KOW > 0) || (m_EPAData.m_E_SoilTheta != 0)) // Ensure no NAN exceptions
			{
				const double LogKOW = log10(m_EPAData.m_AI_KOW);
				const double TSCF = -0.0648*(LogKOW*LogKOW) + 0.241*LogKOW + 0.5822;
				const double SoilConc = TSCF * (pow(10,(0.95*LogKOW -2.05)) + 0.82) *
					m_EPAData.m_E_SoilConcentration*(m_EPAData.m_E_SoilP/(m_EPAData.m_E_SoilTheta + m_EPAData.m_E_SoilP*m_EPAData.m_AI_KOC*m_EPAData.m_E_SoilFoc));
//					m_EPAData.m_E_SoilConcentration*((m_EPAData.m_E_SoilP/m_EPAData.m_E_SoilTheta) + m_EPAData.m_E_SoilP*m_EPAData.m_AI_KOC*m_EPAData.m_E_SoilFoc);
				IncomingConcentration += SoilConc/1000000; // Add Grams AI/Gram Nectar for soil concentration
				//TRACE("Incoming Soil Nectar Pesticide on %s\n",CurDate.Format("%m/%d/%Y"));
			AddEventNotification(CurDate.Format("%m/%d/%Y"), "Incoming Soil Nectar Pesticide");
			}
	}
	}
	return IncomingConcentration;  // Grams AI/Gram
}

// Returns Grams AI/Gram
double CColony::GetIncomingPollenPesticideConcentration(int DayNum)
{
	// NOTE: This code is identical to GetIncomingNectarPesticideConcentration

	double IncomingConcentration = 0;
	COleDateTime* pDate = GetDayNumDate(DayNum);
	const COleDateTime CurDate(pDate->GetYear(), pDate->GetMonth(), pDate->GetDay(), 0,0,0);
	delete pDate;
	// Check to see if we're using only pesticide concentration table
	// If so, query this date and set Nectar pesticide concentration as indicated in the file
	//
	//
	// if not, continue
	if (m_NutrientCT.IsEnabled())
	{
		double NectConc, PolConc;
		m_NutrientCT.GetContaminantConc(CurDate,NectConc, PolConc);
		IncomingConcentration = PolConc;
	}
	else  // Normal process
	{
		if ((m_EPAData.m_FoliarEnabled) && (CurDate >= m_EPAData.m_FoliarAppDate) && (CurDate >= m_EPAData.m_FoliarForageBegin) && (CurDate < m_EPAData.m_FoliarForageEnd))
		{
			IncomingConcentration = 110.0 * m_EPAData.m_E_AppRate/1000000;  // Incoming AI Grams/Grams Pollen due to foliar spray (uG/G = 110  * lbs/AC)
			// Dose reduced due to active ingredient half-life
			const LONG DaysSinceApplication = (CurDate - m_EPAData.m_FoliarAppDate).GetDays();
			if (m_EPAData.m_AI_HalfLife > 0)
			{
				const double k = log(2.0)/m_EPAData.m_AI_HalfLife;
				IncomingConcentration *= exp(-k*DaysSinceApplication);
			}
			//TRACE("Incoming Foliar Spray Pollen Pesticide on %s\n",CurDate.Format("%m/%d/%Y"));
			AddEventNotification(CurDate.Format("%m/%d/%Y"), "Incoming Foliar Spray Pollen Pesticide");
		}
		if ((CurDate >= m_EPAData.m_SeedForageBegin) && (CurDate < m_EPAData.m_SeedForageEnd) && (m_EPAData.m_SeedEnabled))
		{
			IncomingConcentration += m_EPAData.m_E_SeedConcentration/1000000.0; // Add Grams AI/Grams Pollen
			//TRACE("Incoming Seed Pollen Pesticide on %s\n",CurDate.Format("%m/%d/%Y"));
			AddEventNotification(CurDate.Format("%m/%d/%Y"), "Incoming Seed Pollen Pesticide");

		}
		if ((CurDate >= m_EPAData.m_SoilForageBegin) && (CurDate < m_EPAData.m_SoilForageEnd) && (m_EPAData.m_SoilEnabled)) // In soil forage range
		{
			if ((m_EPAData.m_AI_KOW > 0) || (m_EPAData.m_E_SoilTheta != 0)) // Ensure no NAN exceptions
			{
				const double LogKOW = log10(m_EPAData.m_AI_KOW);
				const double TSCF = -0.0648*(LogKOW*LogKOW) + 0.241*LogKOW + 0.5822;
				const double SoilConc = TSCF * (pow(10,(0.95*LogKOW -2.05)) + 0.82) *
					m_EPAData.m_E_SoilConcentration*(m_EPAData.m_E_SoilP/(m_EPAData.m_E_SoilTheta + m_EPAData.m_E_SoilP*m_EPAData.m_AI_KOC*m_EPAData.m_E_SoilFoc));
//					m_EPAData.m_E_SoilConcentration*((m_EPAData.m_E_SoilP/m_EPAData.m_E_SoilTheta) + m_EPAData.m_E_SoilP*m_EPAData.m_AI_KOC*m_EPAData.m_E_SoilFoc);
				IncomingConcentration += SoilConc/1000000;  // Add Grams AI/Grams Pollen
				//TRACE("Incoming Soil Pollen Pesticide on %s\n",CurDate.Format("%m/%d/%Y"));
				AddEventNotification(CurDate.Format("%m/%d/%Y"), "Incoming Soil Pollen Pesticide");
			}
		}
	}
	return IncomingConcentration; // Grams AI/Gram
}




