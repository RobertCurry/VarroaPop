// Worker.h: interface for the CWorker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKER_H__8C6C41B9_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_WORKER_H__8C6C41B9_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Bee.h"

class CWorker : public CBee  {
  public:
	CWorker();
	CWorker(int quantity);
	CWorker(CWorker* oldWorker);
	void Serialize(CArchive &ar);
	virtual ~CWorker();

	CWorker& operator=(const CWorker& theWorker);

};

#endif // !defined(AFX_WORKER_H__8C6C41B9_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
