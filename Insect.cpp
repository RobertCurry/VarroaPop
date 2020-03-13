// Insect.cpp: implementation of the CInsect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "whitefly.h"
#include "Insect.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInsect::CInsect()
{
	m_Age = 0;
	m_Quantity = 0.0;
}

CInsect::~CInsect()
{

}

void CInsect::SetQuantity(double theQuantity)
{
	m_Quantity = theQuantity;
}

void CInsect::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << m_Age;
		ar << m_Quantity;
	}
	else
	{
		ar >> m_Age;
		ar >> m_Quantity;
	}
}
