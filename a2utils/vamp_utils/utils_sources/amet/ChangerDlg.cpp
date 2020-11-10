// ChangerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Changer.h"
#include "ChangerDlg.h"
#include "DensityDlg.h"
#include "ErrorlistDlg.h"
#include "Confirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////// Remark this line to compile RELEASE version
//#define BETA
////////////// Do not forget to change bitmap


///////////////////////// BETA limits ///////////////////////////
#ifdef BETA
// Define beta time limits
const UINT FirstBetaMonth = 1;		/////////////////////////////////////
const UINT LastBetaMonth = 2;		//// Must be disabled in release ////
const UINT BetaYear = 2000;			/////////////////////////////////////

// Define beta map limits			// 6 - Support for Map Editor up to 1.7
const UINT LAST_MAP_VERSION = 6;	// 5 - Support for Map Editor up to 1.5
const UINT FIRST_MAP_VERSION = 6;	// 3 - Support for Map Editor up to 1.0

// Define beta "About" info			/////////////////////////////////////
//const char ABOUT_TEXT[] = "About (Tester: Vyacheslav Gordeyev)";
//const char ABOUT_TEXT[] = "About (Tester: Dmitry Nozhnin)";
const char ABOUT_TEXT[] = "About (Tester: Serge Orlovsky)";
//const char ABOUT_TEXT[] = "About AMET (Tester: Yuri Petrov)";

// Define beta Splash parameters
UINT SplashLength = 3;				// seconds, 3 - for beta version, 2 - for release
#endif


///////////////////////// RELEASE limits ///////////////////////////
#ifndef BETA
// Define release map limits		// 6 - Support for Map Editor up to 1.7
const UINT LAST_MAP_VERSION = 6;	// 5 - Support for Map Editor up to 1.5
const UINT FIRST_MAP_VERSION = 5;	// 3 - Support for Map Editor up to 1.0

// Define release "About" info		/////////////////////////////////////
const char ABOUT_TEXT[] = "About";

// Define release Splash parameters
UINT SplashLength = 2;				// seconds, 3 - for beta version, 2 - for release
#endif

/////////////////////////////////////////////////////////////////////

char FileName[500];				// Full name of file
UINT maxX = 0;					// Max X-limit
UINT maxY = 0;					// Max Y-limit
char* int2char(unsigned int, char*);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangerDlg dialog

