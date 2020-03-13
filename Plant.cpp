// Plant.cpp: implementation of the CPlant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WhiteFly.h"
#include "WhiteFlyDoc.h"
#include "Plant.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CPlant, CObject)

CPlant::CPlant()
{
	m_Initialized = false;
	m_NumLeaves = 1;
	m_pDoc = NULL;
}

CPlant::~CPlant()
{

}

/*

  NOTE:
  All whitefly quantities are kept as total numbers for this plant.  They
  are converted to a per leaf value by division by the current number of leaves on
  the plant.  

 */


void CPlant::InitializePlant()
{
	// Sets the maximum age of each fly list and populates the 
	// lists with the initial conditions from the Document
	m_NumLeaves = m_pDoc->m_LeavesPerPlant;
	if (m_NumLeaves < 1.0) m_NumLeaves = 1.0; // Protect against /0
	m_LeavesPerWeek = m_pDoc->m_LeavesPerWeek;

	m_Eggs_F.SetMaxAge(126);
	m_Eggs_M.SetMaxAge(126);
	m_Immatures_F.SetMaxAge(201);
	m_Immatures_M.SetMaxAge(201);
	m_Adults_F.SetMaxAge(350);
	m_Adults_M.SetMaxAge(350);

	// Initially create 20 boxcars in each list
	#define INITBOXCARS 20

	m_Eggs_F.Populate(126/INITBOXCARS,
		m_pDoc->m_EggsPerLeaf*m_NumLeaves*m_pDoc->m_WFSexRatio);
	m_Eggs_M.Populate(126/INITBOXCARS,
		m_pDoc->m_EggsPerLeaf*m_NumLeaves*(1-m_pDoc->m_WFSexRatio));
	m_Immatures_F.Populate(201/INITBOXCARS,
		m_pDoc->m_ImmsPerLeaf*m_NumLeaves*m_pDoc->m_WFSexRatio);
	m_Immatures_M.Populate(201/INITBOXCARS,
		m_pDoc->m_ImmsPerLeaf*m_NumLeaves*(1-m_pDoc->m_WFSexRatio));
	m_Adults_F.Populate(350/INITBOXCARS,
		m_pDoc->m_AdultsPerLeaf*m_NumLeaves*m_pDoc->m_WFSexRatio);
	m_Adults_M.Populate(350/INITBOXCARS,
		m_pDoc->m_AdultsPerLeaf*m_NumLeaves*(1-m_pDoc->m_WFSexRatio));

	// Initialize Parasites
	m_Parasites.Populate(1,100);

	m_Initialized = true;
}

void CPlant::UpdatePlant(CEvent* pEvent, int DayNum)
{
	// Leaf Growth Function
//	m_NumLeaves += m_LeavesPerWeek/7.0;  // Simple minded function

}


double CPlant::GetTotFlyQuan()
{
	return(
		GetFEggQuan()+
		GetMEggQuan()+
		GetFImmQuan()+
		GetMImmQuan()+
		GetFAdultQuan()+
		GetMAdultQuan()
		);
}

void CPlant::UpdateFlys(CEvent* pEvent, int DayNum)
{
	double AveTemp = (pEvent->m_MaxTemp + pEvent->m_MinTemp) / 2.0;
	CInsect* theFly;

	//  Increment the age for all lists
 	m_Eggs_F.IncrementAge(AveTemp - 12.0);
	m_Eggs_M.IncrementAge(AveTemp - 12.0);
	m_Immatures_F.IncrementAge(AveTemp - 14.0);
	m_Immatures_M.IncrementAge(AveTemp - 14.0);
	m_Adults_F.IncrementAge(AveTemp - 8.0);
	m_Adults_M.IncrementAge(AveTemp - 8.0);

	// Calculate normal mortality from egg -> adult
	double EA_mortality = sin(2.567-.079*AveTemp);
	EA_mortality *= EA_mortality;

	// Calculate normal mortality from egg -> immature
	double EI_mortality = sin(0.808-0.026*AveTemp);
	EI_mortality *= EI_mortality;

	// Calculate normal mortality from immature -> adult
	double IA_mortality = EA_mortality - EI_mortality;

	//  Now advance any maturing flys to the next life stage
	while ((theFly = m_Eggs_F.RemoveMatures()) != NULL) 
	{
		theFly->SetAge(0.0);
		theFly->SetQuantity(theFly->GetQuantity()*(1-EI_mortality));
		m_Immatures_F.AddHead(theFly);
	}
	while ((theFly = m_Immatures_F.RemoveMatures()) != NULL) 
	{
		theFly->SetAge(0.0);
		theFly->SetQuantity(theFly->GetQuantity()*(1-IA_mortality));
		m_Adults_F.AddHead(theFly);
	}
	while ((theFly = m_Adults_F.RemoveMatures()) != NULL) 
	{
		delete theFly;
	}

	while ((theFly = m_Eggs_M.RemoveMatures()) != NULL)  
	{
		theFly->SetAge(0.0);
		theFly->SetQuantity(theFly->GetQuantity()*(1-EI_mortality));
		m_Immatures_M.AddHead(theFly);
	}
	while ((theFly = m_Immatures_M.RemoveMatures()) != NULL)  
	{
		theFly->SetAge(0.0);
		theFly->SetQuantity(theFly->GetQuantity()*(1-IA_mortality));
		m_Adults_M.AddHead(theFly);
	}
	while ((theFly = m_Adults_M.RemoveMatures()) != NULL) 
	{
		delete theFly;
	}




	// Now lay eggs for this day
	double TotalEggs = 0.0;
	double IncEggs = 0.0;
	double AgeDD;
	POSITION pos = m_Adults_F.GetHeadPosition();
	while (pos != NULL)
	{
		theFly = (CInsect*)m_Adults_F.GetNext(pos);
		AgeDD = theFly->GetAge();
		if (AgeDD > 40.0)		// Post-Ovo position
		{
			IncEggs = (-0.119 + 0.01*AveTemp)*AgeDD*exp(-0.008*AgeDD);
			IncEggs *= theFly->GetQuantity();
			if (IncEggs < 0) IncEggs = 0;
			TotalEggs += IncEggs;
		}
	}


	// And put them in the proper lists
	double FemaleProp = (sin(0.498 + .0196*AveTemp));
	FemaleProp *= FemaleProp;

	CInsect* NewEggF = new CInsect;
	NewEggF->SetQuantity(TotalEggs*FemaleProp);
	NewEggF->SetAge(0.0);
	m_Eggs_F.AddHead(NewEggF);

	CInsect* NewEggM = new CInsect;
	NewEggM->SetQuantity(TotalEggs*(1-FemaleProp));
	NewEggM->SetAge(0);
	m_Eggs_M.AddHead(NewEggM);

	double test = GetParasiteQuan();
	// Kill Nymphs based on Parasite density
	if (m_Immatures_M.GetQuantity()>0)
	{
		m_Immatures_M.ChangeQuantity(-0.281*m_Immatures_M.GetQuantity()*GetParasiteQuan());
		if (m_Immatures_M.GetQuantity()<=0) m_Immatures_M.Clear();
	}

	if (m_Immatures_F.GetQuantity()>0)
	{
		m_Immatures_F.ChangeQuantity(-0.281*m_Immatures_F.GetQuantity()*GetParasiteQuan());
		if (m_Immatures_F.GetQuantity()<=0) m_Immatures_F.Clear();
	}

}
