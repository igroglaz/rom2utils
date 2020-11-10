// DensityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Changer.h"
#include "DensityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDensityDlg dialog


CDensityDlg::CDensityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDensityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDensityDlg)
	m_Density = 0;
	//}}AFX_DATA_INIT
}


void CDensityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDensityDlg)
	DDX_Control(pDX, IDC_EDIT_DENSITY, m_DensityControl);
	DDX_Control(pDX, IDC_SPIN_DENSITY, m_Spin);
	DDX_Text(pDX, IDC_EDIT_DENSITY, m_Density);
	DDV_MinMaxUInt(pDX, m_Density, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDensityDlg, CDialog)
	//{{AFX_MSG_MAP(CDensityDlg)
	ON_EN_CHANGE(IDC_EDIT_DENSITY, OnChangeEditDensity)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DENSITY, OnDeltaposSpinDensity)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPIN_DENSITY, OnOutofmemorySpinDensity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDensityDlg message handlers

void CDensityDlg::OnChangeEditDensity() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	//	UpdateData(TRUE);
	//	m_Spin.SetPos(m_Density + 65536);
}


void CDensityDlg::OnDeltaposSpinDensity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR; // Up/Down control
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_Spin.SetPos(m_Density + 65536);
	
	if (m_Density != 0 || pNMUpDown -> iDelta >= 0) // bug fix
		m_Density = m_Spin.GetPos() - 65536 + pNMUpDown -> iDelta;
	if (m_Density < 0) m_Density = 0;		// min
	if (m_Density > 100) m_Density = 100;	// max

	UpdateData(FALSE);

	*pResult = 0;
}


void CDensityDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}


void CDensityDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}


BOOL CDensityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Spin.SetBuddy(GetDlgItem(IDC_EDIT_DENSITY)); // Connect buttons to edit-box
	m_Spin.SetRange(0, 100);
	m_DensityControl.SetLimitText(3); // Allow not more than 3 characters
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDensityDlg::OnOutofmemorySpinDensity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	MessageBox("Out of memory");

	*pResult = 0;
}
