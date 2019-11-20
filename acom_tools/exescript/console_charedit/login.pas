unit login;

interface

uses classes, windows, sysutils, character, utils;

type
  TCharacterEntry = class
  public
    IsError: Boolean;
//  Character: TCharacter;
    Entry: TMemoryStream;
    Id1, Id2: longword; // sync
    constructor Create;
    destructor Free;
    procedure Assign(ce: TCharacterEntry);
    function Caption: string;
    function Description: string;
  private
    procedure Error(s : string);
  end;
  TCharacterList = class (TList)
  public
    IsError: Boolean;
    procedure Kill;
  private
    procedure Error(s : string);
  end;
  TLogin = class
  public
    IsError: Boolean;
    CharList: TCharacterList;
    Paused, Locked: longword;
    Password: string;
    ServerId, LockedId1, LockedId2: longword;
    procedure LoadFromFile(Filename: string);
    procedure SaveToFile(Filename: string);
    procedure LoadFromStream(Stream: TStream);
    procedure SaveToStream(Stream: TStream);
    constructor Create;
    destructor Free;
  private
    procedure Error(s : string);
  end;

implementation

function TCharacterEntry.Caption: string;
var
  c: TCharacter;
begin
  try
    try
      c:=TCharacter.Create;
      c.LoadFromStream(Entry);
      Result:=c.Nick;
      if c.Clan<>'' then Result:=Result+'|'+c.Clan;
      Result:=Result+' ('+IntToStr(Entry.Size)+' байт)';
    finally
      c.Free;
    end;
  except
    on EAbort do Result:='error';
  end;
end;

function TCharacterEntry.Description: string;
var
  c: TCharacter;
begin
  try
    try 
      c:=TCharacter.Create;
      c.LoadFromStream(Entry);
      Result:='ник: '+c.Nick+#13#10;
      if c.Clan<>'' then Result:=Result+'клан: '+c.Clan+#13#10;
      Result:=Result+'размер: '+IntToStr(Entry.Size)+' байт'#13#10;
      Result:=Result+'идентификатор: '+IntToStr(c.Id1)+#13#10;
      Result:=Result+'идентификатор прав: '+IntToStr(c.Id2)+#13#10;
      Result:=Result+'денег: '+IntToStr(c.Money)+#13#10;
      Result:=Result+'смертей: '+IntToStr(c.Deaths)+#13#10;
      Result:=Result+'монстров убито: '+IntToStr(c.MonstersKills)+#13#10;
      Result:=Result+'игроков убито: '+IntToStr(c.PlayersKills)+#13#10;
      Result:=Result+'фрагов: '+IntToStr(c.Frags)+#13#10;
      Result:=Result+'вещей в мешке: '+IntToStr(c.Bag.Count)+#13#10;
    finally
      c.Free;
    end;
  except
    on EAbort do Result:='error';
  end;
end;

procedure TCharacterEntry.Assign(ce: TCharacterEntry);
begin
  Entry.Clear;
  Entry.Size:=ce.Entry.Size;
  ce.Entry.Seek(0, 0);
  Entry.CopyFrom(ce.Entry, 0);
  Id1:=ce.Id1;
  Id2:=ce.Id2;
end;

procedure TCharacterList.Kill;
var
  i: longint;
  ce: TCharacterEntry;
begin
  for i:=0 to Count-1 do begin
    ce:=Get(i);
    ce.Free;
  end;
  Clear;
end;

constructor TCharacterEntry.Create;
begin
//Character:=TCharacter.Create;
  Entry:=TMemoryStream.Create;
  IsError:=false;
end;

destructor TCharacterEntry.Free;
begin
//Character.Free;
  Entry.Free;
end;

constructor TLogin.Create;
begin
  CharList:=TCharacterList.Create;
  IsError:=false;  
end;

destructor TLogin.Free;
begin
  CharList.Kill;
  CharList.Free;
end;

procedure TLogin.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);

  IsError := true;
end;

procedure TCharacterEntry.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);

  IsError := true;
end;

procedure TCharacterList.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);

  IsError := true;
end;

procedure TLogin.SaveToFile(Filename: string);
var
  fs: TFileStream;
begin
  fs:=TFileStream.Create(Filename, fmCreate);
  SaveToStream(fs);
  fs.Free;
end;

procedure TLogin.LoadFromFile(Filename: string);
var
  fs: TFileStream;
begin
  fs:=TFileStream.Create(Filename, fmOpenRead);
  LoadFromStream(fs);
  fs.Free;
end;

procedure TLogin.SaveToStream(Stream: TStream);
var
  sz, tmp0: longword;
  i: longint;
  m: array[0..$234-1] of byte;
  ce: TCharacterEntry;
begin
  with Stream do begin
    for i:=0 to $234-1 do
      m[i]:=0;
    Seek(0, 0);
    Size:=0;
    WriteBuffer(m[1], $234);
    Seek(0, 0);
    tmp0:=$12ED34FB;
    Write(tmp0, 4);
    WriteBuffer(Password[1], Length(Password));
    Seek($104, 0);
    Write(CharList.Count, 4);
    for i:=0 to CharList.Count-1 do begin
      ce:=CharList.Get(i);
      Write(ce.Id1, 4);
      Write(ce.Id2, 4);
    end;
    Seek($188, 0);
    for i:=0 to CharList.Count-1 do begin
      ce:=CharList.Get(i);
      sz:=ce.Entry.Size;
      Write(sz, 4);
    end;
    Seek($234, 0);
    for i:=0 to CharList.Count-1 do begin
      ce:=CharList.Get(i);
      ce.Entry.Seek(0, 0);
      CopyFrom(ce.Entry, ce.Entry.Size);
    end;
    Seek($1E0, 0);
    Write(ServerId, 4);
    Write(LockedId1, 4);
    Write(LockedId2, 4);
    Seek($1C8, 0);
    Write(Locked, 4);
    Write(Paused, 4);
  end;
end;

procedure TLogin.LoadFromStream(Stream: TStream);
var
  tmp0, n, sz: longword;
  b0: byte;
  i: longint;
  ce: TCharacterEntry;
begin
  with Stream do begin
    Seek(0, 0);
    Read(tmp0, 4);
    if tmp0<>$12ED34FB then begin
      Error('неверная сигнатура файла логина');
      exit;
    end;
    Password:='';
    repeat
      Read(b0, 1);
      if b0<>0 then Password:=Password+chr(b0);
    until b0=0;
    CharList.Kill;
    Seek($104, 0);
    Read(n, 4);
    for i:=0 to n-1 do begin
      ce:=TCharacterEntry.Create;
      Read(ce.Id1, 4);
      Read(ce.Id2, 4);
      CharList.Add(ce);
    end;
    Seek($188, 0);
    for i:=0 to n-1 do begin
      ce:=CharList.Get(i);
      Read(sz, 4);
      ce.Entry.SetSize(sz);
    end;
    Seek($234, 0);
    for i:=0 to n-1 do begin
      ce:=CharList.Get(i);
      ce.Entry.CopyFrom(Stream, ce.Entry.Size);
    end;
    Seek($1E0, 0);
    Read(ServerId, 4);
    Read(LockedId1, 4);
    Read(LockedId2, 4);
    Seek($1C8, 0);
    Read(Locked, 4);
    Read(Paused, 4);
  end;
end;

end.
