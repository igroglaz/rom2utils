#if !defined(AFX_CONFIRM_H__856CB501_1FD7_11D3_9D57_E440D94AC25F__INCLUDED_)
#define AFX_CONFIRM_H__856CB501_1FD7_11D3_9D57_E440D94AC25F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Confirm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfirm dialog

class CConfirm : public CDialog
{
// Construction
public:
	CConfirm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfirm)
	enum { IDD = IDD_DIALOG_CONFIRM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfirm)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIRM_H__856CB501_1FD7_11D3_9D57_E440D94AC25F__INCLUDED_)
