{ KOL MCK } // Do not remove this line!
program MultiFormatConverter;

uses
KOL,
windows,
  MainUnit in 'MainUnit.pas' {Form1},
  Plugins in 'Plugins.pas',
  plugins_form in 'plugins_form.pas' {Form2},
  data_classes_archive in 'classes\data_classes_archive.pas',
  Waiting in 'Waiting.pas' {Form3},
  about in 'about.pas' {Form4};

//{$R *.res}
var
  UniqueMapping: THandle;
begin // PROGRAM START HERE -- Please do not remove this comment

{$IFDEF KOL_MCK} {$I MultiFormatConverter_0.inc} {$ELSE}

  //Блок предотвращения запуска нескольких копий
  UniqueMapping:=CreateFileMapping($FFFFFFFF,nil,PAGE_READONLY,0,32,'Multi-format Converter');
  if UniqueMapping=0 then
  begin
    ShowMessage('Ошибка выделения памяти!');
    Halt;
  end else
  if GetLastError=ERROR_ALREADY_EXISTS then
  begin
    ShowMessage('Программа уже запущена!');
    Halt;
  end;
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TForm2, Form2);
  Application.CreateForm(TForm3, Form3);
  Application.CreateForm(TForm4, Form4);
  Application.Run;

{$ENDIF}

end.

