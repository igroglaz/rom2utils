{ KOL MCK } // Do not remove this line!
{$DEFINE KOL_MCK}
unit MainUnit;

interface

{$IFDEF KOL_MCK}
uses Windows, Messages, ShellAPI, KOL, KOLSPCDriveComboBox, KOLSPCFilterComboBox, KOLSPCFileListBox, KOLSPCDirectoryListBox {$IFNDEF KOL_MCK}, mirror, Classes,  Controls, mckCtrls, mckSPCFileListBox,  mckSPCFilterComboBox, mckSPCDirectoryListBox, mckSPCDriveComboBox, mckObjs{$ENDIF}, Plugins, data_classes_archive, data_classes_imagecomplex, cases;
{$ELSE}
{$I uses.inc}
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;
{$ENDIF}

type
  {$IFDEF KOL_MCK}
  {$I MCKfakeClasses.inc}
  PForm1 = ^TForm1;
  TForm1 = object(TObj)
    Form: PControl;
  {$ELSE not_KOL_MCK}
  TForm1 = class(TForm)
  {$ENDIF KOL_MCK}
    KOLProject1: TKOLProject;
    KOLForm1: TKOLForm;
    KOLApplet1: TKOLApplet;
    Panel1: TKOLPanel;
    SPCDriveComboBox1: TSPCDriveComboBox;
    SPCFilterComboBox1: TSPCFilterComboBox;
    PANEL_ARCHIVE: TKOLPanel;
    MainMenu1: TKOLMainMenu;
    GroupBox1: TKOLGroupBox;
    ListView1: TKOLListView;
    Panel2: TKOLPanel;
    GroupBox3: TKOLGroupBox;
    GroupBox2: TKOLGroupBox;
    SPCFileListBox1: TSPCFileListBox;
    SPCDirectoryListBox1: TSPCDirectoryListBox;
    Splitter1: TKOLSplitter;
    Button1: TKOLButton;
    Panel3: TKOLPanel;
    Label1: TKOLLabel;
    Label2: TKOLLabel;
    Label3: TKOLLabel;
    Label4: TKOLLabel;
    Label5: TKOLLabel;
    Label6: TKOLLabel;
    Button2: TKOLButton;
    Button3: TKOLButton;
    Button4: TKOLButton;
    Button5: TKOLButton;
    PANEL_FILTERS: TKOLPanel;
    Label7: TKOLLabel;
    ComboBox1: TKOLComboBox;
    OpenDirDialog1: TKOLOpenDirDialog;
    PANEL_IMAGECOMPLEX: TKOLPanel;
    Panel5: TKOLPanel;
    GroupBox4: TKOLGroupBox;
    GroupBox5: TKOLGroupBox;
    Panel4: TKOLPanel;
    ListView2: TKOLListView;
    Label8: TKOLLabel;
    Label9: TKOLLabel;
    GroupBox6: TKOLGroupBox;
    ScrollBox1: TKOLScrollBox;
    PaintBox1: TKOLPaintBox;
    Button6: TKOLButton;
    Button7: TKOLButton;
    OpenSaveDialog1: TKOLOpenSaveDialog;
    Button8: TKOLButton;
    Button9: TKOLButton;
    procedure ClearAllViews;
    procedure ExecuteSaveImages(fAll: Boolean);
    procedure ExecuteUnpack(fAll: Boolean);
    procedure KOLForm1FormCreate(Sender: PObj);
    procedure SPCFileListBox1SelChange(Sender: PObj);
    procedure SPCFilterComboBox1Change(Sender: PObj);
    procedure PrepairFor(fFlags: Word);
    procedure PANEL_ARCHIVEShow(Sender: PObj);
    procedure Button1Click(Sender: PObj);
    procedure KOLForm1Show(Sender: PObj);
    procedure PANEL_ARCHIVEHide(Sender: PObj);
    procedure Button2Click(Sender: PObj);
    procedure Button3Click(Sender: PObj);
    procedure ComboBox1Change(Sender: PObj);
    procedure MainMenu1N3Menu(Sender: PMenu; Item: Integer);
    procedure Button4Click(Sender: PObj);
    procedure Button5Click(Sender: PObj);
    procedure MainMenu1N6Menu(Sender: PMenu; Item: Integer);
    procedure KOLForm1N7Menu(Sender: PMenu; Item: Integer);
    procedure KOLForm1N8Menu(Sender: PMenu; Item: Integer);
    procedure KOLForm1N9Menu(Sender: PMenu; Item: Integer);
    procedure KOLForm1Close(Sender: PObj; var Accept: Boolean);
    procedure SPCDirectoryListBox1Change(Sender: PObj);
    procedure PANEL_IMAGECOMPLEXShow(Sender: PObj);
    procedure PaintBox1Paint(Sender: PControl; DC: HDC);
    procedure ListView2Click(Sender: PObj);
    procedure ListView2LVStateChange(Sender: PControl; IdxFrom,
      IdxTo: Integer; OldState, NewState: Cardinal);
    procedure PANEL_IMAGECOMPLEXHide(Sender: PObj);
    procedure Button6Click(Sender: PObj);
    procedure Button7Click(Sender: PObj);
    procedure PaintBox1Click(Sender: PObj);
    procedure KOLForm1N11Menu(Sender: PMenu; Item: Integer);
    procedure KOLForm1N13Menu(Sender: PMenu; Item: Integer);
    procedure Button8Click(Sender: PObj);
    procedure Button9Click(Sender: PObj);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  procedure LoadINI;
  procedure SaveINI;
  function MakeTree(Dir: string): Boolean;

