// VarroaPopDoc.cpp : implementation of the CVarroaPopDoc class
//

#include "stdafx.h"
#include "VarroaPop.h"

#include "VarroaPopDoc.h"
#include "VarroaPopView.h"
#include "MainFrm.h"
#include "SelGraph.h"
#include "CombRemDlg.h"
#include "Options.h"
#include "WeatherEditor.h"
#include "afxcoll.h"
#include  <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc

IMPLEMENT_DYNCREATE(CVarroaPopDoc, CDocument)

BEGIN_MESSAGE_MAP(CVarroaPopDoc, CDocument)
	//{{AFX_MSG_MAP(CVarroaPopDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_SELECT_GRAPH, OnSelectGraph)
	ON_COMMAND(ID_VIEW_PLOTDATA, OnViewPlotdata)
	ON_COMMAND(ID_FILE_SAVE_RESULTS, OnFileSaveResults)
	ON_COMMAND(ID_FILE_SAVE_SESSION, OnFileSaveSession)
	ON_COMMAND(ID_VIEW_COMBREMOVALDATE, OnViewCombremovaldate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DATA_FREQ, OnUpdateToggleDataFreq)
	ON_COMMAND(ID_VIEW_DATA_FREQ, OnToggleDataFreq)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_COMMAND(ID_WEATHER_CREATENEWWEATHERFILE, OnWeatherCreatenewweatherfile)
	ON_COMMAND(ID_WEATHER_EDITCURRENTWEATHERFILE, OnWeatherEditcurrentweatherfile)
	ON_COMMAND(ID_WEATHER_EDITWEATHERFILEFROMDISK, OnWeatherEditweatherfilefromdisk)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWWARNINGS, &CVarroaPopDoc::OnUpdateViewShowwarnings)
	ON_COMMAND(ID_VIEW_SHOWWARNINGS, &CVarroaPopDoc::OnToggleShowwarnings)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc construction/destruction

CVarroaPopDoc::CVarroaPopDoc()
{
	CVarroaPopApp* theApp = (CVarroaPopApp*)AfxGetApp();

	// Set Default Path name = App path
	CString modulename = CString(((CVarroaPopApp*)AfxGetApp())->GetModuleFileName());
	SetDefaultPathName(SplitPath(modulename,DRV) + SplitPath(modulename,DIR));


	m_DispWeeklyData = false;  // Initially Set to False

	// Defaults for results file
	m_ColTitles = true;
	m_Version = true;
	m_InitConds = false;
	m_WeatherColony = false;
	m_FieldDelimiter = 0; // Fixed width fields


	// Graph Selection Initial Values
	m_AD = TRUE;
	m_AW = TRUE;
	m_CS = TRUE;
	m_DB = FALSE;
	m_DE = FALSE;
	m_DL = FALSE;
	m_F = FALSE;
	m_MDB = FALSE;
	m_MWB = FALSE;
	m_PDB = FALSE;
	m_PWB = FALSE;
	m_PRM = FALSE;
	m_RM = FALSE;
	m_WB = FALSE;
	m_WE = FALSE;
	m_WL = FALSE;
	m_IM = FALSE;
	m_MD = FALSE;
	m_PMD = FALSE;
	m_AutoScaleChart = 0;
	m_YAxisMin = 0;
	m_YAxisMax = 0;

	availableFilesList.RemoveAll();
	selectedFilesList.RemoveAll();
	selectedFileIndex = -1;


	// Add Header Strings
	m_ResultsHeader.AddTail("Date");
	m_ResultsHeader.AddTail("ColSze");
	m_ResultsHeader.AddTail("AdDrns");
	m_ResultsHeader.AddTail("AdWkrs");
	m_ResultsHeader.AddTail("Forgr");
	m_ResultsHeader.AddTail("DrnBrd");
	m_ResultsHeader.AddTail("WkrBrd");
	m_ResultsHeader.AddTail("DrnLrv");
	m_ResultsHeader.AddTail("WkrLrv");
	m_ResultsHeader.AddTail("DrnEggs");
	m_ResultsHeader.AddTail("WkrEggs");
	m_ResultsHeader.AddTail("FreeMts");
	m_ResultsHeader.AddTail("DBrdMts");
	m_ResultsHeader.AddTail("WBrdMts");
	m_ResultsHeader.AddTail("Mts/DBrd");
	m_ResultsHeader.AddTail("Mts/WBrd");
	m_ResultsHeader.AddTail("Mts Dying");
	m_ResultsHeader.AddTail("PropMts Dying");
	m_ResultsHeader.AddTail("ColPollen(g)");
	m_ResultsHeader.AddTail("PPestConc(ug/g)");
	m_ResultsHeader.AddTail("ColNectar(g)");
	m_ResultsHeader.AddTail("NPestConc(ug/g)");
	m_ResultsHeader.AddTail("Dead DLarv");
	m_ResultsHeader.AddTail("Dead WLarv");
	m_ResultsHeader.AddTail("Dead DAdults");
	m_ResultsHeader.AddTail("Dead WAdults");
	m_ResultsHeader.AddTail("Dead Foragers");
	m_ResultsHeader.AddTail("Queen Strength");
	m_ResultsHeader.AddTail("Temp (DegC)");
	m_ResultsHeader.AddTail("Precip");


	m_ImmigrationType = "None";
	m_TotImmigratingMites = 0;
	m_ImmMitePctResistant = 0;
	m_ImmigrationStartDate = COleDateTime(1999,1,1,0,0,0);
	m_ImmigrationEndDate = COleDateTime(1999,1,1,0,0,0);
	m_ImmigrationEnabled = false;
	m_SimulationComplete = false;
	m_ResultsReady = false;
	m_RQEggLayingDelay = 10;
	m_RQEnableReQueen = false;
	m_RQScheduled = 1;
	m_RQQueenStrength = 5;
	m_RQOnce = 0;
	m_VTTreatmentDuration = 0;
	m_VTEnable = false;
	m_VTMortality = 0;
	m_InitMitePctResistant = 0;

	m_SPEnable = FALSE;
	m_SPInitial = 0;


	m_CombRemoveDate = COleDateTime(1999,1,1,0,0,0);
	m_CombRemoveEnable = FALSE;
	m_CombRemovePct = 0;



	// Find all colony description files and build the list
	CFileFind finder;
	CString name;
	int ok = finder.FindFile("*.col");  // colony files
	int i;
	bool nullify;
	while(ok) {
		ok = finder.FindNextFile();
		name = finder.GetFileName();
		// strip the extension and add the name to the list
		nullify = false;
		for(i=0; i<name.GetLength(); i++) {
			if((nullify) || (name.GetAt(i) == '.')) {
				name.SetAt(i, ' ');
				nullify = true;
			}
			name.TrimRight();
		}
		availableFilesList.AddTail(name);
	}

	m_pWeather = new CWeatherEvents; // Create the WeatherEvents
	m_SessionLoaded = false;
	m_WeatherLoaded = false;
	SetShowWarnings(true);
}

CVarroaPopDoc::~CVarroaPopDoc()
{
	m_MiteTreatments.ClearAll();
	m_pWeather->ClearAllEvents();
	delete m_pWeather;
}



BOOL CVarroaPopDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetModifiedFlag(FALSE);
	SetTitle("Untitled");
	GetColony()->Clear();
	return TRUE;
}

CStringList* CVarroaPopDoc::getAvailableFilesList() {
	return &availableFilesList;
}

CStringList* CVarroaPopDoc::getSelectedFilesList() {
	return &selectedFilesList;
}

int* CVarroaPopDoc::getSelectedFileIndex() {
	return &selectedFileIndex;
}


CColony* CVarroaPopDoc::FindColony(CString ColonyName)
{
	if (theColony.GetName().GetLength()==0) return NULL;
	else return &theColony;
}

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc Simulate Functions
void CVarroaPopDoc::SetSimStart(COleDateTime start)
{
	m_SimStartTime = start;
	theColony.m_InitCond.m_SimStart = m_SimStartTime.Format("%m/%d/%Y");  
}

void CVarroaPopDoc::SetSimEnd(COleDateTime end)
{
	m_SimEndTime = end;
	theColony.m_InitCond.m_SimEnd = m_SimEndTime.Format("%m/%d/%Y");
}


bool CVarroaPopDoc::ReadyToSimulate()
{
	return(theColony.IsInitialized()&&m_pWeather->IsInitialized());
}


int CVarroaPopDoc::GetSimDays()
{
	COleDateTimeSpan ts = GetSimEnd() - GetSimStart();
	return (int)ts.GetDays()+1;
}

int CVarroaPopDoc::GetSimDayNumber(COleDateTime theDate)
{
	COleDateTime ss = GetSimStart();
	COleDateTimeSpan ts = theDate - GetSimStart();
	int num = (int)ts.GetDays()+1;
	return num;
}


COleDateTime CVarroaPopDoc::GetSimDate(int DayNumber)
{
	COleDateTimeSpan ts((long)DayNumber,0,0,0);
	return (GetSimStart()+ts);
}

int CVarroaPopDoc::GetNumSeries()
// Calculates the number of items we are planning to graph
{
	int count = 0;
	if (m_TM) count++;
	if (m_AD) count++;
	if (m_AW) count++;
	if (m_CS) count++;
	if (m_DB) count++;
	if (m_DE) count++;
	if (m_DL) count++;
	if (m_F) count++;
	if (m_MDB) count++;
	if (m_MWB) count++;
	if (m_PDB) count++;
	if (m_PWB) count++;
	if (m_PRM) count++;
	if (m_RM) count++;
	if (m_WB) count++;
	if (m_WE) count++;
	if (m_WL) count++;
	if (m_IM) count++;
	if (m_MD) count++;
	if (m_PMD) count++;
	if (m_NS) count++;
	if (m_PS) count++;
	if (m_NPC) count++;
	if (m_PPC) count++;
	if (m_DDL) count++;
	if (m_DWL) count++;
	if (m_DDA) count++;
	if (m_DWA) count++;
	if (m_DFG) count++;
	return count;
}


bool CVarroaPopDoc::IsImmigrationWindow(CEvent* pEvent)
{
	COleDateTime today = (pEvent->GetTime());

	return((today>=m_ImmigrationStartDate) && (today<=m_ImmigrationEndDate));

}


