#include "windows.h"

#pragma pack(1)    

#include "..\\PicFmt.hpp"

typedef BYTE TColor[3]; 
#define HEADER ((StiHeader *)Data)
#define PALETTE ((TColor *)(HEADER+1))
#define FAT ((StiRecord *)(PALETTE+256))
#define PAGES ((BYTE *)(FAT+HEADER->Count))
#define PAGE ((unsigned short int *)(HEADER+1))

struct StiHeader
{
  char Ident[4];   // строка "STCI"
  int Size0;       // здесь то размер депакованой картинки, то == SizePack
                   // то вообще какая-то ересь - огромнейщее но круглое число
  int SizePack;    // суммарный размер пакованных имаджей
  char Reserv0[4];
  char Type;       // тип STI 0x04 - одностраничный
                   //         0x28,0x29 - многостраничный
  char Reserv1[3];
  short int Height;
  short int Width;
  short int ColorCount; // кто-то предположил, что здесь количество цветов
  char Reserv2[2];
  short int Count;
  char Reserv3[34];
};
struct StiRecord
{
  int Disp;
  int Len;
  short int DispX;
  short int DispY;
  short int Height;
  short int Width;
};

static unsigned short *Position;
static BYTE *Pos;
static StiRecord *Record;
static BYTE *Data;
static int DataSize;
static int Red, Green, Blue;

static struct TGrvApiInfo *GrvApi;

BOOL WINAPI _export GetPluginInfo(struct TPluginInfo *Info, struct TGrvApiInfo *ApiInfo)
{
  GrvApi=ApiInfo;
  return TRUE;
}           

BOOL WINAPI _export IsPicture(char *Name, BYTE *Data, int DataSize)
{
  if(Data[0]!='S'||Data[1]!='T'||Data[2]!='C'||Data[3]!='I')
    return FALSE;
  return TRUE;
}

BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, TPicPar *Par)
{
//  Header=(StiHeader *)Data;
  ::Data=Data;
  ::DataSize=DataSize;
  if(HEADER->Type==0x04)
  {
    Par->Type=0;
    Par->NumPages=DataSize==sizeof(StiHeader)?0:1;
    Par->Transparent=0;
//    Par->Depth=24;
    Par->Depth=16;
    return TRUE;
  }
  else
  {
    Par->Type=1;
    //Palette=(TColor *)(HEADER+1);
    //Fat=(StiRecord *)(Palette+256);
    Par->NumPages=HEADER->Count;
    Par->Transparent=1;
    Par->Red=PALETTE[0][0];
    Par->Green=PALETTE[0][1];
    Par->Blue=PALETTE[0][2];
    Par->Depth=8;
    return TRUE;
  }
  return FALSE;
}

/*BOOL WINAPI _export ClosePicture(){

}*/
int WINAPI _export OpenPage(int Page, TPagePar *Par)
{
  if(HEADER->Type==0x04)
  {
    Par->Width=HEADER->Width;
    Par->Height=HEADER->Height;
    Position=(unsigned short *)(HEADER+1);
    return 0;
  }
  if(HEADER->Type==0x28 || HEADER->Type==0x29)
  {
    if(Page<0 || Page>=HEADER->Count)
      return -1;
    Record=&FAT[Page];
    Par->Width=Record->Width;
    Par->Height=Record->Height;
    Par->DispX=Record->DispX;
    Par->DispY=Record->DispY;
    Pos=(BYTE *)(Record->Disp+(BYTE *)(FAT+HEADER->Count));
    return 0;
  }
  return -1;
}

BOOL GetLine24(BYTE *Line, int Width)
{
/*  if(HEADER->Type==0x04)
  {
    for(int i=0;i<Width;i++)
    {
      *Line=*Position<<3&0xf8;
      Line++;
      *Line=*Position>>3&0xf8;
      Line++;
      *Line=*Position>>8&0xf8;
      Line++;
      Position++;
    }
    return TRUE;
  }
  if(HEADER->Type==0x28 || HEADER->Type==0x29)
  {*/
    for(int i=0;i<Width;)
    {
      int z=(*Pos&0x7f);
      if(*Pos&0x80)
      {
        i+=z;
        Line+=z*3;
        Pos++;
        if(*Pos&0x80) continue; //!!потенциально возможна ошибка. Нужен переход на проверку "конец имаджа"
        z=*Pos;
      }
      Pos++;
      for(;z;z--,i++,Pos++)
      {
        *Line++=PALETTE[*Pos][2];
        *Line++=PALETTE[*Pos][1];
        *Line++=PALETTE[*Pos][0];
      }
    }
    return TRUE;
/*  }
  return FALSE;*/
}

