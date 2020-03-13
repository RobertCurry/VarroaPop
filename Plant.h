// Plant.h: interface for the CPlant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANT_H__D34ED1E1_7B4E_11D3_8919_44455354616F__INCLUDED_)
#define AFX_PLANT_H__D34ED1E1_7B4E_11D3_8919_44455354616F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FlyList.h"
#include "WeatherEvent.h"

class CWhiteFlyDoc;  // Forward reference

class CPlant : public CObject  
{
protected:
	// Attributes
	DECLARE_DYNCREATE(CPlant)
	double	m_NumLeaves;
	double  m_LeavesPerWeek;
	bool	m_Initialized;
	CWhiteFlyDoc* m_pDoc;

	CFlyList m_Eggs_F;
	CFlyList m_Eggs_M;
	CFlyList m_Immatures_F;
	CFlyList m_Immatures_M;
	CFlyList m_Adults_F;
	CFlyList m_Adults_M;
	CInsectList m_Parasites;


	// Operations
public:
	CPlant();
	virtual ~CPlant();
	void SetNumLeaves(long num) {m_NumLeaves = num;}
	double GetNumLeaves() {return m_NumLeaves;}
	double GetFEggQuan() {return m_Eggs_F.GetQuantity();}
	double GetMEggQuan() {return m_Eggs_M.GetQuantity();}
	double GetFImmQuan() {return m_Immatures_F.GetQuantity();}
	double GetMImmQuan() {return m_Immatures_M.GetQuantity();}
	double GetFAdultQuan() {return m_Adults_F.GetQuantity();}
	double GetMAdultQuan() {return m_Adults_M.GetQuantity();}
	double GetTotFlyQuan();
	double GetParasiteQuan() {return m_Parasites.GetQuantity();}
	void SetDocument(CWhiteFlyDoc* ptheDoc) {m_pDoc = ptheDoc;}
	CWhiteFlyDoc* GetDocument() {return m_pDoc;}
	bool IsInitialized() {return m_Initialized;}
	void InitializePlant();
	void InitializeFlys();
	void UpdatePlant(CEvent* pEvent, int DayNum);
	void UpdateFlys(CEvent* pEvent, int DayNum);

//	virtual void Serialize(CArchive& ar);


};

#endif // !defined(AFX_PLANT_H__D34ED1E1_7B4E_11D3_8919_44455354616F__INCLUDED_)