//CMite CVarroaPopDoc::GetImmigrationMites(COleDateTime theDate)
CMite CVarroaPopDoc::GetImmigrationMites(CEvent* pEvent)
{
	//  This routine calculates the number of mites to immigrate on the 
	//  specified date.  It also keeps track of the cumulative number of 
	//  mites that have migrated so far.  First calculate the total quantity
	//  of immigrating mites then return a CMite based on percent resistance to miticide
	/*
		The equations of immigration were derived by identifying the desired function, 
		e.g. f(x) = A*Cos(x), then calculating the constants by setting the integral of
		the function (over the range 0..1) to 1.  This means that the area under the
		curve is = 1.  This ensures that 100% of m_TotImmigratingMites were added to the
		colony.  With the constants were established, a very simple numerical integration 
		is performed using sum(f(x)*DeltaX) for each day of immigration.

		The immigration functions are:

			Cosine -> f(x) = 1.188395*cos(x)

			Sine -> f(x) = 1.57078*sin(PI*x)

			Tangent -> f(x) = 2.648784*tan(1.5*x)

			Exponential -> f(x) = (1.0/(e-2))*(exp(1 - (x)) - 1.0)

			Logarithmic -> f(x) = -1.0*log(x)  day #2 and on

			Polynomial -> f(x) = 3.0*(x) - 1.5*(x*x)

		In the case of Logarithmic, since there is an infinity at x=0, the 
		actual value of the integral over the range (0..DeltaX) is used on the first
		day.

		Mites only immigrate on foraging days.



	*/

	double answer;
	COleDateTime theDate = pEvent->GetTime();
	if ((theDate >= GetImmigrationStart()) && (theDate <= GetImmigrationEnd() && pEvent->IsForageDay()))
	{
		int SimDaytoday = GetSimDayNumber(theDate);
		int SimDayImStart = GetSimDayNumber(GetImmigrationStart());
		int SimDayImStop = GetSimDayNumber(GetImmigrationEnd());

		// Set cumulative immigration to 0 on first day
		if (SimDaytoday == SimDayImStart) m_CumImmigratingMites = 0;

		// If today is the last immigration day, immigrate all remaining mites
		// NOTE: Changed this logic in version 3.2.8.10 when we decided to not immigrade on non-foraging days.
		// In that case, it doesn't make sense to immigrate all remaining mites on the last day
		if (false) {} //(SimDaytoday == SimDayImStop) answer = m_TotImmigratingMites - m_CumImmigratingMites;
		else
		{

			// Calculate the proportion of days into immigration
			double ImProp = (double)(SimDaytoday - SimDayImStart )/
				(double)(1 + SimDayImStop - SimDayImStart);

			double DeltaX = 1.0/(SimDayImStop - SimDayImStart+1);
			double X = ImProp+DeltaX/2;


			// Return function based on immigration type
			if (m_ImmigrationType.MakeUpper() == "NONE") answer = 0;

			else if (m_ImmigrationType.MakeUpper() == "COSINE")  // f(x) = A*Cos(x)
			{
				answer = GetNumImmigrationMites()*1.188395*cos(X)*DeltaX;
			}

			else if (m_ImmigrationType.MakeUpper() == "EXPONENTIAL") // f(x) = A*(exp(1-x) + B) dx
			{
				answer = GetNumImmigrationMites()*(1.0/(exp(1.0) -2))*(exp(1.0 - (X)) -1.0)*DeltaX;
			}

			else if (m_ImmigrationType.MakeUpper() == "LOGARITHMIC") // f(x) = A*log(x) dx
			{
				if (ImProp == 0) // Deal with discontinuity at 0
				{
					answer = GetNumImmigrationMites()*(-1.0*DeltaX*log(DeltaX) - DeltaX);
				}
				else answer = GetNumImmigrationMites()*(-1.0*log(X)*DeltaX);
			}

			else if (m_ImmigrationType.MakeUpper() == "POLYNOMIAL") // f(x) = (A*x + B*x*x) dx
			{
				answer = GetNumImmigrationMites()*(3.0*(X) - 1.5*(X*X))*DeltaX;
			}

			else if (m_ImmigrationType.MakeUpper() == "SINE") // f(x) = A*Sin(PiX) dx
			{
				answer = GetNumImmigrationMites()*1.57078*sin(3.1416*X)*DeltaX;
			}
			else if (m_ImmigrationType.MakeUpper() == "TANGENT") // f(x) = A*Tan(B*X) dx
			{
				answer = GetNumImmigrationMites()*2.648784*tan(1.5*X)*DeltaX;
			}

			else answer = 0;  // m_ImmigrationType not valid


			// Now calculate the correction factor based on number of days of immigration.
			// The equations assume 25 days so correction factor is 25/(#days in sim)
			/*
			if (!(m_ImmigrationType == "Cosine"))
			{
			double CorrFact = (double)25/
				(double)(GetSimDayNumber(GetImmigrationEnd()) - 
				GetSimDayNumber(GetImmigrationStart()) + 1);
			answer = answer*CorrFact;
			}
			*/

			// Constrain to positive number
			if (answer <0.0) answer = 0.0;
		}

		// Increment the running total of mites that have immigrated
		int ResistantMites = int((answer*m_ImmMitePctResistant)/100 + 0.5);
		m_CumImmigratingMites += CMite(ResistantMites, int(answer-ResistantMites + 0.5));
	}
	else answer = 0;
	CMite theImms;
	theImms.SetResistant(int((answer*m_ImmMitePctResistant)/100 + 0.5));
	theImms.SetNonResistant(int(answer-theImms.GetResistant() + 0.5));
	return theImms;
}



void CVarroaPopDoc::UpdateResults(int DayCount, CEvent* pEvent)
{
	if (m_FirstResultEntry) 
	{
		m_SimLabels.RemoveAll();
	}
	int seriesID = 0;
	m_SimResults[seriesID++][DayCount-1] = DayCount;
	if (m_AD)	//	Adult Drones
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.Dadl.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Adult Drones");
	}
	if (m_AW)	// Adult Workers
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.Wadl.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Adult Workers");
	}
	if (m_CS)	// Colony Size
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.GetColonySize();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Colony Size");
	}
	if (m_DB)	// Drone Brood
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.CapDrn.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Drone Brood");
	}
	if (m_DE)	// Drone Eggs
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.Deggs.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Drone Eggs");
	}
	if (m_DL)	// Drone Larvae
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.Dlarv.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Drone Larvae");
	}
	if (m_F)	// Foragers
	{
		//m_SimResults[seriesID++][DayCount-1] = theColony.foragers.GetQuantity();
		m_SimResults[seriesID++][DayCount-1] = theColony.foragers.GetActiveQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Active Foragers");
	}
	if (m_TM)   // Total Mites In Colony
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.CapDrn.GetMiteCount()+
			theColony.CapWkr.GetMiteCount() + theColony.RunMite;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Total Mite Count");
	}
	if (m_MDB)	// Mites in Drone Brood
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.CapDrn.GetMiteCount();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Mites in Drn Brood");
	}
	if (m_MWB)	// Mites in Worker Brood
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.CapWkr.GetMiteCount();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Mites in Wkr Brood");
	}
	if (m_RM)	// Free Running Mites
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.RunMite.GetTotal();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Free Running Mites");
	}
	if (m_WB)	// Worker Brood
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.CapWkr.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Worker Brood");
	}
	if (m_WE)	// Worker Eggs
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.Weggs.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Worker Eggs");
	}
	if (m_WL)	// Worker Larvae
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.Wlarv.GetQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Worker Larvae");
	}
	if (m_PWB)	// Mites per Worker Brood Cell
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.CapWkr.GetMitesPerCell();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Mites/Wkr Cell");
	}
	if (m_PDB)	// Mites per Drone Brood Cell
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.CapDrn.GetMitesPerCell();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Mites/Drn Cell");
	}
	if (m_PRM)	// Proportion of Resistant Free Mites
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.RunMite.GetPctResistant()/100.0;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Prop Res Free Mites");
	}
	if (m_IM)	// Immigrating Mites
	{
		m_SimResults[seriesID++][DayCount-1] = m_IncImmigratingMites.GetTotal();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Immigrating Mites");
	}
	if (m_MD)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.GetMitesDyingToday();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Num Mites Dying/Day");
	}
	if (m_PMD)
	{
		m_SimResults[seriesID++][DayCount-1] = 
			(theColony.GetTotalMiteCount() > 0) ? 
			theColony.GetMitesDyingToday()/double(theColony.GetMitesDyingToday()+theColony.GetTotalMiteCount()) : 0;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Prop Mites Dying");
	}
	if (m_NS)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_Resources.GetNectarQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Nectar Stores(g) ");

	}
	if (m_PS)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_Resources.GetPollenQuantity();
		if (m_FirstResultEntry) m_SimLabels.AddTail("Pollen Stores(g) ");

	}
	if (m_NPC)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_Resources.GetNectarPesticideConcentration() * 1000000;
		if (m_FirstResultEntry) m_SimLabels.AddTail("N-PestConc ug/g ");
	}
	if (m_PPC)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_Resources.GetPollenPesticideConcentration() * 1000000;
		if (m_FirstResultEntry) m_SimLabels.AddTail("P-PestConc ug/g ");
	}
	if (m_DDL)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_DeadDroneLarvaePesticide;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Dead DLarv ");
	}
	if (m_DWL)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_DeadWorkerLarvaePesticide;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Dead WLarv ");

	}
	if (m_DDA)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_DeadDroneAdultsPesticide;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Dead DAdults ");

	}
	if (m_DWA)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_DeadWorkerAdultsPesticide;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Dead WAdults ");

	}
	if (m_DFG)
	{
		m_SimResults[seriesID++][DayCount-1] = theColony.m_DeadForagersPesticide;
		if (m_FirstResultEntry) m_SimLabels.AddTail("Dead Foragers ");

	}

	m_FirstResultEntry = false;
//	if (pEvent != NULL) m_AxisLabels.AddTail(pEvent->GetDateStg("%b/%d/%y"));
}


void CVarroaPopDoc::InitializeSimulation()
{
	// Initialize Results Matrix
	/*  The columns of this CMatrix are defined as follows:

				Col 0 = Day Number
				Col 1 = Data point for first series to be plotted
				Col 2 = Data point for second series to be plotted
				Col n = Data point for Nth series to be plotted

		The rows correspond to the values for each day.  The dimensions of the 
		CMatrix array are set to (Number of series + 1, Number of days being plotted)
	*/

	m_SimResults.SetDimensions(GetNumSeries()+1,GetSimDays());

	m_ResultsText.RemoveAll();
	m_ResultsHeader.RemoveAll();
	m_ResultsFileHeader.RemoveAll();
	m_IncImmigratingMites = 0;
	UpdateResults(1);  // Get Results of initial conditions

	theColony.InitializeColony();
	theColony.SetMiticideTreatment(m_MiteTreatments,m_VTEnable);
	theColony.SetMitePctResistance(m_InitMitePctResistant);

	// Initializing the Spore functions
//	theColony.SetSporeTreatment(GetSimDayNumber(m_SPTreatmentStart),m_SPEnable);
//	theColony.m_Spores.SetMortalityFunction(0.10,,0);
	m_CumImmigratingMites = int(0);
	m_FirstResultEntry = true;



}


