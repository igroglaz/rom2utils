// Confirm.cpp : implementation file
//

#include "stdafx.h"
#include "Changer.h"
#include "Confirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirm dialog


CConfirm::CConfirm(CWnd* pParent /*=NULL*/)
	: CDialog(CConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfirm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfirm, CDialog)
	//{{AFX_MSG_MAP(CConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirm message handlers

void CConfirm::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfirm::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
