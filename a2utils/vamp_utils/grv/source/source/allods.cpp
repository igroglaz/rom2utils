#include <windows.h>

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define EXT(FileName) (FileName+lstrlen(FileName)-3)

static RGBQUAD *Palette;
static BYTE *Position, *Data;
unsigned short int *Pos;
struct TFatRecord
{
  int DispX,DispY;
  int PageSize;
};
static int LineCount,Count,Type;

BYTE *FindNextPage(BYTE *Data, BYTE *Edge=0)
{
  TFatRecord *Record=(TFatRecord *)Data;
  if(Edge && Data+12>Edge)
    return Edge+1;
  if(Edge && (Record->PageSize<0 || Data+12+Record->PageSize>Edge))
    return Edge+1;
  return Data+12+Record->PageSize;
}

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  if(!DataSize || lstrcmpi(EXT(Name), "256"))
    return FALSE;
  BYTE *Addr=Data+1024;
  for(Type=0;Type<1;Type++)
  {
    for(Count=0;;Count++)
    {
      if(Addr>Data+DataSize)
        break;
      if(Addr==Data+DataSize || ( Addr+4<=Data+DataSize && *((int *)Addr)&0x80000000) )
        return TRUE;
      if(Addr+12>Data+DataSize)
        break;
      Addr=FindNextPage(Addr,Data+DataSize);
    }           
    Addr=Data;
  }
  return FALSE;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  ::Data=Data;
  Par->Type=Type;
  Par->NumPages=Count;
  Palette=(RGBQUAD *)Data;
  Par->Transparent=1;
  Par->Red=Palette[0].rgbRed;
  Par->Green=Palette[0].rgbGreen;
  Par->Blue=Palette[0].rgbBlue;
  Par->Depth=8;
  return TRUE;
}

/*BOOL WINAPI _export ClosePicture(){

}*/
int WINAPI _export OpenPage(int Page, TPagePar *Par){
  for(Position=Data+1024;Page;Page--)
    Position=FindNextPage(Position);
  TFatRecord &Fat=*((TFatRecord *)Position);
  Position+=12;
  Pos=(unsigned short int *)Position;
  Par->Width=Fat.DispX;
  Par->Height=Fat.DispY;
  LineCount=0;
  return 0;
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

BOOL WINAPI _export GetLine(BYTE *Line, int Width){
  if(LineCount)
  {
    LineCount--;
    return TRUE;
  }
  if(!Type)
  {
    if(*Position&0x40)
    {
      LineCount=(*Position&0x3f)-1;
      Position++;
      return TRUE;
    }
    for(int i=0; i<Width; )
    {
      if(*Position&0x80)
      {
        Line+=*Position&0x3f;
        i+=*Position&0x3f;
        Position++;
        if(i>=Width)
          return TRUE;
        continue;
      }
      int W=*Position;
      Position++;
      for(int j=0;j<W;i++,j++)
        *Line++=*Position++;
    }
  }
/*  else
  {
    if(*Pos&0x4000)
    {
      LineCount=(*Pos&0x3fff)-1;
      Pos++;
      return TRUE;
    }
    for(int i=0; i<Width; )
    {
      if(*Pos&0x8000)
      {
        Line+=(*Pos&0x3fff)*3;
        i+=(*Pos&0x3fff);
        Pos++;
        if(i>=Width)
          return TRUE;
        continue;
      }
      int W=*Pos;
      Pos++;
      for(int j=0;j<W;i++,j++)
      {
        *Line++=Palette[*Pos&0xff].rgbRed;//(*Pos&0x1f)<<3;
        *Line++=Palette[*Pos&0xff].rgbGreen;//((*Pos>>5)&0x1f)<<3;
        *Line++=Palette[*Pos&0xff].rgbBlue;//((*Pos>>10)&0x1f)<<3;
        Pos++;
      }
    }
  }*/
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par){
  lstrcpy(Par->Game,"Allods");
  if(Type==0)
  {
    lstrcpy(Par->FormatName,"Allods 256");
    lstrcpy(Par->DefaultMasks,"*.256");
    return TRUE;
  }
/*  if(Type==1)
  {
    lstrcpy(Par->FormatName,"Allods 16a");
    lstrcpy(Par->DefaultMasks,"*.16a");
    return TRUE;
  }*/
  return FALSE;
}