void CVarroaPopDoc::Simulate()
{
	if (ReadyToSimulate())
	{
		if (!CheckDateConsistency(IsShowWarnings())) return;

		InitializeSimulation();

		//  Set results frequency 
		int ResFreq = m_DispWeeklyData?7:1;

		// Set results data format string
		if (m_FieldDelimiter == 1)		// Comma Delimited
		{
			m_ResultsFileFormatStg = "%s,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6.2f,%6.2f,%6d,%6.2f,%6.1f,%6.3f,%6.1f,%6.3f,%6d,%6d,%6d,%6d,%6d,%6.3f,%6.3f,%6.3f";
		}
		else if (m_FieldDelimiter == 2) // Tab Delimited
		{
			m_ResultsFileFormatStg = "%s\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6.2f\t%6.2f\t%6d\t%6.2f\t%6.1f\t%6.3f\t%6.1f\t%6.3f\t%6d\t%6d\t%6d\t%6d\t%6d\t%6.3f\t%6.3f\t%6.3f";
		}
		else	// Otherwise space delimited
		{
			m_ResultsFileFormatStg = "%s %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6.2f %6.2f %6d %6.2f %6.1f %6.3f %6.1f %6.3f %6d %6d %6d %6d %6d     %6.3f   %6.3f %6.3f";
		}

		CEvent* pEvent = m_pWeather->GetDayEvent(GetSimStart());
		int DayCount = 1;
		int TotSimDays = GetSimDays();
		int TotImMites = 0;
		int TotForagingDays = 0;

		CString CurSize;
		CurSize.Format("                                        Capped  Capped                                                                      Prop           Conc          Conc                                             ");
		m_ResultsFileHeader.AddTail(CurSize);
		CurSize.Format("            Colony  Adult  Adult         Drone   Wkr    Drone  Wkr    Drone  Wkr  Free   DBrood WBrood DMite  WMite  Mites  Mites  Colony Pollen Colony Nectar   Dead   Dead   Dead   Dead   Dead    Queen      Ave");
		m_ResultsFileHeader.AddTail(CurSize);
		CurSize.Format("     Date   Size   Drones   Wkr   Forgrs Brood  Brood   Larv   Larv    Eggs  Eggs Mites  Mites  Mites  /Cell  /Cell  Dying  Dying  Pollen  Pest  Nectar  Pest    DLarv  WLarv  DAdlt  WAdlt  Forgrs  Strength   Temp   Rain");
		m_ResultsFileHeader.AddTail(CurSize);
		CurSize.Format(m_ResultsFileFormatStg,
				//pEvent->GetDateStg("%m/%d/%Y"), 
				"Initial   ",
				theColony.GetColonySize(),
				theColony.Dadl.GetQuantity(),
				theColony.Wadl.GetQuantity(),
				theColony.foragers.GetActiveQuantity(),
				theColony.CapDrn.GetQuantity(),
				theColony.CapWkr.GetQuantity(),
				theColony.Dlarv.GetQuantity(),
				theColony.Wlarv.GetQuantity(),
				theColony.Deggs.GetQuantity(),
				theColony.Weggs.GetQuantity(),
				theColony.RunMite.GetTotal(),
				theColony.CapDrn.GetMiteCount(),
				theColony.CapWkr.GetMiteCount(),
				theColony.CapDrn.GetMitesPerCell(),
				theColony.CapWkr.GetMitesPerCell(),
				0.0,
				0.0,
				0.0,
				0.0,
				0.0,
				0.0,
				0,0,0,0,
				theColony.queen.GetQueenStrength(),
				0.0,0.0,0.0);
		m_ResultsText.AddTail(CurSize);


		BeginWaitCursor();

		// *************************************************
		// Main Loop for Simulation
		//
		//
		while ((pEvent != NULL) && (DayCount <= TotSimDays)) 
		{

			theColony.ReQueenIfNeeded(
					DayCount,	
					pEvent,
					m_RQEggLayingDelay,
					m_RQWkrDrnRatio,
					m_RQEnableReQueen,
					m_RQScheduled,
					m_RQQueenStrength,
					m_RQOnce,
					m_RQReQueenDate);
			
			// Determine if there is feed available and call theColony.SetFeedingDay(t/f);
			// Alternate approach is to pass the feed dates and quantities to the colony and
			// let the colony keep track - probably a better idea since the pollen is consumed

			theColony.UpdateBees(pEvent, DayCount);
			
			if (IsImmigrationEnabled() && IsImmigrationWindow(pEvent)) 
			{
				m_IncImmigratingMites = (GetImmigrationMites(pEvent));
				m_IncImmigratingMites.SetPctResistant(m_ImmMitePctResistant);
				theColony.AddMites(m_IncImmigratingMites);
			}
			else m_IncImmigratingMites = 0; // Reset to 0 after Immigration Window Closed
			

			theColony.UpdateMites(pEvent, DayCount);

			if (m_CombRemoveEnable && 
				(pEvent->GetTime().GetYear() == m_CombRemoveDate.GetYear()) &&
				(pEvent->GetTime().GetMonth() == m_CombRemoveDate.GetMonth()) &&
				(pEvent->GetTime().GetDay() == m_CombRemoveDate.GetDay())) 
			{
				theColony.RemoveDroneComb(m_CombRemovePct);
				//TRACE("Drone Comb Removed on %s\n",pEvent->GetDateStg());
			}

			theColony.DoPendingEvents(pEvent,DayCount); // Sets colony based on discrete events


			if ((DayCount % ResFreq) == 0 ) // Print once every ResFreq times thru the loop
			{
				double PropMiteDeath = 
					theColony.GetMitesDyingThisPeriod()+theColony.GetTotalMiteCount() > 0?
					theColony.GetMitesDyingThisPeriod()/
					double(theColony.GetMitesDyingThisPeriod()+theColony.GetTotalMiteCount()) : 0;

				double ColPollen = theColony.m_Resources.GetPollenQuantity(); // In Grams
				double ColNectar = theColony.m_Resources.GetNectarQuantity();
				double NectarPesticideConc = theColony.m_Resources.GetNectarPesticideConcentration() * 1000000;
				double PollenPesticideConc = theColony.m_Resources.GetPollenPesticideConcentration() * 1000000;  // convert from g/g to ug/g

				CurSize.Format(m_ResultsFileFormatStg,
					pEvent->GetDateStg("%m/%d/%Y"), 
					theColony.GetColonySize(),
					theColony.Dadl.GetQuantity(),
					theColony.Wadl.GetQuantity(),
					//theColony.foragers.GetQuantity(),
					theColony.foragers.GetActiveQuantity(),
					theColony.CapDrn.GetQuantity(),
					theColony.CapWkr.GetQuantity(),
					theColony.Dlarv.GetQuantity(),
					theColony.Wlarv.GetQuantity(),
					theColony.Deggs.GetQuantity(),
					theColony.Weggs.GetQuantity(),
					theColony.RunMite.GetTotal(),
					theColony.CapDrn.GetMiteCount(),
					theColony.CapWkr.GetMiteCount(),
					theColony.CapDrn.GetMitesPerCell(),
					theColony.CapWkr.GetMitesPerCell(),
					theColony.GetMitesDyingThisPeriod(),
					PropMiteDeath,
					ColPollen,
					PollenPesticideConc,
					ColNectar,
					NectarPesticideConc,
					theColony.m_DeadDroneLarvaePesticide,
					theColony.m_DeadWorkerLarvaePesticide,
					theColony.m_DeadDroneAdultsPesticide,
					theColony.m_DeadWorkerAdultsPesticide,
					theColony.m_DeadForagersPesticide,
					theColony.queen.GetQueenStrength(),
					pEvent->GetTemp(),
					pEvent->GetRainfall());
				m_ResultsText.AddTail(CurSize);
			}
			

			UpdateResults(DayCount,pEvent); 

			if ((DayCount % ResFreq) == 0 ) 
				theColony.SetStartSamplePeriod(); // Get ready for new accumulation period

			DayCount++;

			pEvent->IsForageDay()?TotForagingDays++:TotForagingDays;

			pEvent = m_pWeather->GetNextEvent();
		}
		//delete pEvent;
		m_ResultsReady = true;
		m_SimulationComplete = true;
		if (gl_RunGUI) UpdateAllViews(NULL);
		else
		{
			POSITION pos = GetFirstViewPosition();
			CVarroaPopView* pView = (CVarroaPopView*)GetNextView(pos);
			pView->ChartData(this,gl_RunGUI);
		}

		// If command line switch has /or then save results file
		CString ResultsFileName = ((CVarroaPopApp*)AfxGetApp())->m_OutputResultsFileName;
		if (ResultsFileName.GetLength() > 0) StoreResultsFile(ResultsFileName);

		EndWaitCursor();
		theColony.Clear();
		m_SimulationComplete = false;
	}
}




/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc serialization

