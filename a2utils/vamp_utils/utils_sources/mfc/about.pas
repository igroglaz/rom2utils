{ KOL MCK } // Do not remove this line!
{$DEFINE KOL_MCK}
unit about;

interface

{$IFDEF KOL_MCK}
uses Windows, Messages, ShellAPI, KOL {$IFNDEF KOL_MCK}, mirror, Classes,
  Controls, mckCtrls {$ENDIF};
{$ELSE}
{$I uses.inc}
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;
{$ENDIF}

type
  {$IFDEF KOL_MCK}
  {$I MCKfakeClasses.inc}
  PForm4 = ^TForm4;
  TForm4 = object(TObj)
    Form: PControl;
  {$ELSE not_KOL_MCK}
  TForm4 = class(TForm)
  {$ENDIF KOL_MCK}
    KOLForm1: TKOLForm;
    Button1: TKOLButton;
    LabelEffect1: TKOLLabelEffect;
    Label1: TKOLLabel;
    Label2: TKOLLabel;
    Label3: TKOLLabel;
    Label4: TKOLLabel;
    Label5: TKOLLabel;
    Label6: TKOLLabel;
    Label7: TKOLLabel;
    Label8: TKOLLabel;
    procedure KOLForm1Close(Sender: PObj; var Accept: Boolean);
    procedure Button1Click(Sender: PObj);
    procedure Label7Click(Sender: PObj);
    procedure Label8Click(Sender: PObj);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form4 {$IFDEF KOL_MCK} : PForm4 {$ELSE} : TForm4 {$ENDIF} ;

{$IFDEF KOL_MCK}
procedure NewForm4( var Result: PForm4; AParent: PControl );
{$ENDIF}

implementation

{$IFNDEF KOL_MCK} {$R *.DFM} {$ENDIF}

{$IFDEF KOL_MCK}
{$I about_1.inc}
{$ENDIF}

procedure TForm4.KOLForm1Close(Sender: PObj; var Accept: Boolean);
begin
  Accept:=False;
  Form.ModalResult:=-1;
end;

procedure TForm4.Button1Click(Sender: PObj);
begin
  Form.ModalResult:=-1;
end;

procedure TForm4.Label7Click(Sender: PObj);
begin
  ShellExecuteA(Form.Handle,nil,PChar('mailto:collapse_forever@mail.ru'),'','',SW_SHOW);
end;

procedure TForm4.Label8Click(Sender: PObj);
begin
  ShellExecuteA(Form.Handle,nil,PChar('http://extractor.far.ru'),'','',SW_SHOW);
end;

end.