CChangerDlg::CChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangerDlg)
	m_Xrange1 = 0;
	m_Xrange2 = 0;
	m_Yrange1 = 0;
	m_Yrange2 = 0;
	m_mgroups = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangerDlg)
	DDX_Control(pDX, IDC_EDIT_MGROUP, m_mgroupcontrol);
	DDX_Control(pDX, IDC_EDIT_YRANGE2, m_Yrange2Control);
	DDX_Control(pDX, IDC_EDIT_YRANGE1, m_Yrange1Control);
	DDX_Control(pDX, IDC_EDIT_XRANGE2, m_Xrange2Control);
	DDX_Control(pDX, IDC_EDIT_XRANGE1, m_Xrange1Control);
	DDX_Control(pDX, IDC_LIST_TERRAIN, m_ListTerrain);
	DDX_Control(pDX, IDC_LIST_TO, m_ObjectListTo);
	DDX_Control(pDX, IDC_LIST_FROM, m_ObjectListFrom);
	DDX_Text(pDX, IDC_EDIT_XRANGE1, m_Xrange1);
	DDX_Text(pDX, IDC_EDIT_XRANGE2, m_Xrange2);
	DDX_Text(pDX, IDC_EDIT_YRANGE1, m_Yrange1);
	DDX_Text(pDX, IDC_EDIT_YRANGE2, m_Yrange2);
	DDX_Text(pDX, IDC_EDIT_MGROUP, m_mgroups);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChangerDlg, CDialog)
	//{{AFX_MSG_MAP(CChangerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_LIST_FROM, OnSelchangeListFrom)
	ON_LBN_SELCHANGE(IDC_LIST_TO, OnSelchangeListTo)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_TOOLS_VERIFY, OnToolsVerify)
	ON_COMMAND(ID_TOOLS_CHANGE, OnToolsChange)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(ID_TOOLS_PLACE, OnToolsPlace)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_TOOLS_REMOVEOBJECTS, OnToolsRemoveobjects)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_EN_CHANGE(IDC_EDIT_XRANGE1, OnChangeEditXrange1)
	ON_EN_CHANGE(IDC_EDIT_XRANGE2, OnChangeEditXrange2)
	ON_EN_CHANGE(IDC_EDIT_YRANGE1, OnChangeEditYrange1)
	ON_EN_CHANGE(IDC_EDIT_YRANGE2, OnChangeEditYrange2)
	ON_COMMAND(ID_TOOLS_PLACETERRAIN, OnToolsPlaceterrain)
	ON_COMMAND(ID_TOOLS_PLACESPECIALTILES, OnToolsPlacespecialtiles)
	ON_COMMAND(ID_EDIT_FINDMAXGROUP, OnEditFindmaxgroup)
	ON_COMMAND(ID_TOOLS_FLATTENROADS, OnToolsFlattenroads)
	ON_COMMAND(ID_TOOLS_KILLDUPLICATEOBJECTS, OnToolsKillduplicateobjects)
	ON_COMMAND(ID_TOOLS_GENERATEFOREST_HUMAN, OnToolsGenerateforestHuman)
	ON_COMMAND(ID_TOOLS_GENERATEFOREST_DRUID, OnToolsGenerateforestDruid)
	ON_COMMAND(ID_TOOLS_GENERATEFOREST_NECRO, OnToolsGenerateforestNecro)
	ON_COMMAND(ID_TOOLS_GENERATEFOREST_CUSTOM, OnToolsGenerateforestCustom)
	ON_COMMAND(ID_OBJECTS_DECORATEBEACH_BEACH, OnObjectsDecoratebeachBeach)
	ON_COMMAND(ID_OBJECTS_DECORATEBEACH_MOUNTAINS, OnObjectsDecoratebeachMountains)
	ON_COMMAND(ID_OBJECTS_DECORATEBEACH_CUSTOM, OnObjectsDecoratebeachCustom)
	ON_COMMAND(ID_UNITS_REMOVEALLUNITS, OnUnitsRemoveallunits)
	ON_COMMAND(ID_TILES_GENERATELANDSCAPE, OnTilesGeneratelandscape)
	ON_COMMAND(ID_TILES_SLOPE_LEFTRIGHT, OnTilesSlopeLeftright)
	ON_COMMAND(ID_TILES_SLOPE_RIGHTLEFT, OnTilesSlopeRightleft)
	ON_COMMAND(ID_TILES_SLOPE_TOPBOTTOM, OnTilesSlopeTopbottom)
	ON_COMMAND(ID_TILES_SLOPE_BOTTOMTOP, OnTilesSlopeBottomtop)
	ON_COMMAND(ID_OBJECTS_GENERATEFOREST_TROPIC, OnObjectsGenerateforestTropic)
	ON_COMMAND(ID_FILE_MAKERIP, OnFileMakerip)
	ON_COMMAND(ID_TILES_RANDOMIZEALTITUDES, OnTilesRandomizealtitudes)
	ON_COMMAND(ID_UNITS_CHANGELEVEL_UP, OnUnitsChangelevelUp)
	ON_COMMAND(ID_UNITS_CHANGELEVEL_DOWN, OnUnitsChangelevelDown)
	ON_COMMAND(ID_OBJECTS_KILLOVERLAPS, OnObjectsKilloverlaps)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP_HOMEPAGE, OnHelpHomepage)
	ON_COMMAND(ID_CHANGELEVEL_TOLEVEL1, OnChangelevelTolevel1)
	ON_COMMAND(ID_CHANGELEVEL_TOLEVEL2, OnChangelevelTolevel2)
	ON_COMMAND(ID_CHANGELEVEL_TOLEVEL3, OnChangelevelTolevel3)
	ON_COMMAND(ID_CHANGELEVEL_TOLEVEL4, OnChangelevelTolevel4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangerDlg message handlers

BOOL CChangerDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Update System Menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu -> EnableMenuItem(SC_SIZE, MF_GRAYED);
		pSysMenu -> EnableMenuItem(SC_MAXIMIZE, MF_GRAYED);
		pSysMenu -> AppendMenu(MF_SEPARATOR);
		pSysMenu -> AppendMenu(MF_STRING, IDM_ABOUTBOX, ABOUT_TEXT); // Make "About" in system menu
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	FileName[0] = '\0';	// No file selected

	/// Range edit box limits
	m_Xrange1Control.SetLimitText(3); // Allow not more than 3 characters
	m_Xrange2Control.SetLimitText(3); // Allow not more than 3 characters
	m_Yrange1Control.SetLimitText(3); // Allow not more than 3 characters
	m_Yrange2Control.SetLimitText(3); // Allow not more than 3 characters


///////////////////////////////////////////////////////////////////////////////////
// Beta expired ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#ifdef BETA
	COleDateTime timeNow;
	timeNow = COleDateTime::GetCurrentTime();

	if (timeNow.GetMonth() < FirstBetaMonth || timeNow.GetMonth() > LastBetaMonth || timeNow.GetYear() != BetaYear)
	// April, May 1999
	{
		SetWindowText("AMET: Beta version expired.");
		return TRUE;
	}
#endif
///////////////////////////////////////////////////////////////////////////////////
// Beta expired ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


	// Set items for list "Terrain":

	m_ListTerrain.AddString("ALL"); // (except Rocks and Water)");
	m_ListTerrain.AddString("Cracks");
	m_ListTerrain.AddString("Flowers");
	m_ListTerrain.AddString("Grass");
	m_ListTerrain.AddString("High Rocks");
	m_ListTerrain.AddString("Land");
//	m_ListTerrain.AddString("Roads");
	m_ListTerrain.AddString("Rocky");
	m_ListTerrain.AddString("Sand");
	m_ListTerrain.AddString("Savanna");
	m_ListTerrain.AddString("Water");

//////////////////////////////////////
// Make default selection from list //
//////////////////////////////////////

	m_ListTerrain.SetSel(0, TRUE);	// Initialise list

	// Set objects for list "FROM":

	m_ObjectListFrom.AddString("Bambuk 1");
	m_ObjectListFrom.AddString("Bambuk 2");
	m_ObjectListFrom.AddString("Bambuk 3");

	m_ObjectListFrom.AddString("Big stone 1");
	m_ObjectListFrom.AddString("Big stone 2");
	m_ObjectListFrom.AddString("Big stone 3");

	m_ObjectListFrom.AddString("Bones 1");
	m_ObjectListFrom.AddString("Bones 2");
	m_ObjectListFrom.AddString("Bones 3");
	m_ObjectListFrom.AddString("Bones 4");
	m_ObjectListFrom.AddString("Bones 5");

	m_ObjectListFrom.AddString("Bush 1");
	m_ObjectListFrom.AddString("Bush 1 (dead)");
	m_ObjectListFrom.AddString("Bush 2");
	m_ObjectListFrom.AddString("Bush 2 (dead)");
	m_ObjectListFrom.AddString("Bush 3");
	m_ObjectListFrom.AddString("Bush 3 (dead)");

	m_ObjectListFrom.AddString("Cactus 1");
	m_ObjectListFrom.AddString("Cactus 2");
	m_ObjectListFrom.AddString("Cactus 3");

	m_ObjectListFrom.AddString("CEHO 1");
	m_ObjectListFrom.AddString("CEHO 2");
	m_ObjectListFrom.AddString("CEHO 3");

	m_ObjectListFrom.AddString("Column 1");
	m_ObjectListFrom.AddString("Column 2");

	m_ObjectListFrom.AddString("Dark fence 1");
	m_ObjectListFrom.AddString("Dark fence 2");
	m_ObjectListFrom.AddString("Dark fence 3");

	m_ObjectListFrom.AddString("Dead Bambuk 1");
	m_ObjectListFrom.AddString("Dead Bambuk 2");
	m_ObjectListFrom.AddString("Dead Bambuk 3");

	m_ObjectListFrom.AddString("Dead Cactus 1");
	m_ObjectListFrom.AddString("Dead Cactus 2");
	m_ObjectListFrom.AddString("Dead Cactus 3");

	m_ObjectListFrom.AddString("Dead Draz 1");
	m_ObjectListFrom.AddString("Dead Draz 2");

	m_ObjectListFrom.AddString("Dead Palm 1");
	m_ObjectListFrom.AddString("Dead Palm 2");
	m_ObjectListFrom.AddString("Dead Palm 3");
	m_ObjectListFrom.AddString("Dead Palm 4");

	m_ObjectListFrom.AddString("Dead Tropic Bush 1");
	m_ObjectListFrom.AddString("Dead Tropic Bush 10");
	m_ObjectListFrom.AddString("Dead Tropic Bush 2");
	m_ObjectListFrom.AddString("Dead Tropic Bush 3");
	m_ObjectListFrom.AddString("Dead Tropic Bush 4");
	m_ObjectListFrom.AddString("Dead Tropic Bush 5");
	m_ObjectListFrom.AddString("Dead Tropic Bush 6");
	m_ObjectListFrom.AddString("Dead Tropic Bush 7");
	m_ObjectListFrom.AddString("Dead Tropic Bush 8");
	m_ObjectListFrom.AddString("Dead Tropic Bush 9");
	
	m_ObjectListFrom.AddString("Draz 1");
	m_ObjectListFrom.AddString("Draz 2");

	m_ObjectListFrom.AddString("Druid tree 1");
	m_ObjectListFrom.AddString("Druid tree 1 (dead)");
	m_ObjectListFrom.AddString("Druid tree 2");
	m_ObjectListFrom.AddString("Druid tree 2 (dead)");
	m_ObjectListFrom.AddString("Druid tree 3");
	m_ObjectListFrom.AddString("Druid tree 3 (dead)");

	m_ObjectListFrom.AddString("Fat tree 1");
	m_ObjectListFrom.AddString("Fat tree 1 (dead)");
	m_ObjectListFrom.AddString("Fat tree 2");
	m_ObjectListFrom.AddString("Fat tree 2 (dead)");
	m_ObjectListFrom.AddString("Fat tree 3");
	m_ObjectListFrom.AddString("Fat tree 3 (dead)");

	m_ObjectListFrom.AddString("Fence 1");
	m_ObjectListFrom.AddString("Fence 2");
	m_ObjectListFrom.AddString("Fence 3");
	m_ObjectListFrom.AddString("Fence 4");
	m_ObjectListFrom.AddString("Fence 5");
	m_ObjectListFrom.AddString("Fence 6");
	m_ObjectListFrom.AddString("Fence 7");

	m_ObjectListFrom.AddString("Firewood 1");
	m_ObjectListFrom.AddString("Firewood 2");
	m_ObjectListFrom.AddString("Firewood 3");
	m_ObjectListFrom.AddString("Firewood 4");

	m_ObjectListFrom.AddString("Fur 1");
	m_ObjectListFrom.AddString("Fur 1 (dead)");
	m_ObjectListFrom.AddString("Fur 2");
	m_ObjectListFrom.AddString("Fur 2 (dead)");
	m_ObjectListFrom.AddString("Fur 3");
	m_ObjectListFrom.AddString("Fur 3 (dead)");

	m_ObjectListFrom.AddString("Grave Fence 1");
	m_ObjectListFrom.AddString("Grave Fence 2");
	m_ObjectListFrom.AddString("Grave Fence 3");
	m_ObjectListFrom.AddString("Grave Fence 4");
	m_ObjectListFrom.AddString("Grave Fence 5");
	m_ObjectListFrom.AddString("Grave Fence 6");
	m_ObjectListFrom.AddString("Grave Fence 7");
	m_ObjectListFrom.AddString("Grave Fence 8");

	m_ObjectListFrom.AddString("Iva 1");
	m_ObjectListFrom.AddString("Iva 1 (dead)");
	m_ObjectListFrom.AddString("Iva 2");
	m_ObjectListFrom.AddString("Iva 2 (dead)");
	m_ObjectListFrom.AddString("Iva 3");
	m_ObjectListFrom.AddString("Iva 3 (dead)");

	m_ObjectListFrom.AddString("Maple 1");
	m_ObjectListFrom.AddString("Maple 1 (dead)");
	m_ObjectListFrom.AddString("Maple 2");
	m_ObjectListFrom.AddString("Maple 2 (dead)");
	m_ObjectListFrom.AddString("Maple 3");
	m_ObjectListFrom.AddString("Maple 3 (dead)");

	m_ObjectListFrom.AddString("nuxTa 1");
	m_ObjectListFrom.AddString("nuxTa 1 (dead)");
	m_ObjectListFrom.AddString("nuxTa 2");
	m_ObjectListFrom.AddString("nuxTa 2 (dead)");
	m_ObjectListFrom.AddString("nuxTa 3");
	m_ObjectListFrom.AddString("nuxTa 3 (dead)");

	m_ObjectListFrom.AddString("Oak 1");
	m_ObjectListFrom.AddString("Oak 1 (dead)");
	m_ObjectListFrom.AddString("Oak 2");
	m_ObjectListFrom.AddString("Oak 2 (dead)");
	m_ObjectListFrom.AddString("Oak 3");
	m_ObjectListFrom.AddString("Oak 3 (dead)");

	m_ObjectListFrom.AddString("Palm 1");
	m_ObjectListFrom.AddString("Palm 2");
	m_ObjectListFrom.AddString("Palm 3");
	m_ObjectListFrom.AddString("Palm 4");

	m_ObjectListFrom.AddString("Pine 1");
	m_ObjectListFrom.AddString("Pine 1 (dead)");
	m_ObjectListFrom.AddString("Pine 2");
	m_ObjectListFrom.AddString("Pine 2 (dead)");
	m_ObjectListFrom.AddString("Pine 3");
	m_ObjectListFrom.AddString("Pine 3 (dead)");

	m_ObjectListFrom.AddString("Pointer 1");
	m_ObjectListFrom.AddString("Pointer 2");
	m_ObjectListFrom.AddString("Pointer 3");
	m_ObjectListFrom.AddString("Pointer 4");
	m_ObjectListFrom.AddString("Pointer 5");
	m_ObjectListFrom.AddString("Pointer 6");

	m_ObjectListFrom.AddString("Rock 1");
	m_ObjectListFrom.AddString("Rock 2");
	m_ObjectListFrom.AddString("Rock 3");

	m_ObjectListFrom.AddString("Scull fence 1");
	m_ObjectListFrom.AddString("Scull fence 2");
	m_ObjectListFrom.AddString("Scull fence 3");

	m_ObjectListFrom.AddString("Skulls");
	
	m_ObjectListFrom.AddString("Small Stone 1");
	m_ObjectListFrom.AddString("Small Stone 2");
	m_ObjectListFrom.AddString("Small Stone 3");

	m_ObjectListFrom.AddString("Statue 1");
	m_ObjectListFrom.AddString("Statue 2");

	m_ObjectListFrom.AddString("Stonehenge 1-1");
	m_ObjectListFrom.AddString("Stonehenge 1-2");
	m_ObjectListFrom.AddString("Stonehenge 1-3");
	m_ObjectListFrom.AddString("Stonehenge 2-1");
	m_ObjectListFrom.AddString("Stonehenge 2-2");
	m_ObjectListFrom.AddString("Stonehenge 2-3");

	m_ObjectListFrom.AddString("Strange Device (Palka from Well) 1");
	m_ObjectListFrom.AddString("Strange Device (Palka from Well) 2");

	m_ObjectListFrom.AddString("TA4KA 1");
	m_ObjectListFrom.AddString("TA4KA 2");
	m_ObjectListFrom.AddString("TA4KA 3");

	m_ObjectListFrom.AddString("TeeRose 1");
	m_ObjectListFrom.AddString("TeeRose 1 (dead)");
	m_ObjectListFrom.AddString("TeeRose 2");
	m_ObjectListFrom.AddString("TeeRose 2 (dead)");
	m_ObjectListFrom.AddString("TeeRose 3");
	m_ObjectListFrom.AddString("TeeRose 3 (dead)");

	m_ObjectListFrom.AddString("Totem 1");
	m_ObjectListFrom.AddString("Totem 2");

	m_ObjectListFrom.AddString("Troll Stones 1");
	m_ObjectListFrom.AddString("Troll Stones 2");
	m_ObjectListFrom.AddString("Troll Stones 3");

	m_ObjectListFrom.AddString("Tropic Bush 1");
	m_ObjectListFrom.AddString("Tropic Bush 10");
	m_ObjectListFrom.AddString("Tropic Bush 2");
	m_ObjectListFrom.AddString("Tropic Bush 3");
	m_ObjectListFrom.AddString("Tropic Bush 4");
	m_ObjectListFrom.AddString("Tropic Bush 5");
	m_ObjectListFrom.AddString("Tropic Bush 6");
	m_ObjectListFrom.AddString("Tropic Bush 7");
	m_ObjectListFrom.AddString("Tropic Bush 8");
	m_ObjectListFrom.AddString("Tropic Bush 9");
	
	m_ObjectListFrom.AddString("Tuya 1");
	m_ObjectListFrom.AddString("Tuya 1 (dead)");
	m_ObjectListFrom.AddString("Tuya 2");
	m_ObjectListFrom.AddString("Tuya 2 (dead)");
	m_ObjectListFrom.AddString("Tuya 3");
	m_ObjectListFrom.AddString("Tuya 3 (dead)");

	m_ObjectListFrom.AddString("Vallenwood 1");
	m_ObjectListFrom.AddString("Vallenwood 1 (dead)");
	m_ObjectListFrom.AddString("Vallenwood 2");
	m_ObjectListFrom.AddString("Vallenwood 2 (dead)");
	m_ObjectListFrom.AddString("Vallenwood 3");
	m_ObjectListFrom.AddString("Vallenwood 3 (dead)");

//////////////////////////////////////
// Make default selection from list //
//////////////////////////////////////

//	m_ObjectListFrom.SetSel(0, TRUE);	// Initialise single list

////////////////////////////////
// Set objects for list "TO": //
////////////////////////////////

	m_ObjectListTo.AddString("Bambuk 1");
	m_ObjectListTo.AddString("Bambuk 2");
	m_ObjectListTo.AddString("Bambuk 3");

	m_ObjectListTo.AddString("Big stone 1");
	m_ObjectListTo.AddString("Big stone 2");
	m_ObjectListTo.AddString("Big stone 3");

	m_ObjectListTo.AddString("Bones 1");
	m_ObjectListTo.AddString("Bones 2");
	m_ObjectListTo.AddString("Bones 3");
	m_ObjectListTo.AddString("Bones 4");
	m_ObjectListTo.AddString("Bones 5");

	m_ObjectListTo.AddString("Bush 1");
	m_ObjectListTo.AddString("Bush 1 (dead)");
	m_ObjectListTo.AddString("Bush 2");
	m_ObjectListTo.AddString("Bush 2 (dead)");
	m_ObjectListTo.AddString("Bush 3");
	m_ObjectListTo.AddString("Bush 3 (dead)");

	m_ObjectListTo.AddString("Cactus 1");
	m_ObjectListTo.AddString("Cactus 2");
	m_ObjectListTo.AddString("Cactus 3");

	m_ObjectListTo.AddString("CEHO 1");
	m_ObjectListTo.AddString("CEHO 2");
	m_ObjectListTo.AddString("CEHO 3");

	m_ObjectListTo.AddString("Column 1");
	m_ObjectListTo.AddString("Column 2");

	m_ObjectListTo.AddString("Dark fence 1");
	m_ObjectListTo.AddString("Dark fence 2");
	m_ObjectListTo.AddString("Dark fence 3");

	m_ObjectListTo.AddString("Dead Bambuk 1");
	m_ObjectListTo.AddString("Dead Bambuk 2");
	m_ObjectListTo.AddString("Dead Bambuk 3");

	m_ObjectListTo.AddString("Dead Cactus 1");
	m_ObjectListTo.AddString("Dead Cactus 2");
	m_ObjectListTo.AddString("Dead Cactus 3");

	m_ObjectListTo.AddString("Dead Draz 1");
	m_ObjectListTo.AddString("Dead Draz 2");

	m_ObjectListTo.AddString("Dead Palm 1");
	m_ObjectListTo.AddString("Dead Palm 2");
	m_ObjectListTo.AddString("Dead Palm 3");
	m_ObjectListTo.AddString("Dead Palm 4");

	m_ObjectListTo.AddString("Dead Tropic Bush 1");
	m_ObjectListTo.AddString("Dead Tropic Bush 10");
	m_ObjectListTo.AddString("Dead Tropic Bush 2");
	m_ObjectListTo.AddString("Dead Tropic Bush 3");
	m_ObjectListTo.AddString("Dead Tropic Bush 4");
	m_ObjectListTo.AddString("Dead Tropic Bush 5");
	m_ObjectListTo.AddString("Dead Tropic Bush 6");
	m_ObjectListTo.AddString("Dead Tropic Bush 7");
	m_ObjectListTo.AddString("Dead Tropic Bush 8");
	m_ObjectListTo.AddString("Dead Tropic Bush 9");
	
	m_ObjectListTo.AddString("Draz 1");
	m_ObjectListTo.AddString("Draz 2");

	m_ObjectListTo.AddString("Druid tree 1");
	m_ObjectListTo.AddString("Druid tree 1 (dead)");
	m_ObjectListTo.AddString("Druid tree 2");
	m_ObjectListTo.AddString("Druid tree 2 (dead)");
	m_ObjectListTo.AddString("Druid tree 3");
	m_ObjectListTo.AddString("Druid tree 3 (dead)");

	m_ObjectListTo.AddString("Fat tree 1");
	m_ObjectListTo.AddString("Fat tree 1 (dead)");
	m_ObjectListTo.AddString("Fat tree 2");
	m_ObjectListTo.AddString("Fat tree 2 (dead)");
	m_ObjectListTo.AddString("Fat tree 3");
	m_ObjectListTo.AddString("Fat tree 3 (dead)");

	m_ObjectListTo.AddString("Fence 1");
	m_ObjectListTo.AddString("Fence 2");
	m_ObjectListTo.AddString("Fence 3");
	m_ObjectListTo.AddString("Fence 4");
	m_ObjectListTo.AddString("Fence 5");
	m_ObjectListTo.AddString("Fence 6");
	m_ObjectListTo.AddString("Fence 7");

	m_ObjectListTo.AddString("Firewood 1");
	m_ObjectListTo.AddString("Firewood 2");
	m_ObjectListTo.AddString("Firewood 3");
	m_ObjectListTo.AddString("Firewood 4");

	m_ObjectListTo.AddString("Fur 1");
	m_ObjectListTo.AddString("Fur 1 (dead)");
	m_ObjectListTo.AddString("Fur 2");
	m_ObjectListTo.AddString("Fur 2 (dead)");
	m_ObjectListTo.AddString("Fur 3");
	m_ObjectListTo.AddString("Fur 3 (dead)");

	m_ObjectListTo.AddString("Grave Fence 1");
	m_ObjectListTo.AddString("Grave Fence 2");
	m_ObjectListTo.AddString("Grave Fence 3");
	m_ObjectListTo.AddString("Grave Fence 4");
	m_ObjectListTo.AddString("Grave Fence 5");
	m_ObjectListTo.AddString("Grave Fence 6");
	m_ObjectListTo.AddString("Grave Fence 7");
	m_ObjectListTo.AddString("Grave Fence 8");

	m_ObjectListTo.AddString("Iva 1");
	m_ObjectListTo.AddString("Iva 1 (dead)");
	m_ObjectListTo.AddString("Iva 2");
	m_ObjectListTo.AddString("Iva 2 (dead)");
	m_ObjectListTo.AddString("Iva 3");
	m_ObjectListTo.AddString("Iva 3 (dead)");

	m_ObjectListTo.AddString("Maple 1");
	m_ObjectListTo.AddString("Maple 1 (dead)");
	m_ObjectListTo.AddString("Maple 2");
	m_ObjectListTo.AddString("Maple 2 (dead)");
	m_ObjectListTo.AddString("Maple 3");
	m_ObjectListTo.AddString("Maple 3 (dead)");

	m_ObjectListTo.AddString("nuxTa 1");
	m_ObjectListTo.AddString("nuxTa 1 (dead)");
	m_ObjectListTo.AddString("nuxTa 2");
	m_ObjectListTo.AddString("nuxTa 2 (dead)");
	m_ObjectListTo.AddString("nuxTa 3");
	m_ObjectListTo.AddString("nuxTa 3 (dead)");

	m_ObjectListTo.AddString("Oak 1");
	m_ObjectListTo.AddString("Oak 1 (dead)");
	m_ObjectListTo.AddString("Oak 2");
	m_ObjectListTo.AddString("Oak 2 (dead)");
	m_ObjectListTo.AddString("Oak 3");
	m_ObjectListTo.AddString("Oak 3 (dead)");

	m_ObjectListTo.AddString("Palm 1");
	m_ObjectListTo.AddString("Palm 2");
	m_ObjectListTo.AddString("Palm 3");
	m_ObjectListTo.AddString("Palm 4");

	m_ObjectListTo.AddString("Pine 1");
	m_ObjectListTo.AddString("Pine 1 (dead)");
	m_ObjectListTo.AddString("Pine 2");
	m_ObjectListTo.AddString("Pine 2 (dead)");
	m_ObjectListTo.AddString("Pine 3");
	m_ObjectListTo.AddString("Pine 3 (dead)");

	m_ObjectListTo.AddString("Pointer 1");
	m_ObjectListTo.AddString("Pointer 2");
	m_ObjectListTo.AddString("Pointer 3");
	m_ObjectListTo.AddString("Pointer 4");
	m_ObjectListTo.AddString("Pointer 5");
	m_ObjectListTo.AddString("Pointer 6");

	m_ObjectListTo.AddString("Rock 1");
	m_ObjectListTo.AddString("Rock 2");
	m_ObjectListTo.AddString("Rock 3");

	m_ObjectListTo.AddString("Scull fence 1");
	m_ObjectListTo.AddString("Scull fence 2");
	m_ObjectListTo.AddString("Scull fence 3");

	m_ObjectListTo.AddString("Skulls");
	
	m_ObjectListTo.AddString("Small Stone 1");
	m_ObjectListTo.AddString("Small Stone 2");
	m_ObjectListTo.AddString("Small Stone 3");

	m_ObjectListTo.AddString("Statue 1");
	m_ObjectListTo.AddString("Statue 2");

	m_ObjectListTo.AddString("Stonehenge 1-1");
	m_ObjectListTo.AddString("Stonehenge 1-2");
	m_ObjectListTo.AddString("Stonehenge 1-3");
	m_ObjectListTo.AddString("Stonehenge 2-1");
	m_ObjectListTo.AddString("Stonehenge 2-2");
	m_ObjectListTo.AddString("Stonehenge 2-3");

	m_ObjectListTo.AddString("Strange Device (Palka from Well) 1");
	m_ObjectListTo.AddString("Strange Device (Palka from Well) 2");

	m_ObjectListTo.AddString("TA4KA 1");
	m_ObjectListTo.AddString("TA4KA 2");
	m_ObjectListTo.AddString("TA4KA 3");

	m_ObjectListTo.AddString("TeeRose 1");
	m_ObjectListTo.AddString("TeeRose 1 (dead)");
	m_ObjectListTo.AddString("TeeRose 2");
	m_ObjectListTo.AddString("TeeRose 2 (dead)");
	m_ObjectListTo.AddString("TeeRose 3");
	m_ObjectListTo.AddString("TeeRose 3 (dead)");

	m_ObjectListTo.AddString("Totem 1");
	m_ObjectListTo.AddString("Totem 2");

	m_ObjectListTo.AddString("Troll Stones 1");
	m_ObjectListTo.AddString("Troll Stones 2");
	m_ObjectListTo.AddString("Troll Stones 3");

	m_ObjectListTo.AddString("Tropic Bush 1");
	m_ObjectListTo.AddString("Tropic Bush 10");
	m_ObjectListTo.AddString("Tropic Bush 2");
	m_ObjectListTo.AddString("Tropic Bush 3");
	m_ObjectListTo.AddString("Tropic Bush 4");
	m_ObjectListTo.AddString("Tropic Bush 5");
	m_ObjectListTo.AddString("Tropic Bush 6");
	m_ObjectListTo.AddString("Tropic Bush 7");
	m_ObjectListTo.AddString("Tropic Bush 8");
	m_ObjectListTo.AddString("Tropic Bush 9");
	
	m_ObjectListTo.AddString("Tuya 1");
	m_ObjectListTo.AddString("Tuya 1 (dead)");
	m_ObjectListTo.AddString("Tuya 2");
	m_ObjectListTo.AddString("Tuya 2 (dead)");
	m_ObjectListTo.AddString("Tuya 3");
	m_ObjectListTo.AddString("Tuya 3 (dead)");

	m_ObjectListTo.AddString("Vallenwood 1");
	m_ObjectListTo.AddString("Vallenwood 1 (dead)");
	m_ObjectListTo.AddString("Vallenwood 2");
	m_ObjectListTo.AddString("Vallenwood 2 (dead)");
	m_ObjectListTo.AddString("Vallenwood 3");
	m_ObjectListTo.AddString("Vallenwood 3 (dead)");

//////////////////////////////////////
// Make default selection from list //
//////////////////////////////////////

//	m_ObjectListTo.SetSel(0, TRUE);		// Initialise multiple list

//////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CChangerDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChangerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChangerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChangerDlg::OnSelchangeListFrom() 
{
	// TODO: Add your control notification handler code here
	
}

void CChangerDlg::OnSelchangeListTo() 
{
	// TODO: Add your control notification handler code here
	
}

void CChangerDlg::OnFileExit() 
{
	// TODO: Add your command handler code here
	OnCancel();
}

void CChangerDlg::OnFileOpen() 
{
	// We can make "Open dialog" and find file here

	
///////////////////////////////////////////////////////////////////////////////////
// Beta expired ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#ifdef BETA
	COleDateTime timeNow;
	timeNow = COleDateTime::GetCurrentTime();

	if (timeNow.GetMonth() < FirstBetaMonth || timeNow.GetMonth() > LastBetaMonth || timeNow.GetYear() != BetaYear)
	// April, May 1999
	{
		MessageBox("Beta version expired!");
		return;
	}
#endif
///////////////////////////////////////////////////////////////////////////////////
// Beta expired ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//	char FileTitle[100];
	char PossibleFileName[500]; // Possible file name which may appear of incorrect type
	PossibleFileName[0] = '\0';

	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = NULL;

	ofn.lpstrFilter = TEXT("Map files (*.alm)\0*.alm\0All Files (*.*)\0*.*\0\0");

	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = PossibleFileName;
	ofn.nMaxFile = 500;
//	ofn.lpstrFileTitle = FileTitle;
	ofn.nMaxFileTitle = 99;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Open map file";
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = "ALM";
	ofn.lCustData =  NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	GetOpenFileName(&ofn);
	if (PossibleFileName[0] == '\0') return;


/////////////////////////////////////////
//	We can open and read the file here //
/////////////////////////////////////////	

	CFile f;
	if (f.Open(PossibleFileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	if (FileLength > 0x30) FileLength = 0x30;	// Size limit

	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}

		
	f.Close();

	
	if (FileData[0] != 0x4D || FileData[1] != 0x37 || FileData[2] != 0x52) 
	{
		delete FileData;
		MessageBox("File is not a map");
		return; // Group not found
	}

	if (FileData[0x11] < FIRST_MAP_VERSION)	// Check map for compatibility
	{
		delete FileData;
		MessageBox("Map format is too old. Update your map.");
		return; // Group not found
	}

	if (FileData[0x11] > LAST_MAP_VERSION)	// Check map for compatibility
	{
		delete FileData;
		MessageBox("Map format is not supported. Update your program.");
		return; // Group not found
	}

	
// Initial Range Settings

	maxX = FileData[0x28] + 256 * FileData[0x29];	// Set max possible value
	maxY = FileData[0x2C] + 256 * FileData[0x2D];	// Set max possible value

	m_Xrange1 = 8;
	m_Yrange1 = 8;
	m_Xrange2 = maxX - 9;
	m_Yrange2 = maxY - 5;

	UpdateData(FALSE);
	strcpy(FileName, PossibleFileName); // Possible name becomes a real name
	SetWindowText(FileName);

	CErrorlistDlg dlg_err; // Check map for incorrect objects
	dlg_err.DoModal();

	delete FileData;

	OnEditFindmaxgroup();

/*	if (dlg_err.DoModal() != IDOK)
	{
		FileName[0] = '\0';
		m_Xrange1 = 0;
		m_Yrange1 = 0;
		m_Xrange2 = 0;
		m_Yrange2 = 0;
		UpdateData(FALSE);
		delete FileData;
		return;
	}
*/
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsVerify() 
{
	// TODO: Add your command handler code here
	CErrorlistDlg dlg_err;
	if (dlg_err.DoModal() == IDOK) MessageBox("No errors found!");
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsChange() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	Read information from selected lists "FROM" and "TO"
	
	int MAXSelectedFrom = m_ObjectListFrom.GetSelCount();
	int MAXSelectedTo = m_ObjectListTo.GetSelCount();

	if (MAXSelectedFrom == 0 || MAXSelectedTo == 0) 
	{
		MessageBox("No items selected to change");
		return;
	}
	
	int SelectedFrom[185];
	m_ObjectListFrom.GetSelItems(185, SelectedFrom);
	int SelectedTo[185];
	m_ObjectListTo.GetSelItems(185, SelectedTo);

	if (MAXSelectedFrom == 1 && MAXSelectedTo == 1 && SelectedFrom[0] == SelectedTo[0]) 
	{
		MessageBox("Items for change are equal");
		return;
	}

	unsigned char ObjectCode[185] = {0x5c,0x5d,0x5e,0x16,0x17,0x18,0x4e,0x4f,0x50,0x51,0x52,0x07,0x09,0x31,0x32,0x33,0x34,0x62,0x63,0x64,0x94,0x95,0x96,0x53,0x54,0x56,0x57,0x58,0x5f,0x60,0x61,0x65,0x66,0x67,0x69,0x6b,0x81,0x83,0x85,0x87,0x6d,0x7F,0x6F,0x71,0x73,0x75,0x77,0x79,0x7b,0x7d,0x68,0x6a,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x90,0x91,0x92,0x93,0x0a,0x0c,0x35,0x36,0x37,0x38,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x0d,0x0f,0x39,0x3a,0x3b,0x3c,0x10,0x12,0x3d,0x3e,0x3f,0x40,0xac,0xad,0xae,0xaf,0xb0,0xb1,0x13,0x15,0x41,0x42,0x43,0x44,0x80,0x82,0x84,0x86,0x01,0x03,0x29,0x2a,0x2b,0x2c,0x1f,0x20,0x21,0x22,0x23,0x24,0x19,0x1a,0x1b,0x59,0x5a,0x5b,0x55,0x1c,0x1d,0x1e,0x45,0x46,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0x26,0x25,0xbe,0xbf,0xc0,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0x27,0x28,0x97,0x98,0x99,0x6c,0x7e,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x04,0x06,0x2d,0x2e,0x2f,0x30};
	// HEX codes for objects on a map
	
//	char* ReadMap();
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

	if (FileData[i] == FileData[0x24])
	if (FileData[i+1] == FileData[0x25])
	if (FileData[i+2] == FileData[0x26])
	if (FileData[i+3] == FileData[0x27])
	{
		EndGroup = i - 16;
		break;
	}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =======================
// === Perform changes ===
// =======================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (int k = 0; k < MAXSelectedFrom; k++)	// ObjectListFrom
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (FileData[i] == ObjectCode[SelectedFrom[k]])
		{
			RandomChoice = (float) rand() / RAND_MAX;
			for (int j = 0; j < MAXSelectedTo; j++)
			{
				if (RandomChoice < 1. / MAXSelectedTo * (j + 1))
// new			if ((RandomChoice < 1. / MAXSelectedTo * (j + 1)) && (RandomChoice > 1. / MAXSelectedTo * j))
				{
					for (int jj = 0; jj < MAXSelectedTerrain; jj++)
					switch (SelectedTerrain[jj])
					{
					case 0: //if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = ObjectCode[SelectedTo[j]];
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76) 
							FileData[i] = ObjectCode[SelectedTo[j]];
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6) 
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = ObjectCode[SelectedTo[j]];
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							FileData[i] = ObjectCode[SelectedTo[j]];
					}
					break;
				}
			}
		}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsPlace() 
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	Read information from selected lists "FROM" and "TO"

	unsigned char ObjectCode[185] = {0x5c,0x5d,0x5e,0x16,0x17,0x18,0x4e,0x4f,0x50,0x51,0x52,0x07,0x09,0x31,0x32,0x33,0x34,0x62,0x63,0x64,0x94,0x95,0x96,0x53,0x54,0x56,0x57,0x58,0x5f,0x60,0x61,0x65,0x66,0x67,0x69,0x6b,0x81,0x83,0x85,0x87,0x6d,0x7F,0x6F,0x71,0x73,0x75,0x77,0x79,0x7b,0x7d,0x68,0x6a,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x90,0x91,0x92,0x93,0x0a,0x0c,0x35,0x36,0x37,0x38,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x0d,0x0f,0x39,0x3a,0x3b,0x3c,0x10,0x12,0x3d,0x3e,0x3f,0x40,0xac,0xad,0xae,0xaf,0xb0,0xb1,0x13,0x15,0x41,0x42,0x43,0x44,0x80,0x82,0x84,0x86,0x01,0x03,0x29,0x2a,0x2b,0x2c,0x1f,0x20,0x21,0x22,0x23,0x24,0x19,0x1a,0x1b,0x59,0x5a,0x5b,0x55,0x1c,0x1d,0x1e,0x45,0x46,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0x26,0x25,0xbe,0xbf,0xc0,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0x27,0x28,0x97,0x98,0x99,0x6c,0x7e,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x04,0x06,0x2d,0x2e,0x2f,0x30};
	// HEX codes for objects on a map

	int SelectedTo[185], MAXSelectedTo = m_ObjectListTo.GetSelCount();
	m_ObjectListTo.GetSelItems(185, SelectedTo);

	if (MAXSelectedTo == 0) 
	{
		MessageBox("No items selected to place");
		return;
	}

	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
		RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
		for (int j = 0; j < MAXSelectedTo; j++)
		{
//			RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
//			if (RandomChoice < 1. / MAXSelectedTo * (j + 1))
			if ((RandomChoice < 1. / MAXSelectedTo * (j + 1)) && (RandomChoice > 1. / MAXSelectedTo * j))
			{
				for (int jj = 0; jj < MAXSelectedTerrain; jj++)
				switch (SelectedTerrain[jj])
				{
					case 0: if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76) 
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6) 
							FileData[i] = ObjectCode[SelectedTo[j]];
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76) 
							FileData[i] = ObjectCode[SelectedTo[j]];
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6) 
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = ObjectCode[SelectedTo[j]];
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = ObjectCode[SelectedTo[j]];
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = ObjectCode[SelectedTo[j]];
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36) 
							FileData[i] = ObjectCode[SelectedTo[j]];
				}

