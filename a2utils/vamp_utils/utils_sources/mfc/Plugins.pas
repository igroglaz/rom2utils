unit Plugins;

interface

uses kol, windows, data_classes_archive, data_classes_imagecomplex;

type
  TPlugin=class
  private
    fTitle: string;
    fVersion: string;
    fGameName: string;
    fFilterGroupName: string;
    fFilters: string;
    fExtentions: string;
    fComments: string;
    fPaletteName: string;
    fHandle: THandle;
  protected
  public
    GetPluginInfo: procedure(var Title,Version,GameName,FilterGroupName,Filters,Extentions,Comments: PChar);
    GetFileInfo: function(fName: PChar): Word;
    GetArchiveContent: procedure(fName: PChar; var fArchive: TArchive; fForm,fLabel,fProgressBar: PControl);
    UnpackArchive: procedure (fName: PChar; fArchive: TArchive; var fForm,fLabel,fProgressBar: PControl);
    GetFileAutoload: function (fName: PChar): Boolean;
    GetPluginID: function: Integer;
    GetImageComplex: procedure (fName, fPalName: PChar; var fImageComplex: TImageComplex; fForm,fLabel,fProgressBar: PControl);
    constructor Create(fLibName: string);
    property Title: string read fTitle;
    property Version: string read fVersion;
    property GameName: string read fGameName;
    property FilterGroupName: string read fFilterGroupName;
    property Filters: string read fFilters;
    property Extentions: string read fExtentions;
    property Comments: string read fComments;
    property PaletteName: string read fPaletteName write fPaletteName;
  end;

  TPluginList=class
  private
    fPlugin: PList;
  protected
    function GetItem(Index: Integer): TPlugin;
  public
    constructor Create;
    function Count: LongInt;
    procedure Add(fLibName: string);
    property Plugin[Index: Integer]: TPlugin read GetItem;
  end;

  function LoadPlugins: Integer;

var
  PluginList: TPluginList;

implementation

constructor TPlugin.Create(fLibName: string);
var
  cTitle,cVersion,cGameName,cFilterGroupName,cFilters,cExtentions,cComments: PChar;
begin
  fHandle:=LoadLibrary(PChar(fLibName));
  //Загрузка процедур
  @GetPluginInfo:=nil;
  @GetFileInfo:=nil;
  @GetArchiveContent:=nil;
  @UnpackArchive:=nil;
  @GetFileAutoload:=nil;
  @GetPluginID:=nil;
  @GetImageComplex:=nil;
  @GetPluginInfo:=GetProcAddress(fHandle,'GetPluginInfo');
  @GetFileInfo:=GetProcAddress(fHandle,'GetFileInfo');
  @GetArchiveContent:=GetProcAddress(fHandle,'GetArchiveContent');
  @UnpackArchive:=GetProcAddress(fHandle,'UnpackArchive');
  @GetFileAutoload:=GetProcAddress(fHandle,'GetFileAutoload');
  @GetPluginID:=GetProcAddress(fHandle,'GetPluginID');
  @GetImageComplex:=GetProcAddress(fHandle,'GetImageComplex');
  //Создание первычных данных о плагине
  GetPluginInfo(cTitle,cVersion,cGameName,cFilterGroupName,cFilters,cExtentions,cComments);
  fTitle:=string(cTitle);
  fVersion:=string(cVersion);
  fGameName:=string(cGameName);
  fFilterGroupName:=string(cFilterGroupName);
  fFilters:=string(cFilters);
  fExtentions:=string(cExtentions);
  fComments:=string(cComments);
end;

constructor TPluginList.Create;
begin
  fPlugin:=NewList;
end;

function TPluginList.Count;
begin
  Result:=fPlugin.Count;
end;

procedure TPluginList.Add;
begin
  fPlugin.Add(TPlugin.Create(fLibName));
end;

function TPluginList.GetItem(Index: Integer): TPlugin;
begin
  Result:=fPlugin.Items[Index];
end;

//Процедура загрузки плагинов
function LoadPlugins: Integer;
var
  fTemp: PDirList;
  i,j: LongInt;
  Initialize: function: Boolean;
  fGetPluginInfo: procedure(var Title,Version,GameName,FilterGroupName,Filters,Extentions,Comments: PChar);
  fLibHandle: THandle;
  Errors: Integer;
begin
  fTemp:=NewDirList('Plugins','*.dll',FILE_ATTRIBUTE_NORMAL and not FILE_ATTRIBUTE_DIRECTORY);
  if fTemp.Count=0 then
  begin
    ShowMessage('Не обнаружено ни одного плагина! Работа программы будет завершена.');
    Halt;
  end;
  PluginList:=TPluginList.Create;
  j:=0;
  Errors:=0;
  for i:=0 to fTemp.Count-1 do
  begin
    @fGetPluginInfo:=nil;
    fLibHandle:=LoadLibrary(PChar('Plugins\'+fTemp.Names[i]));
    if fLibHandle>=32 then
    begin
      fGetPluginInfo:=GetProcAddress(fLibHandle,'GetPluginInfo');
      if @fGetPluginInfo<>nil then
      begin
        @Initialize:=nil;
        @Initialize:=GetProcAddress(fLibHandle,'Initialize');
        if (@Initialize=nil)or(Initialize) then
        begin
          FreeLibrary(fLibHandle);
          PluginList.Add('Plugins\'+fTemp.Names[i]);
          Inc(j);
          @fGetPluginInfo:=nil;
        end else
        begin
          FreeLibrary(fLibHandle);
          @fGetPluginInfo:=nil;
        end;
      end;
    end else
    begin
      Inc(Errors);
      FreeLibrary(fLibHandle);
    end;
  end;
  Result:=Errors;
end;

end.
