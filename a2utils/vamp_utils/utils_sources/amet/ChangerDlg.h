// ChangerDlg.h : header file
//
//{{AFX_INCLUDES()
#include "datacombo.h"
//}}AFX_INCLUDES

#if !defined(AFX_CHANGERDLG_H__CA596F07_EED1_11D2_9D55_CEB602A10F3E__INCLUDED_)
#define AFX_CHANGERDLG_H__CA596F07_EED1_11D2_9D55_CEB602A10F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChangerDlg dialog

class CChangerDlg : public CDialog
{
// Construction
public:
	CChangerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChangerDlg)
	enum { IDD = IDD_CHANGER_DIALOG };
	CEdit	m_mgroupcontrol;
	CEdit	m_Yrange2Control;
	CEdit	m_Yrange1Control;
	CEdit	m_Xrange2Control;
	CEdit	m_Xrange1Control;
	CListBox	m_ListTerrain;
	CListBox	m_ObjectListTo;
	CListBox	m_ObjectListFrom;
	UINT	m_Xrange1;
	UINT	m_Xrange2;
	UINT	m_Yrange1;
	UINT	m_Yrange2;
	UINT	m_mgroups;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChangerDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL Check(); // Myself defined function
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeListFrom();
	afx_msg void OnSelchangeListTo();
	afx_msg void OnFileExit();
	afx_msg void OnFileOpen();
	afx_msg void OnHelpAbout();
	afx_msg void OnToolsVerify();
	afx_msg void OnToolsChange();
	afx_msg void OnHelpContents();
	afx_msg void OnToolsPlace();
	afx_msg void OnEditUndo();
	afx_msg void OnToolsRemoveallUnits();
	afx_msg void OnToolsRemoveallObjects();
	afx_msg void OnToolsRemoveallStructures();
	afx_msg void OnEditSelectAllmap();
	afx_msg void OnEditSelectRange();
	afx_msg void OnToolsRemoveobjects();
	afx_msg void OnEditSelectall();
	afx_msg void OnChangeEditXrange1();
	afx_msg void OnChangeEditXrange2();
	afx_msg void OnChangeEditYrange1();
	afx_msg void OnChangeEditYrange2();
	afx_msg void OnToolsDecoratebeach();
	afx_msg void OnToolsPlaceterrain();
	afx_msg void OnToolsPlacespecialtiles();
	afx_msg void OnEditFindmaxgroup();
	afx_msg void OnToolsFlattenroads();
	afx_msg void OnToolsKillduplicateobjects();
	afx_msg void OnToolsGenerateforestHuman();
	afx_msg void OnToolsGenerateforestDruid();
	afx_msg void OnToolsGenerateforestNecro();
	afx_msg void OnToolsGenerateforestCustom();
	afx_msg void OnObjectsDecoratebeachBeach();
	afx_msg void OnObjectsDecoratebeachMountains();
	afx_msg void OnObjectsDecoratebeachCustom();
	afx_msg void OnStructuresRemoveallstructures();
	afx_msg void OnUnitsRemoveallunits();
	afx_msg void OnTilesGeneratelandscape();
	afx_msg void OnTilesSlopeLeftright();
	afx_msg void OnTilesSlopeRightleft();
	afx_msg void OnTilesSlopeTopbottom();
	afx_msg void OnTilesSlopeBottomtop();
	afx_msg void OnObjectsGenerateforestTropic();
	afx_msg void OnFileMakerip();
	afx_msg void OnTilesRandomizealtitudes();
	afx_msg void OnUnitsChangelevelUp();
	afx_msg void OnUnitsChangelevelDown();
	afx_msg void OnObjectsKilloverlaps();
	afx_msg void OnTilesKillduplicatespecialtiles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelpHomepage();
	afx_msg void OnChangelevelTolevel1();
	afx_msg void OnChangelevelTolevel2();
	afx_msg void OnChangelevelTolevel3();
	afx_msg void OnChangelevelTolevel4();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGERDLG_H__CA596F07_EED1_11D2_9D55_CEB602A10F3E__INCLUDED_)
