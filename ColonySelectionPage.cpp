// ColonySelectionPage.cpp : implementation file
//

#include "stdafx.h"
#include "VarroaPop.h"
#include "ColonySelectionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColonySelectionPage property page

IMPLEMENT_DYNCREATE(CColonySelectionPage, CPropertyPage)

CColonySelectionPage::CColonySelectionPage() : CPropertyPage(CColonySelectionPage::IDD)
{
	m_pMyPropSheet = NULL;
	m_pColonyDataPage = NULL;
	m_pAvailableFilesList = NULL;
	m_pSelectedFilesList = NULL;
	m_pSelectedFileIndex = NULL;
	//{{AFX_DATA_INIT(CColonySelectionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CColonySelectionPage::~CColonySelectionPage()
{
}


void CColonySelectionPage::setColonyDataPage(CPropertyPage* thePage) {
	m_pColonyDataPage = thePage;
}


void CColonySelectionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColonySelectionPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColonySelectionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CColonySelectionPage)
	ON_BN_CLICKED(IDC_SEL_HIDE_BUTTON, OnHideButton)
	ON_BN_CLICKED(IDC_DESELECT_COLONY, OnDeselectColony)
	ON_LBN_DBLCLK(IDC_COLONIES_AVAILABLE, OnDblclkColoniesAvailable)
	ON_LBN_DBLCLK(IDC_COLONIES_TO_SIMULATE, OnDblclkColoniesToSimulate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColonySelectionPage message handlers

void CColonySelectionPage::OnHideButton() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pMyPropSheet);
	m_pMyPropSheet->ShowWindow(SW_HIDE);
}

void CColonySelectionPage::OnDeselectColony() 
{
	CListBox* selectedFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_TO_SIMULATE);
	ASSERT(selectedFiles);

	CString theString;
	int index;

	// Remove the selected item
	index = selectedFiles->GetCurSel();
	if(index != LB_ERR) {
		selectedFiles->GetText(index, theString);
		selectedFiles->DeleteString(index);

		// Also remove it from the SelectedFilesList
		CString element;
		POSITION pos;
		POSITION thisPos;
		for(pos = m_pSelectedFilesList->GetHeadPosition(); pos != NULL;) {
			thisPos = pos;
			element = m_pSelectedFilesList->GetNext(pos);
			if(element == theString) {
				m_pSelectedFilesList->RemoveAt(thisPos);
			}
		}
	}
}

BOOL CColonySelectionPage::OnInitDialog() // executed once!
{
	CPropertyPage::OnInitDialog();

	CListBox* availableFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_AVAILABLE);
	ASSERT(availableFiles);

	CListBox* selectedFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_TO_SIMULATE);
	ASSERT(selectedFiles);

	availableFiles->ResetContent();
	selectedFiles->ResetContent();  // both initially empty
	prepListBoxes();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColonySelectionPage::prepListBoxes() {
	// Prepare contents of ListBoxes according to current 
	// contents of their respective lists of filenames
	CListBox* availableFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_AVAILABLE);
	ASSERT(availableFiles);

	availableFiles->ResetContent();  // clear it
    // Transfer Filenames to ListBox
	if(!m_pAvailableFilesList->IsEmpty()) {
		POSITION pos = m_pAvailableFilesList->GetHeadPosition();
		while(pos) {
			availableFiles->AddString(
					m_pAvailableFilesList->GetNext(pos));
		}
	}

	CListBox* selectedFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_TO_SIMULATE);
	ASSERT(selectedFiles);

	selectedFiles->ResetContent();  // clear it
    // Transfer Filenames to ListBox
	if(!m_pSelectedFilesList->IsEmpty()) {
		POSITION pos = m_pSelectedFilesList->GetHeadPosition();
		while(pos) {
			selectedFiles->AddString(
					m_pSelectedFilesList->GetNext(pos));
		}
		selectedFiles->SetCurSel(*m_pSelectedFileIndex);
	}
}


BOOL CColonySelectionPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	rebuildFileLists();
	prepListBoxes();
	return CPropertyPage::OnSetActive();
}

void CColonySelectionPage::rebuildFileLists() {
	// Find all colony description files and build the availableFilesList
	m_pAvailableFilesList->RemoveAll();
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
		m_pAvailableFilesList->AddTail(name);
	}

	// Make sure selectedFilesList contains no orphans
	POSITION selPos;
	POSITION availPos;
	bool found;
	bool done = false;
	CString selItem;
	CString availItem;

	while (!done) {
		done = true;
		for(selPos = m_pSelectedFilesList->GetHeadPosition(); 
		selPos != NULL; ) {
			selItem = m_pSelectedFilesList->GetNext(selPos);
			found = false;
			for(availPos = m_pAvailableFilesList->GetHeadPosition();
			(availPos != NULL) && (found == false); ) {
				availItem = m_pAvailableFilesList->GetNext(availPos);
				if(availItem == selItem) {
					found = true;
				}
			}
			if(!found) {  // selItem is an orphan
				m_pSelectedFilesList->RemoveAt(
					m_pSelectedFilesList->Find(availItem));
				done = false;
				break;
			}
		}
	}
}

void CColonySelectionPage::OnDblclkColoniesAvailable() 
{
	// TODO: Add your control notification handler code here
	CListBox* availableFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_AVAILABLE);
	ASSERT(availableFiles);
	CListBox* selectedFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_TO_SIMULATE);
	ASSERT(selectedFiles);

	CString theString;
	int index;

	index = availableFiles->GetCurSel();
	if(index != LB_ERR) {
		// If it's not already in the selected list, put it there
		availableFiles->GetText(index, theString);
		if(selectedFiles->FindStringExact(-1,theString)==LB_ERR) {
			selectedFiles->AddString(theString);
		}
		// Add it to the selectedFilesList if it's not already there
		bool found = false;
		CString element;
		POSITION pos;
		for(pos = m_pSelectedFilesList->GetHeadPosition(); pos != NULL;) {
			element = m_pSelectedFilesList->GetNext(pos);
			if(element == theString) found = true;
		}
		if(!found)  m_pSelectedFilesList->AddTail(theString);
	}
}

BOOL CColonySelectionPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CListBox* selectedFiles = 
		(CListBox*) GetDlgItem(IDC_COLONIES_TO_SIMULATE);
	ASSERT(selectedFiles);
	ASSERT(m_pSelectedFilesList);
	ASSERT(m_pSelectedFileIndex);

	int index = selectedFiles->GetCurSel();
	if(index != LB_ERR) {
		*m_pSelectedFileIndex = index;
	}
	else {
		*m_pSelectedFileIndex = -1;
	}

	return CPropertyPage::OnKillActive();
}

void CColonySelectionPage::OnDblclkColoniesToSimulate() 
{
	// TODO: Add your control notification handler code here
	// Make the ColonyDataPage active
	ASSERT(m_pColonyDataPage);

	m_pMyPropSheet->SetActivePage(m_pColonyDataPage);
}