void CVarroaPopDoc::Serialize(CArchive& ar)
{
	//  Prior to version 2.1.4, no accommodation was made to read prior file versions.
	//  If the first string read in is 10 asterisks, then we use versioning, otherwise
	//  assume prior to 2.1.4 and first string is the filename.
	//
	//  Valid data file format Versions:
	//
	//		VarroaPop Versions prior to 2.1.4 are data file Version 0
	//		VarroaPop Version 2.1.4 created data file Version 1
	//		VarroaPop Version 2.2.1 created file Version 3.  Adds
	//			data for Fungus control
	//      VarroaPop Version 2.2.2 created file Version 4.  Adds multiple 
	//          mite treatment dates.
	//      VarroaPop Version 2.3.1 created file Version 5.  This causes weather file path + filename
	//          to be stored
	//      VarroaPop Version 3.0.1 created file Version 6.  This added the DateRangeValues for life stage transitions
	//
	//      VarroaPop Version 3.1.2 created file Version 7.  This added a single IEDItem to identify EPA IED Method of pesticide impact
	//
	//		VarroaPop Version 3.1.2 also created file version 8.  This adds EPA user data.
	//      
	//		VarroaPop Version 3.2.2 added created file version 9.  This continues to add the EPA user data.
	//
	//		VarroaPop Version 3.2.3 added version 10. More EPA user data.
	//
	//		VarroaPop Version 3.2.5.3 added version 11.  Added Graphing capability for bees killed by pesticide
	//		
	//		VarroaPop Version 3.2.6.2 added version 13.  Added parameters to track manual scaling for graphs
	//		
	//		VarroaPop Version 3.2.6.2 also added version 14.  Added nectar/pollen direct contamination table
	//
	//		VarroaPop Version 3.2.6.8 added version 15.  This changed Queen Strength from integer to double
	//
	//		VarroaPop Version 3.2.6.9 added version 16.  Added supplemental feeding data
	//
	//		VarroaPop Version 3.2.7.2 added version 17.  Added some overwintering and foraging data.  Also added some spare variables
	//													 to make it easier to add variables in the future without changing file format
	//		VarroaPop Version 3.2.8.0 added version 18.  Allows user to select whether to have notifications or not - selected from view menu.
	//
	//		VarroaPop Version 3.2.8.2 added version 19.  Serializes user selection of whether lack of resources causes colony to die.  Serialized in CColony


#define VERSIONING_VALID "**********"
#define THIS_VERSION 19

	//TRACE("Entering VarroaPopDoc::Serialize\n");
	int bval;
	int FileFormatVersion = THIS_VERSION;
	CString vv = VERSIONING_VALID;
	if (ar.IsStoring())
	{
		ar << vv;				 // Version addition
		ar << THIS_VERSION;		 // Version addition
		// Modified for Version 2
		CString weathername = m_pWeather->GetFileName();
		//ar << SplitPath(m_pWeather->GetFileName(),FNAME) + 
		//	SplitPath(m_pWeather->GetFileName(),EXT);
		ar << m_pWeather->GetFileName();
		ar << m_SimStartTime;
		ar << m_SimEndTime;
		ar << m_ImmigrationType;
		ar << m_TotImmigratingMites.GetTotal();
		ar << int(m_ImmMitePctResistant);
		ar << m_ImmigrationStartDate;
		ar << m_ImmigrationEndDate;
		ar << m_RQEggLayingDelay;
		ar << m_RQWkrDrnRatio;
		ar << m_RQReQueenDate;
		bval = (m_RQEnableReQueen)?0:1;
		ar << bval;
		ar << m_RQScheduled;
		if (FileFormatVersion >= 15)
		{
			ar << m_RQQueenStrength;
		}
		else
		{
			int iRQQS = (int)m_RQQueenStrength;
			ar << iRQQS;
		}
		ar << m_RQOnce;
		ar << m_VTTreatmentDuration;
		ar << m_VTMortality;
		ar << m_VTEnable;
		ar << m_VTTreatmentStart;
		ar << int(m_InitMitePctResistant);
		
		ar << m_SPEnable;
		ar << m_SPTreatmentStart;
		ar << m_SPInitial;
		ar << m_Mort10;
		ar << m_Mort25;
		ar << m_Mort50;
		ar << m_Mort75;
		ar << m_Mort90;

		// EPA IED
		ar << m_IEDItem.m_IEDDate;
		ar << m_IEDItem.m_MortEggs;
		ar << m_IEDItem.m_MortLarvae;
		ar << m_IEDItem.m_MortBrood;
		ar << m_IEDItem.m_MortAdults;
		ar << m_IEDItem.m_MortForagers;
		

		bval = (m_ImmigrationEnabled)?0:1;
		ar << bval;
		ar << (m_TM ? 1:0);
		ar << (m_AD ? 1:0);
		ar << (m_AW ? 1:0);
		ar << (m_CS ? 1:0);
		ar << (m_DB ? 1:0);
		ar << (m_DE ? 1:0);
		ar << (m_DL ? 1:0);
		ar << (m_F  ? 1:0);
		ar << (m_MDB ? 1:0);
		ar << (m_MWB ? 1:0);
		ar << (m_PDB ? 1:0);
		ar << (m_PWB ? 1:0);
		ar << (m_PRM ? 1:0);
		ar << (m_RM ? 1:0);
		ar << (m_WB ? 1:0);
		ar << (m_WE ? 1:0);
		ar << (m_WL ? 1:0);
		ar << (m_IM ? 1:0);
		//  Added with Version 1
		ar << (m_MD ? 1:0);
		ar << (m_PMD ? 1:0);
		// Added with Version 9
		ar << (m_NS ? 1:0);
		ar << (m_PS ? 1:0);
		ar << (m_NPC ? 1:0);
		ar << (m_PPC ? 1:0);
		// Added with Version 11
		ar << (m_DDL ? 1:0);
		ar << (m_DWL ? 1:0);
		ar << (m_DDA ? 1:0);
		ar << (m_DWA ? 1:0);
		ar << (m_DFG ? 1:0);

		// Added with Version 13
		ar << m_AutoScaleChart;
		ar << m_YAxisMax;
		ar << m_YAxisMin;

		// Added with Version 18
		bval = IsShowWarnings() ? 1 : 0;
		ar << bval;

	}
	else
	{
		m_pWeather->ClearAllEvents();
		CString temp;
		int InitMitePctRes;
		int ImmMiteQty;
		ar >> temp;
		// Version 1 addition
		if (temp == VERSIONING_VALID)
		{
			ar >> FileFormatVersion;
			ar >> temp;  // now put the weather file name into "temp"
		}
		ar >> m_SimStartTime;
		SetSimStart(m_SimStartTime);
		ar >> m_SimEndTime;
		SetSimEnd(m_SimEndTime);
		ar >> m_ImmigrationType;
		ar >> ImmMiteQty;   //m_TotImmigratingMites;
		ar >> InitMitePctRes;
		m_TotImmigratingMites = ImmMiteQty;
		m_ImmMitePctResistant = double(InitMitePctRes);
		m_TotImmigratingMites.SetPctResistant(m_ImmMitePctResistant);
		ar >> m_ImmigrationStartDate;
		ar >> m_ImmigrationEndDate;
		ar >> m_RQEggLayingDelay;
		ar >> m_RQWkrDrnRatio;
		ar >> m_RQReQueenDate;
		ar >> bval;
		m_RQEnableReQueen = (bval == 0);
		ar >> m_RQScheduled;
		if (FileFormatVersion >= 15)
		{
			ar >> m_RQQueenStrength;
		}
		else
		{
			int iRQQS;
			ar >> iRQQS;
			m_RQQueenStrength = iRQQS;
		}
		ar >> m_RQOnce;
		ar >> m_VTTreatmentDuration;
		ar >> m_VTMortality;
		ar >> m_VTEnable;
		ar >> m_VTTreatmentStart;
		ar >> InitMitePctRes;
		m_InitMitePctResistant = double(InitMitePctRes);
		if (FileFormatVersion >= 3)
		{
			ar >> m_SPEnable;
			ar >> m_SPTreatmentStart;
			ar >> m_SPInitial;
			ar >> m_Mort10;
			ar >> m_Mort25;
			ar >> m_Mort50;
			ar >> m_Mort75;
			ar >> m_Mort90;
		}
		if (FileFormatVersion >= 7) // EPA IED
		{
			ar >> m_IEDItem.m_IEDDate;
			ar >> m_IEDItem.m_MortEggs;
			ar >> m_IEDItem.m_MortLarvae;
			ar >> m_IEDItem.m_MortBrood;
			ar >> m_IEDItem.m_MortAdults;
			ar >> m_IEDItem.m_MortForagers;
		}


		ar >> bval;
		m_ImmigrationEnabled = (bval == 0);
		ar >> bval;
		m_TM = (bval == 1);
		ar >> bval;
		m_AD = (bval == 1);
		ar >> bval;
		m_AW = (bval == 1);
		ar >> bval;
		m_CS = (bval == 1);
		ar >> bval;
		m_DB = (bval == 1);
		ar >> bval;
		m_DE = (bval == 1);
		ar >> bval;
		m_DL = (bval == 1);
		ar >> bval;
		m_F = (bval == 1);
		ar >> bval;
		m_MDB = (bval == 1);
		ar >> bval;
		m_MWB = (bval == 1);
		ar >> bval;
		m_PDB = (bval == 1);
		ar >> bval;
		m_PWB = (bval == 1);
		ar >> bval;
		m_PRM = (bval == 1);
		ar >> bval;
		m_RM = (bval == 1);
		ar >> bval;
		m_WB = (bval == 1);
		ar >> bval;
		m_WE = (bval == 1);
		ar >> bval;
		m_WL = (bval == 1);
		ar >> bval;
		m_IM = (bval == 1);
		if (FileFormatVersion >= 1)
		{
			ar >> bval;
			m_MD = (bval == 1);
			ar >> bval;
			m_PMD = (bval == 1);
		}
		if (FileFormatVersion >= 9)
		{
			ar >> bval;
			m_NS = (bval == 1);
			ar >> bval;
			m_PS = (bval == 1);
			ar >> bval;
			m_NPC = (bval == 1);
			ar >> bval;
			m_PPC = (bval == 1);
		}
		if (FileFormatVersion >= 11)
		{
			ar >> bval;
			m_DDL = (bval == 1);
			ar >> bval;
			m_DWL = (bval == 1);
			ar >> bval;
			m_DDA = (bval == 1);
			ar >> bval;
			m_DWA = (bval == 1);
			ar >> bval;
			m_DFG = (bval == 1);
		}
		if (FileFormatVersion >= 13)
		{
			ar >> m_AutoScaleChart;
			ar >> m_YAxisMax;
			ar >> m_YAxisMin;

		}

		if (FileFormatVersion >= 18)
		{
			ar >> bval;
			SetShowWarnings(bval == 1);
		}


		// Set Default Path name = SessionFile Path
		CString pathname = ar.GetFile()->GetFilePath();
		SetDefaultPathName(SplitPath(pathname,DRV) + SplitPath(pathname,DIR));


		if ((m_ImmigrationEnabled) && (gl_RunGUI))
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetImmigration(true);
		else if (gl_RunGUI)
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetImmigration(false);

		// Load the weather file associated with the session
		if (temp.GetLength() == 0) // no weather file name
		{
			CString msg = "  This Session has no associated Weather File\n";
			msg += "You will have to specify one before you run a simulation";
			MyMessageBox(msg);
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_WeatherFileName = "";
		}
		else 
		{
			// Early versions store weather filename only
			// If this is the case, add path name and extension info
			if (FileFormatVersion == 0) temp = GetDefaultPathName() + temp + ".wth";

			// Version 1 and 5+ stores entire path of weather file so temp contains path
			if ((FileFormatVersion == 1) || (FileFormatVersion >= 5)) ;  // do nothing

			// Version 2 thru 4 stores filename and extension only so must add Default Path
			if ((FileFormatVersion >= 2) && (FileFormatVersion <= 4))
				temp = GetDefaultPathName() + temp;
			
			// Version 2.3.1 of VarroaPop changed back to store the entire path+filename.
			// Session Files that were created with version 2 - 4 will have the default path name 
			// appended to the file name.  Session files version 5 and up have the entire path name stored
				
				
			m_WeatherFileName = temp;
			m_WeatherLoaded = LoadWeatherFile(m_WeatherFileName); // Try to load using
			if (m_WeatherLoaded)   
			{
				if (gl_RunGUI)
					((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls(); 
			}
			else
			{
				CString msg = "  Reading Session File: The Specified Weather file  ";
				msg += temp+" was not found\n";
				msg += "You will have to specify one before you run a simulation";
				MyMessageBox(msg);
			}

		}

	}

	theColony.Serialize(ar, FileFormatVersion);  // Added colony check for FileFormatVersion
	
	if (FileFormatVersion >= 4)  // Version with multiple mite-treatment dates
	{
		m_MiteTreatments.Serialize(ar);
		//theColony.m_MiteTreatmentInfo.Serialize(ar);
	}

	// After reading .vrp file, update any variables which were changed
	// by the command line input file
	CString InputFileName = ((CVarroaPopApp*)AfxGetApp())->m_InputFileName;
	if ((!ar.IsStoring()) & (InputFileName.GetLength()>0))
	{
		ProcessInputFile(InputFileName);
	}

	if (gl_RunGUI)
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->EnableDialogBar(ReadyToSimulate());

	m_SessionLoaded = true;
	//TRACE("***Leaving VarroaPopDoc::Serialize\n");

}

bool CVarroaPopDoc::LoadWeatherFile(CString WeatherFileName)
{
	
	bool success = false;
	if (m_pWeather == NULL) 
	{
		m_WeatherFileName = "";
	}
	else
	{
		m_WeatherFileName = WeatherFileName;
		success = m_pWeather->LoadWeatherFile(m_WeatherFileName);
		if (success && (gl_RunGUI))
		{
			/////FIX - Do not reset simstart and simend unless new weather file loaded
			//SetSimStart(m_pWeather->GetBeginningTime());
			//SetSimEnd(m_pWeather->GetEndingTime());
			//((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls(); 
		}
	}
	return success;
}




//  ProcessInputFile reads the lines in the command line input file and makes the appropriate
//  changes to internal variables based on the name and the value contained in the line.  The function is called
//  after the .vrp file is loaded.
//
//  Some would not like the use of so many if statements because it requires evaluation of a lot of invalid
//  ifs until a valid if is found.  This seemed the most clear to me since it is only used once per execution of VP
//  and because we are selecting on a string, we can't use a switch statement.  Also, it's pretty clear how to read and add.
//
//  To expose a new variable to the input file, add a new if clause with the lower case version of the name string
//  and update the appropriate internal variables.  
void CVarroaPopDoc::ProcessInputFile(CString FileName)
{
	try
	{
		theColony.m_RQQueenStrengthArray.RemoveAll();
		CString Line;
		CString Name;
		CString Value;
		CStdioFile InputFile(FileName,CFile::shareDenyNone|CFile::modeRead);
		while (InputFile.ReadString(Line))
		{
			int leftchars = Line.Find("=");
			int rightchars = Line.GetLength() - leftchars - 1;
			if ((leftchars <1) || (rightchars <1)) continue; // Go to next line
			Name = Line.Left(leftchars);
			Value = Line.Right(rightchars);
			Name.Trim();
			Value.Trim();
			Name.MakeLower();
			Value.MakeLower();
			//TRACE("Name = %s,  Value = %s\n",Name,Value);

			// switch structure to allocate Name/Value to specific Initial Condition
			if (Name == "weatherfilename")
			{
				//Updated to set m_WeatherLoaded in 3.2.8.16. 
				m_WeatherLoaded = m_pWeather->LoadWeatherFile(Value);
				if (m_WeatherLoaded)
				{
					m_WeatherFileName = Value;
					if (gl_RunGUI)
						((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls(); 
				}
				else
				{
					CString msg = "  Processing Input File: The Specified Weather file  ";
					msg += Value+" was not found\n";
					msg += "You will have to specify one before you run a simulation";
					MyMessageBox(msg);
				}
				continue;
			}
			if (Name == "simstart")
			{
				COleDateTime theDate;
				theDate.ParseDateTime(Value,VAR_DATEVALUEONLY);
				COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
				m_SimStartTime = tempDate;
				SetSimStart(m_SimStartTime);
				//theColony.m_InitCond.m_SimStart = m_SimStartTime.Format("%m/%d/%Y");  // Have sim start in two places - refactor to just VPDoc
				if (gl_RunGUI)
				{
					if (m_WeatherLoaded) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls(); 
				}
				continue;
			}
			if (Name == "simend")
			{
				COleDateTime theDate;
				theDate.ParseDateTime(Value,VAR_DATEVALUEONLY);
				COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
				m_SimEndTime = tempDate;
				SetSimEnd(m_SimEndTime);
				//theColony.m_InitCond.m_SimEnd = m_SimEndTime.Format("%m/%d/%Y");
				if (gl_RunGUI)
				{
					if (m_WeatherLoaded) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls(); 
				}
				continue;
			}
			if (Name == "icdroneadults")
			{
				theColony.m_InitCond.m_droneAdultsField = atoi(Value);
				continue;
			}
			if (Name == "icworkeradults")
			{
				theColony.m_InitCond.m_workerAdultsField = atoi(Value);
				continue;
			}
			if (Name == "icdronebrood")
			{
				theColony.m_InitCond.m_droneBroodField = atoi(Value);
				continue;
			}
			if (Name == "icworkerbrood")
			{
				theColony.m_InitCond.m_workerBroodField = atoi(Value);
				continue;
			}
			if (Name == "icdronelarvae")
			{
				theColony.m_InitCond.m_droneLarvaeField = atoi(Value);
				continue;
			}
			if (Name == "icworkerlarvae")
			{
				theColony.m_InitCond.m_workerLarvaeField = atoi(Value);
				continue;
			}
			if (Name == "icdroneeggs")
			{
				theColony.m_InitCond.m_droneEggsField = atoi(Value);
				continue;
			}
			if (Name == "icworkereggs")
			{
				theColony.m_InitCond.m_workerEggsField = atoi(Value);
				continue;
			}
			if (Name == "icqueenstrength")
			{
				theColony.m_InitCond.m_QueenStrength = atof(Value);
				continue;
			}
			if (Name == "icforagerlifespan")
			{
				theColony.m_InitCond.m_ForagerLifespan = atoi(Value);
				continue;
			}
			if (Name == "icdroneadultinfest")
			{
				theColony.m_InitCond.m_droneAdultInfestField = float(atof(Value));
				continue;
			}
			if (Name == "icdronebroodinfest")
			{
				theColony.m_InitCond.m_droneBroodInfestField = float(atof(Value));
				continue;
			}
			if (Name == "icdronemiteoffspring")
			{
				theColony.m_InitCond.m_droneMiteOffspringField = float(atof(Value));
				continue;
			}
			if (Name == "icdronemitesurvivorship")
			{
				theColony.m_InitCond.m_droneMiteSurvivorshipField = float(atof(Value));
				continue;
			}
			if (Name == "icworkeradultinfest")
			{
				theColony.m_InitCond.m_workerAdultInfestField = float(atof(Value));
				continue;
			}
			if (Name == "icworkerbroodinfest")
			{
				theColony.m_InitCond.m_workerBroodInfestField = float(atof(Value));
				continue;
			}
			if (Name == "icworkermiteoffspring")
			{
				theColony.m_InitCond.m_workerMiteOffspring = float(atof(Value));
				continue;
			}
			if (Name == "icworkermitesurvivorship")
			{
				theColony.m_InitCond.m_workerMiteSurvivorship = float(atof(Value));
				continue;
			}
			if (Name == "plotnone")
			{
				m_AD = FALSE;
				m_AW = FALSE;
				m_CS = FALSE;
				m_DB = FALSE;
				m_DE = FALSE;
				m_DL = FALSE;
				m_F = FALSE;
				m_TM = FALSE;
				m_MDB = FALSE;
				m_MWB = FALSE;
				m_PDB = FALSE;
				m_PWB = FALSE;
				m_PRM = FALSE;
				m_RM = FALSE;
				m_WB = FALSE;
				m_WE = FALSE;
				m_WL = FALSE;
				m_IM = FALSE;
				continue;
			}
			if (Name == "plotad")
			{
				m_AD = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotaw")
			{
				m_AW = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotcs")
			{
				m_CS = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotdb")
			{
				m_DB = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotde")
			{
				m_DE = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotdl")
			{
				m_DL = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotf")
			{
				m_F = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plottm")
			{
				m_TM = (Value=="true")?TRUE:FALSE;
				continue;
			}
			else if (Name == "plotmdb")
			{
				m_MDB = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotmwb")
			{
				m_MWB = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotpdb")
			{
				m_PDB = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotpwb")
			{
				m_PWB = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotprm")
			{
				m_PRM = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotrm")
			{
				m_RM = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotwb")
			{
				m_WB = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotwe")
			{
				m_WE = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "plotwl")
			{
				m_WL = (Value=="true")?TRUE:FALSE;
				continue;
			}
			else if (Name == "plotim")
			{
				m_IM = (Value=="true")?TRUE:FALSE;
				continue;
			}
			if (Name == "denone")
			{
				if (Value=="true") theColony.AddDiscreteEvent("07/01/1999",DE_NONE);
				continue;
			}
			if (Name == "deswarm")
			{
				if (Value=="true") theColony.AddDiscreteEvent("07/01/1999",DE_SWARM);
				continue;
			}
			if (Name == "dechalkbrood")
			{
				if (Value=="true") theColony.AddDiscreteEvent("07/01/1999",DE_CHALKBROOD);				
				continue;
			}
			if (Name == "deresource")
			{
				if (Value=="true") theColony.AddDiscreteEvent("07/01/1999",DE_RESOURCEDEP);				
				continue;
			}
			if (Name == "desupercedure")
			{
				if (Value=="true") theColony.AddDiscreteEvent("07/01/1999",DE_SUPERCEDURE);				
				continue;
			}
			if (Name == "depesticide")
			{
				if (Value=="true") theColony.AddDiscreteEvent("07/01/1999",DE_PESTICIDE);				
				continue;
			}
			if (Name == "immtype")
			{
				m_ImmigrationType = Value;
				continue;
			}
			if (Name == "totalimmmites")
			{
				m_TotImmigratingMites = atoi(Value);
				continue;
			}
			if (Name == "pctimmmitesresistant")
			{
				m_ImmMitePctResistant = atof(Value);
				m_TotImmigratingMites.SetPctResistant(m_ImmMitePctResistant);
				continue;
			}
			if (Name == "immstart")
			{
				COleDateTime theDate;
				theDate.ParseDateTime(Value,VAR_DATEVALUEONLY);
				m_ImmigrationStartDate = theDate;
				continue;
			}
			if (Name == "immend")
			{
				COleDateTime theDate;
				theDate.ParseDateTime(Value,VAR_DATEVALUEONLY);
				m_ImmigrationEndDate = theDate;
				continue;
			}
			if (Name == "immenabled")
			{
				m_ImmigrationEnabled = (Value == "true")?true:false;
				continue;
			}
			if (Name == "rqegglaydelay")
			{
				m_RQEggLayingDelay = UINT(atoi(Value));
				continue;
			}
			if (Name == "rqwkrdrnratio")
			{
				m_RQWkrDrnRatio = atof(Value);
				continue;
			}
			if (Name == "rqrequeendate")
			{
				COleDateTime theDate;
				theDate.ParseDateTime(Value,VAR_DATEVALUEONLY);
				COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
				m_RQReQueenDate = tempDate;
				continue;
			}
			if (Name == "rqenablerequeen")
			{
				m_RQEnableReQueen = (Value == "true")?true:false;	
				continue;
			}
			if (Name == "rqscheduled")
			{
				m_RQScheduled = (Value == "true")?0:1;
				continue;
			}
			if (Name == "rqqueenstrength")
				// This value is placed into an array which is used in Colony::RequeenIfNeeded.  Each occurance of this parameter name in the 
				// input file will result in another requeen strength value being added to the end of the array.
				// Thus rqqueenstrength doesn't overwrite the previous value if supplied more than once
			{
				m_RQQueenStrength = atof(Value);
				theColony.m_RQQueenStrengthArray.Add(m_RQQueenStrength);				
				continue;
			}
			if (Name == "rqonce")
			{
				m_RQOnce = (Value == "true")?0:1;
				continue;
			}
			if (Name == "vttreatmentduration")
			{
				m_VTTreatmentDuration = UINT(atoi(Value));
				continue;
			}
			if (Name == "vtmortality")
			{
				m_VTMortality = UINT(atoi(Value));
				continue;
			}
			if (Name == "vtenable")
			{
				m_VTEnable = (Value == "true")?TRUE:FALSE;				
				continue;
			}
			if (Name == "vttreatmentstart")
			{
				COleDateTime theDate;
				if(theDate.ParseDateTime(Value,VAR_DATEVALUEONLY)) // Verifies this is a valid date
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					m_VTTreatmentStart = tempDate;
				}
				continue;
			}
			if (Name == "initmitepctresistant")
			{
				m_InitMitePctResistant = atof(Value);
				continue;
			}
			if (Name == "ainame")
			{
				// TEST TEST TEST  Be sure pointers all work and no issue with name not found
				//AIItem pItem;
				//AIItem* pEPAItem;
				//pEPAItem = theColony.m_EPAData.GetAIItemPtr(Value);
				//theColony.m_EPAData.SetCurrentAIItem(pEPAItem);
				continue;
			}
			if (Name == "aiadultslope")
			{
				theColony.m_EPAData.m_AI_AdultSlope = atof(Value);
				continue;
			}
			if (Name == "aiadultld50")
			{
				theColony.m_EPAData.m_AI_AdultLD50 = atof(Value);
				continue;
			}
			if (Name == "aiadultslopecontact")
			{
				theColony.m_EPAData.m_AI_AdultSlope_Contact = atof(Value);
				continue;
			}
			if (Name == "aiadultld50contact")
			{
				theColony.m_EPAData.m_AI_AdultLD50_Contact = atof(Value);
				continue;
			}
			if (Name == "ailarvaslope") // note: this was incorrectly "ailarvatslope" in 3.2.5.7
			{
				theColony.m_EPAData.m_AI_LarvaSlope = atof(Value);
				continue;
			}
			if (Name == "ailarvald50")
			{
				theColony.m_EPAData.m_AI_LarvaLD50 = atof(Value);
				continue;
			}
			if (Name == "aikow")
			{
				theColony.m_EPAData.m_AI_KOW = atof(Value);
				continue;
			}
			if (Name == "aikoc")
			{
				theColony.m_EPAData.m_AI_KOC = atof(Value);
				continue;
			}
			if (Name == "aihalflife")
			{
				theColony.m_EPAData.m_AI_HalfLife = atof(Value);
				continue;
			}
			if (Name == "aicontactfactor")
			{
				theColony.m_EPAData.m_AI_ContactFactor = atof(Value);
				continue;
			}
			if (Name == "cl4pollen")
			{
				theColony.m_EPAData.m_C_L4_Pollen = atof(Value);
				continue;
			}
			if (Name == "cl4nectar")
			{
				theColony.m_EPAData.m_C_L4_Nectar = atof(Value);
				continue;
			}
			if (Name == "cl5pollen")
			{
				theColony.m_EPAData.m_C_L5_Pollen = atof(Value);
				continue;
			}
			if (Name == "cl5nectar")
			{
				theColony.m_EPAData.m_C_L5_Nectar = atof(Value);
				continue;
			}
			if (Name == "cldpollen")
			{
				theColony.m_EPAData.m_C_LD_Pollen = atof(Value);
				continue;
			}
			if (Name == "cldnectar")
			{
				theColony.m_EPAData.m_C_LD_Nectar = atof(Value);
				continue;
			}
			if (Name == "ca13pollen")
			{
				theColony.m_EPAData.m_C_A13_Pollen = atof(Value);
				continue;
			}
			if (Name == "ca13nectar")
			{
				theColony.m_EPAData.m_C_A13_Nectar = atof(Value);
				continue;
			}
			if (Name == "ca410pollen")
			{
				theColony.m_EPAData.m_C_A410_Pollen = atof(Value);
				continue;
			}
			if (Name == "ca410nectar")
			{
				theColony.m_EPAData.m_C_A410_Nectar = atof(Value);
				continue;
			}
			if (Name == "ca1120pollen")
			{
				theColony.m_EPAData.m_C_A1120_Pollen = atof(Value);
				continue;
			}
			if (Name == "ca1120nectar")
			{
				theColony.m_EPAData.m_C_A1120_Nectar = atof(Value);
				continue;
			}
			if (Name == "cadpollen")
			{
				theColony.m_EPAData.m_C_AD_Pollen = atof(Value);
				continue;
			}
			if (Name == "cadnectar")
			{
				theColony.m_EPAData.m_C_AD_Nectar = atof(Value);
				continue;
			}
			if (Name == "cforagerpollen")
			{
				theColony.m_EPAData.m_C_Forager_Pollen = atof(Value);
				continue;
			}
			if (Name == "cforagernectar")
			{
				theColony.m_EPAData.m_C_Forager_Nectar = atof(Value);
				continue;
			}
			if (Name == "ipollentrips")
			{
				theColony.m_EPAData.m_I_PollenTrips = (int)atof(Value); // no fail on float input but convert to int
				continue;
			}
			if (Name == "inectartrips")
			{
				theColony.m_EPAData.m_I_NectarTrips = (int)atof(Value);
				continue;
			}
			if (Name == "ipercentnectarforagers")
			{
				theColony.m_EPAData.m_I_PercentNectarForagers = atof(Value);
				continue;
			}
			if (Name == "ipollenload")
			{
				theColony.m_EPAData.m_I_PollenLoad = atof(Value);
				continue;
			}
			if (Name == "inectarload")
			{
				theColony.m_EPAData.m_I_NectarLoad = atof(Value);
				continue;
			}
			if (Name == "foliarenabled")
			{
				theColony.m_EPAData.m_FoliarEnabled = (Value == "true")?true:false;
				continue;
			}
			if (Name == "soilenabled")
			{
				theColony.m_EPAData.m_SoilEnabled = (Value == "true")?true:false;
				continue;
			}
			if (Name == "seedenabled")
			{
				theColony.m_EPAData.m_SeedEnabled = (Value == "true")?true:false;
				continue;
			}
			if (Name == "eapprate")
			{
				theColony.m_EPAData.m_E_AppRate = atof(Value);
				continue;
			}
			if (Name == "esoiltheta")
			{
				theColony.m_EPAData.m_E_SoilTheta = atof(Value);
				continue;
			}
			if (Name == "esoilp")
			{
				theColony.m_EPAData.m_E_SoilP = atof(Value);
				continue;
			}
			if (Name == "esoilfoc")
			{
				theColony.m_EPAData.m_E_SoilFoc = atof(Value);
				continue;
			}
			if (Name == "esoilconcentration")
			{
				theColony.m_EPAData.m_E_SoilConcentration = atof(Value);
				continue;
			}
			if (Name == "eseedconcentration")
			{
				theColony.m_EPAData.m_E_SeedConcentration = atof(Value);
				continue;
			}
			if (Name == "foliarappdate")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_EPAData.m_FoliarAppDate = tempDate;
				}
				continue;
			}
			if (Name == "foliarforagebegin")
			{
				COleDateTime theDate;
				theDate.ParseDateTime(Value,VAR_DATEVALUEONLY);
				COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
				theColony.m_EPAData.m_FoliarForageBegin = tempDate;
				continue;
			}
			if (Name == "foliarforageend")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_EPAData.m_FoliarForageEnd = tempDate;
				}
				continue;
			}
			if (Name == "soilforagebegin")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_EPAData.m_SoilForageBegin = tempDate;
				}
				continue;
			}
			if (Name == "soilforageend")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_EPAData.m_SoilForageEnd = tempDate;
				}
				continue;
			}
			if (Name == "seedforagebegin")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_EPAData.m_SeedForageBegin = tempDate;
				}
				continue;
			}
			if (Name == "seedforageend")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_EPAData.m_SeedForageEnd = tempDate;
				}
				continue;
			}
			if (Name == "necpolfileenable")
			{
				theColony.m_NutrientCT.m_NutrientContEnabled = (Value == "true") ? true : false;
				continue;
			}
			if (Name == "necpolfilename")
			{
				theColony.m_NutrientCT.m_ContaminantFileName = Value;
				continue;
			}
			if (Name == "initcolnectar")
			{
				theColony.m_ColonyNecInitAmount = atof(Value);
				continue;
			}
			if (Name == "initcolpollen")
			{
				theColony.m_ColonyPolInitAmount = atof(Value);
				continue;
			}
			if (Name == "maxcolnectar")
			{
				theColony.m_ColonyNecMaxAmount = atof(Value);
				continue;
			}
			if (Name == "maxcolpollen")
			{
				theColony.m_ColonyPolMaxAmount = atof(Value);
				continue;
			}
			if (Name == "suppollenenable")
			{
				theColony.m_SuppPollenEnabled = (Value == "true") ? true : false;
				continue;
			}
			if (Name == "suppollenamount")
			{
				theColony.m_SuppPollen.m_StartingAmount = atof(Value);
				continue;
			}
			if (Name == "suppollenbegin")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_SuppPollen.m_BeginDate = tempDate;
				}
				continue;
			}
			if (Name == "suppollenend")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_SuppPollen.m_EndDate = tempDate;
				}
				continue;
			}
			if (Name == "supnectarenable")
			{
				theColony.m_SuppNectarEnabled = (Value == "true")?true:false;
				continue;
			}
			if (Name == "supnectaramount")
			{
				theColony.m_SuppNectar.m_StartingAmount = atof(Value);
				continue;
			}
			if (Name == "supnectarbegin")
			{
				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_SuppNectar.m_BeginDate = tempDate;
				}
				continue;
			}
			if (Name == "supnectarend")
			{

				COleDateTime theDate;
				if( theDate.ParseDateTime(Value,VAR_DATEVALUEONLY) ) // Verify this Value is a valid date expression
				{
					COleDateTime tempDate(theDate.GetYear(),theDate.GetMonth(),theDate.GetDay(),0,0,0);
					theColony.m_SuppNectar.m_EndDate = tempDate;
				}
				continue;
			}
			if (Name == "foragermaxprop")
			{
				theColony.foragers.SetPropActualForagers(atof(Value));
				continue;
			}
			if (Name == "needresourcestolive")
			{
				theColony.m_NoResourceKillsColony = (Value == "true") ? true : false;
				continue;

			}
/*
EToLXition=
LToBXition=
BToAXition=
AToFXition=
ALifeSpan=
FLifeSpan=
The Value string for the next several items is in the format StartDate, EndDate, number where commmas are separators in the string
*/
			if (Name == "etolxition")
			{
				if (Value == "clear")
				{
					theColony.m_InitCond.m_EggTransitionDRV.ClearAll();
				}
				else
				{
					int curpos = 0;
					double NumVal = 0.0;
					CString StartDateStg = Value.Tokenize(",", curpos);
					if (StartDateStg.GetLength() > 0) // Was Start Date found?
					{
						CString EndDateStg = Value.Tokenize(",", curpos);
						if (EndDateStg.GetLength() > 0) // Was End Date Found?
						{
							CString NumValStg = Value.Tokenize(",", curpos);
							if (NumValStg.GetLength() > 0) // Was the % of survivors found?
							{
								NumVal = atof(NumValStg); //  Note that if NumValStg cannot be converted, NumVal will be set to 0.0
								theColony.m_InitCond.m_EggTransitionDRV.AddItem(StartDateStg, EndDateStg, NumVal);
							}
						}
					}
				}
				continue;
			}
			if (Name == "ltobxition")
			{
				if (Value == "clear")
				{
					theColony.m_InitCond.m_LarvaeTransitionDRV.ClearAll();
				}
				else
				{
					int curpos = 0;
					double NumVal = 0.0;
					CString StartDateStg = Value.Tokenize(",", curpos);
					if (StartDateStg.GetLength() > 0) // Was Start Date found?
					{
						CString EndDateStg = Value.Tokenize(",", curpos);
						if (EndDateStg.GetLength() > 0) // Was End Date Found?
						{
							CString NumValStg = Value.Tokenize(",", curpos);
							if (NumValStg.GetLength() > 0) // Was the % of survivors found?
							{
								NumVal = atof(NumValStg); //  Note that if NumValStg cannot be converted, NumVal will be set to 0.0
								theColony.m_InitCond.m_LarvaeTransitionDRV.AddItem(StartDateStg, EndDateStg, NumVal);
							}
						}
					}
				}
				continue;
			}
			if (Name == "btoaxition")
			{
				if (Value == "clear")
				{
					theColony.m_InitCond.m_BroodTransitionDRV.ClearAll();
				}
				else
				{
					int curpos = 0;
					double NumVal = 0.0;
					CString StartDateStg = Value.Tokenize(",", curpos);
					if (StartDateStg.GetLength() > 0) // Was Start Date found?
					{
						CString EndDateStg = Value.Tokenize(",", curpos);
						if (EndDateStg.GetLength() > 0) // Was End Date Found?
						{
							CString NumValStg = Value.Tokenize(",", curpos);
							if (NumValStg.GetLength() > 0) // Was the % of survivors found?
							{
								NumVal = atof(NumValStg); //  Note that if NumValStg cannot be converted, NumVal will be set to 0.0
								theColony.m_InitCond.m_BroodTransitionDRV.AddItem(StartDateStg, EndDateStg, NumVal);
							}
						}
					}
				}
				continue;
			}
			if (Name == "atofxition")
			{
				if (Value == "clear")
				{
					theColony.m_InitCond.m_AdultTransitionDRV.ClearAll();
				}
				else
				{
					int curpos = 0;
					double NumVal = 0.0;
					CString StartDateStg = Value.Tokenize(",", curpos);
					if (StartDateStg.GetLength() > 0) // Was Start Date found?
					{
						CString EndDateStg = Value.Tokenize(",", curpos);
						if (EndDateStg.GetLength() > 0) // Was End Date Found?
						{
							CString NumValStg = Value.Tokenize(",", curpos);
							if (NumValStg.GetLength() > 0) // Was the % of survivors found?
							{
								NumVal = atof(NumValStg); //  Note that if NumValStg cannot be converted, NumVal will be set to 0.0
								theColony.m_InitCond.m_AdultTransitionDRV.AddItem(StartDateStg, EndDateStg, NumVal);
							}
						}
					}
				}
				continue;
			}
			if (Name == "alifespan")
			{
				if (Value == "clear")
				{
					theColony.m_InitCond.m_AdultLifespanDRV.ClearAll();
				}
				else
				{
					int curpos = 0;
					double NumVal = 0.0;
					CString StartDateStg = Value.Tokenize(",", curpos);
					if (StartDateStg.GetLength() > 0) // Was Start Date found?
					{
						CString EndDateStg = Value.Tokenize(",", curpos);
						if (EndDateStg.GetLength() > 0) // Was End Date Found?
						{
							CString NumValStg = Value.Tokenize(",", curpos);
							if (NumValStg.GetLength() > 0) // Was the % of survivors found?
							{
								NumVal = atof(NumValStg); //  Note that if NumValStg cannot be converted, NumVal will be set to 0.0
								if ((NumVal >= 7) && (NumVal <= 21))  // Adult bee age constraint
								{
									theColony.m_InitCond.m_AdultLifespanDRV.AddItem(StartDateStg, EndDateStg, NumVal);
								}
							}
						}
					}
				}
				continue;
			}
			if (Name == "flifespan")
			{
				if (Value == "clear")
				{
					theColony.m_InitCond.m_ForagerLifespanDRV.ClearAll();
				}
				else
				{
					int curpos = 0;
					double NumVal = 0.0;
					CString StartDateStg = Value.Tokenize(",", curpos);
					if (StartDateStg.GetLength() > 0) // Was Start Date found?
					{
						CString EndDateStg = Value.Tokenize(",", curpos);
						if (EndDateStg.GetLength() > 0) // Was End Date Found?
						{
							CString NumValStg = Value.Tokenize(",", curpos);
							if (NumValStg.GetLength() > 0) // Was the % of survivors found?
							{
								NumVal = atof(NumValStg); //  Note that if NumValStg cannot be converted, NumVal will be set to 0.0
								if ((NumVal >= 0) && (NumVal <= 20))  // Constraint on forager lifespan
								{
									theColony.m_InitCond.m_ForagerLifespanDRV.AddItem(StartDateStg, EndDateStg, NumVal);
								}
							}
						}
					}
				}
				continue;
			}

			if (Name == "etolxitionen")
			{
				theColony.m_InitCond.m_EggTransitionDRV.SetEnabled(Value == "true");
				continue;
			}

			if (Name == "ltobxitionen")
			{
				theColony.m_InitCond.m_LarvaeTransitionDRV.SetEnabled(Value == "true");
				continue;
			}

			if (Name == "btoaxitionen")
			{
				theColony.m_InitCond.m_BroodTransitionDRV.SetEnabled(Value == "true");
				continue;
			}

			if (Name == "atofxitionen") 
			{
				theColony.m_InitCond.m_AdultTransitionDRV.SetEnabled(Value == "true");
				continue;
			}

			if (Name == "alifespanen")
			{
				theColony.m_InitCond.m_AdultLifespanDRV.SetEnabled(Value == "true");
				continue;
			}

			if (Name == "flifespanen")
			{
				theColony.m_InitCond.m_ForagerLifespanDRV.SetEnabled(Value == "true");
				continue;
			}

			//Fall through - no match
			{
				MyMessageBox("Input File Variable Name "+Name+" not Recognized");
				continue;
			}
		}
	}

	catch (CFileException *e)
	{
		TCHAR stg[255];
		e->GetErrorMessage(stg,255);
		MyMessageBox(CString("Error Pocessing Input File: ") + CString(stg));
	}

}

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc diagnostics

