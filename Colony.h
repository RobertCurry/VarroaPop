#if !defined(AFX_COLONY_H__8C6C41B4_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_COLONY_H__8C6C41B4_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Colony.h : header file
//

#include "Bee.h"
#include "Egg.h"
#include "Queen.h"
#include "Larva.h"
#include "Brood.h"
#include "Adult.h"
#include "Mite.h"
#include "WeatherEvents.h"
#include "Spores.h"
#include "MiteTreatments.h"
#include "MiteTreatmentItem.h"
#include "DateRangeValues.h"
#include "ColonyResource.h"
#include "EPAData.h"
#include "NutrientContaminationTable.h"



class CColony;  // Forward declaration

/////////////////////////////////////////////////////////////////////////////
// List classes for all bee life stages


/////////////////////////////////////////////////////////////////////////////
//
// CBeelist - this is the base class for all bee list classes.  Contains the functions and attributes common to all
//
class CBeelist : public CObList
{
protected:
	int m_ListLength;
	CColony* m_pColony;
	static const int m_DefaultPropTransition = 1;
	double m_PropTransition;  // The proportion of bees from this list that go to the caboose
							  // A number between 0.0 .. 1.0
	CDateRangeValues* m_pDRVList;

public:
	CBeelist(){}
	~CBeelist();
	void SetLength(int len) {m_ListLength = len;}
	int GetLength() {return m_ListLength;}
	int GetQuantity();
	int GetQuantityAt(int index);
	int GetQuantityAt(int from, int to);
	void SetQuantityAt(int index, int Quan);
	void SetQuantityAt(int from, int to, int Quan);
	void SetQuantityAtProportional(int from, int to, double Proportion);
	void KillAll();
	void SetColony(CColony* pCol) {m_pColony = pCol;}
	CColony* GetColony() {return m_pColony;}
	void AddMember(CBee* element);
	virtual void Serialize(CArchive &ar) = 0;
	CString Status();
	void FactorQuantity(double factor);
	//void SetQuantityAt(int Quan);
	void SetPropTransition(double Prop) {m_PropTransition = Prop;}
	double GetPropTransition() {return m_PropTransition;}

	static int DroneCount;
	static int ForagerCount;
	static int WorkerCount;


};

/////////////////////////////////////////////////////////////////////////////
//
// CAdultlist - Drones and Workers
//
class CAdultlist : public CBeelist
{
protected:
	CAdult* Caboose;
public:

	CAdultlist() {Caboose = NULL;}	
	CAdult* GetCaboose() {return Caboose;}
	void ClearCaboose() {Caboose = NULL;}
	void Update(CBrood* theBrood, CColony* theColony, CEvent* theEvent, bool bWorkder = true);
	void Serialize(CArchive &ar);
	void KillAll();
	void UpdateLength(int len, bool bWorker = true);
	int MoveToEnd(int QuantityToMove, int MinAge);
};

/////////////////////////////////////////////////////////////////////////////
//
// CForagerlist - this is a type of CAdultList implementing Forager behaviors
//
class CForagerlist : public CAdultlist
{
private:
	CAdultlist PendingForagers;
	CAdult m_UnemployedForagers;
	double m_PropActualForagers;

public:
	CForagerlist();
	~CForagerlist();
	void Update(CAdult* theAdult, CEvent* theEvent);
	void ClearPendingForagers();
	void KillAll();
	int GetQuantity();  // Total Forarger Quantity including UnemployedForagers
	int GetActiveQuantity(); // Total Forager Quantity minus UnemployedForagers
	int GetUnemployedQuantity();
	void SetUnemployedForagerQuantity(int Quan) { m_UnemployedForagers.SetNumber(Quan); }
	void SetLength(int len);
	void SetPropActualForagers(double proportion) { m_PropActualForagers = proportion; }
	double GetPropActualForagers() { return m_PropActualForagers; }
};

