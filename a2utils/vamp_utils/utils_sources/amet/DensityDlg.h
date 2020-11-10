#if !defined(AFX_DENSITYDLG_H__C8ED0901_F5BC_11D2_9D55_E1A4E08B3518__INCLUDED_)
#define AFX_DENSITYDLG_H__C8ED0901_F5BC_11D2_9D55_E1A4E08B3518__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DensityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDensityDlg dialog

class CDensityDlg : public CDialog
{
// Construction
public:
	CDensityDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDensityDlg)
	enum { IDD = IDD_DIALOG_DENSITY };
	CEdit	m_DensityControl;
	CSpinButtonCtrl	m_Spin;
	UINT	m_Density;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDensityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDensityDlg)
	afx_msg void OnChangeEditDensity();
	afx_msg void OnDeltaposSpinDensity(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnOutofmemorySpinDensity(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DENSITYDLG_H__C8ED0901_F5BC_11D2_9D55_E1A4E08B3518__INCLUDED_)