#ifdef _DEBUG
void CVarroaPopDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVarroaPopDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc commands

void CVarroaPopDoc::SetTitle(LPCTSTR lpszTitle) 
{
	
	CDocument::SetTitle(lpszTitle);
}


void CVarroaPopDoc::OnFileNew() 
{
	POSITION pos = GetFirstViewPosition();
	CVarroaPopView* pFirstView = (CVarroaPopView*)GetNextView( pos );

	if (((GetWeather()->IsInitialized())||(GetColony()->IsInitialized()))&&IsModified())
	{
		//  Give opportunity to save old file

		if (MyMessageBox("Save Changes to "+GetPathName()+"?\n",MB_YESNO) == IDYES)
		{
			CFileDialog MyFileDialog(	FALSE,
									".vrp",
									GetPathName(),
									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									_T("VarroaPop Files (*.vrp)|*.vrp|All Files|*.*||"));

			if (MyFileDialog.DoModal() == IDOK) OnFileSaveSession();
		}
	}
	OnNewDocument();

	pFirstView->ShowPropsheet();
	
}


void CVarroaPopDoc::OnSelectGraph() 
{
	// We get here from the toolbar selection
	CSelGraph SelectGraph(this);
	SelectGraph.DoModal();
	int i = 0;

	
}

void CVarroaPopDoc::OnViewPlotdata() 
{
	//  We get here from the menu selection
	OnSelectGraph();
	
}


