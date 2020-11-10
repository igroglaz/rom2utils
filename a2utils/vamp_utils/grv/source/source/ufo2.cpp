#include "windows.h"

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define EXT(FileName) (FileName+lstrlen(FileName)-3)

static BYTE *Position, *Data;
typedef BYTE TColor[3];
static TColor *Palette;
static int Type;

extern "C" BOOL WINAPI DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
  if(dwReason==DLL_PROCESS_ATTACH)
  {
    HRSRC hResInfo=
      FindResource((HMODULE)hDll,MAKEINTRESOURCE(100),"#256");
    HGLOBAL hResData=LoadResource((HMODULE)hDll,hResInfo);
    Palette=(TColor *)LockResource(hResData);
  }
  return TRUE;
}

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  char *Ext=EXT(Name);
  return(   (DataSize==64000 && !lstrcmpi(Ext, "scr")) 
         || (DataSize==64006 && !lstrcmpi(Ext, "spk")) );
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  ::Data=Data;
  if(DataSize==64000)
    Type=Par->Type=0;
  if(DataSize==64006)
    Type=Par->Type=1;
  Par->NumPages=1;
  Par->Transparent=0;
  Par->Depth=8;
  return TRUE;
}

int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
  Position=Data+(Type?6:0);
  Par->Width=320;
  Par->Height=200;
  return 0;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  for(int i=0;i<256;i++)
  {
    Pal[i].peBlue=Palette[i][0];
    Pal[i].peGreen=Palette[i][1];
    Pal[i].peRed=Palette[i][1];
  }
//  MoveMem((BYTE *)Pal, (BYTE *)Palette, 1024);
  return TRUE;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
  for(int i=0;i<Width;i++)
    *Line++=*Position++;
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  lstrcpy(Par->Game,"UFO2");
  if(Type==0)
  {
    lstrcpy(Par->FormatName,"UFO2 SCR");
    lstrcpy(Par->DefaultMasks,"*.scr");
    return TRUE;
  }
  if(Type==1)
  {
    lstrcpy(Par->FormatName,"UFO2 spk");
    lstrcpy(Par->DefaultMasks,"*.spk");
    return TRUE;
  }
  return FALSE;
}
