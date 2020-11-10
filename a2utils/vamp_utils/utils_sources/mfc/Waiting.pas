{ KOL MCK } // Do not remove this line!
{$DEFINE KOL_MCK}
unit Waiting;

interface

{$IFDEF KOL_MCK}
uses Windows, Messages, ShellAPI, KOL {$IFNDEF KOL_MCK}, mirror, Classes,
  mckCtrls, Controls {$ENDIF};
{$ELSE}
{$I uses.inc}
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;
{$ENDIF}

type
  {$IFDEF KOL_MCK}
  {$I MCKfakeClasses.inc}
  PForm3 = ^TForm3;
  TForm3 = object(TObj)
    Form: PControl;
  {$ELSE not_KOL_MCK}
  TForm3 = class(TForm)
  {$ENDIF KOL_MCK}
    KOLForm1: TKOLForm;
    Label1: TKOLLabel;
    ProgressBar1: TKOLProgressBar;
    procedure KOLForm1Close(Sender: PObj; var Accept: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form3 {$IFDEF KOL_MCK} : PForm3 {$ELSE} : TForm3 {$ENDIF} ;

{$IFDEF KOL_MCK}
procedure NewForm3( var Result: PForm3; AParent: PControl );
{$ENDIF}

implementation

{$IFNDEF KOL_MCK} {$R *.DFM} {$ENDIF}

{$IFDEF KOL_MCK}
{$I Waiting_1.inc}
{$ENDIF}

procedure TForm3.KOLForm1Close(Sender: PObj; var Accept: Boolean);
begin
  Accept:=False;
  Form.ModalResult:=-1;
end;

end.


