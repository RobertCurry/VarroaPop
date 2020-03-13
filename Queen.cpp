// Queen.cpp: implementation of the CQueen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarroaPop.h"
#include "Bee.h"
#include "Egg.h"
#include "Queen.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int initialSperm = 5500000;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueen::CQueen() {
	number = 1;
	age = 0;  // days
	CumWeggs = 0;
	m_CurQueenDay_1 = 1;
	m_EggLayingDelay = 0;
	m_InitialSperm = initialSperm;
	m_Strength = 1;

	// Set up queen strength matrix
	// For each strength value [1..5], a corresponding MaxEggs and InitialSperm count are given.
	//int m_Vars[][2] = 
	//{
	//	{ 1000,1800000 },
	//	{ 1500,2720000 },
	//	{ 2000,3650000 },
	//	{ 2500,4750000 },
	//	{ 3000,5500000 }
	//};

	m_Vars[0][0] = 1000; m_Vars[0][1] = 1800000;
	m_Vars[1][0] = 1500; m_Vars[1][1] = 2720000;
	m_Vars[2][0] = 2000; m_Vars[2][1] = 3650000;
	m_Vars[3][0] = 2500; m_Vars[3][1] = 4750000;
	m_Vars[4][0] = 3000; m_Vars[4][1] = 5500000;
}

CQueen::~CQueen()
{
}

// copy constructor
CQueen::CQueen(CQueen* oldQueen) {
	number = 1;
	age = oldQueen->age;
	m_InitialSperm = oldQueen->m_InitialSperm;
	m_CurrentSperm = oldQueen->m_CurrentSperm;
	m_MaxEggs = oldQueen->m_MaxEggs;
	m_Strength = oldQueen->m_Strength;
}

// assignment operator
CQueen& CQueen::operator=(const CQueen& theQueen) {
	if(this == &theQueen) {
		return *this;
	}

	// copy variables
	number = theQueen.number;
	age = theQueen.age;
	m_InitialSperm = theQueen.m_InitialSperm;
	m_CurrentSperm = theQueen.m_CurrentSperm;
	m_MaxEggs = theQueen.m_MaxEggs;
	m_Strength = theQueen.m_Strength;

	return *this;
}

void CQueen::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		ar << (int)m_InitialSperm;  //FIX - need to convert to double
	}
	else
	{	// loading code
		int TEMP;
		ar >> TEMP;
		m_InitialSperm = TEMP;
		//ar >> m_InitialSperm;  // FIX
		//m_CurrentSperm = m_InitialSperm;
	}
	CBee::Serialize(ar);
}






void CQueen::LayEggs(int LayDays, double DegreeDays, double DaylightHours, int NumForagers, 
					 double LarvPerBee)
{
	/* 
	The egg laying algorithm is taken from BEEPOP: A HONEYBEE POPULATION
	DYNAMICS SIMULATION MODEL, G. DeGrandi-Hoffman, et. al.  October 1988
	*/

	double E, P, DD, L, N, Z, B, F, S;

	// First test to see if egg laying is turned on based on having enough
	// house bees to tend to the larvae

	if (LarvPerBee > 2)  // Not enough House Bees ?
	{
		m_Weggs = 0;
		m_Deggs = 0;
	}
	else if (--m_EggLayingDelay > 0) // Still some egg laying delay from re-queening?
	{
		m_Weggs = 0;
		m_Deggs = 0;
	}
	else
	{

		//  Eggs laid on this day  =  E
		LayDays = LayDays - m_CurQueenDay_1;  // Correct if there has been a re-queening
		P = m_MaxEggs + -0.0027*LayDays*LayDays + 0.395*LayDays;
		if (P<0) P = 0;

		DD = -0.0006*DegreeDays*DegreeDays + 0.05*DegreeDays + 0.021;
		if (DD < 0) DD = 0;
		if (DD > 1) DD = 1;
		if (DegreeDays == 0) DD = 0;

		if (DaylightHours <= 9.5) L = 0;
		else
		{
			L = log10((DaylightHours+0.3)*0.1)*7.889;
//			L = -5.15 + 0.809*DaylightHours - (0.0262*DaylightHours*DaylightHours);
		}
		if (L < 0) L = 0;
		if (L > 1.0) L = 1.0;

		N = (log10((NumForagers*0.001)+1))*0.672;
		if (N<0) N = 0;
		if (N>1) N = 1;

		E = DD*L*N*P;

		// Proportion of Drone Eggs = Z
		S = GetPropDroneEggs();

		L = log10(DaylightHours*0.1) * 0.284;
		if (L < 0) L = 0;

		F = log10(NumForagers*0.0006) * 0.797;
		if (F < 0) F = 0; 

		B = L*F;

		Z = S + B;
		if (Z>1.0) Z = 1.0;

		// Numbers of Drone Eggs and Worker Eggs
		double DblDeggs = E*Z;
		double DblWeggs = E - DblDeggs;

		m_Deggs = int(E*Z);
		m_Weggs = int(E - m_Deggs);
		m_CurrentSperm -= 10*m_Weggs;
		if (m_CurrentSperm < 0) m_CurrentSperm = 0;

		// Only 85% of eggs become adults
		m_Deggs = int(m_Deggs*0.85);
		m_Weggs = int(m_Weggs*0.85);


		CumWeggs += m_Weggs;

	}
}


