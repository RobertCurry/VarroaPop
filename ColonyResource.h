#pragma once

// CColonyResource command target

struct SResourceItem
{
	double m_ResourseQuantity;
	double m_PesticideQuantity;
};


class CColonyResource : public CObject
{
private:

	////////////////////////////////////////////////
	// The Colony Resource quantities.  Units are in grams.
	double m_PollenQuantity;
	double m_NectarQuantity;
	double m_PollenPesticideQuantity;
	double m_NectarPesticideQuantity;



public:
	CColonyResource();
	virtual ~CColonyResource();

	//////////////////////////////////////////////////////////////
	// Gets and Sets
	double GetPollenQuantity(){return m_PollenQuantity;}
	double GetNectarQuantity(){return m_NectarQuantity;}
	double GetPollenPesticideQuantity(){return m_PollenPesticideQuantity;}
	double GetNectarPesticideQuantity(){return m_NectarPesticideQuantity;}
	double GetPollenPesticideConcentration();
	double GetNectarPesticideConcentration();

	void SetPollenQuantity(double Quan){m_PollenQuantity = Quan;}
	void SetNectarQuantity(double Quan){m_NectarQuantity = Quan;}
	void SetPollenPesticideQuantity(double Quan){m_PollenPesticideQuantity = Quan;}
	void SetNectarPesticideQuantity(double Quan){m_NectarPesticideQuantity = Quan;}


	//////////////////////////////////////////////////////////////////////
	// Methods
	void Initialize(void);
	void Initialize(double InitPollen, double InitNectar);
	void AddPollen(SResourceItem Pollen);
	void AddNectar(SResourceItem Nectar);
	void RemovePollen(double Grams);
	void RemovePollen(double Grams, SResourceItem &Pollen);
	void RemoveNectar(double Grams);
	void RemoveNectar(double Grams, SResourceItem &Nectar);


};