const
  FT_ARCHIVE                    = $0001;
  FT_IMAGESIMPLE                = $0002;
  FT_IMAGESIMPLECUSTOMALETTE    = $0004;
  FT_IMAGECOMPLEX               = $0008;
  FT_IMAGECOMPLEXCUSTOMPALETTE  = $0010;

  AUTOLOAD_NEVER                = $00;
  AUTOLOAD_AUTO                 = $01;
  AUTOLOAD_ALWAYS               = $02;

  INI_NAME='mfcconverter.ini';

var
  Form1 {$IFDEF KOL_MCK} : PForm1 {$ELSE} : TForm1 {$ENDIF} ;
  CurPlugin,CurFilter,CurImage: Integer;
  StartDir: string;
  Shown: Boolean;
  CurDir,CurSaveDir,CurImageSaveDir: string;
  Class_archive: TArchive;
  Class_ImageComplex: TImageComplex;
  Ini: PIniFile;

  Options: record
             AutoloadMode: Integer;
             LastPath: string;
             LastPluginID: LongInt;
           end;

{$IFDEF KOL_MCK}
procedure NewForm1( var Result: PForm1; AParent: PControl );
{$ENDIF}

implementation

uses plugins_form, Waiting, about;

{$IFNDEF KOL_MCK} {$R *.DFM} {$ENDIF}

{$IFDEF KOL_MCK}
{$I MainUnit_1.inc}
{$ENDIF}

{Построение дерева каталогов}
function MakeTree(Dir: string): Boolean;
begin
  Result := True;
  if Length(Dir) = 0 then Exit;
  if Dir[Length(Dir)]='\' then Delete(Dir,Length(Dir),1);
  if (Length(Dir) < 3) or DirectoryExists(Dir) or (ExtractFilePath(Dir) = Dir) then Exit; // avoid 'xyz:\' problem.
  Result := MakeTree(ExtractFilePath(Dir)) and CreateDir(Dir);
end;

procedure TForm1.KOLForm1FormCreate(Sender: PObj);
var
  Errors,i: Integer;
begin
  //Загрузка плагинов
  Errors:=LoadPlugins;
  CurPlugin:=0;
  //Загружаем фильтры
  for i:=1 to PluginList.Count do
  begin
    SPCFilterComboBox1.Add(PluginList.Plugin[i-1].FilterGroupName+'|'+PluginList.Plugin[i-1].Filters);
  end;
  //Применяем первый по списку фильтр
  GetDir(0,CurDir);
  GetDir(0,StartDir);
  CurSaveDir:=CurDir;
  CurImageSaveDir:=CurDir;