BOOL GetLine16(unsigned short *Line, int Width)
{
  GrvApi->MoveMem(Line, Position, Width<<1);
  Position+=Width;
  return TRUE;
}

BOOL GetLine8(BYTE *Line, int Width){
  for(int i=0;i<Width;)
  {
    int z=(*Pos&0x7f);
    if(*Pos&0x80)
    {
      i+=z;
      Line+=z;    /*      for(; z; z--, *Line++=0); */
      Pos++;
      if(*Pos&0x80) continue; 
      z=*Pos;
    }
    Pos++;
    GrvApi->MoveMem(Line, Pos, z);
    Pos+=z;
    Line+=z;
    i+=z;
  }
  return TRUE;
}

BOOL WINAPI _export GetLine(BYTE *Line, int Width)
{
  if(HEADER->Type==0x04)
    return GetLine16((unsigned short *)Line, Width);
  return GetLine8(Line, Width);
}

BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal)
{
  for(int i=0;i<256;i++)
  {
    Pal[i].peBlue=PALETTE[i][2];
    Pal[i].peGreen=PALETTE[i][1];
    Pal[i].peRed=PALETTE[i][0];
  }
  return TRUE;
}

BOOL WINAPI _export GetFormatInfo(int Type, TFormatPar *Par)
{
  lstrcpy(Par->FormatName,"Jagged Alliance");
  if(!Type || Type==1)
  {
    lstrcpy(Par->FormatName,"STI");
    if(Type)
      lstrcpy(Par->TypeName,"MultiPage");
    else
      lstrcpy(Par->TypeName,"Single");
    lstrcpy(Par->DefaultMasks,"*.sti");
    return TRUE;
  }
  return FALSE;
}

BOOL WINAPI _export CreatePicture(int Type, BYTE *Data, int DataSize)
{
  BYTE Header[]=
  {
    0x53, 0x54, 0x43, 0x49,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 8, 0, 8, 8,
    8, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0
  };
  ((StiHeader *)Header)->Type=Type?0x28:0x04;
  return (Data=GrvApi->Insert(Data, Header, sizeof(StiHeader), &DataSize)) != NULL;
}

int WINAPI _export DeletePage(int Index)
{
  if(HEADER->Type==0x04)
  {
    DataSize=GrvApi->Delete(PAGE, DataSize-sizeof(StiHeader));
    HEADER->Width=HEADER->Height=HEADER->SizePack=HEADER->Size0=0;
    return sizeof(StiHeader);
  }  
  BYTE *Disp=PAGES+FAT[Index].Disp;
  int Len=FAT[Index].Len;
//  GrvApi->MoveMem(FAT+Index,(BYTE *)(FAT+Index+1),Disp-(BYTE *)(FAT+Index+1));
  GrvApi->Delete(FAT+Index, sizeof(StiRecord));
//  GrvApi->MoveMem(Disp-sizeof(StiRecord), Disp+Len, Data+DataSize-Disp-Len);
  GrvApi->Delete(Disp-sizeof(StiRecord), Len);
  HEADER->Count--;
  HEADER->SizePack-=Len; //лучше изменить на повторный подсчет суммы длин
  for(int i=Index; i<HEADER->Count; i++)
    FAT[i].Disp-=Len;
  return DataSize-Len-sizeof(StiRecord);
}

int GetAdaptColor(BYTE *Pix)
{
  int dR,dG,dB;
  int Dist, MinDist=16777216;
  int Color=-1;
//  if(Pix[0]==PALETTE[0][2] && Pix[1]==PALETTE[0][1] && Pix[2]==PALETTE[0][0])
  if(Pix[2]==Red && Pix[1]==Green && Pix[0]==Blue)
//  if(Pix[0]==R && Pix[1]==G && Pix[2]==B)
    return -1;
//  for(int i=255; i/*>=0*/; i--)
  for(int i=0; i<256; i++) //для изготовления фонтов
  {
    dB=Pix[0]-PALETTE[i][2];
    dG=Pix[1]-PALETTE[i][1];
    dR=Pix[2]-PALETTE[i][0];
    Dist=dR*dR+dB*dB+dG*dG;
    if(Dist<MinDist)
    {
      MinDist=Dist;
      Color=i;
    }
  }
  return Color;
}

