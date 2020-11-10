#pragma pack(1)

#include <windows.h>
#include "..\\PicFmt.hpp"

#define EXT(FileName) (FileName+lstrlen(FileName)-3)

struct RleHeader
{
  char ID[2];
  int FileLen;
  char Reserv0[4];
  char Reserv1[8];
  int Width,Height;
  char Reserv2[28];
};
static struct RleHeader *Header;
static BYTE *Pos;
static RGBQUAD *Palette;
static int *LineDisp;
static int Count;

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  return !lstrcmpi(EXT(Name), "rle") && Data[0]=='B' && Data[1]=='M' && *((int *)(Data+6))==0 /*&& *((int *)(Data+2))==DataSize*/;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  Header=(RleHeader *)Data;
  Palette=(RGBQUAD *)(Header+1);
  Par->Type=0;
  Par->NumPages=1;
  Par->Transparent=1;
  Par->Red=Par->Green=Par->Blue=0;
  Par->Depth=8;
  return TRUE;
}

int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
//  ShowInt("a","w",Header->Width,"h",Header->Height);
  Par->Width=Header->Width;
  Par->Height=Header->Height;
  LineDisp=(int *)(((BYTE *)(Header+1))+1024+12);
  Pos=(BYTE *)(LineDisp+Header->Height);
//  ShowInt("b","LineDisp",(BYTE *)LineDisp-(BYTE *)Header,"Pos",(BYTE *)Pos-(BYTE *)Header);
  Count=0;
  return 0;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
/*  if(Count++>1)
    return TRUE;*/
  for(int i=0;i<Width;)
  {
    if(*Pos==255)
    {
      Pos++;
      i+=*Pos;
      Pos++;
      continue;
    }
    if(*Pos==254)
    {
      Pos++;
      continue;
    }
    int Count=*Pos;
    Pos++;
    for(; Count; Count--, Line[i++]=*Pos++);
  }
  return TRUE;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  for(int i=0;i<256;i++)
  {
    Pal[i].peRed=Palette[i].rgbRed;
    Pal[i].peGreen=Palette[i].rgbGreen;
    Pal[i].peBlue=Palette[i].rgbBlue;
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  if(!Type)
  {
    lstrcpy(Par->Game,"Commandos");
    lstrcpy(Par->FormatName,"Commandos RLE");
    lstrcpy(Par->TypeName,"RLE");
    lstrcpy(Par->DefaultMasks,"*.rle");
    return TRUE;
  }
  return FALSE;
}

