#include <windows.h>

#pragma pack(1)
#include "..\\PicFmt.hpp"

#define EXT(FileName) ( FileName+lstrlen(FileName)-min( 4, lstrlen(FileName) ) )
#define RIX 0
#define FRM 1

struct TRixHeader
{
  char ID[4];
  unsigned short int Width,Height;
  char Reserv[2];
};
struct TFrmHeader
{
  char ID[4];
  unsigned short int PointerCount;
  unsigned int Count;
  char Reserv[52];
};
struct TPageRecord
{
  unsigned short int Width, Height;
  unsigned int PageSize;
  unsigned short int DispX,DispY;
};

static TRixHeader *RixHeader;
static TFrmHeader *FrmHeader;
static BYTE *Pos,*Pages;
typedef BYTE TColor[3];
static TColor *Palette;
static int Type;
char PaletteName[MAX_PATH];

extern "C" BOOL WINAPI DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
  if(dwReason==DLL_PROCESS_ATTACH)
  {
    HRSRC hResInfo=
      FindResource((HMODULE)hDll,MAKEINTRESOURCE(100),"#256");
    HGLOBAL hResData=LoadResource((HMODULE)hDll,hResInfo);
    Palette=(TColor *)LockResource(hResData);
  }
  return TRUE;
}

BYTE Round(double Color)
{
  Color+=0.5;
  return Color>255?255:Color<0?0:(BYTE)Color;
} 

unsigned short int IntelShort(unsigned short int Motorola)
{
  return (Motorola<<8)+(Motorola>>8);
}

unsigned int IntelInt(unsigned int Motorola)
{
  return (Motorola<<24)+(Motorola>>24)+((Motorola<<8)&0xff0000)+((Motorola>>8)&0xff00);
}

BYTE *FindPage(int Index)
{
  int i;
  for(i=0;Index;Index--)
  {
    TPageRecord *Record=(TPageRecord *)(Pages+i);
//    ShowInt("record disp","",(BYTE *)Record-(BYTE *)FrmHeader);
    i+=sizeof(TPageRecord)+IntelInt(Record->PageSize);
  }
  return Pages+i; 
}

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  DWORD ID=*((DWORD *)Data);
  if(ID==0x33584952) // "RIX3"
  {
    Type=RIX;
    return TRUE;
  }
  char *Ext=EXT(Name);
  char Mask[5]=".frm";
  for(char i='0'-1; i<'7'; i++)
  {
    if(!lstrcmpi(Ext, Mask))
    {
      if( ID==0x4000000 || ID==0x3000000 )
      {
        lstrcpy(PaletteName, Name);
        lstrcpy(EXT(PaletteName), ".pal");
        Type=FRM;
        return TRUE;
      }
    }
    Mask[3]=i;
  }
  return FALSE; 
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, struct TPicPar *Par)
{
  Par->Type=Type;
  if(Type==RIX)
  {
    RixHeader=(TRixHeader *)Data;
    Par->NumPages=1;
    Par->Transparent=0;
  }
  else
  {
    FrmHeader=(TFrmHeader *)Data;
    Pages=Data+sizeof(TFrmHeader);
 /*   Par->NumPages=IntelInt(FrmHeader->Count);
    if(Par->NumPages==0)
      Par->NumPages=1;*/
    for(Par->NumPages=0; FindPage(Par->NumPages)<Data+DataSize; Par->NumPages++)
    Par->Transparent=1;
    Par->Red=Par->Green=Par->Blue=255<<2;
  }
  Par->Depth=8;
  return TRUE;
}

int WINAPI _export OpenPage(int Index, struct TPagePar *Par)
{
  if(Type==RIX)
  {
    Par->Width=RixHeader->Width;
    Par->Height=RixHeader->Height;
    Pos=(BYTE *)RixHeader+768+sizeof(TRixHeader);
  }
  else
  {
    TPageRecord *PageRecord=(TPageRecord *)FindPage(Index);
    Pos=(BYTE *)(PageRecord+1);
//    ShowInt("record disp","",(BYTE *)PageRecord-(BYTE *)FrmHeader);

    Par->Width=IntelShort(PageRecord->Width);
    Par->Height=IntelShort(PageRecord->Height);
//    ShowInt("s","w",IntelShort(PageRecord->Width),"w",PageRecord->Width);
  }
  return 0;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
  for(int i=0;i<Width;Line[i++]=*Pos++);
//  CopyMemory(Line,Pos,Width);
//  Pos+=Width;
  return TRUE;
}

void CopyPalette(PALETTEENTRY *Pal, TColor *Palet, int Size, double Gamma=1)
{
  for(int i=0; i<min(Size, 256); i++)
  {
    Pal[i].peRed  =Round(Gamma*Palet[i][0]);
    Pal[i].peGreen=Round(Gamma*Palet[i][1]);
    Pal[i].peBlue =Round(Gamma*Palet[i][2]);
  }
}
BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  LPVOID MapData=NULL;
  if(Type==RIX)
  {
    TColor *Palet=(TColor *)(RixHeader+1);
    for(int i=0;i<256;i++)
    {
      Pal[i].peBlue=(int)Palet[i][2]*255/63;
      Pal[i].peGreen=(int)Palet[i][1]*255/63;
      Pal[i].peRed=(int)Palet[i][0]*255/63;
    }
  }
  else
  {
    HANDLE Handle=
      CreateFile(PaletteName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(Handle!= INVALID_HANDLE_VALUE)
    {
      DWORD FileSize=GetFileSize(Handle,NULL);
      HANDLE MapHandle=CreateFileMapping(Handle,NULL,PAGE_READONLY,0,0,NULL);
      if(MapHandle!=NULL)
      {
        MapData=MapViewOfFile(MapHandle,FILE_MAP_READ,0,0,0);
        if(MapData)
        {
          CopyPalette(Pal, (TColor *)MapData, FileSize/3, 4);
          UnmapViewOfFile(MapData);
        }
        CloseHandle(MapHandle);
      }
      CloseHandle(Handle);
    }
    if(MapData==NULL)  
      CopyPalette(Pal,Palette,256,4);
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, struct TFormatPar *Par)
{
  lstrcpy(Par->Game,"Fallout");
  if(Type==RIX)
  {
    lstrcpy(Par->FormatName,"Fallout RIX");
    lstrcpy(Par->TypeName,"RIX");
    lstrcpy(Par->DefaultMasks,"*.rix");
    return TRUE;
  }
  if(Type==FRM)
  {
    lstrcpy(Par->FormatName,"Fallout FMT");
    lstrcpy(Par->TypeName,"FMT");
    lstrcpy(Par->DefaultMasks,"*.frm");
    return TRUE;
  }
/*  if(Type>1 && Type<8)
  {
    lstrcpy(Par->FormatName,"Fallout FMT");
    lstrcpy(Par->TypeName,"FMT");
    wsprintf(Par->DefaultMasks,"*.fr%d",Type-2);
    return TRUE;
  }*/
  return FALSE;
}