void CVarroaPopDoc::OnFileSaveResults() 
{
	
	CFileDialog MyFileDialog(	FALSE,
								".res",
								NULL,
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								_T("Simulation Results Files (*.res)|*.res|All Files|*.*||"));

	if (MyFileDialog.DoModal() == IDOK)
	{
		StoreResultsFile(MyFileDialog.GetPathName());
	}
}


void CVarroaPopDoc::StoreResultsFile(CString PathName)
{
	try
	{
		CStdioFile theFile(PathName,CFile::modeCreate|CFile::modeWrite|CFile::typeText);

		// Write VarroaPop Version
		if (m_Version)
		{
			CString titlestg;
			titlestg.Format("Varroa Population Simulation - %s\n",COleDateTime::GetCurrentTime().Format("%b %d,%Y  %I:%M:%S %p"));
			theFile.WriteString(titlestg);
			CString verstg;
			verstg.LoadString(IDS_VPVERSION);
			theFile.WriteString(verstg + "\n\n");
		}
		// Write Header Info to file
		if (m_WeatherColony)
		{
			theFile.WriteString("Weather File: "+GetWeatherFileName() + "\n");
			theFile.WriteString("Colony File:  "+GetColonyFileName() + "\n\n");
		}

		// Write Initial Conditions to file
		if (m_InitConds)
		{
			CString OutStg;
			OutStg.Format("Drone Adult Infestation=%4.2f\n",theColony.m_InitCond.m_droneAdultInfestField);
			theFile.WriteString(OutStg);
			OutStg.Format("Drone Brood Infestation=%4.2f\n",theColony.m_InitCond.m_droneBroodInfestField);
			theFile.WriteString(OutStg);
			OutStg.Format("Drone Mite Survivorship=%4.2f\n",theColony.m_InitCond.m_droneMiteSurvivorshipField);
			theFile.WriteString(OutStg);
			OutStg.Format("Drone Mite Offspring=%4.2f\n",theColony.m_InitCond.m_droneMiteOffspringField);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Adult Infestation=%4.2f\n",theColony.m_InitCond.m_workerAdultInfestField);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Brood Infestation=%4.2f\n",theColony.m_InitCond.m_workerBroodInfestField);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Mite Survivorship=%4.2f\n",theColony.m_InitCond.m_workerMiteSurvivorship);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Mite Offspring=%4.2f\n",theColony.m_InitCond.m_workerMiteOffspring);
			theFile.WriteString(OutStg);
			OutStg.Format("Drone Adults=%d\n",theColony.m_InitCond.m_droneAdultsField);
			theFile.WriteString(OutStg);
			OutStg.Format("Drone Brood=%d\n",theColony.m_InitCond.m_droneBroodField);
			theFile.WriteString(OutStg);
			OutStg.Format("Drone Larvae=%d\n",theColony.m_InitCond.m_droneLarvaeField);
			theFile.WriteString(OutStg);
			OutStg.Format("Drone Eggs=%d\n",theColony.m_InitCond.m_droneEggsField);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Adults=%d\n",theColony.m_InitCond.m_workerAdultsField);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Brood=%d\n",theColony.m_InitCond.m_workerBroodField);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Larvae=%d\n",theColony.m_InitCond.m_workerLarvaeField);
			theFile.WriteString(OutStg);
			OutStg.Format("Worker Eggs=%d\n",theColony.m_InitCond.m_workerEggsField);
			theFile.WriteString(OutStg);
			OutStg.Format("Queen Sperm=%d\n",(int)(theColony.m_InitCond.m_QueenSperm));
			theFile.WriteString(OutStg);
			OutStg.Format("Maximum Eggs per Day=%d\n",(int)(theColony.m_InitCond.m_MaxEggs));
			theFile.WriteString(OutStg);
			OutStg.Format("Forager Lifespan=%d\n",theColony.m_InitCond.m_ForagerLifespan);
			theFile.WriteString(OutStg);

			if (m_ImmigrationEnabled) theFile.WriteString("Immigration Enabled=TRUE\n");
			else  theFile.WriteString("Immigration Enabled=FALSE\n");

			if (m_RQEnableReQueen) theFile.WriteString("Requeening Enabled=TRUE\n");
			else  theFile.WriteString("Requeening Enabled=FALSE\n");

			if (m_VTEnable) theFile.WriteString("Varroa Treatment Enabled=TRUE\n");
			else  theFile.WriteString("Varroa Treatment Enabled=FALSE\n");

			if (m_CombRemoveEnable) theFile.WriteString("Comb Removal Enabled=TRUE\n");
			else  theFile.WriteString("Comb Removal Enabled=FALSE\n");

			theFile.WriteString("\n");

		}



		// Write Column Headers to file
		POSITION pos = m_ResultsFileHeader.GetHeadPosition();
		if (m_ColTitles)
		{
			while (pos!= NULL)
			{
				theFile.WriteString(m_ResultsFileHeader.GetNext(pos) + "\n");
			}
		}

		// Now write results to the file
		pos = m_ResultsText.GetHeadPosition();
		while (pos != NULL)
		{
			theFile.WriteString(m_ResultsText.GetNext(pos) + "\n");
		}
		theFile.Close();
	}
	catch (CFileException* e)
	{
		TCHAR stg[255];
		e->GetErrorMessage(stg,255);
		MyMessageBox(CString("Error Storing Results: ") + CString(stg));
	}

}