/////////////////////////////////////////////////////////////////////////////
//
// CForagerlistA - An update from CForagerlist (created in version 3.2.8.0) to implement a new data structure logic for foragers.
// The design intent is to maintain the same interface as CForagerlist but implement to the new data structure which is a longer CAdultlist.
//
class CForagerlistA : public CAdultlist
{
private:
	CAdultlist PendingForagers;
	double m_PropActualForagers;
	

public:
	CForagerlistA();
	~CForagerlistA();
	void Update(CAdult* theAdult, CEvent* theEvent);
	void ClearPendingForagers();
	void KillAll();
	int GetQuantity();  // Total Forarger Quantity including UnemployedForagers
	int GetActiveQuantity(); // Total Forager Quantity minus UnemployedForagers
	int GetUnemployedQuantity();
	//void SetUnemployedForagerQuantity(int Quan) { m_UnemployedForagers.SetNumber(Quan); }
	void SetLength(int len);
	void SetPropActualForagers(double proportion) { m_PropActualForagers = proportion; }
	double GetPropActualForagers() { return m_PropActualForagers; }
};


/////////////////////////////////////////////////////////////////////////////
//
// CBroodlist - capped brood
//
class CBroodlist : public CBeelist
{
protected:
	CBrood* Caboose;
public:
	double GetMitesPerCell();
	void Update(CLarva* theLarva);
	void Serialize(CArchive &ar);
	int GetMiteCount();
	void KillAll();
	void DistributeMites(CMite theMites);
	float GetPropInfest();
	CBrood* GetCaboose() {return Caboose;}
	void ClearCaboose() {Caboose = NULL;}
};


/////////////////////////////////////////////////////////////////////////////
//
// CLarvalist
//
class CLarvalist : public CBeelist
{
protected:
	CLarva* Caboose;
public:
	void Update(CEgg* theEggs);
	void Serialize(CArchive &ar);
	CLarva* GetCaboose() {return Caboose;}
	void KillAll();
	void ClearCaboose() {Caboose = NULL;}
};


/////////////////////////////////////////////////////////////////////////////
//
// CEgglist
//
class CEgglist : public CBeelist
{
protected:
	CEgg* Caboose;
public:
	void Update(CEgg* theEggs);
	void Serialize(CArchive &ar);
	void KillAll();
	CEgg* GetCaboose() {return Caboose;}
	void ClearCaboose() {Caboose = NULL;}
};


/////////////////////////////////////////////////////////////////////////////
// CColony 

struct ColonyInitCond
{
	float	m_droneAdultInfestField;
	float	m_droneBroodInfestField;
	float	m_droneMiteOffspringField;
	float	m_droneMiteSurvivorshipField;
	float	m_workerAdultInfestField;
	float	m_workerBroodInfestField;
	float	m_workerMiteOffspring;
	float	m_workerMiteSurvivorship;
	int		m_droneAdultsField;
	int		m_droneBroodField;
	int		m_droneEggsField;
	int		m_droneLarvaeField;
	int		m_workerAdultsField;
	int		m_workerBroodField;
	int		m_workerEggsField;
	int		m_workerLarvaeField;
	//  From Simulation Initial Conditions
	double	m_QueenStrength;
	double	m_QueenSperm;
	double	m_MaxEggs;
	int		m_ForagerLifespan;
	CString m_SimStart;
	CString	m_SimEnd;
	CDateRangeValues m_EggTransitionDRV;
	CDateRangeValues m_LarvaeTransitionDRV;
	CDateRangeValues m_BroodTransitionDRV;
	CDateRangeValues m_AdultTransitionDRV;
	CDateRangeValues m_ForagerLifespanDRV;
	CDateRangeValues m_AdultLifespanDRV;
};

struct SupplementalFeedingResource
{
	double m_StartingAmount;  //In Grams
	double m_CurrentAmount;	  //In Grams
	COleDateTime m_BeginDate;
	COleDateTime m_EndDate;
};


