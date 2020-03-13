// Adult.h: interface for the CAdult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADULT_H__8C6C41B6_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_ADULT_H__8C6C41B6_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Bee.h"
#include "Mite.h"

class CAdult : public CBee  {
  private:
	float m_Lifespan;
	int m_CurrentAge;
	CMite m_Mites;
	double m_Virgins;
	double m_ForageInc;

  public:
	CAdult();
	CAdult(int Num);
	CAdult(CAdult* oldAdult);
	void SetLifespan(int span) {m_Lifespan = (float)span;}
	void SetCurrentAge(int age) {m_CurrentAge = age;}
	void SetPropVirgins(double prop) {m_Virgins = prop;}
	double GetPropVirgins() {return m_Virgins;}
	void IncrementAge() {m_CurrentAge++;}
	int GetCurrentAge() {return m_CurrentAge;}
	int GetLifespan() {return int(m_Lifespan);}
	void SetForageInc( double Inc ) {m_ForageInc = Inc;}
	double GetForageInc() {return m_ForageInc;}
	void Serialize(CArchive &ar);
	virtual ~CAdult();

	CAdult& operator=(const CAdult& theAdult);

	void SetMites(CMite theMites) {m_Mites = theMites;}
	CMite GetMites() {return m_Mites;}

};

#endif // !defined(AFX_ADULT_H__8C6C41B6_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
