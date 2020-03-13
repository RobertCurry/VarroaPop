#pragma once
#include "afx.h"


struct SNCElement 
{
	COleDateTime m_NCDate;
	double m_NCPollenCont;
	double m_NCNectarCont;
};

class CNutrientContaminationTable :
	public CObject
{
private:
public:
	CArray <SNCElement, SNCElement> m_ContDateArray;

public:
	CNutrientContaminationTable(void);
	~CNutrientContaminationTable(void);
	bool LoadTable(CString FilePath);
	void RemoveAll(void) {m_ContDateArray.RemoveAll();}
	void GetContaminantConc(COleDateTime Date, double &NecConc, double &PolConc);
	void operator = (const CNutrientContaminationTable &CNTable);
	CString GetFileName() {return m_ContaminantFileName;}
	virtual void Serialize(CArchive& ar);
	BOOL m_NutrientContEnabled;
	CString m_ContaminantFileName;
	BOOL IsEnabled() {return m_NutrientContEnabled;}

};
