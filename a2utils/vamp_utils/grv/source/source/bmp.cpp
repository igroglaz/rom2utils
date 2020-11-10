#include <windows.h>

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define HEADER ((BITMAPFILEHEADER *)Data)
#define INFOHEADER ((BITMAPINFOHEADER *)(HEADER+1))
#define LINELEN ((INFOHEADER->biWidth*(INFOHEADER->biBitCount>>3)+3)>>2<<2)
#define PALETTE ((RGBQUAD *)((BYTE *)INFOHEADER+INFOHEADER->biSize))
#define POS (Data+HEADER->bfOffBits+LINELEN*(INFOHEADER->biHeight-1))

static BYTE *Data, *Pos/*, *Position*/;
static int DataSize;
//static RGBQUAD *Palette;
//static BITMAPFILEHEADER *Header;
//static BITMAPINFOHEADER *InfoHeader;
//static LONG LineLen;
static TGrvApiInfo *GrvApi;

BOOL WINAPI _export GetPluginInfo(TPluginInfo *Info, TGrvApiInfo *ApiInfo)
{
  GrvApi=ApiInfo;
}

BOOL CompareRight(char *Name, char *Ext)
{
  int NameLen=lstrlen(Name);
  int ExtLen=lstrlen(Ext);
  return NameLen>=ExtLen && !lstrcmpi(Name+NameLen-ExtLen, Ext);
}

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  //GrvApi->ShowInt("h","free",*((int *)(Data+6)),"fileLen",*((int *)(Data+2)));
  return /*GrvApi->*/CompareRight(Name, ".bmp") 
         && Data[0]=='B' && Data[1]=='M' && *((int *)(Data+6))==0 
         /*&& *((int *)(Data+2))==DataSize*/;
  //не проверяется длина файла тк frmcon20 неверно ее формирует,
  //а мне еще тестировать и тестировать...
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  ::Data=Data;
  ::DataSize=DataSize;
//  GrvApi->ShowInt("a","Depth",InfoHeader->biBitCount,"RGB",InfoHeader->biCompression==BI_RGB);
  Par->Type=0;//InfoHeader->biCompression;
  Par->NumPages=1;

//  LineLen=( INFOHEADER->biWidth* (INFOHEADER->biBitCount>>3) +3 )>>2<<2;
  Par->Depth=INFOHEADER->biBitCount;
  Par->Transparent=0;
  
  if(INFOHEADER->biCompression==BI_RGB)
    if(INFOHEADER->biBitCount==24 || INFOHEADER->biBitCount==8)
      return TRUE;
  if(INFOHEADER->biCompression==BI_BITFIELDS && INFOHEADER->biBitCount==16)
    return TRUE;
  return FALSE;
}

int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
  Pos=POS;//Position;
  Par->Width=INFOHEADER->biWidth;
  Par->Height=INFOHEADER->biHeight;
  return 0;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  RGBQUAD *Palette=PALETTE;
  for(int i=0;i<256;i++)
  {
    Pal[i].peRed=Palette[i].rgbRed;
    Pal[i].peGreen=Palette[i].rgbGreen;
    Pal[i].peBlue=Palette[i].rgbBlue;
  }
  return TRUE;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
  GrvApi->MoveMem(Line, Pos, Width*(INFOHEADER->biBitCount>>3));
  Pos-=LINELEN;
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  if(Type==0)
  {
    lstrcpy(Par->FormatName,"BMP Windows Bitmap");
    lstrcpy(Par->DefaultMasks,"*.bmp");
    return TRUE;
  }
  return FALSE;
}
