#include "windows.h"

#pragma pack(1)

#include "..\\PicFmt.hpp"

struct R16Header{
  unsigned short Width;
  unsigned short Height;
  char Reserv0[9];
  unsigned short TranspR; //возможно, что переставил их местами :)
  unsigned short TranspG;
  unsigned short TranspB;
  unsigned short Reserv1/*[2]*/;
  unsigned short Reserv2/*[2]*/; //Width ?
  unsigned short Reserv3/*[2]*/; //Height ?
  int Len; //длина_файла-размер_заголовка
};

static R16Header *Header;
static unsigned short *Position,Transparent;

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  Header=(R16Header *)Data;
  if(Header->Len==DataSize-sizeof(R16Header)) return TRUE;
  return FALSE;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  Header=(R16Header *)Data;
//  if(Header->Type==0x04){
    Par->Type=0;
    Par->NumPages=1;
    Par->Depth=24;
    Par->Transparent=1;
    Par->Red=(Header->TranspR&0x1f)<<3;
    Par->Green=(Header->TranspG&0x1f)<<3;
    Par->Blue=(Header->TranspB&0x1f)<<3;
    Transparent=  (Header->TranspR&0x1f)
                |((Header->TranspG&0x1f)<<5)
                |((Header->TranspB&0x1f)<<10);
    return TRUE;
/*  }else{
    *Type=1;
    *NumPages=0;
  }
  return FALSE;*/
}

int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
//  if(Header->Type==0x04){
    Position=(unsigned short *)((BYTE *)Header+sizeof(R16Header));
    Par->Width=Header->Width;
    Par->Height=Header->Height;
    return 0;
/*  }
  return 1;*/
}
BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
//  if(Header->Type==0x04)
  for(int i=0;i<Width;i++)
  {
    if((*Position&0x7fff)!=Transparent) //прозрачный цвет пропускаем
    {
      *Line=*Position<<3&0xf8;
      Line++;
      *Line=*Position>>2&0xf8;
      Line++;
      *Line=*Position>>7&0xf8;
      Line++;
    }
    else 
      Line+=3;
    Position++;
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  if(Type==0)
  {
    lstrcpy(Par->Game,"Shadow Watch");
    lstrcpy(Par->FormatName,"Shadow Watch");
    lstrcpy(Par->DefaultMasks,"*.r16");
    return TRUE;
  }
  return FALSE;
}
