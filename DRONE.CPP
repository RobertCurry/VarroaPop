// Drone.cpp: implementation of the CDrone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarroaPop.h"
#include "Drone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrone::CDrone() {
	age = 0;
	number = 1;
}

CDrone::CDrone(int quantity) {
	age = 0;
	number = quantity;
}

CDrone::~CDrone()
{

}

CDrone::CDrone(CDrone* oldDrone) {
	number = oldDrone->number;
	age = oldDrone->age;
}

// assignment operator
CDrone& CDrone::operator=(const CDrone& theDrone) {
	if(this == &theDrone) {
		return *this;
	}

	// copy variables
	number = theDrone.number;
	age = theDrone.age;

	return *this;
}

void CDrone::Serialize(CArchive& ar) 
{
	CBee::Serialize(ar);
}