CEgg* CQueen::GetDeggs()
{
	CEgg* theEggs = new CEgg(m_Deggs);
	return theEggs;
//	return NULL;
}

CEgg* CQueen::GetWeggs()
{
	CEgg* theEggs = new CEgg(m_Weggs);
	return theEggs;
//	return NULL;
}


void CQueen::ReQueen(int EggLayingDelay, double QueenStrength, int SimDayNum)
{
	m_EggLayingDelay = EggLayingDelay;
	m_CurQueenDay_1 = SimDayNum;
	SetStrength(QueenStrength);

}

double CQueen::GetPropDroneEggs()
{
	double PDE;
//	double propsperm = (double)(5500000-m_CurrentSperm)/(double)5500000.0;  // Fixed this 10/6/16.  Giving too low a number for weak queens
	if (GetInitialSperm() == 0) return 0.0;  // Prevent Divide by zero
	double propsperm = (float)(GetInitialSperm()-GetCurrentSperm())/(float)GetInitialSperm();

	if (propsperm > 0.85)
	{
		int i = 1;
	}
	if (propsperm < 0.6) PDE = 0;
	else
		PDE = 1 - (-6.355*propsperm*propsperm*propsperm + 
				7.657*propsperm*propsperm -2.3*propsperm + 1.002);
	if (PDE < 0) PDE = 0;

	return (PDE);


}

/*****************************************************************************************
 The Queen Strength calculations.  

 The m_Vars table is a 5 row array where column 0 and column 1 contain the MaxEggs laid per day and the initial sperm count respectively.
 Each row, 1-5, of the array define an arbitrary queen strength of 1 to 5.  With a double valued strength value, we retrieve the corresponding 
 sperm count and maxeggs by interpolating between array values.  Similarly, we can determine the corresponding strength value by
 interpolating given either a MaxEggs value or an Initial Sperm value.  All these values are constrained to be inside the range and domain of the
 array - anything outside that is set to the appropriate array limit.

SetStrength - sets the internal values of the CQueen object based on the input strength

/*****************************************************************************************/

void CQueen::SetStrength(double Strength)
{

	// Scans through the m_Vars array that was initialized in the CQueen constructor

	m_Strength = Strength;

	// Linearly interpolate between each of these points based on Strength value
	// Constrain Strength to be  between 1 and 5 inclusive.  Ensure (int) of strength is between 1 and 4
	if (Strength > 5.0) Strength = 4.99999999;
	if (Strength < 1.0) Strength = 1.00000001;
	double MaxEggs1, MaxEggs2, Sperm1, Sperm2;
	int iStrength = (int)Strength;

	MaxEggs1 = m_Vars[iStrength-1][0];
	Sperm1 = m_Vars[iStrength-1][1];
	MaxEggs2 = m_Vars[iStrength][0];
	Sperm2 = m_Vars[iStrength][1];

	m_MaxEggs = MaxEggs1 + (MaxEggs2-MaxEggs1)*(Strength - iStrength);  // Note Delta X is always = 1 in this interpolation
	m_InitialSperm = Sperm1 + (Sperm2-Sperm1)*(Strength - iStrength);
	m_CurrentSperm = m_InitialSperm;
}