//  SPCDriveComboBox1.Drive:=CurDir[1];
//  SPCDirectoryListBox1.Path:=CurDir;
  //Создание полей в списке состава псевдо-архивов
  ListView1.LVColAdd('Имя файла',taLeft,130);
  ListView1.LVColAdd('Размер (в архиве)',taLeft,80);
  ListView1.LVColAdd('Размер (реальный)',taLeft,80);
  //Создание полей в списке спрайтов составных изображений
  ListView2.LVColAdd('Имя',taLeft,100);
  ListView2.LVColAdd('Размер',taLeft,54);
  LoadINI;
  //Применяем настройки
  if not DirectoryExists(Options.LastPath) then Options.LastPath:=CurDir;
  SPCDriveComboBox1.Drive:=Options.LastPath[1];
  SPCDirectoryListBox1.Path:=Options.LastPath;
  SPCFileListBox1.Filters:=SPCFilterComboBox1.Filter;
  case Options.AutoloadMode of
    AUTOLOAD_NEVER: MainMenu1.RadioCheck(4);
    AUTOLOAD_AUTO: MainMenu1.RadioCheck(5);
    AUTOLOAD_ALWAYS: MainMenu1.RadioCheck(6);
  end;
end;

procedure TForm1.SPCFileListBox1SelChange(Sender: PObj);
var
  fFileInfo: Word;
begin
  if FileSize(SPCFileListBox1.GetFullFileName)=0 then
  begin
    Form.SimpleStatusText:='Файл поврежден!';
    Exit;
  end;
  Form.SimpleStatusText:='';
  fFileInfo:=PluginList.Plugin[CurPlugin].GetFileInfo(PChar(SPCFileListBox1.GetFullFileName));
  PrepairFor(fFileInfo);
end;

procedure TForm1.SPCFilterComboBox1Change(Sender: PObj);
begin
  if CurPlugin<>SPCFilterComboBox1.CurIndex then
  begin
    CurPlugin:=SPCFilterComboBox1.CurIndex;
    ClearAllViews;
    Options.LastPluginID:=PluginList.Plugin[SPCFilterComboBox1.CurIndex].GetPluginID;
  end;
end;

procedure TForm1.PrepairFor(fFlags: Word);
var
  i: Integer;
begin
  ClearAllViews;
  //Отображаем нужную панель и действуем по обстоятельствам
  case fFlags of
  FT_ARCHIVE:begin
               PANEL_ARCHIVE.Visible:=True;
               PANEL_ARCHIVE.BringToFront;
             end;
  FT_IMAGESIMPLE:;
  FT_IMAGECOMPLEX,FT_IMAGECOMPLEXCUSTOMPALETTE:begin
                    PANEL_IMAGECOMPLEX.Visible:=True;
                    PANEL_IMAGECOMPLEX.BringToFront;
                  end;
  else ShowMessage('Неизвестная ошибка!');
  end;
end;

procedure TForm1.PANEL_ARCHIVEShow(Sender: PObj);
begin
  //Инициализируем состояние кнопок и надписей
  PANEL_FILTERS.Visible:=False;
  Button1.Enabled:=True;
  Button2.Enabled:=False;
  Button3.Enabled:=False;
  Button4.Enabled:=False;
  Button5.Enabled:=False;
  Label4.Caption:='N/A';
  Label5.Caption:='N/A';
  Label6.Caption:='N/A';
  case Options.AutoloadMode of
    AUTOLOAD_NEVER:;
    AUTOLOAD_AUTO: if PluginList.Plugin[CurPlugin].GetFileAutoload(PChar(SPCFileListBox1.GetFullFileName)) then Button1.OnClick(@Self);
    AUTOLOAD_ALWAYS: Button1.OnClick(@Self);
  end;
end;

procedure TForm1.Button1Click(Sender: PObj);
var
  i,j: Integer;
  TotalSizeCompressed: LongInt;
  TotalSizeReal: LongInt;