/*				if (X1 > m_Xrange1)
				if (MAXSelectedTo == 1)
				{
					if (FileData[i - 1] == FileData[i] && dlg.m_Density < 50) FileData[i] = 0; //Remove duplicates
				}
				else
					if (FileData[i - 1] == FileData[i]) FileData[i] = 0; //Remove duplicates
*/				break;
			}
		}
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void CChangerDlg::OnHelpContents() 
{
	// TODO: Add your command handler code here
//	WinExec("explorer.exe amet.htm", SW_SHOWNORMAL);
	WinExec("hh.exe amet.chm", SW_MAXIMIZE);
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void CChangerDlg::OnEditUndo() 
{
	// TODO: Add your command handler code here
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}
	
	//	We can open and restore the file here
	CFileStatus rStatus; // No use for that, just to check file existance

	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (CFile::GetStatus(BakFileName, rStatus))
	{
		CFile::Remove(FileName);
		CFile::Rename(BakFileName, FileName);
		MessageBox("Backup copy restored!");		
	}
	else MessageBox("Nothing to undo!");
}

//
//
//
//
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsRemoveobjects() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check
	
//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}

	
//	Read information from selected lists "FROM" and "TO"

	unsigned char ObjectCode[185] = {0x5c,0x5d,0x5e,0x16,0x17,0x18,0x4e,0x4f,0x50,0x51,0x52,0x07,0x09,0x31,0x32,0x33,0x34,0x62,0x63,0x64,0x94,0x95,0x96,0x53,0x54,0x56,0x57,0x58,0x5f,0x60,0x61,0x65,0x66,0x67,0x69,0x6b,0x81,0x83,0x85,0x87,0x6d,0x7F,0x6F,0x71,0x73,0x75,0x77,0x79,0x7b,0x7d,0x68,0x6a,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x90,0x91,0x92,0x93,0x0a,0x0c,0x35,0x36,0x37,0x38,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x0d,0x0f,0x39,0x3a,0x3b,0x3c,0x10,0x12,0x3d,0x3e,0x3f,0x40,0xac,0xad,0xae,0xaf,0xb0,0xb1,0x13,0x15,0x41,0x42,0x43,0x44,0x80,0x82,0x84,0x86,0x01,0x03,0x29,0x2a,0x2b,0x2c,0x1f,0x20,0x21,0x22,0x23,0x24,0x19,0x1a,0x1b,0x59,0x5a,0x5b,0x55,0x1c,0x1d,0x1e,0x45,0x46,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0x26,0x25,0xbe,0xbf,0xc0,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0x27,0x28,0x97,0x98,0x99,0x6c,0x7e,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x04,0x06,0x2d,0x2e,0x2f,0x30};
	// HEX codes for objects on a map

	int SelectedFrom[185], MAXSelectedFrom = m_ObjectListFrom.GetSelCount();
	m_ObjectListFrom.GetSelItems(185, SelectedFrom);

	if (MAXSelectedFrom == 0) 
	{
		MessageBox("No items selected to remove");
		return;
	}

	
	//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	

	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

	if (FileData[i] == FileData[0x24])
	if (FileData[i+1] == FileData[0x25])
	if (FileData[i+2] == FileData[0x26])
	if (FileData[i+3] == FileData[0x27])
	{
		EndGroup = i - 16;
		break;
	}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// ======================================
