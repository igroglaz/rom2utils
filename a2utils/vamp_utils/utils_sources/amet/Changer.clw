; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChangerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Changer.h"

ClassCount=7
Class1=CChangerApp
Class2=CChangerDlg
Class3=CAboutDlg

ResourceCount=14
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CHANGER_DIALOG
Resource4=IDD_PROPPAGE_MEDIUM (English (U.S.))
Resource5=IDD_CHANGER_DIALOG (Neutral (Sys. Default))
Resource6=IDB_BITMAP (English (U.S.))
Resource7=IDD_PROPPAGE_SMALL (English (U.S.))
Resource8=IDD_DIALOG_CONFIRM
Resource9=IDR_ACCELERATOR (Neutral (Sys. Default))
Class4=CSplashDlg
Resource10=IDD_ABOUTBOX (Neutral (Sys. Default))
Class5=CDensityDlg
Resource11=IDD_DIALOG_ERRORLIST (Neutral (Sys. Default))
Class6=CErrorlistDlg
Resource12=IDD_SPLASH (Neutral (Sys. Default))
Class7=CConfirm
Resource13=IDD_DIALOG_DENSITY (Neutral (Sys. Default))
Resource14=IDR_CHANGER_MENU (Neutral (Sys. Default))

[CLS:CChangerApp]
Type=0
HeaderFile=Changer.h
ImplementationFile=Changer.cpp
Filter=N
LastObject=ID_ACCEL_EXIT
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CChangerDlg]
Type=0
HeaderFile=ChangerDlg.h
ImplementationFile=ChangerDlg.cpp
Filter=D
LastObject=ID_TILES_SLOPE_BOTTOMTOP
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ChangerDlg.h
ImplementationFile=ChangerDlg.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_CHANGER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CChangerDlg

[DLG:IDD_CHANGER_DIALOG (Neutral (Sys. Default))]
Type=1
Class=CChangerDlg
ControlCount=17
Control1=IDC_EDIT_XRANGE1,edit,1350639744
Control2=IDC_EDIT_YRANGE1,edit,1350639744
Control3=IDC_EDIT_XRANGE2,edit,1350639744
Control4=IDC_EDIT_YRANGE2,edit,1350639744
Control5=IDC_LIST_TERRAIN,listbox,1352730883
Control6=IDC_LIST_FROM,listbox,1352730883
Control7=IDC_LIST_TO,listbox,1352730883
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308353
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308354
Control15=IDC_STATIC,static,1342308354
Control16=IDC_STATIC,button,1342177287
Control17=IDC_EDIT_MGROUP,edit,1350576257

[DLG:IDB_BITMAP (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342177287

[DLG:IDD_SPLASH (Neutral (Sys. Default))]
Type=1
Class=CSplashDlg
ControlCount=0

[DLG:IDD_PROPPAGE_MEDIUM (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_SMALL (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:CSplashDlg]
Type=0
HeaderFile=SplashDlg.h
ImplementationFile=SplashDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSplashDlg
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX (Neutral (Sys. Default))]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_DENSITY (Neutral (Sys. Default))]
Type=1
Class=CDensityDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_DENSITY,edit,1350639616
Control4=IDC_STATIC,static,1342308352
Control5=IDC_SPIN_DENSITY,msctls_updown32,1342177312

[CLS:CDensityDlg]
Type=0
HeaderFile=DensityDlg.h
ImplementationFile=DensityDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDensityDlg

[CLS:CErrorlistDlg]
Type=0
HeaderFile=ErrorlistDlg.h
ImplementationFile=ErrorlistDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_BUTTON_FIXALL
VirtualFilter=dWC

[DLG:IDD_DIALOG_ERRORLIST (Neutral (Sys. Default))]
Type=1
Class=CErrorlistDlg
ControlCount=3
Control1=ID_BUTTON_FIXALL,button,1342242817
Control2=IDC_ERRORLIST,listbox,1352728835
Control3=IDCANCEL,button,1342242817

[MNU:IDR_CHANGER_MENU (Neutral (Sys. Default))]
Type=1
Class=CChangerDlg
Command1=ID_FILE_OPEN
Command2=ID_FILE_MAKERIP
Command3=ID_FILE_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_SELECTALL
Command6=ID_TOOLS_VERIFY
Command7=ID_TOOLS_PLACETERRAIN
Command8=ID_TOOLS_PLACESPECIALTILES
Command9=ID_TILES_GENERATELANDSCAPE
Command10=ID_TILES_SLOPE_LEFTRIGHT
Command11=ID_TILES_SLOPE_RIGHTLEFT
Command12=ID_TILES_SLOPE_TOPBOTTOM
Command13=ID_TILES_SLOPE_BOTTOMTOP
Command14=ID_TILES_RANDOMIZEALTITUDES
Command15=ID_TOOLS_FLATTENROADS
Command16=ID_TOOLS_PLACE
Command17=ID_TOOLS_REMOVEOBJECTS
Command18=ID_TOOLS_CHANGE
Command19=ID_TOOLS_KILLDUPLICATEOBJECTS
Command20=ID_OBJECTS_KILLOVERLAPS
Command21=ID_OBJECTS_DECORATEBEACH_BEACH
Command22=ID_OBJECTS_DECORATEBEACH_MOUNTAINS
Command23=ID_OBJECTS_DECORATEBEACH_CUSTOM
Command24=ID_TOOLS_GENERATEFOREST_HUMAN
Command25=ID_TOOLS_GENERATEFOREST_DRUID
Command26=ID_OBJECTS_GENERATEFOREST_TROPIC
Command27=ID_TOOLS_GENERATEFOREST_CUSTOM
Command28=ID_EDIT_FINDMAXGROUP
Command29=ID_UNITS_REMOVEALLUNITS
Command30=ID_UNITS_CHANGELEVEL_UP
Command31=ID_UNITS_CHANGELEVEL_DOWN
Command32=ID_HELP_CONTENTS
Command33=ID_HELP_ABOUT
CommandCount=33

[DLG:IDD_DIALOG_CONFIRM]
Type=1
Class=CConfirm
ControlCount=3
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242817
Control3=IDC_STATIC,static,1342308353

[CLS:CConfirm]
Type=0
HeaderFile=Confirm.h
ImplementationFile=Confirm.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CConfirm

[ACL:IDR_ACCELERATOR (Neutral (Sys. Default))]
Type=1
Class=CChangerDlg
Command1=ID_ACCEL_EXIT
CommandCount=1

