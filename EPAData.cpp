#include "stdafx.h"
#include "EPAData.h"
#include "math.h"


///////////////////////////////////////////////////////////////////////////
// Class AIItem
///////////////////////////////////////////////////////////////////////////
void AIItem::Serialize(CArchive& ar, int FileFormatVersion)
{
	if (ar.IsStoring())
	{
		ar<<m_AI_Name;
		ar<<m_AI_AdultSlope;
		ar<<m_AI_AdultLD50;
		if (FileFormatVersion >= 10)
		{
			ar<<m_AI_AdultSlope_Contact;
			ar<<m_AI_AdultLD50_Contact;
		}
		ar<<m_AI_LarvaSlope;
		ar<<m_AI_LarvaLD50;
		ar<<m_AI_KOW;
		ar<<m_AI_KOC;
		ar<<m_AI_HalfLife;
		ar<<m_AI_ContactFactor;
	}
	else
	{
		ar>>m_AI_Name;
		ar>>m_AI_AdultSlope;
		ar>>m_AI_AdultLD50;
		if (FileFormatVersion >= 10)
		{
			ar>>m_AI_AdultSlope_Contact;
			ar>>m_AI_AdultLD50_Contact;
		}
		ar>>m_AI_LarvaSlope;
		ar>>m_AI_LarvaLD50;
		ar>>m_AI_KOW;
		ar>>m_AI_KOC;
		ar>>m_AI_HalfLife;
		ar>>m_AI_ContactFactor;
	}
}