int Pack8(BYTE *Buf, BYTE *Line, int Width)
{
/*  GrvApi->MoveMem(Buf+1, Line, Len);
  Buf[Len+1]=0;
  Buf[0]=Len;
  return Len+2;*/

  BYTE x,z,*CountX;
  int Color;
  int j, Len;

  for(j=0, Len=0; ; )
  {
    for(z=0, x=0; j<Width && GetAdaptColor(Line+j*3)<0 && z<0x7f;j++,z++);
    if(z)
    {
      Buf[Len++]=0x80|z; 
      z=0; 
      continue;
    }
    CountX=&Buf[Len++];
    for(x=0;j<Width && (Color=GetAdaptColor(Line+j*3))>=0 && x<0x7f;j++)
    {
      Buf[Len++]=(BYTE)Color;
      x++;
    }
    if(x || j==Width) 
      *CountX=x; 
    if(x && j==Width)
      Buf[Len++]=0;
    if(j==Width) 
      break;
  }
  return Len;
}

void CreatePalette(BYTE *Lines[], int Width, int Height)
{
  int i,j,k,m;
  for(j=0, k=1; j<Height && k<256; j++)  
    for(i=0; i<Width*3 && k<256; i+=3)
    {
      for(m=1; m<k; m++)
        if(Lines[j][i]==PALETTE[m][2] && 
           Lines[j][i+1]==PALETTE[m][1] && 
           Lines[j][i+2]==PALETTE[m][0])          
          break;
      if(k==m)
      {
        PALETTE[k][2]=Lines[j][i];
        PALETTE[k][1]=Lines[j][i+1];
        PALETTE[k][0]=Lines[j][i+2];
        k++;
      }
    }
//  GrvApi->ShowInt("a","b",k);
}

BOOL CreateSinglePage(int Index, BYTE *Lines[], TPagePar *Par)
{
  DeletePage(0);
  Data=GrvApi->Insert((BYTE *)PAGE, NULL, Par->Height*Par->Width*2, &DataSize);
  if(Data)
  {
    Position=PAGE;
    for(int i=0; i<Par->Height; i++)
    {
//      GrvApi->MoveMem(Position, Lines[i], Par->Width*2);
      for(int j=0; j<Par->Width*3; Position++)
      {
        *Position=((Lines[i][j++]&0xf8)>>3);
        *Position=*Position+((Lines[i][j++]&0xf8)<<3);
        *Position=*Position+((Lines[i][j++]&0xf8)<<8);
      }
    }
    HEADER->Size0=HEADER->SizePack=Par->Height*Par->Width*2;
    HEADER->Height=Par->Height;
    HEADER->Width=Par->Width;
    return TRUE;
  }  
  return FALSE;
}

BOOL WINAPI _export CreatePage(int Index, BYTE *Lines[], TPagePar *Par)
{
  if(HEADER->Type==0x04)
    return CreateSinglePage(Index, Lines, Par);
  if(HEADER->Type==0x28 || HEADER->Type==0x29)
  {
    StiRecord Record;
    int PageLen;
    GrvApi->ZeroMem(&Record, sizeof(StiRecord));
    Record.Width=Par->Width;
    Record.Height=Par->Height;
    Record.DispX=Par->DispX;
    Record.DispY=Par->DispY;
    Red=Par->Red;
    Green=Par->Green;
    Blue=Par->Blue;
    if(!HEADER->Count)
    {
      Record.Disp=0;
      CreatePalette(Lines, Par->Width, Par->Height);
    }
    else
      if(Index==HEADER->Count)
        Record.Disp=FAT[Index-1].Disp+FAT[Index-1].Len;
      else
        Record.Disp=FAT[Index].Disp;
    BYTE Buf[4096];//  BYTE *Buf=new BYTE[Record.Width*3];
    PageLen=0;
    for(int i=0; i<Record.Height; i++)
    {
      int LineLen=Pack8(Buf, Lines[i], Record.Width);
      Data=GrvApi->Insert(&PAGES[Record.Disp+PageLen], Buf, LineLen, &DataSize);
      if(!Data)
        return FALSE;
      PageLen+=LineLen;
    }
    Record.Len=PageLen;
    Data=GrvApi->Insert(FAT+Index, &Record, sizeof(StiRecord), &DataSize);
    if(!Data)
      return FALSE;
    HEADER->Count++;
    HEADER->SizePack+=PageLen;
    for(int i=Index+1;i<HEADER->Count;i++)
      FAT[i].Disp+=PageLen;
  //  delete Buf;
    return TRUE;
  }
  return FALSE;
}
