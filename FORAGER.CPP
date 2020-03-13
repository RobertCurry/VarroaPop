// Forager.cpp: implementation of the CForager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarroaPop.h"
#include "Forager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForager::CForager()
{

}

CForager::~CForager()
{

}

void CForager::Serialize(CArchive& ar) 
{
	CBee::Serialize(ar);
	if (ar.IsStoring())
	{	// storing code
		ar << daysInLifetime;
	}
	else
	{	// loading code
		ar >> daysInLifetime;
	}
	CBee::Serialize(ar);
}