int CVarroaPopDoc::GetDocumentLength()
{
	if (m_ResultsText.IsEmpty()) return 0;
	else return(m_ResultsText.GetCount());
}


void CVarroaPopDoc::OnFileSaveSession() 
{
	OnSaveDocument(GetPathName());
}


bool CVarroaPopDoc::CheckDateConsistency(bool ShowWarning)
{
	/*  Checks Dates for Immigration, Varroa Treatment and Re-Queening to verify 
		they fall inside the Simulation range.  If not, a warning message is displayed
		and the user is given the opportunity to continue or quit the simulation 

		Return value:  True if simulation should continue, False otherwise.  User can 
		override consistency check and continue from warning message box.  Otherwise,
		inconsistent times will return false.
	*/

	bool Consistent = true;

	if (ShowWarning)
	{
		CString WarnString = "";
		COleDateTime   ImStart(m_ImmigrationStartDate.GetYear(),
			m_ImmigrationStartDate.GetMonth(),
			m_ImmigrationStartDate.GetDay(), 0, 0, 0);

		COleDateTime     ImEnd(m_ImmigrationEndDate.GetYear(),
			m_ImmigrationEndDate.GetMonth(),
			m_ImmigrationEndDate.GetDay(), 0, 0, 0);

		//  Check all dates of interest.  Flag only if operation enabled

		if (m_RQEnableReQueen && (!DateInRange(m_SimStartTime, m_SimEndTime, m_RQReQueenDate)))
		{
			WarnString += "     ReQueening\n";
			Consistent = false;
		}


		// NOTE:  This block needs to be reworked since we use a list of mite treatment items instead of the single m_VTTreatmentStart.
		//        Fix on c# version.
		/*
		if (m_VTEnable && (!DateInRange(m_SimStartTime, m_SimEndTime, m_VTTreatmentStart)))
		{
			WarnString += "     Varroa Treatment Start\n";
			Consistent = false;
		}

		if (m_VTEnable && (!DateInRange(m_SimStartTime, m_SimEndTime,
			m_VTTreatmentStart + COleDateTimeSpan(m_VTTreatmentDuration*7,0,0,0))))
		{
			WarnString += "     Varroa Treatment End\n";
			Consistent = false;
		}
		*/

		if (m_ImmigrationEnabled && (!DateInRange(m_SimStartTime, m_SimEndTime, ImStart)))
		{
			WarnString += "     Immigration Start\n";
			Consistent = false;
		}

		if (m_ImmigrationEnabled && (!DateInRange(m_SimStartTime, m_SimEndTime, ImEnd)))
		{
			WarnString += "     Immigration End\n";
			Consistent = false;
		}

		// Display warning message if enabled
		if (ShowWarning && !Consistent)
		{
			CString DispString =
				"Warning - The Following Events are Outside the Simulation Period:\n\n";
			DispString += WarnString;
			DispString += "\nPress OK to Continue the Simulation\nPress Cancel to Stop the Simulation";

			Consistent = (MyMessageBox(DispString, MB_OKCANCEL) == IDOK);

		}
	}


	return Consistent;
}



