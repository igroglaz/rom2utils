{ KOL MCK } // Do not remove this line!
{$DEFINE KOL_MCK}
unit plugins_form;

interface

{$IFDEF KOL_MCK}
uses Windows, Messages, ShellAPI, KOL {$IFNDEF KOL_MCK}, mirror, Classes,
  Controls, mckCtrls {$ENDIF}, plugins;
{$ELSE}
{$I uses.inc}
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;
{$ENDIF}

type
  {$IFDEF KOL_MCK}
  {$I MCKfakeClasses.inc}
  PForm2 = ^TForm2;
  TForm2 = object(TObj)
    Form: PControl;
  {$ELSE not_KOL_MCK}
  TForm2 = class(TForm)
  {$ENDIF KOL_MCK}
    KOLForm1: TKOLForm;
    ListView1: TKOLListView;
    Button1: TKOLButton;
    procedure Button1Click(Sender: PObj);
    procedure KOLForm1Close(Sender: PObj; var Accept: Boolean);
    procedure KOLForm1FormCreate(Sender: PObj);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2 {$IFDEF KOL_MCK} : PForm2 {$ELSE} : TForm2 {$ENDIF} ;

{$IFDEF KOL_MCK}
procedure NewForm2( var Result: PForm2; AParent: PControl );
{$ENDIF}

implementation

{$IFNDEF KOL_MCK} {$R *.DFM} {$ENDIF}

{$IFDEF KOL_MCK}
{$I plugins_form_1.inc}
{$ENDIF}

procedure TForm2.Button1Click(Sender: PObj);
begin
  Form.ModalResult:=1;
end;

procedure TForm2.KOLForm1Close(Sender: PObj; var Accept: Boolean);
begin
  Accept:=False;
  Form.ModalResult:=-1;
end;

procedure TForm2.KOLForm1FormCreate(Sender: PObj);
var
  i,j: LongInt;
begin
  ListView1.LVColAdd('Название',taLeft,140);
  ListView1.LVColAdd('Версия',taLeft,48);
  ListView1.LVColAdd('Игра',taLeft,120);
  ListView1.LVColAdd('Форматы',taLeft,80);
  ListView1.LVColAdd('Комментарии',taLeft,200);
  //Загрузка сведений о плагинах
  for i:=1 to PluginList.Count do
  begin
    j:=ListView1.LVItemAdd(PluginList.Plugin[i-1].Title);
    ListView1.LVSetItem(j,1,PluginList.Plugin[i-1].Version,0,[],0,0,0);
    ListView1.LVSetItem(j,2,PluginList.Plugin[i-1].GameName,0,[],0,0,0);
    ListView1.LVSetItem(j,3,PluginList.Plugin[i-1].Extentions,0,[],0,0,0);
    ListView1.LVSetItem(j,4,PluginList.Plugin[i-1].Comments,0,[],0,0,0);
  end;
end;

end.


