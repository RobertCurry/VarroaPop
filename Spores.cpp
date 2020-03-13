// Spores.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "Spores.h"


// CSpores

CSpores::CSpores()
{

	// The Spore Vitality Data is from William Meikle's email of May 23, 2005.
	// The Vitality is the percentage of live spores left after a number of days
	// The DayPoints are the days after application that the vitality was measured.
	// Most likely will change to a polynomial function

	for (int i = 0; i<10; i++)
	{
		m_SporeVitalityMatrix[i].IntVal = 0;
		m_MortalityFunctionPts[i].IntVal = 0;
	}

	m_SporeVitalityMatrix[0].DoubVal = 1.0;  // Sample Values
	m_SporeVitalityMatrix[1].DoubVal = 0.16;
	m_SporeVitalityMatrix[2].DoubVal = 0.26;
	m_SporeVitalityMatrix[3].DoubVal = 0.13; 
	m_SporeVitalityMatrix[4].DoubVal = 0.01;
	m_SporeVitalityMatrix[5].DoubVal = 0.003;
	m_SporeVitalityMatrix[0].IntVal = 1;  // Sample Days
	m_SporeVitalityMatrix[1].IntVal = 3;
	m_SporeVitalityMatrix[2].IntVal = 6;
	m_SporeVitalityMatrix[3].IntVal = 13;
	m_SporeVitalityMatrix[4].IntVal = 20;
	m_SporeVitalityMatrix[5].IntVal = 42;
	m_SporeVitalityPtCount = 6;

	m_MortFunctPtCount = 0;

}

CSpores::~CSpores()
{
}


// CSpores member functions
void CSpores::SetMortalityFunction(double PercentMortality, int SporeDensity,int Index)
{
	// This sets the Varroa mortality rate as a function of spore density.
	// For now, this will be set from the CAction dialog and will simply consist
	// of pairs of proportion of mortality and spore density (spores per mite).
	// This may ultimately become a polynomial function once we have data
	m_MortalityFunctionPts[Index].DoubVal = PercentMortality;
	m_MortalityFunctionPts[Index].IntVal = SporeDensity;
	m_MortFunctPtCount++;

}

double CSpores::GetDayMortalityRate(int TreatmentDayNumber)
{
	// The mortality rate for a specific day after treatment begins is a function
	// of the Mortality Rate vs Spore Density and the Degradation of spores over time.
	double MortRate = 1.0;

	int CurrentSpores = (int)(m_InitialSporeCount * 
						Interpolation(m_SporeVitalityMatrix, 
									  m_SporeVitalityPtCount, 
									  TreatmentDayNumber));

	MortRate = Interpolation(m_MortalityFunctionPts, m_MortFunctPtCount, CurrentSpores);


	return MortRate;
}

double CSpores::Interpolation(IntDoub* IDArray,int ArraySize, int IntVal)
{
	// Performs a linear interpolation of a IntDoub array using the IntVal as the
	// Independant variable.  If the interpolation is not found, 0 is returned.
	bool foundit = false;
	double Value = 0;
	int index = 0;

	// Independant Var > max in array, return max IntVal
	if (IntVal >= IDArray[ArraySize-1].IntVal) 
	{
		Value = IDArray[ArraySize-1].DoubVal;
		return Value;
	}
	// Independant Var < min in array, return min IntVal
	else if (IntVal <= IDArray[0].IntVal)
	{
		Value = IDArray[0].DoubVal;
		return Value;
	}
	else
	{
		for (index = 0; index < ArraySize - 2; index++)
		{

			if ((IntVal >= IDArray[index].IntVal)&&(IntVal <= IDArray[index+1].IntVal))
			{
				foundit = true;
				break;
			}
		}
	}

	if (foundit)
	{
		// Now do linear interpolation
		Value = ((IntVal - IDArray[index].IntVal)/
				(IDArray[index+1].IntVal - IDArray[index].IntVal)) * 
				(IDArray[index+1].DoubVal - IDArray[index].DoubVal) + 
				IDArray[index].DoubVal;
	}

	return Value;
}

int CSpores::Interpolation(IntDoub* IDArray,int ArraySize, double DoubVal)
{
	// Performs a linear interpolation of a IntDoub array using the DoubVal as the
	// Independant variable.  If the interpolation is not found, 0 is returned.
	bool foundit = false;
	int Value = 0;
	int index = 0;
	
	// Independant Var > max in array, return max IntVal
	if (DoubVal >= IDArray[ArraySize-1].DoubVal) 
	{
		Value = IDArray[ArraySize-1].IntVal;
		return Value;
	}
	// Independant Var < min in array, return min IntVal
	else if (DoubVal <= IDArray[0].DoubVal)
	{
		Value = IDArray[0].IntVal;
		return Value;
	}
	else
	{
		for (index = 0; index < ArraySize-2; index++)
		{
			if ((DoubVal >= IDArray[index].DoubVal)&&(DoubVal <= IDArray[index+1].DoubVal))
			{
				foundit = true;
				break;
			}
		}
	}
	if (foundit)
	{
		// Now do linear interpolation
		Value = (int)(((DoubVal - IDArray[index].DoubVal)/
			     (IDArray[index+1].DoubVal - IDArray[index].DoubVal)) * 
				 (IDArray[index+1].IntVal - IDArray[index].IntVal) + 
				 IDArray[index].IntVal);
	}
	return Value;
}