// Bee.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "Bee.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBee

IMPLEMENT_DYNCREATE(CBee, CCmdTarget)

CBee::CBee()
{
	Alive = true;
}

CBee::~CBee()
{
}

CBee CBee::operator = (CBee& bee)
{
	CBee temp;
	temp.Alive = bee.Alive;
	temp.number = bee.number;
	return temp;
}

CBee::CBee(int Num)
{
	number = Num;
}

CBee::CBee(CBee& bee)
{
	Alive = bee.Alive;
	number = bee.number;
}



BEGIN_MESSAGE_MAP(CBee, CCmdTarget)
	//{{AFX_MSG_MAP(CBee)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBee message handlers

void CBee::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		ar << number;
		ar << age;
	}
	else
	{	// loading code
		ar >> number;
		ar >> age;
	}
}
