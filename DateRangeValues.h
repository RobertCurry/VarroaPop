#pragma once

// CDateRangeValue command target

	class DR_ITEM 
	{
	public:
		COleDateTime StartTime; 
		COleDateTime EndTime; 
		double Value;
	};


class CDateRangeValues : public CObject
{

		
protected:
	CObList* pItemList;
	bool m_Enabled;
	int FileFormatVersion;

public:
	CDateRangeValues();
	virtual ~CDateRangeValues();
	CDateRangeValues& operator =(CDateRangeValues& DRV);

	bool GetItem(int Index, DR_ITEM& theItem);
	DR_ITEM* GetItemPtr(int Index);
	bool GetActiveItem(COleDateTime theDate, DR_ITEM& theItem);
	bool GetActiveValue(COleDateTime theDate, double& theValue);
	void AddItem(DR_ITEM& theItem);
	void AddItem(COleDateTime theStartTime, COleDateTime theEndTime, double theValue);
	void AddItem(CString theStartTimeStg, CString theEndTimeStg, double theValue);
	void DeleteItem(int Index);
	int GetCount();
	void ClearAll();
	void Serialize(CArchive& ar, int Version);
	void Copy(CDateRangeValues* pDestination);
	bool IsEnabled() {return m_Enabled;}
	void SetEnabled(bool EnableVal) {m_Enabled = EnableVal;}
	void SetFileFormatVersion(int version) {FileFormatVersion = version;}
	

};


