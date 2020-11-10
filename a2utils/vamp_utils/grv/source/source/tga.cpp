//Формат кололся не скорую руку, чтобы просмотреть несколько конкретных
//имаджей. Ни на какую полноту не претендует.
#include "windows.h"

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define EXT(FileName) (FileName+lstrlen(FileName)-3)

struct TgaHeader
{
  char Reserv0[2];
  char Type; //возможно, что ошибаюсь :)
  char Reserv1[9];
  unsigned short Width;
  unsigned short Height;
  char Reserv2[2];
};

static TgaHeader *Header;
static unsigned short *Position;

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  Header=(TgaHeader *)Data;
  if(Header->Type==2 
     && 2*Header->Width*Header->Height+sizeof(TgaHeader)==DataSize
     && !lstrcmpi(EXT(Name), "tga"))
    return TRUE;
  return FALSE;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  Header=(TgaHeader *)Data;
  if(Header->Type==0x02)
  {
    Par->Type=0;//Header->Type;
    Par->NumPages=1;
    Position=(unsigned short *)(Data+sizeof(TgaHeader));
    Par->Transparent=0;
    Par->Depth=24;
    return TRUE;
  }
  else
  {
    Par->Type=-1;
    Par->NumPages=0;
  }
  return FALSE;
}

/*BOOL WINAPI _export ClosePicture(){

}*/
int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
  if(Header->Type==0x02){
    Par->Width=Header->Width;
    Par->Height=Header->Height;
    return 0;
  }
  return 1;
}
BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
  if(Header->Type==0x02)
  {
    for(int i=0;i<Width;i++)
    {
      *Line=*Position<<3&0xf8;
      Line++;
      *Line=*Position>>2&0xf8;
      Line++;
      *Line=*Position>>7&0xf8;
      Line++;
      Position++;
    }
    return TRUE;
  }
  return FALSE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  if(Type==0)
  {
    lstrcpy(Par->FormatName,"Truevision Targa");
    lstrcpy(Par->DefaultMasks,"*.tga");
    return TRUE;
  }
  return FALSE;
}