//  Durations of each life stage
#define EGGLIFE 3
#define DLARVLIFE 7
#define WLARVLIFE 5
#define DBROODLIFE 14
#define WBROODLIFE 13
#define DADLLIFE 21
#define WADLLIFE 21

//  Discrete Events
#define DE_NONE 1
#define DE_SWARM 2
#define DE_CHALKBROOD 3
#define DE_RESOURCEDEP 4
#define DE_SUPERCEDURE 5
#define DE_PESTICIDE 6

class CColony : public CCmdTarget
{
	DECLARE_DYNCREATE(CColony)

	CColony();           // protected constructor used by dynamic creation

// Attributes
protected:
	BOOL GetDiscreteEvents(CString key, CUIntArray*& theArray);
	CString name;
	bool HasBeenInitialized;
	int m_VTStart;
	int m_SPStart;
	UINT m_VTDuration;
	UINT m_VTMortality;
	bool m_VTEnable;
	bool m_SPEnable;
	bool m_VTTreatmentActive;
	bool m_SPTreatmentActive;
	double  m_InitMitePctResistant;
	int m_MitesDyingToday;
	bool m_PollenFeedingDay;  // Specifies this is a day when man-made feed is available.
	bool m_NectarFeedingDay;  // Specifies this is a day when man-made feed is available.


	
	// Data structure for discrete events
	CMapStringToOb m_EventMap;


public:
	ColonyInitCond m_InitCond;
	double LongRedux[8]; // Longevity Reduction as a function of mite infestation
	int m_CurrentForagerLifespan;
	CArray<double, double> m_RQQueenStrengthArray;
	CStringList m_ColonyEventList;


	// Bee Attributes
	CQueen queen;
	//CForagerlist foragers;
	CForagerlistA foragers;
	CAdultlist Dadl;
	CAdultlist Wadl;
	CBroodlist CapWkr;
	CBroodlist CapDrn;
	CLarvalist Wlarv;
	CLarvalist Dlarv;
	CEgglist Weggs;
	CEgglist Deggs;

	// Mite Attributes
	CMite WMites;			// # of mites under capped worker cells
	CMite DMites;			// # of mites under capped drone cells
	CMite NewMitesW;		// # of mated female mites emerging from worker cells in 1st step
	CMite NewMitesD;		// # of mated female mites emerging from drone cells in 1st step
	CMite RunMiteW;		// # of mites on adult workers
	CMite RunMiteD;		// # of mites on adult drones
	CMite PrevEmergMite;	// # of mites that emerged from previous day
	CMite RunMite;		// Total # of mites outside of cells
	double PropRMVirgins;// Proportion of Free Mites that have not yet infested
	float PropInfstW;	// Proportion of infested workers
	float PropInfstD;	// Proportion of infested drones
	CSpores m_Spores;	// The spore population for in the colony.

	CMiteTreatments m_MiteTreatmentInfo;  // This is public since CVarroaPopDoc will serialize it based on file version

	CColonyResource m_Resources;				// The stored Pollen and Nectar in the colony
	SupplementalFeedingResource m_SuppPollen;	// Supplemental Feeding.  SAmount in grams
	SupplementalFeedingResource m_SuppNectar;	// Supplemental Feeding.  Amount in grams
	double m_ColonyNecInitAmount;				// Starting amount of Nectar(g) at the beginning of the simulation
	double m_ColonyNecMaxAmount;
	double m_ColonyPolInitAmount;				// Starting amount of Pollen(g) at the beginning of the simulation
	double m_ColonyPolMaxAmount;
	bool m_SuppPollenEnabled;
	bool m_SuppNectarEnabled;
	bool m_SuppPollenAnnual;
	bool m_SuppNectarAnnual;
	bool m_NoResourceKillsColony; // Determines if lack of nectar or pollen will cause colony to die.  If true, colony dies.  User parameter.

