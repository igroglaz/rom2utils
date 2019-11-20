unit utils;

interface

uses windows, sysutils;

function ConvertAnsiToOem(const S : string) : string;
function ConvertOemToAnsi(const S : string) : string;
procedure AnsiWriteLn(const S: string);
procedure OEMWriteLn(const S: string);

implementation

function ConvertAnsiToOem(const S : string) : string;
{ ConvertAnsiToOem translates a string into the OEM-defined character set }
{$IFNDEF WIN32}
var
  Source, Dest : array[0..255] of Char;
{$ENDIF}
begin
  {$IFDEF WIN32}
  SetLength(Result, Length(S));
  if Length(Result) > 0 then
    AnsiToOem(PChar(S), PChar(Result));
  {$ELSE}
  if Length(Result) > 0 then
  begin
    AnsiToOem(StrPCopy(Source, S), Dest);
    Result := StrPas(Dest);
  end;
  {$ENDIF}
end; { ConvertAnsiToOem }

function ConvertOemToAnsi(const S : string) : string;
{ ConvertOemToAnsi translates a string from the OEM-defined
character set into either an ANSI or a wide-character string }
{$IFNDEF WIN32}
var
  Source, Dest : array[0..255] of Char;
{$ENDIF}
begin
  {$IFDEF WIN32}
  SetLength(Result, Length(S));
  if Length(Result) > 0 then
  OemToAnsi(PChar(S), PChar(Result));
  {$ELSE}
  if Length(Result) > 0 then
  begin
    OemToAnsi(StrPCopy(Source, S), Dest);
    Result := StrPas(Dest);
  end;
  {$ENDIF}
end; { ConvertOemToAnsi }

procedure AnsiWriteLn(const S: string);
begin
  writeln(ConvertOemToAnsi(S));
end;

procedure OEMWriteLn(const S: string);
begin
  writeln(ConvertAnsiToOem(S));
end;

end.