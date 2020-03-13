// Brood.cpp: implementation of the CBrood class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "varroapop.h"
#include "Brood.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBrood::CBrood()
{
	m_Mites = 0;
}

CBrood::~CBrood()
{
}


void CBrood::Serialize(CArchive& ar) 
{
	int tempval = 0;
	if (ar.IsStoring())
	{	// storing code
	//	ar << m_Mites;
		ar << tempval;  // bogus - fix later by serializing CMite
	}
	else
	{	// loading code
	//	ar >> m_Mites;
		ar >> tempval;  // bogus - fix later by serializing CMite
	}
	CBee::Serialize(ar);
}