// === Perform Objects delete process ===
// ======================================
	
	UINT X1;

	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		for (int j = 0; j < MAXSelectedFrom; j++)
		{
			if (FileData[i] == ObjectCode[SelectedFrom[j]])
			{
					for (int jj = 0; jj < MAXSelectedTerrain; jj++)
					switch (SelectedTerrain[jj])
					{
					case 0: //if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = 0;
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = 0;
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = 0;
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = 0;
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = 0;
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = 0;
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = 0;
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = 0;
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = 0;
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = 0;
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = 0;
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							FileData[i] = 0;
					}
					break;
			}
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength);	// Write file from memory
	f.Close();
	MessageBox("Objects removed!");

	delete FileData; // Free allocated memory for file data	
}


//////////////////////////////////////////////////////////
void CChangerDlg::OnEditSelectall() 
{
	// TODO: Add your command handler code here

	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}

	m_Xrange1 = 8;
	m_Yrange1 = 8;
	m_Xrange2 = maxX - 9;
	m_Yrange2 = maxY - 5;

	UpdateData(FALSE);
}


void CChangerDlg::OnChangeEditXrange1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);	
}


void CChangerDlg::OnChangeEditXrange2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
}


void CChangerDlg::OnChangeEditYrange1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);	
}


void CChangerDlg::OnChangeEditYrange2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
}


