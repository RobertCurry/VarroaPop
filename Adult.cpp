// Forager.cpp: implementation of the CAdult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarroaPop.h"
#include "Adult.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdult::CAdult()
{
	m_CurrentAge = 0;
	m_Mites = 0;
	m_ForageInc = 0;
	number = 0;
}

CAdult::CAdult(int theNumber)
{
	m_CurrentAge = 0;
	m_Mites = 0;
	m_ForageInc = 0;
	number = theNumber;
}


CAdult::~CAdult()
{
}

void CAdult::Serialize(CArchive& ar) 
{
	CBee::Serialize(ar);
	if (ar.IsStoring())
	{	// storing code
		ar << m_Lifespan;
	}
	else
	{	// loading code
		ar >> m_Lifespan;
	}
	CBee::Serialize(ar);
}
