#include "windows.h"

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define EXT(FileName) (FileName+lstrlen(FileName)-4)

typedef BYTE TColor[3];

static BYTE *Position;
static BYTE *Data;
static TColor *Palette;

extern "C" BOOL WINAPI DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
  if(dwReason==DLL_PROCESS_ATTACH)
  {
    HRSRC hResInfo=
      FindResource((HMODULE)hDll,MAKEINTRESOURCE(100),"#256");
    HGLOBAL hResData=LoadResource((HMODULE)hDll,hResInfo);
    Palette=(TColor *)((BYTE *)LockResource(hResData)+8);
  }
  return TRUE;
}

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  short int *WH=(short int *)Data;
  return !lstrcmpi(EXT(Name), ".raw") && WH[0]*WH[1]+4==DataSize;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  ::Data=Data;
  Par->Type=0;
  Par->NumPages=1;
  Par->Transparent=0;
  Par->Depth=8;
  return TRUE;
}

int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
  short int *WH=(short int *)Data;
  Position=Data+4;
  //Palette=(TColor *)(Data+(768+320*200)*Page);
  Par->Width=WH[0];
  Par->Height=WH[1];
  return 1;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
/*  typedef BYTE TColor[3];
  TColor *Palette=(TColor *)Data;
  for(int i=0;i<256;i++)
  {
    Pal[i].peBlue=i;
    Pal[i].peGreen=i;
    Pal[i].peRed=i;
  }*/
  for(int i=0;i<256;i++)
  {
    Pal[i].peBlue=Palette[i][2];
    Pal[i].peGreen=Palette[i][1];
    Pal[i].peRed=Palette[i][0];
  }
  return TRUE;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
  for(int i=0;i<Width;i++)
  {
    *Line=*Position;
    Line++;
    Position++;
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  if(Type==0)
  {
    lstrcpy(Par->FormatName,"Clash");
    lstrcpy(Par->DefaultMasks,"*.raw");
    return TRUE;
  }
  return FALSE;
}
