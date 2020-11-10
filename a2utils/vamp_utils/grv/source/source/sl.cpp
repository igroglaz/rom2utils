#include "windows.h"

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define EXT(FileName) (FileName+lstrlen(FileName)-3)

struct FatRecord
{
  char ID[4];
  int Disp;
};
struct PageRecord
{
  char ID[4];
  short int Width, Height;
  char Reserv[36];
};

static FatRecord *Fat;
static PageRecord *Pages;
static BYTE *Position, *Data;

/*static struct TGrvApiInfo *GrvApi;
BOOL WINAPI _export GetPluginInfo(struct TPluginInfo *Info, struct TGrvApiInfo *ApiInfo)
{
  GrvApi=ApiInfo;
  return TRUE;
}*/           

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  return ((int *)Data)[1]==0x50414C31;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  ::Data=Data;
  Fat=(FatRecord *)(Data+4);
  Pages=(PageRecord *)(Data+4+sizeof(FatRecord)* *((int *)Data));
  Par->Type=0;
  Par->NumPages=*((int *)Data)-1;
  Par->Transparent=0;
  Par->Depth=8;
//  Par->Depth=24;
  //GrvApi->ShowInt("", "Fat", (BYTE *)Fat-Data, "Pages", (BYTE *)Pages-Data);
  return TRUE;
}

int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
  PageRecord *pr=(PageRecord *)((BYTE *)Pages+Fat[Page+1].Disp);
  //GrvApi->ShowInt("", "Page", (BYTE *)pr-Data);
  Par->Width=pr->Width;
  Par->Height=pr->Height;
  Position=(BYTE *)(pr+1);
  return 0;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  typedef BYTE Color[4];
  Color *Palette=(Color *)Pages;
  for(int i=0;i<256;i++)
  {
    Pal[i].peBlue=Palette[i][0];
    Pal[i].peGreen=Palette[i][1];
    Pal[i].peRed=Palette[i][2];
  }
  return TRUE;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width){
  for(int i=0;i<Width;i++)
  {
    *Line++=*Position++;
/*    *Line++=*Position++;
    *Line++=*Position++;
    Position++;*/
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par){
  if(Type==0)
  {
    lstrcpy(Par->Game,"Scuad Leader");
    lstrcpy(Par->FormatName,"Scuad Leader Graphics");
    lstrcpy(Par->DefaultMasks,"*.gra");
    return TRUE;
  }
  return FALSE;
}
