#pragma once

// CIEDItem command target

class CIEDItem //: public CObject
{
public:
	CIEDItem();
	virtual ~CIEDItem();
	
	COleDateTime m_IEDDate;
	double m_MortEggs;
	double m_MortLarvae;
	double m_MortBrood;
	double m_MortAdults;
	double m_MortForagers;
	
protected:
};


