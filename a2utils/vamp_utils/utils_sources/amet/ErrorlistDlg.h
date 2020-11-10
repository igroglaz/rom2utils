#if !defined(AFX_ERRORLISTDLG_H__9EE19AE9_F8FF_11D2_9D56_A953B241741B__INCLUDED_)
#define AFX_ERRORLISTDLG_H__9EE19AE9_F8FF_11D2_9D56_A953B241741B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorlistDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CErrorlistDlg dialog

class CErrorlistDlg : public CDialog
{
// Construction
public:
	CErrorlistDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CErrorlistDlg)
	enum { IDD = IDD_DIALOG_ERRORLIST };
	CListBox	m_Errorlist;
	CString	m_String;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErrorlistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CErrorlistDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonFixall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORLISTDLG_H__9EE19AE9_F8FF_11D2_9D56_A953B241741B__INCLUDED_)