BOOL CChangerDlg::Check()
{
	// TODO: Add your command handler code here

	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return FALSE;
	}


///////////////////////////////////////////////////////////////////////////////////
// Beta expired ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#ifdef BETA
	COleDateTime timeNow;
	timeNow = COleDateTime::GetCurrentTime();

	if (timeNow.GetMonth() < FirstBetaMonth || timeNow.GetMonth() > LastBetaMonth || timeNow.GetYear() != BetaYear)
	// April, May 1999
	{
		MessageBox("Beta version expired!");
		return FALSE;
	}
#endif
///////////////////////////////////////////////////////////////////////////////////
// Beta expired ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


	if (!UpdateData(TRUE)) return FALSE;	// Check if no value is entered

//	CErrorlistDlg dlg_err; // Check map for incorrect objects
//	dlg_err.DoModal();

	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return FALSE;
	}
	
	unsigned long FileLength = (unsigned long) f.GetLength();	// Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return FALSE;
	}

	unsigned char *FileData;

	if (FileLength > 0x12) FileLength = 0x12;	// Size limit
	FileData = new unsigned char [FileLength];	// Allocate memory for a file data
	
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return FALSE;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return FALSE;
	}
	
	f.Close();
	

	if (FileData[0] != 0x4D || FileData[1] != 0x37 || FileData[2] != 0x52) 
	{
		delete FileData;
		MessageBox("File is not a map");
		return FALSE; // Group not found
	}

	if (FileData[0x11] < FIRST_MAP_VERSION)	// Check map for compatibility
	{
		delete FileData;
		MessageBox("Map format is too old. Update your map.");
		return FALSE; // Group not found
	}

	if (FileData[0x11] > LAST_MAP_VERSION)	// Check map for compatibility
	{
		delete FileData;
		MessageBox("Map format is not supported. Update your program.");
		return FALSE; // Group not found
	}


//////// Range check /////////

	BOOL CheckOk = TRUE; // flag of successful check

	if (m_Xrange1 > maxX - 9)
	{
		m_Xrange1 = maxX - 9;
		MessageBox("X1-range value is too high");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}

	if (m_Xrange1 < 8)
	{
		m_Xrange1 = 8;
		MessageBox("X1-range value is too low");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}
///////	
	
	if (m_Yrange1 > maxY - 5)
	{
		m_Yrange1 = maxY - 5;
		MessageBox("Y1-range value is too high");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}
	
	if (m_Yrange1 < 8)
	{
		m_Yrange1 = 8;
		MessageBox("Y1-range value is too low");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}
///////

	if (m_Xrange2 > maxX - 9)
	{
		m_Xrange2 = maxX - 9;
		MessageBox("X2-range value is too high");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}
	
	if (m_Xrange2 < 8)
	{
		m_Xrange2 = 8;
		MessageBox("X2-range value is too low");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}	
///////	

	if (m_Yrange2 > maxY - 5)
	{
		m_Yrange2 = maxY - 5;
		MessageBox("Y2-range value is too high");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}

	if (m_Yrange2 < 8)
	{
		m_Yrange2 = 8;
		MessageBox("Y2-range value is too low");
		UpdateData(FALSE);
		CheckOk = FALSE;
	}
///////

	if (m_Xrange2 < m_Xrange1)
	{
		MessageBox("Incorrect X-range");
		CheckOk = FALSE;
	}

	if (m_Yrange2 < m_Yrange1)
	{
		MessageBox("Incorrect Y-range");
		CheckOk = FALSE;
	}
///////

	if (CheckOk == TRUE) return TRUE;
	else return FALSE;
}

/*
BOOL CheckSelectedTiles(char terrain, char tile)
{

	if (!terrain) return FALSE;
	switch(tile)
	{
		case 0x11:	//
		case 0x11:	//
		case 0x11:	//
		case 0x11:	// Water

		case 0x11:
		case 0x11:
		case 0x11:


	return TRUE;

}*/


//////////////////////////////////////////////////////
void CChangerDlg::OnObjectsDecoratebeachBeach()
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected lists "FROM" and "TO"

	unsigned char ObjectCode[] =
	{
		0x0D,0x0D,0x0D,0x0D,0x0D,						// 5 Iva - 1
		0x39,0x39,0x39,0x39,							// 4 Iva - 2
		0x3B,0x3B,0x3B,0x3B,							// 4 Iva - 3

//		0x07,											// 1 Bush - 1
//		0x31,											// 1 Bush - 2
//		0x33,											// 1 Bush - 3

//		0x70,											// 1 Tropic Bush - 3
//		0x76,											// 1 Tropic Bush - 6
//		0x7A,											// 1 Tropic Bush - 8

		0x0F,											// 1 Dead Iva - 1
		0x3A,											// 1 Dead Iva - 2
		0x3C											// 1 Dead Iva - 3
	};
		
	int OBJECTCODESIZE = sizeof(ObjectCode);
	// HEX codes for objects on a map

	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
			RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
			for (int j = 0; j < OBJECTCODESIZE; j++)
			{
			if ((RandomChoice < 1. / OBJECTCODESIZE * (j + 1)) && (RandomChoice > 1. / OBJECTCODESIZE * j))
			{
				if (FileData[i1] == 2)
				// Check for Water and beach
				FileData[i] = ObjectCode[j];

				if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1 || FileData[i1 - 1] == 0x30 || FileData[i1 - 1] == 0x31 || FileData[i1 - 1] == 0x32 || FileData[i1 - 1] == 0x33 || FileData[i1 - 1] == 0x34 || FileData[i1 - 1] == 0x35 || FileData[i1 - 1] == 0x36))
				// Check for Water and its special tiles only
				FileData[i] = 0;
				break; 
			}
			}
		}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}






///////////////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsPlaceterrain() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}

	if (MAXSelectedTerrain > 1) 
	{
		MessageBox("Too many terrain types selected");
		return;
	}

	if (MAXSelectedTerrain == 1 && SelectedTerrain[0] == 0) 
	{
		MessageBox("Incorrect terrain selected");
		return;
	}

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
	
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
	
		switch (SelectedTerrain[0])
		{
			case 1: // Check for Cracks
					FileData[i1] = 0;
					FileData[i1 - 1] = 0x72;
					break;
			case 2: // Check for Flowers
					FileData[i1] = 1;
					FileData[i1 - 1] = 0x91;
					break; 
			case 3:	// Check for Grass
					FileData[i1] = 0;
					FileData[i1 - 1] = 0x30;
					break; 
			case 4:	// Check for High Rocks
					FileData[i1] = 1;
					FileData[i1 - 1] = 0xF1;
					break; 
			case 5:	// Check for Lands
					FileData[i1] = 0;
					FileData[i1 - 1] = 0x14;
					break; 
			case 6:	// Check for Rocky
					FileData[i1] = 1;
					FileData[i1 - 1] = 0x31;
					break; 
			case 7:	// Check for Sand
					FileData[i1] = 0;
					FileData[i1 - 1] = 0xB1;
					break;
			case 8: // Check for Savanna
					FileData[i1] = 0;
					FileData[i1 - 1] = 0xF1;
					break; 
			case 9:	// Check for Water
					FileData[i1] = 2;
					FileData[i1 - 1] = 0x11;
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(ABOUT_TEXT); // Put "About" info

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CChangerDlg::OnToolsPlacespecialtiles() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}

	if (MAXSelectedTerrain > 1) 
	{
		MessageBox("Too many terrain types selected");
		return;
	}

/*	if (MAXSelectedTerrain == 1 && SelectedTerrain[0] == 0) 
	{
		MessageBox("Incorrect terrain selected");
		return;
	}
*/

	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// ===================================
// === Perform place special tiles ===
// ===================================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	char delta;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
		RandomChoice = (float) rand() / RAND_MAX; // Choose which special tile to place
		delta = (char) (RandomChoice * 6.);
		for (int jj = 0; jj < MAXSelectedTerrain; jj++)
		{
			switch (SelectedTerrain[jj])
			{
			case 0: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51))
					// Check for Cracks
					{
						FileData[i1] = 1;
						if (FileData[i1 - 3] != 0x70 + delta) FileData[i1 - 1] = 0x70 + delta;
						else if (delta > 0) FileData[i1 - 1] = 0x70 + delta - 1;
							else FileData[i1 - 1] = 0x70 + delta + 1;
					}
					if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
					// Check for Grass / Flowers
					{
						FileData[i1] = 1;
						if (FileData[i1 - 3] != 0xB0 + delta) FileData[i1 - 1] = 0xB0 + delta;
						else if (delta > 0) FileData[i1 - 1] = 0xB0 + delta - 1;
							else FileData[i1 - 1] = 0xB0 + delta + 1;
					}
					if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
					// Check for Water
					{
						FileData[i1] = 2;
						if (FileData[i1 - 3] != 0x30 + delta) FileData[i1 - 1] = 0x30 + delta;
						else if (delta > 0) FileData[i1 - 1] = 0x30 + delta - 1;
							else FileData[i1 - 1] = 0x30 + delta + 1;
					}
					break;
			case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
					// Check for Cracks
					{
						FileData[i1] = 1;
						if (FileData[i1 - 3] != 0x70 + delta) FileData[i1 - 1] = 0x70 + delta;
						else if (delta > 0) FileData[i1 - 1] = 0x70 + delta - 1;
							else FileData[i1 - 1] = 0x70 + delta + 1;
					}
					break;
			case 2:
			case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
					// Check for Grass / Flowers
					{
						FileData[i1] = 1;
						if (FileData[i1 - 3] != 0xB0 + delta) FileData[i1 - 1] = 0xB0 + delta;
						else if (delta > 0) FileData[i1 - 1] = 0xB0 + delta - 1;
							else FileData[i1 - 1] = 0xB0 + delta + 1;
					}
					break; 
			case 4:	//if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
					// Check for High Rocks
					break; 
			case 5:	//if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
					// Check for Lands
					break; 
			case 6:	//if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
					// Check for Rocky
					break; 
			case 7:	//if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
					// Check for Sand
					break;
			case 8: //if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
					// Check for Savanna
					break; 
			case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
					// Check for Water
					{
						FileData[i1] = 2;
						if (FileData[i1 - 3] != 0x30 + delta) FileData[i1 - 1] = 0x30 + delta;
						else if (delta > 0) FileData[i1 - 1] = 0x30 + delta - 1;
							else FileData[i1 - 1] = 0x30 + delta + 1;
					}
			}
		}
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}



