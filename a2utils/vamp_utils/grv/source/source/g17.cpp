#include <windows.h>
#include <string.h>
#include <stdio.h>

#pragma pack(1)

#include "..\\PicFmt.hpp"

#define TEXTURE 0
#define SPRITE  1

static BYTE *Pos;
typedef BYTE TColor[4]; 
static TColor *Palette;
static int Width,Height,Type,DataEnd,CurWidth;

struct TSprHeader
{
  char ID[4];
  char Reserv0[4];
  int Count;
};

struct TSprPageHeader
{
  int Disp;
  short int Width,Height;
};
static TSprHeader *SprHeader;
static TSprPageHeader *Fat;
static unsigned short int *PosWord;
static bool Sign;
static int Disp2, Disp7, Mask;

void CalcDisp(int Height)
{
  typedef BYTE TColor[3]; 
  TColor *Buf=new TColor[CurWidth];
  if(Buf)
  {
    int i;
    Mask=0xfc;
    Disp2=3;
    Disp7=8;
    unsigned short int *TempPosWord=PosWord;
    for (int j=0; j<Height; j++)
    {
      ZeroMemory(Buf, CurWidth*3);
      GetLine(Buf, CurWidth);
      for(i=0; i<CurWidth; i++)
        if(Buf[i][2]&0x84)
        {
          j=Height;
          if(Buf[i]&0x4)
          {
            Disp2=2;
            Disp7=7;
          }
          break;
        }
    }
    Mask=0xf8;
    PosWord=TempPosWord;
    delete Buf;
  }
}

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  if(Data[0]=='3'&&Data[1]=='d'&&Data[2]=='f')
  {
    Type=TEXTURE;
    return TRUE;
  }
  if(*((unsigned int *)Data)==0x31727073) //spr1
  {
    Type=SPRITE;
    return TRUE;
  }
  return FALSE;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
  Par->Type=Type;
  if(Type==TEXTURE)
  {
    int Temp;
    DataEnd=(int)Data+DataSize; 
    char *NextString=strchr((char *)Data,0x0A)+1;
    if ( !strncmp(NextString,"p8",2) )
    {
      int KoefX,KoefY;
      int i;
      NextString=strchr(NextString,0x0A)+1;
      sscanf(NextString,"lod range: %d %d",&Temp,&Width);
      NextString=strchr(NextString,0x0A)+1;
      sscanf(NextString,"aspect ratio: %d %d",&KoefY,&KoefX);
      Width/=KoefX;
      Height=Width/KoefY;
      Palette=(TColor *)(strchr(NextString,0x0A)+1);
      Pos=(BYTE *)(Palette+256);
      for( i=0; Pos<Data+DataSize && Width>>i && Height>>i; i++)
        Pos+=(Width>>i)*(Height>>i);
      Par->NumPages=i;
      Par->Transparent=0;
      Par->Depth=8;
      return TRUE;
    }
  }
  if(Type==SPRITE)
  {
    SprHeader=(TSprHeader *)Data;
    Par->NumPages=SprHeader->Count;
    Fat=(TSprPageHeader *)(Data+sizeof(TSprHeader));
    Pos=Data;
    Par->Transparent=1;
    Par->Red=255;
    Par->Green=255;
    Par->Blue=255;
    Par->Depth=24;
    return TRUE;
  }
  return FALSE;
}

int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
  if( Type==TEXTURE )
  {
    Par->Width=Width>>Page;
    Par->Height=Height>>Page;
    Pos=(BYTE *)(Palette+256);
    for(int i=0; i<Page; i++)
      Pos+=(Height>>i)*(Width>>i);
    CurWidth=Width>>Page;
  }
  if(Type==SPRITE)
  {
    CurWidth=Par->Width=(Fat[Page].Width)&0x7fff;
    Sign=(Fat[Page].Width)&0x8000;
    Par->Height=(Fat[Page].Height)&0x7fff;
    PosWord=(unsigned short int *)(Pos+Fat[Page].Disp);
    if(Sign)
      PosWord+=2;
    CalcDisp(Par->Height);
  }
  return 0;
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  for(int i=0;i<256;i++)
  {
    Pal[i].peRed=Palette[i][1];
    Pal[i].peGreen=Palette[i][2];
    Pal[i].peBlue=Palette[i][3];
  }
  return TRUE;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
  if(Type==TEXTURE)
  {
    for(int i=0;i<Width;i++)
      *Line++=*Pos++;
  }
  if(Type==SPRITE)
  {
    if(Sign)
      for(int i=0;i<Width;i++,PosWord++)
      {
        *Line++=(*PosWord<<3)&Mask;
        *Line++=(*PosWord>>Disp2)&Mask;
        *Line++=(*PosWord>>Disp7)&Mask;
      }
    else
    {
      int Len=*PosWord;
      PosWord++;
      while(*PosWord)
      {
        if(((*PosWord)&0xC000)==0xC000)//байты изображения
        {
          int Count=(*PosWord)&0x3fff;
          PosWord++;
          for(int i=0;i<Count ;i++,PosWord++)
          {
            *Line++=(*PosWord<<3)&Mask;
            *Line++=(*PosWord>>Disp2)&Mask;
            *Line++=(*PosWord>>Disp7)&Mask;
          }
        }
        else //прозрачность
        {
          Line+=*PosWord*3;
          PosWord++;
        }
      }
      PosWord++;
    }
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  lstrcpy(Par->Game,"Gorky-17");
  if(Type==TEXTURE)
  {
    lstrcpy(Par->FormatName,"Gorky-17 textures");
    lstrcpy(Par->DefaultMasks,"*.3df");
    return TRUE;
  }
  if(Type==SPRITE)
  {
    lstrcpy(Par->FormatName,"Gorky-17 sprites");
    lstrcpy(Par->DefaultMasks,"*.spr");
    return TRUE;
  }
  return FALSE;
}
