#include "stdafx.h"
#include "NutrientContaminationTable.h"
#include "varroapop.h"

CNutrientContaminationTable::CNutrientContaminationTable(void)
{
	m_ContaminantFileName = "No File Loaded ";
}

CNutrientContaminationTable::~CNutrientContaminationTable(void)
{
}

void CNutrientContaminationTable::Serialize(CArchive& ar)
{
	m_ContDateArray.Serialize(ar);
	if (ar.IsLoading())
	{
		int Value;
		ar >> Value;
		m_NutrientContEnabled = (Value == 1);
		ar >> m_ContaminantFileName;
	}
	else
	{
		int Value;
		if (m_NutrientContEnabled) Value = 1;
		else Value = 0;
		ar << Value;
		ar << m_ContaminantFileName;
	}
}


// Assignment Operator overload
void CNutrientContaminationTable::operator = (const CNutrientContaminationTable &CNTable)
{
	RemoveAll();
	m_ContDateArray.Copy(CNTable.m_ContDateArray);
	m_NutrientContEnabled = CNTable.m_NutrientContEnabled;
	m_ContaminantFileName = CNTable.m_ContaminantFileName;
}
////////////////////////////////////////////////
// LoadTable
//
// Loads the Nutrient Contamination file
// Assumes file format is a CSV text file with three fields:
//	Date (in a common format such as mm/dd/yyyy
//	Nectar Contamination in g AI/g Nectar
//	Pollen Contamination in g AI/g Pollen
//
// Table will be loaded until the end of file is reached.
//
// If anything goes wrong, LoadTable returns a value of false, otherwise true
//
bool CNutrientContaminationTable::LoadTable(CString FilePath)
{
	bool Success = false;		
	CString Line;
	CString Name;
	CString Value;
	try
	{
		m_ContaminantFileName = "";
		CStdioFile InputFile(FilePath,CFile::shareDenyNone|CFile::modeRead);
		// Clear any existing table
		RemoveAll();

		int TokPos;
		CString Token;
		while (InputFile.ReadString(Line))
		{
			Line.Trim();
			// Remove comment lines i.e. remove everything to the right of '//' including the slashes
			int keep = Line.Find("//"); //keep = -1 means, not found.  keep = 0 means double slash first characters in string
			if (keep > 0)
			{
				Line = Line.Left(keep);
			}
			if (keep != 0) // no comment found.  If comment found in first position in string, keep = 0 and this block is skipped
			{
				TokPos = 0;
				SNCElement theElement;
				Token = Line.Tokenize(",",TokPos);
				theElement.m_NCDate.ParseDateTime(Token);

				if (TokPos >= 0) 
				{
					Token = Line.Tokenize(",",TokPos);
					theElement.m_NCNectarCont = atof(Token);
				}
				else theElement.m_NCNectarCont = 0.0;  // Didn't find token so no contamination

				if (TokPos >= 0)
				{
					Token = Line.Tokenize(",",TokPos);
					theElement.m_NCPollenCont = atof(Token);
				}
				else theElement.m_NCPollenCont = 0.0;  // Didn't find token so no contamination

				TRACE("Date:%s NectConc=%e PollenConc=%e\n",theElement.m_NCDate.Format("%m/%d/%Y"),theElement.m_NCNectarCont, theElement.m_NCPollenCont);
				m_ContDateArray.Add(theElement);
			}
		}
		Success = true;
		m_ContaminantFileName = FilePath;
	}
	catch (CFileException* pe)
	{
		TCHAR stg[255];
		pe->GetErrorMessage(stg,255);
		MyMessageBox(CString("Error Reading Nutrient Contamination Table: ") + CString(stg));
		Success = false;
	}
	return Success;
}



void CNutrientContaminationTable::GetContaminantConc(COleDateTime Date, double &NecConc, double &PolConc)
{
	COleDateTime tableDate;
	int testyear=Date.GetYear();
	int testmonth = Date.GetMonth();
	int testday = Date.GetDay();
	int tableyear, tablemonth, tableday;
	NecConc = 0; //If date not found, return contaminant concentration = 0;
	PolConc = 0;
	for (int i=0; i<m_ContDateArray.GetCount(); i++)
	{
		tableDate = m_ContDateArray[i].m_NCDate;
		tableyear = tableDate.GetYear();
		tablemonth = tableDate.GetMonth();
		tableday = tableDate.GetDay();
		if ((Date.GetMonth() == tableDate.GetMonth()) && (Date.GetDay() == tableDate.GetDay()) && (Date.GetYear() == tableDate.GetYear()))
		{
			NecConc = m_ContDateArray[i].m_NCNectarCont;
			PolConc = m_ContDateArray[i].m_NCPollenCont;
			break;  //Found a date match - break out of loop
		}
	}
}
