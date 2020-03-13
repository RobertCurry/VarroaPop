#pragma once
#include "afx.h"

	class AIItem : public CObject
	{
	public:
		CString m_AI_Name;
		double m_AI_AdultSlope;
		double m_AI_AdultLD50;
		double m_AI_AdultSlope_Contact;
		double m_AI_AdultLD50_Contact;
		double m_AI_LarvaSlope;
		double m_AI_LarvaLD50;
		double m_AI_KOW;
		double m_AI_KOC;
		double m_AI_HalfLife;
		double m_AI_ContactFactor;
		
		virtual void Serialize(CArchive& ar, int FileFormatVersion);

		AIItem& operator =(const AIItem &rhs);

	};

class CEPAData : public CObject
{

public:
	CEPAData(void);
	~CEPAData(void);


	// Active Ingredients
	CObList m_AIItemList; // List of AIs

	// Currently selected AI
	CString m_AI_Name;
	double m_AI_AdultSlope;
	double m_AI_AdultLD50;
	double m_AI_AdultSlope_Contact;
	double m_AI_AdultLD50_Contact;
	double m_AI_LarvaSlope;
	double m_AI_LarvaLD50;
	double m_AI_KOW;
	double m_AI_KOC;
	double m_AI_HalfLife;
	double m_AI_ContactFactor;

	// Consumption
	double m_C_L4_Pollen;
	double m_C_L4_Nectar;
	double m_C_L5_Pollen;
	double m_C_L5_Nectar;
	double m_C_LD_Pollen;
	double m_C_LD_Nectar;
	double m_C_A13_Pollen;
	double m_C_A13_Nectar;
	double m_C_A410_Pollen;
	double m_C_A410_Nectar;
	double m_C_A1120_Pollen;
	double m_C_A1120_Nectar;
	double m_C_AD_Pollen;
	double m_C_AD_Nectar;
	double m_C_Forager_Pollen;
	double m_C_Forager_Nectar;

	// Incoming
	int m_I_PollenTrips;
	int m_I_NectarTrips;
	double m_I_PercentNectarForagers;
	double m_I_PollenLoad;
	double m_I_NectarLoad;

	// Exposure
	BOOL m_FoliarEnabled;
	BOOL m_SoilEnabled;
	BOOL m_SeedEnabled;
	double m_E_AppRate;
	double m_E_SoilTheta;
	double m_E_SoilP;
	double m_E_SoilFoc;
	double m_E_SoilConcentration; //needs to be added to serialization
	double m_E_SeedConcentration;
	COleDateTime m_FoliarAppDate;
	COleDateTime m_FoliarForageBegin, m_FoliarForageEnd;
	COleDateTime m_SoilForageBegin, m_SoilForageEnd;
	COleDateTime m_SeedForageBegin, m_SeedForageEnd;

	// Nectar/Pollen Bypass File Processing
	BOOL m_NecPolFileEnabled;
	CString m_NecPolFileName;

	// Current Dosage/bee
	// Note:  This changes daily - doesn't need to be serialized
	double m_D_L4;				// Current dosage for 4 day old worker larvae
	double m_D_L5;				// Current dosage for 5 day old worker larvae
	double m_D_LD;				// Current dosage for drone larvae
	double m_D_A13;				// Current dosage for adult workers 1-3 days old
	double m_D_A410;			// Current dosage for adult workers 4-10 days old
	double m_D_A1120;			// Current dosage for adult workers 11-20 days old
	double m_D_AD;				// Current dosage for adult drones
	double m_D_C_Foragers;		// Current contact dosage for foragers
	double m_D_D_Foragers;		// Current diet dosage for foragers

	// Maximum doses seen so far for each life stage
	double m_D_L4_Max;
	double m_D_L5_Max;
	double m_D_LD_Max;
	double m_D_A13_Max;
	double m_D_A410_Max;
	double m_D_A1120_Max;
	double m_D_AD_Max;
	double m_D_C_Foragers_Max;
	double m_D_D_Foragers_Max;


	//Operations
public:
	CEPAData& operator =(const CEPAData &rhs);  // Assignment Operator
	virtual void Serialize(CArchive& ar, int FileFormatVersion);
	void AddAIItem(AIItem* theItem);
	bool RemoveAIItem(CString theName);
	BOOL GetAIItem(CString theName, AIItem* pItem);
	AIItem* GetAIItemPtr(CString Name);
	int GetAIItemCount(){return m_AIItemList.GetCount();}
	void SetCurrentAIItem(AIItem* pItem);
	double DoseResponse(double Dose, double LD50, double Slope);
};
