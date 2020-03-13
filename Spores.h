#pragma once



// CSpores command target

class CSpores : public CObject
{
public:
	CSpores();
	virtual ~CSpores();
	
	double GetDayMortalityRate(int TreatmentDayNumber);
	void SetMortalityFunction(double PercentMortality, int SporeDensity, int Index);


protected:
	typedef struct IntDoubtag
	{
		int IntVal;
		double DoubVal;
	} IntDoub;


	int m_InitialSporeCount; 
	IntDoub m_SporeVitalityMatrix[10];
	int m_MortFunctPtCount;
	int m_SporeVitalityPtCount;
	IntDoub m_MortalityFunctionPts[10];

	double Interpolation(IntDoub* IDMatrix, int ArraySize, int IntVar);
	int Interpolation(IntDoub* IDMatrix, int ArraySize, double DoubVal);

};


