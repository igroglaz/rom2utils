#include "windows.h"

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define EXT(FileName) (FileName+lstrlen(FileName)-4)
#define SPACE 4
#define SQSIZE 16
#define COLS 16
#define ROWS 16

static BYTE *Data;
static int Lines;

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  return DataSize==768/* && !lstrcmpi(EXT(Name), ".acm")*/;
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
  Par->Width=COLS*(SQSIZE+SPACE)-SPACE;
  Par->Height=ROWS*(SQSIZE+SPACE)-SPACE;
  Lines=0;
  return 0;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  typedef BYTE TColor[3];
  TColor *Palette=(TColor *)Data;
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
  int Index=Lines/(SQSIZE+SPACE)*COLS;
  if(Lines++%(SQSIZE+SPACE)>SQSIZE)
    return TRUE;
  for(int i=0; i<COLS; i++, Index++)
  {
    for(int j=0; j<SQSIZE; j++)
      *Line++=Index;
    Line+=SPACE;
    Index++;
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  if(Type==0)
  {
    lstrcpy(Par->Game,"");
    lstrcpy(Par->FormatName,"Palette");
    lstrcpy(Par->DefaultMasks,"*.acm");
    return TRUE;
  }
  return FALSE;
}