AIItem& AIItem::operator =(const AIItem &rhs)
{
	m_AI_Name = rhs.m_AI_Name;
	m_AI_AdultSlope = rhs.m_AI_AdultSlope;
	m_AI_AdultLD50 = rhs.m_AI_AdultLD50;
	m_AI_AdultSlope_Contact = rhs.m_AI_AdultSlope_Contact;
	m_AI_AdultLD50_Contact = rhs.m_AI_AdultLD50_Contact;
	m_AI_LarvaSlope = rhs.m_AI_LarvaSlope;
	m_AI_LarvaLD50 = rhs.m_AI_LarvaLD50;
	m_AI_KOW = rhs.m_AI_KOW;
	m_AI_KOC = rhs.m_AI_KOC;
	m_AI_HalfLife = rhs.m_AI_HalfLife;
	m_AI_ContactFactor = rhs.m_AI_ContactFactor;

	return *this;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class EPAData
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEPAData::CEPAData(void)
{

	// Active Ingredients
	m_AI_Name = "";
	m_AI_AdultSlope = 0.0;
	m_AI_AdultLD50 = 0.0;
	m_AI_AdultSlope_Contact = 0.0;
	m_AI_AdultLD50_Contact = 0.0;
	m_AI_LarvaSlope = 0.0;
	m_AI_LarvaLD50 = 0.0;
	m_AI_KOW = 0.0;
	m_AI_KOC = 0.0;
	m_AI_HalfLife = 0.0;
	m_AI_ContactFactor = 0.0;

	// Consumption
	m_C_L4_Pollen = 0.0;
	m_C_L4_Nectar = 0.0;
	m_C_L5_Pollen = 0.0;
	m_C_L5_Nectar = 0.0;
	m_C_LD_Pollen = 0.0;
	m_C_LD_Nectar = 0.0;
	m_C_A13_Pollen = 0.0;
	m_C_A13_Nectar = 0.0;
	m_C_A410_Pollen = 0.0;
	m_C_A410_Nectar = 0.0;
	m_C_A1120_Pollen = 0.0;
	m_C_A1120_Nectar = 0.0;
	m_C_AD_Pollen = 0.0;
	m_C_AD_Nectar = 0.0;
	m_C_Forager_Pollen = 0.0;
	m_C_Forager_Nectar = 0.0;

	// Exposure
	m_FoliarEnabled = FALSE;
	m_SoilEnabled = FALSE;
	m_SeedEnabled = FALSE;
	m_E_AppRate = 0.0;
	m_E_SoilTheta = 0.0;
	m_E_SoilP = 0.0;
	m_E_SoilFoc = 0.0;
	m_E_SeedConcentration = 0.0;
	m_E_SoilConcentration = 0.0;

	m_FoliarAppDate = COleDateTime::GetCurrentTime();
	m_FoliarForageBegin = COleDateTime::GetCurrentTime(); 
	m_FoliarForageEnd = COleDateTime::GetCurrentTime();
	m_SoilForageBegin = COleDateTime::GetCurrentTime(); 
	m_SoilForageEnd = COleDateTime::GetCurrentTime();
	m_SeedForageBegin = COleDateTime::GetCurrentTime(); 
	m_SeedForageEnd = COleDateTime::GetCurrentTime();

	m_D_L4 = 0;
	m_D_L5 = 0;
	m_D_LD = 0;
	m_D_A13 = 0;
	m_D_A410 = 0;
	m_D_A1120 = 0;
	m_D_AD = 0;
	m_D_C_Foragers = 0;
	m_D_D_Foragers = 0;

	m_D_L4_Max = 0;
	m_D_L5_Max = 0;
	m_D_LD_Max = 0;
	m_D_A13_Max = 0;
	m_D_A410_Max = 0;
	m_D_A1120_Max = 0;
	m_D_AD_Max = 0;
	m_D_C_Foragers_Max = 0;
	m_D_D_Foragers_Max = 0;

	// Incoming
	m_I_PollenTrips = 0;
	m_I_NectarTrips = 0;
	m_I_PercentNectarForagers = 0;
	m_I_PollenLoad = 0;
	m_I_NectarLoad = 0;



}

CEPAData::~CEPAData(void)
{
	// Memory Cleanup for AIItemList
	while (!m_AIItemList.IsEmpty()) delete m_AIItemList.RemoveTail(); 
}

//Assignment operator
CEPAData& CEPAData::operator =(const CEPAData &rhs)
{
	
	// Active Ingredient - copy list
	m_AIItemList.RemoveAll();
	POSITION pos = rhs.m_AIItemList.GetHeadPosition();
	while (pos != NULL)
	{
		AIItem* pItem = new AIItem;
		*pItem = *((AIItem*)rhs.m_AIItemList.GetNext(pos));
		m_AIItemList.AddTail(pItem);
	}

	// Current Active Ingredient
	m_AI_Name = rhs.m_AI_Name;
	m_AI_AdultSlope = rhs.m_AI_AdultSlope;
	m_AI_AdultLD50 = rhs.m_AI_AdultLD50;
	m_AI_AdultSlope_Contact = rhs.m_AI_AdultSlope_Contact;
	m_AI_AdultLD50_Contact = rhs.m_AI_AdultLD50_Contact;
	m_AI_LarvaSlope = rhs.m_AI_LarvaSlope;
	m_AI_LarvaLD50 = rhs.m_AI_LarvaLD50;
	m_AI_KOW = rhs.m_AI_KOW;
	m_AI_KOC = rhs.m_AI_KOC;
	m_AI_HalfLife = rhs.m_AI_HalfLife;
	m_AI_ContactFactor = rhs.m_AI_ContactFactor;

	// Consumption
	m_C_L4_Pollen = rhs.m_C_L4_Pollen;
	m_C_L4_Nectar = rhs.m_C_L4_Nectar;
	m_C_L5_Pollen = rhs.m_C_L5_Pollen;
	m_C_L5_Nectar = rhs.m_C_L5_Nectar;
	m_C_LD_Pollen = rhs.m_C_LD_Pollen;
	m_C_LD_Nectar = rhs.m_C_LD_Nectar;
	m_C_A13_Pollen = rhs.m_C_A13_Pollen;
	m_C_A13_Nectar = rhs.m_C_A13_Nectar;
	m_C_A410_Pollen = rhs.m_C_A410_Pollen;
	m_C_A410_Nectar = rhs.m_C_A410_Nectar;
	m_C_A1120_Pollen = rhs.m_C_A1120_Pollen;
	m_C_A1120_Nectar = rhs.m_C_A1120_Nectar;
	m_C_AD_Pollen = rhs.m_C_AD_Pollen;
	m_C_AD_Nectar = rhs.m_C_AD_Nectar;
	m_C_Forager_Pollen = rhs.m_C_Forager_Pollen;
	m_C_Forager_Nectar = rhs.m_C_Forager_Nectar;

	// Exposure
	m_FoliarEnabled = rhs.m_FoliarEnabled;
	m_SoilEnabled = rhs.m_SoilEnabled;
	m_SeedEnabled = rhs.m_SeedEnabled;
	m_E_AppRate = rhs.m_E_AppRate;
	m_E_SoilTheta = rhs.m_E_SoilTheta;
	m_E_SoilP = rhs.m_E_SoilP;
	m_E_SoilFoc = rhs.m_E_SoilFoc;
	m_E_SeedConcentration = rhs.m_E_SeedConcentration;
	m_E_SoilConcentration = rhs.m_E_SoilConcentration;

	m_FoliarAppDate = rhs.m_FoliarAppDate;
	m_FoliarForageBegin = rhs.m_FoliarForageBegin;
	m_FoliarForageEnd = rhs.m_FoliarForageEnd;
	m_SoilForageBegin = rhs.m_SoilForageBegin;
	m_SoilForageEnd = rhs.m_SoilForageEnd;
	m_SeedForageBegin = rhs.m_SeedForageBegin; 
	m_SeedForageEnd = rhs.m_SeedForageEnd;

	// Incoming
	m_I_PollenTrips = rhs.m_I_PollenTrips;
	m_I_NectarTrips = rhs.m_I_NectarTrips;
	m_I_PercentNectarForagers = rhs.m_I_PercentNectarForagers;
	m_I_PollenLoad = rhs.m_I_PollenLoad;
	m_I_NectarLoad = rhs.m_I_NectarLoad;


	return *this;
}


void CEPAData::Serialize(CArchive& ar, int FileFormatVersion) 
{
	if (ar.IsStoring())
	{
		// Serialize the AI list
		int AICount = GetAIItemCount();
		AIItem* pItem;
		ar<<AICount;
		if (AICount > 0)
		{
			POSITION pos = m_AIItemList.GetHeadPosition();
			while (pos!=NULL)
			{
				pItem = (AIItem*)m_AIItemList.GetNext(pos);
				pItem->Serialize(ar, FileFormatVersion);
			}
		}

		// Active Ingredients
		ar<<m_AI_Name;
		ar<<m_AI_AdultSlope;
		ar<<m_AI_AdultLD50;
		if (FileFormatVersion >= 10)
		{
			ar<<m_AI_AdultSlope_Contact;
			ar<<m_AI_AdultLD50_Contact;
		}
		ar<<m_AI_LarvaSlope;
		ar<<m_AI_LarvaLD50;
		ar<<m_AI_KOW;
		ar<<m_AI_KOC;
		ar<<m_AI_HalfLife;
		ar<<m_AI_ContactFactor;

		// Consumption
		ar<<m_C_L4_Pollen;
		ar<<m_C_L4_Nectar;
		ar<<m_C_L5_Pollen;
		ar<<m_C_L5_Nectar;
		ar<<m_C_LD_Pollen;
		ar<<m_C_LD_Nectar;
		ar<<m_C_A13_Pollen;
		ar<<m_C_A13_Nectar;
		ar<<m_C_A410_Pollen;
		ar<<m_C_A410_Nectar;
		ar<<m_C_A1120_Pollen;
		ar<<m_C_A1120_Nectar;
		ar<<m_C_AD_Pollen;
		ar<<m_C_AD_Nectar;
		ar<<m_C_Forager_Pollen;
		ar<<m_C_Forager_Nectar;

		// Exposure
		ar<<(m_FoliarEnabled?1:0);
		ar<<(m_SoilEnabled?1:0);
		ar<<(m_SeedEnabled?1:0);
		ar<<m_E_AppRate;
		ar<<m_E_SoilTheta;
		ar<<m_E_SoilP;
		ar<<m_E_SoilFoc;
		ar<<m_E_SeedConcentration;
		ar<<m_E_SoilConcentration;

		ar<<m_FoliarAppDate;
		ar<<m_FoliarForageBegin; 
		ar<<m_FoliarForageEnd;
		ar<<m_SoilForageBegin; 
		ar<<m_SoilForageEnd;
		ar<<m_SeedForageBegin; 
		ar<<m_SeedForageEnd;

		// Incoming 
		ar<<m_I_PollenTrips;
		ar<<m_I_NectarTrips;
		ar<<m_I_PercentNectarForagers;
		ar<<m_I_PollenLoad;
		ar<<m_I_NectarLoad;


	}
	else
	{	// loading code

		// Serialize the AI list
		int AICount;
		ar>>AICount;
		m_AIItemList.RemoveAll();
		for (int i = 0; i < AICount; i++)
		{
			AIItem* pItem = new AIItem;
			pItem->Serialize(ar, FileFormatVersion);
			m_AIItemList.AddTail(pItem);
		}

		// Active Ingredients
		ar>>m_AI_Name;
		ar>>m_AI_AdultSlope;
		ar>>m_AI_AdultLD50;
		if (FileFormatVersion >= 10)
		{
			ar>>m_AI_AdultSlope_Contact;
			ar>>m_AI_AdultLD50_Contact;
		}
		ar>>m_AI_LarvaSlope;
		ar>>m_AI_LarvaLD50;
		ar>>m_AI_KOW;
		ar>>m_AI_KOC;
		ar>>m_AI_HalfLife;
		ar>>m_AI_ContactFactor;

		// Consumption
		ar>>m_C_L4_Pollen;
		ar>>m_C_L4_Nectar;
		ar>>m_C_L5_Pollen;
		ar>>m_C_L5_Nectar;
		ar>>m_C_LD_Pollen;
		ar>>m_C_LD_Nectar;
		ar>>m_C_A13_Pollen;
		ar>>m_C_A13_Nectar;
		ar>>m_C_A410_Pollen;
		ar>>m_C_A410_Nectar;
		ar>>m_C_A1120_Pollen;
		ar>>m_C_A1120_Nectar;
		ar>>m_C_AD_Pollen;
		ar>>m_C_AD_Nectar;
		ar>>m_C_Forager_Pollen;
		ar>>m_C_Forager_Nectar;

		// Exposure
		int EnableCheck;
		ar>>EnableCheck;
		m_FoliarEnabled = (EnableCheck == 1);
		ar>>EnableCheck;
		m_SoilEnabled = (EnableCheck == 1);
		ar>>EnableCheck;
		m_SeedEnabled = (EnableCheck == 1);
		ar>>m_E_AppRate;
		ar>>m_E_SoilTheta;
		ar>>m_E_SoilP;
		ar>>m_E_SoilFoc;
		ar>>m_E_SeedConcentration;
		ar>>m_E_SoilConcentration;

		ar>>m_FoliarAppDate;
		ar>>m_FoliarForageBegin; 
		ar>>m_FoliarForageEnd;
		ar>>m_SoilForageBegin; 
		ar>>m_SoilForageEnd;
		ar>>m_SeedForageBegin; 
		ar>>m_SeedForageEnd;

		// Incoming 
		ar>>m_I_PollenTrips;
		ar>>m_I_NectarTrips;
		ar>>m_I_PercentNectarForagers;
		ar>>m_I_PollenLoad;
		ar>>m_I_NectarLoad;


	}
}

void CEPAData::AddAIItem(AIItem* pItem)
{
	m_AIItemList.AddTail(pItem);
}

bool CEPAData::RemoveAIItem(CString ItemName)
{
	// Returns true if item was found and deleted
	bool Success = false;
	AIItem* pTestItem;
	POSITION pos = m_AIItemList.GetHeadPosition();
	POSITION oldpos;
	while (pos != NULL)
	{
		oldpos = pos; //Save prior POSITION for later use
		pTestItem = (AIItem*)m_AIItemList.GetNext(pos);
		if (ItemName == pTestItem->m_AI_Name) 
		{
			m_AIItemList.RemoveAt(oldpos);
			delete pTestItem;
			Success = true;
		}
	}
	return Success;
}

BOOL CEPAData::GetAIItem(CString theName, AIItem* pItem)
{
	ASSERT(pItem);  //pItem is expected to point to an allocated AIItem
	BOOL Result = false;
	AIItem* pTestItem;
	POSITION pos = m_AIItemList.GetHeadPosition();
	while (pos != NULL)
	{
		pTestItem = (AIItem*)m_AIItemList.GetNext(pos);
		if (theName.MakeLower() == pTestItem->m_AI_Name.MakeLower()) // Not case sensitive
		{
			pItem->m_AI_Name = pTestItem->m_AI_Name;
			pItem->m_AI_AdultSlope = pTestItem->m_AI_AdultSlope;
			pItem->m_AI_AdultLD50 = pTestItem->m_AI_AdultLD50;
			pItem->m_AI_AdultSlope_Contact = pTestItem->m_AI_AdultSlope_Contact;
			pItem->m_AI_AdultLD50_Contact = pTestItem->m_AI_AdultLD50_Contact;
			pItem->m_AI_LarvaSlope = pTestItem->m_AI_LarvaSlope;
			pItem->m_AI_LarvaLD50 = pTestItem->m_AI_LarvaLD50;
			pItem->m_AI_KOW = pTestItem->m_AI_KOW;
			pItem->m_AI_KOC = pTestItem->m_AI_KOC;
			pItem->m_AI_HalfLife = pTestItem->m_AI_HalfLife;
			pItem->m_AI_ContactFactor = pTestItem->m_AI_ContactFactor;
			Result = true;
			break;
		}
	}
	return Result;
}

void CEPAData::SetCurrentAIItem(AIItem* pItem)
{
	ASSERT(pItem);
	m_AI_Name = pItem->m_AI_Name;
	m_AI_AdultSlope = pItem->m_AI_AdultSlope;
	m_AI_AdultLD50 = pItem->m_AI_AdultLD50;
	m_AI_AdultSlope_Contact = pItem->m_AI_AdultSlope_Contact;
	m_AI_AdultLD50_Contact = pItem->m_AI_AdultLD50_Contact;
	m_AI_LarvaSlope = pItem->m_AI_LarvaSlope;
	m_AI_LarvaLD50 = pItem->m_AI_LarvaLD50;
	m_AI_KOW = pItem->m_AI_KOW;
	m_AI_KOC = pItem->m_AI_KOC;
	m_AI_HalfLife = pItem->m_AI_HalfLife;
	m_AI_ContactFactor = pItem->m_AI_ContactFactor;

}

AIItem* CEPAData::GetAIItemPtr(CString Name)
{
	AIItem* pItem = NULL;
	AIItem* pTestItem;
	BOOL Result = false;
	POSITION pos = m_AIItemList.GetHeadPosition();
	while (pos != NULL)
	{
		pTestItem = (AIItem*)m_AIItemList.GetNext(pos);
		if (Name.MakeLower() == pTestItem->m_AI_Name.MakeLower()) 
		{
			pItem = pTestItem;
			break;
		}
	}
	return pItem;
}

////////////////////////////////////////////////////////////////
// DoseResponse
//
// Implements the Dose Response transfer function.  For a given dosage, calculates the proporation of 
// the population that is killed.
//
// As of 8/10/14, there are several approaches in the literature.  This function will eventually settle on one.
//
double CEPAData::DoseResponse(double Dose, double LD50, double Slope)  // Incoming Dose is in Grams
{
	double PropKilled;

	Dose *= 1000000.0;  // Convert grams to micrograms.  Assume LD50 is in micrograms/bee.
	// Validate input
	bool Valid = (Dose > LD50*0.05) && (LD50 > 0) && (Slope >= 0) && (Slope < 20);

	if (!Valid) PropKilled = 0.0; // If  Dose < 5% of LD50, approximate with 0, If Slope or LD50 not in right range, set to 0
	else
	{
		PropKilled = 1.0 / (1.0 + pow(Dose/LD50, -Slope));
	}
	return PropKilled;
}