begin
  //Загрузка базы
  PluginList.Plugin[CurPlugin].GetArchiveContent(PChar(SPCFileListBox1.GetFullFileName),Class_Archive,Form3.Form,Form3.Label1,Form3.ProgressBar1);
  //Создание списка
  TotalSizeCompressed:=0;
  TotalSizeReal:=0;
  Form3.Form.Show;
  Form3.Label1.Caption:='Создаем список файлов...';
  Form3.ProgressBar1.Visible:=False;
  Form3.Form.Update;
  for i:=1 to Class_Archive.TotalFiles do
  begin
    j:=ListView1.LVItemAdd(Class_Archive.Items[i-1].Name);
    ListView1.LVSetItem(j,1,Int2Str(Class_Archive.Items[i-1].SizeCompressed),0,[],0,0,0);
    ListView1.LVSetItem(j,2,Int2Str(Class_Archive.Items[i-1].SizeReal),0,[],0,0,0);
    TotalSizeCompressed:=TotalSizeCompressed+Class_Archive.Items[i-1].SizeCompressed;
    TotalSizeReal:=TotalSizeReal+Class_Archive.Items[i-1].SizeReal;
  end;
  Form3.ProgressBar1.Visible:=True;
  //Создание списка фильтров
  if Class_Archive.FlagUseFilters then
  begin
    PANEL_FILTERS.Visible:=True;
    ComboBox1.Clear;
    ComboBox1.Add('*.*');
    for i:=1 to Class_Archive.CountFilters do ComboBox1.Add(Class_Archive.Filters[i-1].Filter);
  end else PANEL_FILTERS.Visible:=False;
  CurFilter:=0;
  Form3.Form.Hide;
  ListView1.LVCurItem:=0;
  Label4.Caption:=Int2Ths(Class_Archive.TotalFiles);
  Label5.Caption:=Int2Ths(TotalSizeCompressed);
  Label6.Caption:=Int2Ths(TotalSizeReal);
  Button1.Enabled:=False;
  Button2.Enabled:=True;
  Button3.Enabled:=True;
  Button4.Enabled:=True;
  Button5.Enabled:=True;
end;

procedure TForm1.KOLForm1Show(Sender: PObj);
var
  i: Integer;
begin
  //Создание классов
  PANEL_ARCHIVE.Align:=caClient;
  PANEL_IMAGECOMPLEX.Align:=caClient;
  Class_Archive:=TArchive.Create;
  Class_ImageComplex:=TImageComplex.Create;

  for i:=0 to PluginList.Count-1 do
  if PluginList.Plugin[i].GetPluginID=Options.LastPluginID then
  SPCFilterComboBox1.CurIndex:=i;
  SPCFilterComboBox1.Update;
end;

procedure TForm1.PANEL_ARCHIVEHide(Sender: PObj);
begin
  //Очищаем список файлов
  ListView1.Clear;
end;

procedure TForm1.Button2Click(Sender: PObj);
begin
  ListView1.LVSelectAll;
end;

procedure TForm1.Button3Click(Sender: PObj);
var
  i: Integer;
begin
  for i:=1 to ListView1.LVCount do if (lvisSelect in ListView1.LVItemState[i-1]) then ListView1.LVItemState[i-1]:=[] else ListView1.LVItemState[i-1]:=[lvisSelect];
end;

procedure TForm1.ComboBox1Change(Sender: PObj);
var
  i,j,z: Integer;
  Allow: Boolean;
  TotalSizeCompressed: LongInt;
  TotalSizeReal: LongInt;
begin
  if ComboBox1.CurIndex=CurFilter then Exit;
  //Показываем форму ожидания
  Form3.Form.Show;
  Form3.Label1.Caption:='Применяем новый фильтр...';
  Form3.ProgressBar1.Visible:=False;
  Form3.Form.Update;
  //Применяем фильтр
  CurFilter:=ComboBox1.CurIndex;
  ListView1.Clear;
  TotalSizeCompressed:=0;
  TotalSizeReal:=0;
  z:=0;
  for i:=1 to Class_Archive.TotalFiles do
  begin
    if CurFilter=0 then Allow:=True else if CurFilter-1=Class_Archive.Items[i-1].FilterIndex then Allow:=True else Allow:=False;
    if Allow then
    begin
      j:=ListView1.LVItemAdd(Class_Archive.Items[i-1].Name);
      ListView1.LVSetItem(j,1,Int2Str(Class_Archive.Items[i-1].SizeCompressed),0,[],0,0,0);
      if Class_Archive.Items[i-1].SizeReal=-1 then ListView1.LVSetItem(j,2,'N/A',0,[],0,0,0) else ListView1.LVSetItem(j,2,Int2Str(Class_Archive.Items[i-1].SizeReal),0,[],0,0,0);
      TotalSizeCompressed:=TotalSizeCompressed+Class_Archive.Items[i-1].SizeCompressed;
      TotalSizeReal:=TotalSizeReal+Class_Archive.Items[i-1].SizeReal;
      Inc(z);
    end;
  end;
  Label4.Caption:=Int2Ths(z);
  Label5.Caption:=Int2Ths(TotalSizeCompressed);
  Label6.Caption:=Int2Ths(TotalSizeReal);
  //Скрываем форму ожидания
  Form3.ProgressBar1.Visible:=True;
  Form3.Form.Hide;
