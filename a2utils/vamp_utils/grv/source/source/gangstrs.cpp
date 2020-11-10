#include "windows.h"

#pragma pack(1)

#include "..\\PicFmt.hpp"

static BYTE *Position, *Data;
static int Count;
struct Record
{
  unsigned short Width,Height;
  char Reserved[8];
  int Disp;
};
static struct Record *Fat;

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  Count=*((int *)Data);
  Fat=(Record *)(Data+4);
  return Fat->Disp==sizeof(Record)*Count+4;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  ::Data=Data;
  Par->Type=0;
  Par->NumPages=Count;
  Par->Transparent=0;
  Par->Depth=8;
  return TRUE;
}

/*BOOL WINAPI _export ClosePicture(){

}*/
int WINAPI _export OpenPage(int Page, TPagePar *Par){
  Par->Width=Fat[Page].Width;
  Par->Height=Fat[Page].Height;
  Position=Data+Fat[Page].Disp;
  return 0;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  for(int i=0;i<256;i++)
  {
    Pal[i].peBlue=i;
    Pal[i].peGreen=i;
    Pal[i].peRed=i;
  }
  return TRUE;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width){
  for(int i=0;i<Width;i++)
    *Line++=*Position++;
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par){
  if(Type==0)
  {
    lstrcpy(Par->Game,"Gangsters");
    lstrcpy(Par->FormatName,"Gangsters sprite");
    lstrcpy(Par->DefaultMasks,"*.spr");
    return TRUE;
  }
  return FALSE;
}
