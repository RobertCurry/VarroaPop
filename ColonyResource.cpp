// ColonyResource.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "ColonyResource.h"


// CColonyResource
/////////////////////////////////////////////////////////////////////////////////////////////////
//  The CColonyResource class is used to manage the pollen and nectar stored in the colony.
//
//  The CColonyResource member variables store the current quantities of pollen and nectar along
//  with the associated quantities of pesticide in each of these food stores.  
//
//  An object of this class is a member variable in the CColony object so the colony manages it's
//  resources by updating the resource store.

CColonyResource::CColonyResource()
{
	Initialize();
}

CColonyResource::~CColonyResource()
{
}

void CColonyResource::Initialize(double InitPollen, double InitNectar)
{
	m_PollenQuantity = InitPollen;
	m_NectarQuantity = InitNectar;
	m_PollenPesticideQuantity = 0.0;
	m_NectarPesticideQuantity = 0.0;

}

void CColonyResource::Initialize(void)
{
	m_PollenQuantity = 0.0;
	m_NectarQuantity = 0.0;
	m_PollenPesticideQuantity = 0.0;
	m_NectarPesticideQuantity = 0.0;
}


double CColonyResource::GetNectarPesticideConcentration()
{
	double ans = 0;
	if (m_NectarQuantity > 0) ans = m_NectarPesticideQuantity/m_NectarQuantity;
	return ans;
}

double CColonyResource::GetPollenPesticideConcentration()
{
	double ans = 0;
	if (m_PollenQuantity > 0) ans = m_PollenPesticideQuantity/m_PollenQuantity;
	return ans;
}

void CColonyResource::AddPollen(SResourceItem Pollen)
{
	m_PollenQuantity += Pollen.m_ResourseQuantity;
	m_PollenPesticideQuantity += Pollen.m_PesticideQuantity;
}

void CColonyResource::AddNectar(SResourceItem Nectar)
{
	m_NectarQuantity += Nectar.m_ResourseQuantity;
	m_NectarPesticideQuantity += Nectar.m_PesticideQuantity;
}

// This version just removes the pollen and pesticide from the resources
void CColonyResource::RemovePollen(double Amount)
{
	// Pollen cannot go below zero
	if (m_PollenQuantity >= Amount)
	{
		m_PollenQuantity -= Amount;
		m_PollenPesticideQuantity -= Amount*GetPollenPesticideConcentration();
		if (m_PollenPesticideQuantity < 0) m_PollenPesticideQuantity = 0;
	}
	else
	{
		m_PollenQuantity = 0.0;
		m_PollenPesticideQuantity = 0.0;
	}
}
// This version removes the desired number of Amount and returns a SResourceItem containing the quantity of pollen and pesticide
void CColonyResource::RemovePollen(double Amount, SResourceItem &Pollen)
{
	if (m_PollenQuantity >= Amount)
	{
		// Pollen
		Pollen.m_ResourseQuantity = Amount;
		m_PollenQuantity -= Amount;
		if (m_PollenQuantity < 0) m_PollenQuantity = 0;

		// Pesticide
		Pollen.m_PesticideQuantity = GetPollenPesticideConcentration()*Pollen.m_ResourseQuantity;
		m_PollenPesticideQuantity -= Pollen.m_PesticideQuantity;
		if (m_PollenPesticideQuantity < 0) m_PollenPesticideQuantity = 0;
	}
	else
	{
		Pollen.m_ResourseQuantity = m_PollenQuantity;
		Pollen.m_PesticideQuantity = m_PollenPesticideQuantity;
		m_PollenQuantity = 0.0;
		m_PollenPesticideQuantity = 0.0;
	}
}

void CColonyResource::RemoveNectar(double Amount)
{
	// Nectar cannot go below zero
	if (m_NectarQuantity >= Amount)
	{
		m_NectarQuantity -= Amount;
		m_NectarPesticideQuantity -= Amount*GetNectarPesticideConcentration();
		if (m_NectarPesticideQuantity < 0) m_NectarPesticideQuantity = 0;
	}
	else
	{
		m_NectarQuantity = 0.0;
		m_NectarPesticideQuantity = 0.0;

	}
}

void CColonyResource::RemoveNectar(double Amount, SResourceItem &Nectar)
{
	if (m_NectarQuantity >= Amount)
	{
		// Nectar
		Nectar.m_ResourseQuantity = Amount;
		m_NectarQuantity -= Amount;
		if (m_NectarQuantity < 0) m_NectarQuantity = 0;

		// Pesticide
		Nectar.m_PesticideQuantity = GetNectarPesticideConcentration()*Nectar.m_ResourseQuantity;
		m_NectarPesticideQuantity -= Nectar.m_PesticideQuantity;
		if (m_NectarPesticideQuantity < 0) m_NectarPesticideQuantity = 0;
	}
	else
	{
		Nectar.m_ResourseQuantity = m_NectarQuantity;
		Nectar.m_PesticideQuantity = m_NectarPesticideQuantity;
		m_NectarQuantity = 0.0;
		m_NectarPesticideQuantity = 0.0;

	}

}