end;

procedure TForm1.MainMenu1N3Menu(Sender: PMenu; Item: Integer);
begin
  Form.Close;
end;

procedure TForm1.Button4Click(Sender: PObj);
var
  i: Integer;
begin
  //Извлечение всего списка с учетом фильтра
  ExecuteUnpack(True);
end;

procedure TForm1.Button5Click(Sender: PObj);
var
  i,Selected: Integer;
begin
  //Извлечение выделенного с учетом фильтра
  Selected:=0;
  for i:=0 to ListView1.LVCount-1 do if lvisSelect in ListView1.LVItemState[i] then Inc(Selected);
  if Selected>0 then
  begin
    Form.SimpleStatusText:='';
    ExecuteUnpack(False)
  end
  else Form.SimpleStatusText:='Не выделено ни одного файла!';
end;

procedure LoadINI;
var
  z: Integer;
  Found: Boolean;
  i: Integer;
begin
  if not FileExists(INI_NAME) then
  begin
    Options.AutoloadMode:=AUTOLOAD_AUTO;
    Options.LastPath:=CurDir;
    SaveINI;
    Exit;
  end;
  INI:=OpenIniFile(CurDir+'\'+INI_NAME);
  INI.Section:='General';
  INI.Mode:=ifmRead;
  Options.AutoloadMode:=Ini.ValueInteger('AutoloadMode',1);
  Options.LastPath:=Ini.ValueString('LastPath','');
  Options.LastPluginID:=Ini.ValueInteger('LastPluginID',-1);
  Found:=False;
  for i:=0 to PluginList.Count-1 do
  begin
    if PluginList.Plugin[i].GetPluginID=Options.LastPluginID then
    begin
      Found:=True;
      Break;
    end;
  end;
  if not Found then
  begin
    Options.LastPluginID:=PluginList.Plugin[0].GetPluginID;
  end;
  for z:=1 to PluginList.Count do
  begin
    INI.Section:='Plugin'+BuildStr(Int2Str(PluginList.Plugin[z-1].GetPluginID),'0',3);
    PluginList.Plugin[z-1].PaletteName:=Ini.ValueString('CurrentPalette','');
  end;
  INI.Free;
end;

procedure SaveINI;
var
  z: Integer;
begin
  INI:=OpenIniFile(CurDir+'\'+INI_NAME);
  INI.Section:='General';
  INI.Mode:=ifmWrite;
  Ini.ValueInteger('AutoloadMode',Options.AutoloadMode);
  Ini.ValueInteger('LastPluginID',Options.LastPluginID);
  Ini.ValueString('LastPath',Options.LastPath);
  for z:=1 to PluginList.Count do
  begin
    INI.Section:='Plugin'+BuildStr(Int2Str(PluginList.Plugin[z-1].GetPluginID),'0',3);
    Ini.ValueString('CurrentPalette',PluginList.Plugin[z-1].PaletteName);
  end;
  INI.Free;
end;

procedure TForm1.MainMenu1N6Menu(Sender: PMenu; Item: Integer);
begin
  Form2.Form.ShowModal;
  Form2.Form.Hide;
end;

procedure TForm1.KOLForm1N7Menu(Sender: PMenu; Item: Integer);
begin
  Options.AutoloadMode:=AUTOLOAD_NEVER;
end;

procedure TForm1.KOLForm1N8Menu(Sender: PMenu; Item: Integer);
begin
  Options.AutoloadMode:=AUTOLOAD_AUTO;
end;

procedure TForm1.KOLForm1N9Menu(Sender: PMenu; Item: Integer);
begin
  Options.AutoloadMode:=AUTOLOAD_ALWAYS;
end;

procedure TForm1.KOLForm1Close(Sender: PObj; var Accept: Boolean);
begin
  SaveINI;
  Halt;
end;

procedure TForm1.SPCDirectoryListBox1Change(Sender: PObj);
begin
  Options.LastPath:=SPCDirectoryListBox1.Path;
end;

procedure TForm1.ClearAllViews;
begin
  //Очищаем предыдущие данные
  //Очищаем данные предыдущего архива
  Class_Archive.Clear;
  Class_Archive.ClearFilters;
  Class_ImageComplex.Clear;
  //Скрываем все Панели
  PANEL_ARCHIVE.Visible:=False;
  PANEL_IMAGECOMPLEX.Visible:=False;
  CurImage:=-1;
end;

procedure TForm1.PANEL_IMAGECOMPLEXShow(Sender: PObj);
var
  i,j: Integer;
begin
  Label9.Caption:='N/A';
  ListView2.Clear;
  CurImage:=-1;
  PluginList.Plugin[CurPlugin].GetImageComplex(PChar(SPCFileListBox1.GetFullFileName), PChar(''), Class_ImageComplex,Form3.Form,Form3.Label1,Form3.ProgressBar1);
  if Class_ImageComplex.Count=0 then Exit;
  for i:=1 to Class_ImageComplex.Count do
  begin
    j:=ListView2.LVItemAdd(Class_ImageComplex.Items[i-1].Name);
    ListView2.LVSetItem(j,1,Int2Str(Class_ImageComplex.Items[i-1].Width)+'x'+Int2Str(Class_ImageComplex.Items[i-1].Height),0,[],0,0,0);
  end;
  ListView2.LVCurItem:=0;
  Label9.Caption:=Int2Str(Class_ImageComplex.Count);
  Button6.Enabled:=True;
  Button7.Enabled:=True;
end;

procedure TForm1.PaintBox1Paint(Sender: PControl; DC: HDC);
begin
  if CurImage>-1 then
  begin
    PaintBox1.Width:=Class_ImageComplex.Items[CurImage].Bitmap.Width;
    PaintBox1.Height:=Class_ImageComplex.Items[CurImage].Bitmap.Height;
    Class_ImageComplex.Items[CurImage].Bitmap.Draw(PaintBox1.Canvas.Handle,0,0);
  end else
  begin
    PaintBox1.Width:=0;
    PaintBox1.Height:=0;
  end;
end;

procedure TForm1.ListView2Click(Sender: PObj);
begin
  if ListView2.LVCurItem=-1 then Exit;
  CurImage:=ListView2.LVCurItem;
  PaintBox1.OnPaint(@Self,PaintBox1.Handle);
  PaintBox1.Update;
end;

procedure TForm1.ListView2LVStateChange(Sender: PControl; IdxFrom,
  IdxTo: Integer; OldState, NewState: Cardinal);
begin
  ListView2.OnClick(@Self);
end;

procedure TForm1.PANEL_IMAGECOMPLEXHide(Sender: PObj);
begin
  Button6.Enabled:=False;
  Button7.Enabled:=False;
end;

procedure TForm1.Button6Click(Sender: PObj);
begin
  Form.SimpleStatusText:='';
  ExecuteSaveImages(True);
end;

procedure TForm1.ExecuteSaveImages(fAll: Boolean);
var
  fPath: string;
  i: Integer;
begin
  OpenDirDialog1.InitialPath:=CurSaveDir;
  if OpenDirDialog1.Execute then
  begin
    Form3.Form.Show;
    Form3.Label1.Caption:='Сохраняем изображения...';
    Form3.ProgressBar1.Visible:=True;
    Form3.Form.Update;
    for i:=1 to Class_ImageComplex.Count do
    begin
      if fAll or ((not fAll)and(lvisSelect in ListView2.LVItemState[i-1]))then
      begin
        fPath:=OpenDirDialog1.Path+ExtractFilePath(ListView2.LVItems[i-1,0]);
        if not DirectoryExists(fPath) then MakeTree(fPath);
        Class_ImageComplex.Items[i-1].Bitmap.SaveToFile(OpenDirDialog1.Path+'\'+ListView2.LVItems[i-1,0]);
      end;
      Form3.ProgressBar1.Progress:=Round(Int(100*(i/Class_ImageComplex.Count)));
    end;
    CurSaveDir:=OpenDirDialog1.Path;
    Form3.Form.Hide;
  end;
end;

procedure TForm1.ExecuteUnpack(fAll: Boolean);
var
  i,z: Integer;
begin
  if CurFilter=0 then
  begin
    for i:=1 to Class_Archive.TotalFiles do
    if fAll or ((not fAll)and (lvisSelect in ListView1.LVItemState[i-1]))then Class_Archive.Items[i-1].FlagExtract:=True else Class_Archive.Items[i-1].FlagExtract:=False;
  end
  else
  begin
    z:=-1;
    for i:=1 to Class_Archive.TotalFiles do
    if CurFilter-1=Class_Archive.Items[i-1].FilterIndex then
    begin
      Inc(z);
      if fAll or ((not fAll)and(lvisSelect in ListView1.LVItemState[z]))then Class_Archive.Items[i-1].FlagExtract:=True else Class_Archive.Items[i-1].FlagExtract:=False;
    end else Class_Archive.Items[i-1].FlagExtract:=False;
  end;
  OpenDirDialog1.InitialPath:=CurSaveDir;
  if OpenDirDialog1.Execute then
  begin
    PluginList.Plugin[CurPlugin].UnpackArchive(PChar(OpenDirDialog1.Path), Class_Archive,Form3.Form,Form3.Label1,Form3.ProgressBar1);
    CurSaveDir:=OpenDirDialog1.Path;
  end;
end;

procedure TForm1.Button7Click(Sender: PObj);
var
  i,Selected: Integer;
begin
  Selected:=0;
  for i:=0 to ListView2.LVCount-1 do if lvisSelect in ListView2.LVItemState[i] then Inc(Selected);
  if Selected>0 then
  begin
    Form.SimpleStatusText:='';
    ExecuteSaveImages(False)
  end
  else Form.SimpleStatusText:='Не выделено ни одного изображения!';
end;

procedure TForm1.PaintBox1Click(Sender: PObj);
begin
  if not (CurImageSaveDir[Length(CurImageSaveDir)]='\') then CurImageSaveDir:=CurImageSaveDir+'\';
  OpenSaveDialog1.InitialDir:=CurImageSaveDir;
  OpenSaveDialog1.FileName:=CurImageSaveDir+Class_ImageComplex.Items[CurImage].Name;
//  OpenSaveDialog1.FileName:=CurImageSaveDir+ListView2.LVItems[ListView2.LVCurItem,0];
  if OpenSaveDialog1.Execute then
  begin
    CurImageSaveDir:=ExtractFilePath(OpenSaveDialog1.Filename);
    Class_ImageComplex.Items[ListView2.LVCurItem].Bitmap.SaveToFile(OpenSaveDialog1.FileName);
  end;
end;

procedure TForm1.KOLForm1N11Menu(Sender: PMenu; Item: Integer);
begin
  ShellExecuteA(Form.Handle,nil,PChar(StartDir+'\readme.rtf'),'','',SW_SHOW);
end;

procedure TForm1.KOLForm1N13Menu(Sender: PMenu; Item: Integer);
begin
  Form4.Form.ShowModal;
  Form4.Form.Hide;
end;

procedure TForm1.Button8Click(Sender: PObj);
begin
  ListView2.LVSelectAll;
end;

procedure TForm1.Button9Click(Sender: PObj);
var
  i: Integer;
begin
  for i:=1 to ListView2.LVCount do if (lvisSelect in ListView2.LVItemState[i-1]) then ListView2.LVItemState[i-1]:=[] else ListView2.LVItemState[i-1]:=[lvisSelect];
end;

end.

