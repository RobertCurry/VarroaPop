// VPCommandLineInfo.h: interface for the CVPCommandLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VPCOMMANDLINEINFO_H__C892A221_21F7_11D6_B486_00A024E290BA__INCLUDED_)
#define AFX_VPCOMMANDLINEINFO_H__C892A221_21F7_11D6_B486_00A024E290BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVPCommandLineInfo : public CCommandLineInfo  
{
	//  Attributes
public:
	CString CLSessionFileName;
	CString CLInputFileName;
	CString CLOutputGraphFileName;
	CString CLOutputResultsFileName;
	CString CLOutputLogFileName;
	bool CLBackground;

	enum FlagType {	FLNone,
					FLInputFile,
					FLOutputGraph,
					FLOutputResults, 
					FLOutputLog, 
					FLBackground,
					FLInvalid};
	FlagType m_CurFlag;

public:
	CVPCommandLineInfo();
	virtual ~CVPCommandLineInfo();

	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );



};

#endif // !defined(AFX_VPCOMMANDLINEINFO_H__C892A221_21F7_11D6_B486_00A024E290BA__INCLUDED_)