///////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnEditFindmaxgroup() 
{
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}

	m_mgroups = 0;	// Initial settings of monster groups
	UpdateData(FALSE);
	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	

// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Monsters allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
//		MessageBox("There is no monsters on the map");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}

// =================================
// === Perform Monsters analisys ===
// =================================
	
	UINT MaxGroup = 0, CurrentGroup = 0;

	for (i = StartGroup; i < EndGroup; i++)// += 48)
	{
		if (FileData[i] == 0x80)
		if (FileData[i+4] == 0x80)
//			if (FileData[i+36] == 255)
		if (FileData[i+42] == 0x80)
		{
			CurrentGroup = FileData[i+44] + FileData[i+45] * 256;
			if (MaxGroup < CurrentGroup) MaxGroup = CurrentGroup;
		}
	}

	delete FileData; // Free allocated memory for file data
	
	m_mgroups = MaxGroup;
	UpdateData(FALSE);
}


///////////////////////////////////////////////////
char* int2char(unsigned int a, char* String)

{
  for (char i = 0; i < 4; i++) String[i] = '\0';

  if (a > 999) return String;

  if (a >= 100 && a <= 999)
  {
     String[0] = a / 100 + '0';
     String[1] = a / 10 - a / 100 * 10 + '0';
     String[2] = a - a / 10 * 10 + '0';
     return String;
  }

  if (a >= 10 && a <= 99)
  {
     String[0] = a / 10 + '0';
     String[1] = a - a / 10 * 10 + '0';
     return String;
  }

  if (a <= 9) String[0] = a + '0';
  return String;
}


////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsFlattenroads() 
{
	// TODO: Add your command handler code here
	// Not available in demo version
}


////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsKillduplicateobjects() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check
	
//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}

	
//	Read information from selected lists "FROM" and "TO"

/*	unsigned char ObjectCode[185] = {0x5c,0x5d,0x5e,0x16,0x17,0x18,0x4e,0x4f,0x50,0x51,0x52,0x07,0x09,0x31,0x32,0x33,0x34,0x62,0x63,0x64,0x94,0x95,0x96,0x53,0x54,0x56,0x57,0x58,0x5f,0x60,0x61,0x65,0x66,0x67,0x69,0x6b,0x81,0x83,0x85,0x87,0x6d,0x7F,0x6F,0x71,0x73,0x75,0x77,0x79,0x7b,0x7d,0x68,0x6a,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x90,0x91,0x92,0x93,0x0a,0x0c,0x35,0x36,0x37,0x38,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x0d,0x0f,0x39,0x3a,0x3b,0x3c,0x10,0x12,0x3d,0x3e,0x3f,0x40,0xac,0xad,0xae,0xaf,0xb0,0xb1,0x13,0x15,0x41,0x42,0x43,0x44,0x80,0x82,0x84,0x86,0x01,0x03,0x29,0x2a,0x2b,0x2c,0x1f,0x20,0x21,0x22,0x23,0x24,0x19,0x1a,0x1b,0x59,0x5a,0x5b,0x55,0x1c,0x1d,0x1e,0x45,0x46,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0x26,0x25,0xbe,0xbf,0xc0,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0x27,0x28,0x97,0x98,0x99,0x6c,0x7e,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x04,0x06,0x2d,0x2e,0x2f,0x30};
	// HEX codes for objects on a map

	int SelectedFrom[185], MAXSelectedFrom = m_ObjectListFrom.GetSelCount();
	m_ObjectListFrom.GetSelItems(185, SelectedFrom);

	if (MAXSelectedFrom == 0) 
	{
		MessageBox("No items selected to check");
		return;
	}
*/
	
	//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	

	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

	if (FileData[i] == FileData[0x24])
	if (FileData[i+1] == FileData[0x25])
	if (FileData[i+2] == FileData[0x26])
	if (FileData[i+3] == FileData[0x27])
	{
		EndGroup = i - 16;
		break;
	}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// ======================================
// === Perform Objects delete process ===
// ======================================
	
	UINT X1;

	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		//for (int j = 0; j < MAXSelectedFrom; j++)
		//{
			if (X1 > m_Xrange1 && FileData[i - 1] == FileData[i])
			{
					for (int jj = 0; jj < MAXSelectedTerrain; jj++)
					switch (SelectedTerrain[jj])
					{
					case 0: //if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = 0;
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = 0;
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = 0;
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = 0;
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = 0;
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = 0;
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = 0;
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = 0;
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = 0;
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = 0;
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = 0;
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							FileData[i] = 0;
					}
		//			break;
			}
	//	}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength);	// Write file from memory
	f.Close();
	MessageBox("Duplicate objects removed!");

	delete FileData; // Free allocated memory for file data	
}	


////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsGenerateforestHuman() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


	unsigned char ObjectCode[] = 
	{
		0xB2,0xB2,0xB2,0xB2,0xB2,0xB2,0xB2,					// 7 Teeroses - 1
		0xB4,0xB4,0xB4,0xB4,0xB4,0xB4,0xB4,					// 7 Teeroses - 2
		0xB6,0xB6,0xB6,0xB6,0xB6,0xB6,0xB6,					// 7 Teeroses - 3

		0xAC,0xAC,0xAC,0xAC,0xAC,							// 5 nuxta - 1
		0xAE,0xAE,0xAE,0xAE,0xAE,							// 5 nuxta - 2
		0xB0,0xB0,0xB0,0xB0,0xB0,							// 5 nuxta - 3

		0xB8,0xB8,0xB8,0xB8,0xB8,							// 5 Tuya - 1
		0xBA,0xBA,0xBA,0xBA,0xBA,							// 5 Tuya - 2
		0xBC,0xBC,0xBC,0xBC,0xBC,							// 5 Tuya - 3

		0x07,0x07,0x07,0x07,								// 4 Bush - 1
		0x31,0x31,0x31,0x31,								// 4 Bush - 2
		0x33,0x33,0x33,0x33,								// 4 Bush - 3

		0x10,0x10,0x10,										// 3 Maple - 1
		0x3D,0x3D,0x3D,										// 3 Maple - 2
		0x3F,0x3F,0x3F,										// 3 Maple - 3
		
		0x01,												// 1 Pine - 1
		0x29,												// 1 Pine - 2
		0x2B,												// 1 Pine - 3
		
		0xB9,												// 1 Dead Tuya - 1
		0xBB,												// 1 Dead Tuya - 1
		0xBD,												// 1 Dead Tuya - 1
		
		0x12,												// 1 Dead Maple - 1
		0x3E,												// 1 Dead Maple - 1
		0x40,												// 1 Dead Maple - 1

		0x13,												// 1 Oak - 1
		0x41,												// 1 Oak - 2
		0x43												// 1 Oak - 3
	};
	
	int OBJECTCODESIZE = sizeof(ObjectCode);
	// HEX codes for objects on a map


	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
		RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
		for (int j = 0; j < OBJECTCODESIZE; j++)
		{
			if ((RandomChoice < 1. / OBJECTCODESIZE * (j + 1)) && (RandomChoice > 1. / OBJECTCODESIZE * j))
			{
				for (int jj = 0; jj < MAXSelectedTerrain; jj++)
				switch (SelectedTerrain[jj])
				{
					case 0: if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = ObjectCode[j];
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = ObjectCode[j];
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = ObjectCode[j];
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = ObjectCode[j];
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = ObjectCode[j];
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							FileData[i] = ObjectCode[j];
				}

/*				if (X1 > m_Xrange1)
				if (MAXSelectedTo == 1)
				{
					if (FileData[i - 1] == FileData[i] && dlg.m_Density < 50) FileData[i] = 0; //Remove duplicates
				}
				else
					if (FileData[i - 1] == FileData[i]) FileData[i] = 0; //Remove duplicates
*/				break;
			}
		}
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}


////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsGenerateforestDruid() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


	unsigned char ObjectCode[] = 
	{
		0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,					// 7 Druid Trees - 1
		0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,0xA2,					// 7 Druid Trees - 2
		0xA4,0x04,0x04,0x04,0xA4,0xA4,0xA4,					// 7 Druid Trees - 3

		0x04,0x04,0x04,0x04,0x04,0x04,0x04,					// 7 Wallenwood - 1
		0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,0x2D,					// 7 Wallenwood - 2
		0x2F,0x2F,0x2F,0x2F,0x2F,0x2F,0x2F,					// 7 Wallenwood - 3

		0x01,0x01,0x01,										// 3 Pine - 1
		0x29,0x29,0x29,										// 3 Pine - 2
		0x2B,0x2B,0x2B,										// 3 Pine - 3
		
		0x13,0x13,0x13,										// 3 Oak - 1
		0x41,0x41,0x41,										// 3 Oak - 2
		0x43,0x43,0x43,										// 3 Oak - 3

		0x07,0x07,											// 2 Bush - 1
		0x31,0x31,											// 2 Bush - 2
		0x33,0x33,											// 2 Bush - 3
		
		0x09,												// 1 Dead Bush - 1
		0x32,												// 1 Dead Bush - 2
		0x34,												// 1 Dead Bush - 3

		0x15,												// 1 Dead Oak - 1
		0x42,												// 1 Dead Oak - 2
		0x44,												// 1 Dead Oak - 3
		
		0x03,												// 1 Dead Pine - 1
		0x2A,												// 1 Dead Pine - 2
		0x2C,												// 1 Dead Pine - 3

		0x10,												// 1 Maple - 1
		0x3D,												// 1 Maple - 2
		0x3F,												// 1 Maple - 3
		
		0xA1,												// 1 Dead Druid Tree - 1
		0xA3,												// 1 Dead Druid Tree - 2
		0xA5,												// 1 Dead Druid Tree - 3

		0x06,												// 1 Dead Wallenwood - 1
		0x2E,												// 1 Dead Wallenwood - 1
		0x30,												// 1 Dead Wallenwood - 1
		
		0x0A,												// 1 Fur - 1
		0x35,												// 1 Fur - 2
		0x37												// 1 Fur - 3
	};
	
	int OBJECTCODESIZE = sizeof(ObjectCode);
	// HEX codes for objects on a map


	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
		RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
		for (int j = 0; j < OBJECTCODESIZE; j++)
		{
			if ((RandomChoice < 1. / OBJECTCODESIZE * (j + 1)) && (RandomChoice > 1. / OBJECTCODESIZE * j))
			{
				for (int jj = 0; jj < MAXSelectedTerrain; jj++)
				switch (SelectedTerrain[jj])
				{
					case 0: if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = ObjectCode[j];
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = ObjectCode[j];
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = ObjectCode[j];
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = ObjectCode[j];
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = ObjectCode[j];
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							FileData[i] = ObjectCode[j];
				}

/*				if (X1 > m_Xrange1)
				if (MAXSelectedTo == 1)
				{
					if (FileData[i - 1] == FileData[i] && dlg.m_Density < 50) FileData[i] = 0; //Remove duplicates
				}
				else
					if (FileData[i - 1] == FileData[i]) FileData[i] = 0; //Remove duplicates
*/				break;
			}
		}
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}

///////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnToolsGenerateforestCustom() 
{
	// TODO: Add your command handler code here
	// Not available in light version
}