bool CVarroaPopDoc::DateInRange(COleDateTime StartRange, COleDateTime StopRange, COleDateTime theTime)
{
	return ((theTime >= StartRange) && (theTime <= StopRange));

}

//void CVarroaPopDoc::LoadMiteTreatments(CMiteTreatments* theTreatments)
//{
//    if (theTreatments != NULL)
//    {
//        CMiteTreatmentItem theItem;
//        m_MiteTreatments.ClearAll();
//        for (int i = 0; i<theTreatments->GetCount(); i++)
//        {
//            theTreatments->GetItem(i,theItem);
//            m_MiteTreatments.AddItem(theItem);
//        }
//    }
//}

void CVarroaPopDoc::OnViewCombremovaldate() 
{
	CCombRemDlg MyDialog;
	MyDialog.m_RemovalDate = m_CombRemoveDate;
	MyDialog.m_IsEnabled = m_CombRemoveEnable;
	MyDialog.m_Percent = m_CombRemovePct;

	if (MyDialog.DoModal() == IDOK)
	{
		m_CombRemoveDate = MyDialog.m_RemovalDate;
		m_CombRemoveEnable = MyDialog.m_IsEnabled;
		m_CombRemovePct = MyDialog.m_Percent;
	}

	
}


void CVarroaPopDoc::OnUpdateToggleDataFreq(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	//if (!m_DispWeeklyData) pCmdUI->SetText("Weekly Data");
	//else pCmdUI->SetText("Daily Data");
	if (m_DispWeeklyData) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CVarroaPopDoc::OnToggleDataFreq() 
{
	m_DispWeeklyData = !m_DispWeeklyData;
}



void CVarroaPopDoc::OnViewOptions() 
{
	COptions OptDlg(this);

	OptDlg.DoModal();

	
}

void CVarroaPopDoc::OnWeatherCreatenewweatherfile() 
{
	// Creates a new CWeatherEvents and sends it to the weather editor
	// Upon completion of editing, if user closed the editor with OK and 
	// has not yet saved the weather file, prompt for saving and prompt to make
	// this the current weather for the session

	CWeatherEvents* pWeather = new CWeatherEvents;
	CWeatherEditor WeatherEditor(pWeather);
	if (WeatherEditor.DoModal() == IDOK)
	{
		if(WeatherEditor.IsModified())
		{
			// Prompt to set this as the active weather file and prompt to save
			if(AfxMessageBox("Save Weather File ?",MB_YESNO) == IDYES)
			{
				// Save weather
				// TODO - Add file save dialog
				if (AfxMessageBox("Replace current weather with this one ?",MB_YESNO) == IDYES)
				{
					if (AfxIsValidAddress(m_pWeather,sizeof(CWeatherEvents))) delete m_pWeather;
					m_pWeather = pWeather;
				}
				else delete pWeather;
			}
		}
	}
	
}

void CVarroaPopDoc::OnWeatherEditcurrentweatherfile() 
{
	CWeatherEditor WeatherEditor(m_pWeather);
	if (WeatherEditor.DoModal() == IDOK)
	{
		if(WeatherEditor.IsModified())
		{
			// Prompt to save
			// TODO - Add file save dialog
		}
	}
	
}

void CVarroaPopDoc::OnWeatherEditweatherfilefromdisk() 
{
	// Loads a weather file from disk and edits.  After editing, prompts to
	// save and to make current
	CFileDialog fd(	TRUE,
					".wth",
					NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("Weather Files (*.wth)|*.wth|All Files|*.*||"));

	if (fd.DoModal() == IDOK)
	{
		CString FileName = fd.GetPathName();
		CWeatherEvents* pWeather = new CWeatherEvents;
		if (pWeather->LoadWeatherFile(FileName))
		{
			CWeatherEditor WeatherEditor(pWeather);
			if (WeatherEditor.DoModal() == IDOK)
			{
				if (WeatherEditor.IsModified())
				{
					// Prompt to save file
					// TODO - Add File Save dialog
					if (AfxMessageBox("Replace current weather with this one ?",MB_YESNO) == IDYES)
					{
						if (AfxIsValidAddress(m_pWeather,sizeof(CWeatherEvents))) delete m_pWeather;
						m_pWeather = pWeather;
					}
					else delete pWeather;
				}
			}
		}
	}
}



BOOL CVarroaPopDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}



void CVarroaPopDoc::OnUpdateViewShowwarnings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if (IsShowWarnings()) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CVarroaPopDoc::OnToggleShowwarnings()
{
	SetShowWarnings(!IsShowWarnings()); // Toggle
}