	CEPAData m_EPAData;		// EPA-related data structure



// Operations
public:
	int m_MitesDyingThisPeriod;
	int GetMitesDyingThisPeriod();
	void SetStartSamplePeriod();
	int GetTotalMiteCount();
	int GetMitesDyingToday();
	int GetNurseBees();
	void RemoveDiscreteEvent(CString datestg, UINT EventID);
	void AddDiscreteEvent(CString datestg, UINT EventID);
	void DoPendingEvents(CEvent* pWeatherEvent, int CurSimDay);
//	double GetMitesPerDBrood();
//	double GetMitesPerWBrood();
	virtual ~CColony();
	void Clear();
	CString GetName() {return name;}
	void SetName(CString stg) {name = stg;}
	CColony operator = (CColony col); // Assignment operator
	CColony(CColony& col);             // Copy Constructor
	void InitializeColony();
	void InitializeBees();
	void InitializeMites();
	void SetInitialized(bool val) {HasBeenInitialized = val;}
	bool IsInitialized() {return HasBeenInitialized;}
	void UpdateBees(CEvent* pEvent, int DayNum);
	void UpdateMites(CEvent* pEvent, int DayNum);
	int GetForagerLifespan() {return m_InitCond.m_ForagerLifespan;}
	void AddMites(CMite NewMites);
	void SetMitePctResistance(double pct) {m_InitMitePctResistant = pct;}
	int GetColonySize();
	void RemoveDroneComb(double pct);
	void ReQueenIfNeeded(
		int		DayNum,
		CEvent* theEvent,
		UINT	EggLayingDelay,
		double	WkrDrnRatio,
		BOOL	EnableReQueen,
		int		Scheduled,
		double		QueenStrength,
		int		Once,
		COleDateTime	ReQueenDate);
	void SetMiticideTreatment(
		int StartDayNum, 
		UINT Duration, 
		UINT Mortality, 
		BOOL Enable);
	void SetMiticideTreatment(CMiteTreatments& theTreatments, BOOL Enable);
	void SetSporeTreatment(
		int StartDayNum,
		BOOL Enable);
//	double GetSporeMortality(int TreatmentDayNum);
	bool IsPollenFeedingDay(CEvent* pEvent);
	bool IsNectarFeedingDay(CEvent* pEvent);
	void KillColony();
	COleDateTime* GetDayNumDate(int theDayNum);
	void AddEventNotification(CString DateStg, CString Msg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColony)
	public:
	virtual void Serialize(CArchive& ar, int FileFormatVersion = 0);
	//}}AFX_VIRTUAL


	// Generated message map functions
	//{{AFX_MSG(CColony)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	// Colony Resource Management
	double GetPollenNeeds(CEvent* pEvent);
	double GetNectarNeeds(CEvent* pEvent);
	void InitializeColonyResources(void);
	double GetIncomingNectarQuant(void);
	double GetIncomingNectarPesticideConcentration(int DayNum);
	double GetIncomingPollenQuant(void);
	double GetIncomingPollenPesticideConcentration(int DayNum);

	//EPA Pesticide Dose and Mortality
	void ConsumeFood(CEvent* pEvent, int DayNum);
	void DetermineFoliarDose(int DayNum);
	void ApplyPesticideMortality();
	int ApplyPesticideToBees(CBeelist* pList, int from, int to, double CurrentDose, double MaxDose, double LD50, double Slope);
	int QuantityPesticideToKill(CBeelist* pList, double CurrentDose, double MaxDose, double LD50, double Slope);

	public:
	CNutrientContaminationTable m_NutrientCT;
	bool m_NutrientContEnabled;
	int m_DeadWorkerLarvaePesticide;
	int m_DeadDroneLarvaePesticide;
	int m_DeadWorkerAdultsPesticide;
	int m_DeadDroneAdultsPesticide;
	int m_DeadForagersPesticide;
protected:
	void AddPollenToResources(SResourceItem theResource);
public:
	void AddNectarToResources(SResourceItem theResource);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLONY_H__8C6C41B4_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