void CChangerDlg::OnObjectsDecoratebeachMountains() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected lists "FROM" and "TO"

	unsigned char ObjectCode[] = 
	{
		0x0A,						// 1 Fur - 1
		0x35,						// 1 Fur - 2
		0x37						// 1 Fur - 3
	};

	int OBJECTCODESIZE = sizeof(ObjectCode);
	// HEX codes for objects on a map

	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
		RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
		for (int j = 0; j < OBJECTCODESIZE; j++)
		{
			if ((RandomChoice < 1. / OBJECTCODESIZE * (j + 1)) && (RandomChoice > 1. / OBJECTCODESIZE * j))
			{
				if (FileData[i1] == 1)
				// Check for High Rocks and Rocky
				FileData[i] = ObjectCode[j];

				if (FileData[i1] == 1 && (FileData[i1 - 1] == 0xD1 || FileData[i1 - 1] == 0xF1 || FileData[i1 - 1] == 0xF2 || FileData[i1 - 1] == 0xF3 || FileData[i1 - 1] == 0xF4 || FileData[i1 - 1] == 0xF5 || FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x31 || FileData[i1 - 1] == 0x32 || FileData[i1 - 1] == 0x33 || FileData[i1 - 1] == 0x34 || FileData[i1 - 1] == 0x35 || FileData[i1 - 1] == 0x51))
//				if (FileData[i1] == 1 && (FileData[i1 - 1] == 0xD1 || (FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5)))
//				if (FileData[i1] == 1 && (FileData[i1 - 1] == 0xD1 || (FileData[i1 - 1] <= 0xF5)))
				// Check for High Rocks only
				FileData[i] = 0;
				else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
				FileData[i] = 0;
				else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
				FileData[i] = 0;
				break; 
			}
		}
		}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnObjectsDecoratebeachCustom() 
{
	// TODO: Add your command handler code here
	
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnStructuresRemoveallstructures() 
{
	// TODO: Add your command handler code here
	
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnUnitsRemoveallunits() 
{
	// TODO: Add your command handler code here
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}


	CConfirm dlg_confirm;
	if (dlg_confirm.DoModal() != IDOK) //MessageBox("Operation cancelled");
	return;

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Monsters allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
//			StartGroup = i + 8;
			StartGroup = i - 12;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("There is no monsters on the map");
		return; // Group not found
	}

/*
	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}
*/
// ===============================
// === Perform Monsters delete ===
// ===============================

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, StartGroup); // Write file from memory
	f.Close();

	delete FileData; // Free allocated memory for file data
	MessageBox("Done!");	
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnTilesGeneratelandscape() 
{
	// TODO: Add your command handler code here
	
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnTilesSlopeLeftright() 
{
	// TODO: Add your command handler code here
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}
	
//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;

//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 2)	// Group number of Altitudes allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}
	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


// =======================
// === Perform changes ===
// =======================
	
	UINT j; 
	int DeltaAltitude = 0;
	unsigned long AverageAltitude = 0;
	
	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
		AverageAltitude += FileData[i];
	AverageAltitude /= j;

	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
	{
		if (j > maxX) j = 1;
		DeltaAltitude = j * 127 / maxX - AverageAltitude;
		if (FileData[i] + DeltaAltitude < 0) FileData[i] = 0;
		else if (FileData[i] + DeltaAltitude < 128)	FileData[i] += DeltaAltitude;
		else FileData[i] = 127;
	}



	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnTilesSlopeRightleft() 
{
	// TODO: Add your command handler code here
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}

//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;

//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 2)	// Group number of Altitudes allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}
	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


// =======================
// === Perform changes ===
// =======================
	
	UINT j;
	int DeltaAltitude = 0;
	unsigned long AverageAltitude = 0;
	
	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
		AverageAltitude += FileData[i];
	AverageAltitude /= j;

	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
	{
		if (j > maxX) j = 1;
		DeltaAltitude = 127 - (j * 127 / maxX) - AverageAltitude;
		if (FileData[i] + DeltaAltitude < 0) FileData[i] = 0;
		else if (FileData[i] + DeltaAltitude < 128)	FileData[i] += DeltaAltitude;
		else FileData[i] = 127;
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnTilesSlopeTopbottom() 
{
	// TODO: Add your command handler code here
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}
	
//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;

//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 2)	// Group number of Altitudes allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}
	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


// =======================
// === Perform changes ===
// =======================
	
	int j, DeltaAltitude = 0;
	unsigned long AverageAltitude = 0;
	
	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
		AverageAltitude += FileData[i];
	AverageAltitude /= j;

	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
	{
		DeltaAltitude = (maxY - j / maxX) * 127 / maxY - AverageAltitude;
		if (FileData[i] + DeltaAltitude < 0) FileData[i] = 0;
		else if (FileData[i] + DeltaAltitude < 128) FileData[i] += DeltaAltitude;
		else FileData[i] = 127;
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnTilesSlopeBottomtop() 
{
	// TODO: Add your command handler code here
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}
	
//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;

//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 2)	// Group number of Altitudes allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}
	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}


// =======================
// === Perform changes ===
// =======================
	
	int j, DeltaAltitude = 0;
	unsigned long AverageAltitude = 0;
	
	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
		AverageAltitude += FileData[i];
	AverageAltitude /= j;

	for (j = 1, i = StartGroup; i < EndGroup; i++, j++)
	{
		DeltaAltitude = 127 - ((maxY - j / maxX) * 127 / maxY) - AverageAltitude;
		if (FileData[i] + DeltaAltitude < 0) FileData[i] = 0;
		else if (FileData[i] + DeltaAltitude < 128)	FileData[i] += DeltaAltitude;
		else FileData[i] = 127;
	}



	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnObjectsGenerateforestTropic() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


	unsigned char ObjectCode[] = 
	{
		0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,0x5C,	// 20 Bambuk - 1
		0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,	// 20 Bambuk - 2
		0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,	// 20 Bambuk - 3
		
		0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,	// 12 Cactus - 1
		0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,	// 12 Cactus - 2
		0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x64,	// 12 Cactus - 3

		0x80,0x80,0x80,0x80,0x80,0x80,									// 6 Palm - 1
		0x82,0x82,0x82,0x82,0x82,0x82,									// 6 Palm - 2

		0x68,0x68,0x68,0x68,											// 4 Draz - 1
		0x6A,0x6A,0x6A,0x6A,											// 4 Draz - 2
		
		0x84,0x84,0x84,													// 3 Palm - 3
		0x86,0x86,0x86,													// 3 Palm - 4

		0x6C,0x6C,														// 2 Tropic Bush - 1
		0x6E,0x6E,														// 2 Tropic Bush - 2
		0x70,															// 1 Tropic Bush - 3
		0x72,0x72,														// 2 Tropic Bush - 4
		0x74,0x74,														// 2 Tropic Bush - 5
		0x76,															// 1 Tropic Bush - 6
		0x78,0x78,														// 2 Tropic Bush - 7
		0x7A,															// 1 Tropic Bush - 8
		0x7C,															// 1 Tropic Bush - 9
		0x7E,															// 1 Tropic Bush - 10

		0x5F,															// 1 Dead Bambuk - 1
		0x60,															// 1 Dead Bambuk - 2
		0x61,															// 1 Dead Bambuk - 3

		0x65,															// 1 Dead Cactus - 1
		0x66,															// 1 Dead Cactus - 2
		0x67,															// 1 Dead Cactus - 3

		0x69,															// 1 Dead Draz - 1
		0x6B,															// 1 Dead Draz - 2

		0x81,															// 1 Dead Palm - 1
		0x83,															// 1 Dead Palm - 2
		0x85,															// 1 Dead Palm - 3
		0x87															// 1 Dead Palm - 4
	};
	
	int OBJECTCODESIZE = sizeof(ObjectCode);
	// HEX codes for objects on a map


	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
		RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
		for (int j = 0; j < OBJECTCODESIZE; j++)
		{
			if ((RandomChoice < 1. / OBJECTCODESIZE * (j + 1)) && (RandomChoice > 1. / OBJECTCODESIZE * j))
			{
				for (int jj = 0; jj < MAXSelectedTerrain; jj++)
				switch (SelectedTerrain[jj])
				{
					case 0: if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = ObjectCode[j];
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = ObjectCode[j];
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = ObjectCode[j];
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = ObjectCode[j];
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = ObjectCode[j];
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							FileData[i] = ObjectCode[j];
				}

/*				if (X1 > m_Xrange1)
				if (MAXSelectedTo == 1)
				{
					if (FileData[i - 1] == FileData[i] && dlg.m_Density < 50) FileData[i] = 0; //Remove duplicates
				}
				else
					if (FileData[i - 1] == FileData[i]) FileData[i] = 0; //Remove duplicates
*/				break;
			}
		}
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}


///////////////////////////////////////////////////////////////
void CChangerDlg::OnFileMakerip() 
{
	// TODO: Add your command handler code here
	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		return;
	}


	CConfirm dlg_confirm;
	if (dlg_confirm.DoModal() != IDOK) //MessageBox("Operation cancelled");
	return;

//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Monsters allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
//		MessageBox("There is no monsters on the map");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}

// ===============================
// === Perform Monsters delete ===
// ===============================

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, EndGroup); // Write file from memory
	f.Close();

	delete FileData; // Free allocated memory for file data
	MessageBox("Done!");		
}


//////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnTilesRandomizealtitudes() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 2)	// Group number of Altitudes allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

	if (FileData[i] == FileData[0x24])
	if (FileData[i+1] == FileData[0x25])
	if (FileData[i+2] == FileData[0x26])
	if (FileData[i+3] == FileData[0x27])
	{
		EndGroup = i - 16;
		break;
	}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =======================
// === Perform changes ===
// =======================
	
	srand((unsigned) time(NULL));
	int RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		{
			RandomChoice = 0;
			for (int jj = 0; jj < MAXSelectedTerrain; jj++)
			switch (SelectedTerrain[jj])
				{
					case 0: if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							{
								RandomChoice = rand() * 32 / RAND_MAX - 16;
								if (FileData[i] + RandomChoice < 128) FileData[i] += RandomChoice;
							}
				}
		}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnUnitsChangelevelUp() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Units allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("No monsters found");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}



// =======================
// === Perform changes ===
// =======================
	for (i = StartGroup; i < EndGroup - 46; i++)	
	{
		i1 =  StartTiles + 1 + 2 * (FileData[i + 1] + maxX * FileData[i + 5]); // Find tile for unit
		for (int jj = 0; jj < MAXSelectedTerrain; jj++)
			switch (SelectedTerrain[jj])
				{
					case 0: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 1: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							}
							break;
					case 2:
					case 3:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							}
							break; 
					case 4:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 5:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 6:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 7:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 8: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 9:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] += 1;
									FileData[i + 10] += 1;
									FileData[i + 20] += 1;
									FileData[i + 32] = 0x0A;
								}
							}
				}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnUnitsChangelevelDown() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Units allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("No monsters found");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}



// =======================
// === Perform changes ===
// =======================
	for (i = StartGroup; i < EndGroup - 46; i++)	
	{
		i1 =  StartTiles + 1 + 2 * (FileData[i + 1] + maxX * FileData[i + 5]); // Find tile for unit
		for (int jj = 0; jj < MAXSelectedTerrain; jj++)
			switch (SelectedTerrain[jj])
				{
					case 0: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 1: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							}
							break;
					case 2:
					case 3:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							}
							break; 
					case 4:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 5:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 6:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 7:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 8: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 9:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] -= 1;
									FileData[i + 10] -= 1;
									FileData[i + 20] -= 1;
									FileData[i + 32] = 0x0A;
								}
							}
				}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


//////////////////////////////////////////////////////////////////////////////
void CChangerDlg::OnObjectsKilloverlaps() 
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Units allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("No monsters found");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}



// =======================
// === Perform changes ===
// =======================
	for (i = StartGroup; i < EndGroup - 46; i++)	
		if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
			FileData[StartTiles + FileData[i + 1] + maxX * FileData[i + 5]] = 0;
			
	
	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


