// Drone.h: interface for the CDrone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRONE_H__8C6C41B5_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_DRONE_H__8C6C41B5_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Bee.h"

class CDrone : public CBee  
{
public:
	CDrone();
	CDrone(int quantity);
	CDrone(CDrone* oldDrone);
	void Serialize(CArchive &ar);
	virtual ~CDrone();

	CDrone& operator=(const CDrone& theDrone);
};

#endif // !defined(AFX_DRONE_H__8C6C41B5_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
