// Changer.h : main header file for the CHANGER application
//

#if !defined(AFX_CHANGER_H__CA596F05_EED1_11D2_9D55_CEB602A10F3E__INCLUDED_)
#define AFX_CHANGER_H__CA596F05_EED1_11D2_9D55_CEB602A10F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChangerApp:
// See Changer.cpp for the implementation of this class
//

class CChangerApp : public CWinApp
{
public:
	CChangerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangerApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CChangerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGER_H__CA596F05_EED1_11D2_9D55_CEB602A10F3E__INCLUDED_)
