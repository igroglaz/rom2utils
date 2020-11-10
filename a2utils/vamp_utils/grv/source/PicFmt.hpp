#ifndef PICFMT27052000
#define PICFMT27052000

#define MAX_INFO_LEN 128

struct TPicPar   //Параметры имаджа
{
  int Type;        //Тип.
  int NumPages;    //Число страниц имаджа
  int Depth;       //Глубина цвета (bit).
  int Transparent; //Наличие прозрачности.
  int Red;         //Компоненты цвета, который считается прозрачным.
  int Green;
  int Blue;
};

struct TPagePar    //Структура, параметров страницы имаджа.
{
  int Width;         //Ширина и высота, страницы.
  int Height;
  int DispX;         //Смещение странички. Довольно часто используются,
  int DispY;         //если странички - кадры анимации.
  int Red;           //Компоненты цвета, который считается прозрачным.
  int Green;         //Используются только при вставке новой страницы в имадж.
  int Blue;
};

struct TFormatPar
{
  char Game[MAX_INFO_LEN]; //нужно для чтения каталога игры из настроек
  char FormatName[MAX_INFO_LEN];
  char TypeName[MAX_INFO_LEN];
  char DefaultMasks[MAX_INFO_LEN];
  char Coment[MAX_INFO_LEN];
  int Depth;
  int Transparent;
  int CreateFlag;
};

struct TPluginInfo
{
};

struct TGrvApiInfo
{
  void(* ShowInt)(char *Title, char *Caption0, int Var0, char *Caption1=NULL, int Var1=0);
  VOID(* MoveMem)(PVOID Destination, CONST VOID *Source, DWORD Length);
  VOID(* ZeroMem)(PVOID Destination, DWORD Length);
  
  BYTE *(* Insert)(PVOID Dest, PVOID Source, int Len, int *NewDataSize);
  //возвращает новый адрес и размер рисунка. NULL, если ошибка.

  int (* Delete)(PVOID Dest, int Len);
};

extern "C" {

  BOOL WINAPI _export IsPicture(char *Name, /*char *Ext,*/ BYTE *Data, int DataSize);
  BOOL WINAPI _export GetFormatInfo(int Type, struct TFormatPar *Par);
  BOOL WINAPI _export GetPluginInfo(TPluginInfo *Info, TGrvApiInfo *ApiInfo);

  BOOL WINAPI _export OpenPicture(BYTE *Data, int DataSize, struct TPicPar *Par);
  BOOL WINAPI _export CreatePicture(int Type, BYTE *Data, int DataSize);

  int WINAPI _export OpenPage(int Page, struct TPagePar *Par);
  int WINAPI _export CreatePage(int Index, BYTE *Lines[], struct TPagePar *Par);
  int WINAPI _export DeletePage(int Index);

  BOOL WINAPI _export GetLine(BYTE *Line, int Width);

  BOOL WINAPI _export GetPalette(PALETTEENTRY *Pal);
  //BOOL WINAPI _export SetPalette(PALETTEENTRY *Pal);

  BOOL WINAPI _export ClosePage();
  BOOL WINAPI _export ClosePicture();

}
#endif
