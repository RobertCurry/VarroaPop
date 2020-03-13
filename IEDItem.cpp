// IEDItem.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "IEDItem.h"


// CIEDItem

CIEDItem::CIEDItem()
{
    m_MortEggs = 0;
    m_MortLarvae = 0;
    m_MortBrood = 0;
    m_MortAdults = 0;
    m_MortForagers = 0;
    m_IEDDate = COleDateTime::GetCurrentTime();  //Initialize to a valid date
}

CIEDItem::~CIEDItem()
{
}


// CIEDItem member functions
