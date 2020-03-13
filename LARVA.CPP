// Larva.cpp: implementation of the CLarva class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarroaPop.h"
#include "Larva.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLarva::CLarva()
{
}

CLarva::~CLarva()
{
}


void CLarva::Serialize(CArchive& ar) 
{
#define INFESTED 1
#define NOT_INFESTED 0
#define FERTILIZED 1
#define NOT_FERTILIZED 0

	int temp;
	if (ar.IsStoring())
	// have to add a little complexity since there is no >> operator for bool types
	{	// storing code
		temp = (infested==true) ? INFESTED : NOT_INFESTED; 
		ar << temp;
		temp = (fertilized==true) ? FERTILIZED : NOT_FERTILIZED;
		ar << temp;
		ar << mites;
		ar << infestProbability;
	}
	else
	{	// loading code
		ar >> temp;
		infested = (temp == INFESTED) ? true : false;
		ar >> temp;
		fertilized = (temp == FERTILIZED) ? true : false;
		ar >> mites;
		ar >> infestProbability;
	}
	CBee::Serialize(ar);
}