/////////////////////////////////////////////////////////////////////////////

void CChangerDlg::OnToolsGenerateforestNecro()
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check


//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


	unsigned char ObjectCode[] = 
	{
		0x06,0x06,0x06,0x06,0x06,0x06,0x06,	// 7 Dead Wallenwood - 1
		0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,0x2E,	// 7 Dead Wallenwood - 1
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,	// 7 Dead Wallenwood - 1

		0xA1,0xA1,0xA1,0xA1,0xA1,			// 5 Dead Druid Tree - 1
		0xA3,0xA3,0xA3,0xA3,0xA3,			// 5 Dead Druid Tree - 2
		0xA5,0xA5,0xA5,0xA5,0xA5,			// 5 Dead Druid Tree - 3

		0x03,0x03,0x03,0x03,				// 4 Dead Pine - 1
		0x2A,0x2A,0x2A,0x2A,				// 4 Dead Pine - 2
		0x2C,0x2C,0x2C,0x2C,				// 4 Dead Pine - 3

		0x12,0x12,0x12,						// 3 Dead Maple - 1
		0x3E,0x3E,0x3E,						// 3 Dead Maple - 2
		0x40,0x40,0x40,						// 3 Dead Maple - 3

		0xB9,0xB9,							// 2 Dead Tuya - 1
		0xBB,0xBB,							// 2 Dead Tuya - 2
		0xBD,0xBD,							// 2 Dead Tuya - 3
		
		0x0F,								// 1 Dead Iva - 1
		0x3A,								// 1 Dead Iva - 2
		0x3C,								// 1 Dead Iva - 3

		0x09,								// 1 Dead Bush - 1
		0x32,								// 1 Dead Bush - 2
		0x34,								// 1 Dead Bush - 3

		0x15,								// 1 Dead Oak - 1
		0x42,								// 1 Dead Oak - 2
		0x44								// 1 Dead Oak - 3
	};
	
	int OBJECTCODESIZE = sizeof(ObjectCode);
	// HEX codes for objects on a map


	CDensityDlg dlg; // Run "Density" dialog
	if (dlg.DoModal() == IDCANCEL) return;	// Operation cancelled
	if (dlg.m_Density == 0) return;			// Zero value is defined

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0;//, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/
	

//////////////////////////////////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartGroup; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndGroup = i - 16;
			break;
		}
 
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/

// =====================
// === Perform place ===
// =====================
	
	srand((unsigned) time(NULL));
	float RandomChoice;
	UINT X1;
	
	for (X1 = m_Xrange1, i = StartGroup + m_Xrange1 + maxX * m_Yrange1, i1 = StartTiles + 1 + 2 * (m_Xrange1 + maxX * m_Yrange1); i <= StartGroup + m_Xrange2 + maxX * m_Yrange2; X1++, i++, i1 += 2)
	{
		RandomChoice = (float) rand() / RAND_MAX; // Choose to place object or not
		if (X1 >= maxX) X1 = 0;
		if (X1 >= m_Xrange1 && X1 <= m_Xrange2)
		if (RandomChoice < dlg.m_Density / 100.) // Place objects with selected density
		{
		RandomChoice = (float) rand() / RAND_MAX; // Choose what object to place
		for (int j = 0; j < OBJECTCODESIZE; j++)
		{
			if ((RandomChoice < 1. / OBJECTCODESIZE * (j + 1)) && (RandomChoice > 1. / OBJECTCODESIZE * j))
			{
				for (int jj = 0; jj < MAXSelectedTerrain; jj++)
				switch (SelectedTerrain[jj])
				{
					case 0: if (!FileData[i1])	// Check for rocks (=1), water (=2) and roads (=3)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break;
					case 1: if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							FileData[i] = ObjectCode[j];
							break;
					case 2:
					case 3:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							FileData[i] = ObjectCode[j];
							break; 
					case 4:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
							FileData[i] = ObjectCode[j];
							break; 
					case 5:	if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
							FileData[i] = ObjectCode[j];
							break; 
					case 6:	if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
							FileData[i] = ObjectCode[j];
							break; 
					case 7:	if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
							FileData[i] = ObjectCode[j];
							break;
					case 8: if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
							FileData[i] = ObjectCode[j];
							break; 
					case 9:	if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							FileData[i] = ObjectCode[j];
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							FileData[i] = ObjectCode[j];
				}

/*				if (X1 > m_Xrange1)
				if (MAXSelectedTo == 1)
				{
					if (FileData[i - 1] == FileData[i] && dlg.m_Density < 50) FileData[i] = 0; //Remove duplicates
				}
				else
					if (FileData[i - 1] == FileData[i]) FileData[i] = 0; //Remove duplicates
*/				break;
			}
		}
		}
	}

	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");	
}


void CChangerDlg::OnHelpHomepage()
{
	// TODO: Add your command handler code here
	WinExec("explorer.exe http://vamp.restoration.ru", SW_MAXIMIZE);
}


void CChangerDlg::OnChangelevelTolevel1()
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Units allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("No monsters found");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}



// =======================
// === Perform changes ===
// =======================
	for (i = StartGroup; i < EndGroup - 46; i++)	
	{
		i1 =  StartTiles + 1 + 2 * (FileData[i + 1] + maxX * FileData[i + 5]); // Find tile for unit
		for (int jj = 0; jj < MAXSelectedTerrain; jj++)
			switch (SelectedTerrain[jj])
				{
					case 0: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 1: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							}
							break;
					case 2:
					case 3:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							}
							break; 
					case 4:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 5:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 6:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 7:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 8: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 9:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 2 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 1;
									FileData[i + 10] = 1;
									FileData[i + 20] = 1;
									FileData[i + 32] = 0x0A;
								}
							}
				}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


void CChangerDlg::OnChangelevelTolevel2()
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Units allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("No monsters found");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}



// =======================
// === Perform changes ===
// =======================
	for (i = StartGroup; i < EndGroup - 46; i++)	
	{
		i1 =  StartTiles + 1 + 2 * (FileData[i + 1] + maxX * FileData[i + 5]); // Find tile for unit
		for (int jj = 0; jj < MAXSelectedTerrain; jj++)
			switch (SelectedTerrain[jj])
				{
					case 0: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 1: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							}
							break;
					case 2:
					case 3:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							}
							break; 
					case 4:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 5:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 6:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 7:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 8: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 9:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 2;
									FileData[i + 10] = 2;
									FileData[i + 20] = 2;
									FileData[i + 32] = 0x0A;
								}
							}
				}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


void CChangerDlg::OnChangelevelTolevel3()
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Units allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("No monsters found");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}



// =======================
// === Perform changes ===
// =======================
	for (i = StartGroup; i < EndGroup - 46; i++)	
	{
		i1 =  StartTiles + 1 + 2 * (FileData[i + 1] + maxX * FileData[i + 5]); // Find tile for unit
		for (int jj = 0; jj < MAXSelectedTerrain; jj++)
			switch (SelectedTerrain[jj])
				{
					case 0: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 1: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							}
							break;
					case 2:
					case 3:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							}
							break; 
					case 4:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 5:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 6:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 7:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 8: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 9:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 4)
								{
//									FileData[i + 8] = 3;
									FileData[i + 10] = 3;
									FileData[i + 20] = 3;
									FileData[i + 32] = 0x0A;
								}
							}
				}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}


void CChangerDlg::OnChangelevelTolevel4()
{
	// TODO: Add your command handler code here
	if (!Check()) return;	// Initial range check

//	Read information from selected list "TERRAIN"

	int SelectedTerrain[10], MAXSelectedTerrain = m_ListTerrain.GetSelCount();
	m_ListTerrain.GetSelItems(10, SelectedTerrain);

	if (MAXSelectedTerrain == 0) 
	{
		MessageBox("No terrain selected");
		return;
	}


//	We can open and read the file here
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		return;
	}
	
	unsigned long i, i1, FileLength = (unsigned long) f.GetLength(); // Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		return;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength]; // Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		return;
	}
	
	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		return;
	}
	
	f.Close();
	
	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".bak");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;
	unsigned long StartTiles = 0;//, EndTiles = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 1)	// Group number of Landscape (Tiles) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartTiles = i + 8;
			break;
		}

	}

	if (StartTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}

/*
	for (i = StartTiles; i < FileLength - 3; i++)
    {

		if (FileData[i] == FileData[0x24])
		if (FileData[i+1] == FileData[0x25])
		if (FileData[i+2] == FileData[0x26])
		if (FileData[i+3] == FileData[0x27])
		{
			EndTiles = i - 16;
			break;
		}
 
	}
	
	if (EndTiles == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		return; // Group not found
	}
*/



//////////////////////////////////////////////////////////

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 6)	// Group number of Units allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("No monsters found");
		return; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("Monsters allocation table is damaged");
		return; // Group not found
	}



// =======================
// === Perform changes ===
// =======================
	for (i = StartGroup; i < EndGroup - 46; i++)	
	{
		i1 =  StartTiles + 1 + 2 * (FileData[i + 1] + maxX * FileData[i + 5]); // Find tile for unit
		for (int jj = 0; jj < MAXSelectedTerrain; jj++)
			switch (SelectedTerrain[jj])
				{
					case 0: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 1: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x72 && FileData[i1 - 1] <= 0x75) || FileData[i1 - 1] == 0x51) || (FileData[i1 - 1] == 0x51 && FileData[i1] == 1))
							// Check for Cracks
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0x70 && FileData[i1 - 1] <= 0x76)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							}
							break;
					case 2:
					case 3:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x34) || FileData[i1 - 1] == 0x11))
							// Check for Grass / Flowers
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 1 && FileData[i1 - 1] >= 0xB0 && FileData[i1 - 1] <= 0xB6)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							}
							break; 
					case 4:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for High Rocks
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 5:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if ((!FileData[i1] && (FileData[i1 - 1] == 0x14 || FileData[i1 - 1] == 0x54 || FileData[i1 - 1] == 0x94 || FileData[i1 - 1] == 0xD4)) || (FileData[i1 - 1] == 0x14 && FileData[i1] == 1))
							// Check for Lands
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 6:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 1 && ((FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x35) || FileData[i1 - 1] == 0x11))
							// Check for Rocky
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 7:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xB1 && FileData[i1 - 1] <= 0xB5) || FileData[i1 - 1] == 0x91))
							// Check for Sand
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							break;
					case 8: if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (!FileData[i1] && ((FileData[i1 - 1] >= 0xF1 && FileData[i1 - 1] <= 0xF5) || FileData[i1 - 1] == 0xD1))
							// Check for Savanna
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							break; 
					case 9:	if (FileData[i + 1] >= m_Xrange1 && FileData[i + 1] <= m_Xrange2)
							if (FileData[i + 5] >= m_Yrange1 && FileData[i + 5] <= m_Yrange2)
							if (FileData[i1] == 2 && (FileData[i1 - 1] == 0x11 || FileData[i1 - 1] == 0x51 || FileData[i1 - 1] == 0x91 || FileData[i1 - 1] == 0xD1))
							// Check for Water
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							}
							else if (FileData[i1] == 2 && FileData[i1 - 1] >= 0x30 && FileData[i1 - 1] <= 0x36)
							{
								if (FileData[i] == 0x80 && FileData[i + 4] == 0x80 && FileData[i + 42] == 0x80 && FileData[i + 10] >= 1 && FileData[i + 10] <= 3)
								{
//									FileData[i + 8] = 4;
									FileData[i + 10] = 4;
									FileData[i + 20] = 4;
									FileData[i + 32] = 0x0A;
								}
							}
				}
	}


	// Write file
	if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("File can't be written");
		return;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();
	

	delete FileData; // Free allocated memory for file data	
	MessageBox("Done!");
}
