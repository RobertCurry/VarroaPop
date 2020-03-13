// Mite.h: interface for the CMite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MITE_H__8B31770F_E1C2_497D_87E8_5B55D593932D__INCLUDED_)
#define AFX_MITE_H__8B31770F_E1C2_497D_87E8_5B55D593932D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMite : public CObject  
{
	
protected:
	double m_Resistant;	// Number resistant to miticide
	double m_NonResistant; // Number susceptible to miticide;

public:
	CMite();
	CMite(int Res, int NonRes);
	CMite(CMite& mite);	// Copy Constructor
	virtual ~CMite();

	int GetResistant() {return int(m_Resistant + 0.5);}
	int GetNonResistant() {return int(m_NonResistant + 0.5);}
	void SetResistant(int num) {m_Resistant = double(num);}
	void SetNonResistant(int num) {m_NonResistant = double(num);}

	int GetTotal() {return int(m_Resistant + m_NonResistant);}
	double GetPctResistant() {return (GetTotal()>0) ? 
						(100.0*m_Resistant/(m_Resistant+m_NonResistant)) : 0;}
	void SetPctResistant(double pct);

	void operator += (CMite mite);
	void operator -= (CMite mite);
	void operator += (int value);
	void operator -= (int value);
	CMite operator + (CMite mite);
	CMite operator - (CMite mite);
	CMite operator = (int value); // Assign an integer to a CMite
	CMite operator = (CMite& theMite);
	CMite operator * (int value);
	CMite operator * (double value);
	operator int();

};

#endif // !defined(AFX_MITE_H__8B31770F_E1C2_497D_87E8_5B55D593932D__INCLUDED_)
