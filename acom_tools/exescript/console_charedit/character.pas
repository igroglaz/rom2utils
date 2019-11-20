unit character;

interface

uses classes, windows, sysutils, utils, a2consts;

type
  TEffect = class
  public
    Id, Id2: longword;
    Value, Value2: longword;
    procedure Assign(Source: TEffect);
    function Description: string;
  private
    procedure Error(s : string);
  end;
  TEffectList = class (TList)
  public
    procedure Kill;
    procedure Assign(Source: TEffectList);
    function Description: string;
  private
    procedure Error(s : string);
  end;
  TItem = class
  public
    Id: longword;
    IsMagic: boolean;
    Price: longword;
    Count: word;
    Effects: TEffectList;
    function Caption: string;
    function Description: string;
    constructor Create;
    destructor Free;
    procedure Assign(Source: TItem);
  private
    procedure Error(s : string);
  end;
  TItemList = class (TList)
  public
    UnknownValue0,
    UnknownValue1,
    UnknownValue2,
    UnknownValue5,
    UnknownValue6: byte;
    procedure Kill;
    procedure LoadFromStream(Stream: TStream);
    procedure SaveToStream(Stream: TStream);
    procedure Assign(Source: TItemList);
  private
    procedure Error(s : string);
  end;
  TCharacter = class
  public
    IsError : Boolean;
    MonstersKills,
    PlayersKills,
    Frags,
    Deaths,
    Money,
    Spells,
    ActiveSpell,
    ExpFireBlade,
    ExpWaterAxe,
    ExpAirBludgeon,
    ExpEarthPike,
    ExpAstralShooting,
    Id1, Id2, HatId: longword;
    UnknownValue1,
    UnknownValue2,
    UnknownValue3,
    Picture,
    Body,
    Reaction,
    Mind,
    Spirit,
    Sex: byte;
    Nick, Clan: string;
    Bag, Dress: TItemList;
//  SectionAAAAAAAA,
    Section55555555,
    Section40A40A40 : TMemoryStream;
//  SectionDE0DE0DE,
//  Section41392521,
//  Section3A5A3A5A
    procedure LoadFromFile(Filename: string);
    procedure SaveToFile(Filename: string);
    procedure LoadFromStream(Stream: TStream);
    procedure SaveToStream(Stream: TStream);
    function GetSection(ssec: longword; copy: integer): TMemoryStream;
    constructor Create;
    destructor Free;
  private
    procedure Error(s : string);
  end;

implementation

function TEffect.Description: string;
begin
  Result:='';
  case id of
    0..40, 43, 49, 50: Result:=Result+effects1[id]+' '+IntToStr(value);
    44..48: Result:=Result+effects1[id]+' '+IntToStr(value)+'-'+IntToStr(value2);
    41: Result:=Result+effects1[id]+' '+spells1[value]+#13#10+effects1[id2]+' '+IntToStr(value2);
    42: Result:=Result+effects1[id]+' '+spells1[value];
    else Result:=Result+'value'+IntToStr(id)+' '+IntToStr(value);;
  end;
  Result:=Result+#13#10;
end;

function TEffectList.Description: string;
var
  i: integer;
begin
  Result:='';
  for i:=0 to Count-1 do
    Result:=Result+TEffect(Get(i)).Description;
end;

function TItem.Caption: string;
var
  i: integer;
  idx: integer;
begin
  Result:='';
  if id=0 then Result:=Result+'None' else begin
    idx:=0;
    for i:=1 to MAXITEMS do
      if Id=items0[i] then begin
        idx:=i;
        break;
      end;
//    if idx=0 then
//      Error('неверный предмет');
    if idx<>0 then
      Result:=Result+items1[idx]
    else Result:=Result+'Error Item';
  end;
  if IsMagic then Result:='Magic '+Result;
  if Count<>1 then Result:=Result+' ('+IntToStr(count)+' шт.)';
end;

function TItem.Description: string;
var
  i: integer;
  idx: integer;
  a, b, sz0, pr: integer;
begin
  if IsMagic then Result:='Magic '+Result;
  Result:='название предмета: '+Result;
  if id=0 then Result:=Result+'None' else begin
    idx:=0;
    for i:=1 to MAXITEMS do
      if Id=items0[i] then begin
        idx:=i;
        break;
      end;
//    if idx=0 then
//      Error('неверный предмет');
    if idx<>0 then
      Result:=Result+items1[idx]
    else Result:=Result+'Error Item';
  end;
  Result:=Result+#13#10;
  Result:=Result+'количество: '+IntToStr(count)+' шт.'#13#10;
  if not IsMagic then begin
    pr:=ord(items2[idx][7] ) shl $00 +
        ord(items2[idx][8] ) shl $08 +
        ord(items2[idx][9] ) shl $10 +
        ord(items2[idx][10]) shl $18;
    Result:=Result+'стоимость: '+IntToStr(Pr)+#13#10;
  end;
  sz0:=(ord(items2[idx][5])-5)div 2;
  for i:=0 to sz0-1 do begin
    a:=ord(items2[idx][i*2+$b]);
    b:=ord(items2[idx][i*2+$c]);
      if a=14 then b:=b+ord(items2[idx][(i-1)*2+$c]);
      case a of
        41,42: Result:=Result+effects1[a]+' '+spells1[b]+#13#10;
        else if a<$33 then Result:=Result+effects1[a]+' '+IntToStr(b)+#13#10;
      end;
  end;
  if IsMagic then begin
    Result:=Result+'стоимость: '+IntToStr(Price)+#13#10;
    Result:=Result+'магия: '#13#10;
    Result:=Result+Effects.Description+#13#10;
  end;
end;

procedure TEffect.Assign(Source: TEffect);
begin
  Id:=Source.Id;
  Id2:=Source.Id2;
  Value:=Source.Value;
  Value2:=Source.Value2;
end;

procedure TEffectList.Assign(Source: TEffectList);
var
  i: integer;
  sp: TEffect;
begin
  Kill;
  for i:=0 to Source.Count-1 do begin
    sp:=TEffect.Create;
    sp.Assign(Source.Get(i));
    Add(sp);
  end;
end;

procedure TItem.Assign(Source: TItem);
begin
  Id:=Source.Id;
  IsMagic:=Source.IsMagic;
  Price:=Source.Price;
  Count:=Source.Count;
  Effects.Assign(Source.Effects);
end;

procedure TItemList.Assign(Source: TItemList);
var
  i: integer;
  sp: TItem;
begin
  Kill;
  for i:=0 to Source.Count-1 do begin
    sp:=TItem.Create;
    sp.Assign(Source.Get(i));
    Add(sp);
  end;
end;

procedure TEffectList.Kill;
var
  i: longint;
  s: TEffect;
begin
  for i:=0 to Count-1 do begin
    s:=Get(i);
    s.Free;
  end;
  Clear;
end;

procedure TItemList.Kill;
var
  i: longint;
  it: TEffect;
begin
  for i:=0 to Count-1 do begin
    it:=Get(i);
    it.Free;
  end;
  Clear;
end;

procedure TItemList.SaveToStream(Stream: TStream);
var
  b00000 : byte;
  sz, cnt, j : longword;
  i: longint;
  Item : TItem;
  Effect : TEffect;
  ec: integer;
begin
  with Stream do begin
    Size:=9;
    Seek(0, 0);
    Write(UnknownValue0, 1);
    Write(UnknownValue1, 1);
    Write(UnknownValue2, 1);
    Seek(5, 0);
    Write(UnknownValue5, 1);
    Write(UnknownValue6, 1);
    Seek(9, 0);
    for i:=0 to Count-1 do begin
      Item:=Get(i);
      if Item.IsMagic then begin
        Write(Item.Id, 2);
        ec:=0;
        for j:=0 to Item.Effects.Count-1 do begin
          Effect:=Item.Effects.Get(j);
          inc(ec);
          if Effect.Id in [$29, $2C..$30] then
            inc(ec);
        end;
        if ec>$0F then Error('слишком много заклинаний!');
        b00000:=ec or $20;
        Write(b00000, 1);
        Write(Item.Price, 4);
        for j:=0 to Item.Effects.Count-1 do begin
          Effect:=Item.Effects.Get(j);
          Write(Effect.Id, 1);
          Write(Effect.Value, 1);
          if Effect.Id in [$29, $2C..$30] then // урон ХХ-ХХ, заклинание ХХ с мощностью ХХ
          begin
            Write(Effect.Id2, 1);
            Write(Effect.Value2, 1);
          end;
        end;
      end else if Item.Count<=$7F then begin
        Write(Item.Id, 2);
        b00000:=Item.Count or $80;
        Write(b00000, 1);
      end else begin
        Write(Item.Id, 2);
        b00000:=0;
        Write(b00000, 1);
        Write(Item.Count, 2);
      end;
    end;
    Seek(3, 0);
    cnt:=Count;
    sz:=Size-9;
    Write(cnt, 4);
    Write(sz, 2);
  end;
end;

procedure TItemList.LoadFromStream(Stream: TStream);
var
  Item: TItem;
  Effect: TEffect;
  b000: byte;
  i : longword;
  effsz : longword;
  zzz: array[1..9] of byte;
begin
  with Stream do begin
    Seek(0, 0);
    Read(UnknownValue0, 1);
    Read(UnknownValue1, 1);
    Read(UnknownValue2, 1);
    Seek(5, 0);
    Read(UnknownValue5, 1);
    Read(UnknownValue6, 1);
    Seek(9, 0);
    while Position<Size do begin
      Item:=TItem.Create;
      Read(Item.Id, 2);
      Read(b000, 1);
      if (b000 and $80) = $80 then begin // немного простых вещей
        Item.Count:=b000-$80;
        Item.IsMagic:=false;
        Item.Price:=0;
        Item.Effects.Kill;
        Self.Add(Item);
      end else if (b000 and $20) = $20 then begin // магическая вещь
        effsz:=b000 and $f;
        Read(Item.Price,4);
        Item.IsMagic:=true;        
        Item.Count:=1;
        Item.Effects.Kill;
        i:=0;
        while i<effsz do begin
          Effect:=TEffect.Create;
          Read(Effect.Id, 1);
          Read(Effect.Value, 1);
          Effect.Id2:=0;
          Effect.Value2:=0;
          if Effect.Id in [$29, $2C..$30] then // урон ХХ-ХХ
          begin
            Read(Effect.Id2, 1);
            Read(Effect.Value2, 1);
            effsz:=effsz-1;
          end;
          Item.Effects.Add(Effect);
          inc(i);
        end;
        Self.Add(Item);
      end else if b000 = $00 then begin // много простых вещей
        Read(Item.Count, 2);
        Item.IsMagic:=false;
        Item.Price:=0;
        Item.Effects.Kill;
        Self.Add(Item);
      end else Error('неверная структура вещей');
    end;
  end;
end;

constructor TItem.Create;
begin
  Id:=0; Count:=1; Price:=0; IsMagic:=false;
  Effects:=TEffectList.Create;
end;

destructor TItem.Free;
begin
  Effects.Free;
end;

constructor TCharacter.Create;
begin
//SectionAAAAAAAA:=TMemoryStream.Create;
  Section55555555:=TMemoryStream.Create;
  Section40A40A40:=TMemoryStream.Create;
//SectionDE0DE0DE:=TMemoryStream.Create;
//Section41392521:=TMemoryStream.Create;
//Section3A5A3A5A:=TMemoryStream.Create;
  Bag:=TItemList.Create;
  Dress:=TItemList.Create;
  IsError := false;
end;

destructor TCharacter.Free;
begin
//SectionAAAAAAAA.Free;
  Section55555555.Free;
  Section40A40A40.Free;
//SectionDE0DE0DE.Free;
//Section41392521.Free;
//Section3A5A3A5A.Free;
  Bag.Free;
  Dress.Free;
  IsError := true;
end;

procedure TCharacter.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);

  IsError := true;
end;

procedure TEffectList.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);
  Abort;
end;

procedure TEffect.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);
  Abort;
end;

procedure TItemList.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);
  Abort;
end;

procedure TItem.Error(s : string);
begin
  if IsConsole then
    oemwriteln('Ошибка: '+s)
  else
    MessageBox(0, PChar(s), 'Ошибка', MB_OK+MB_ICONERROR);
  Abort;
end;

procedure TCharacter.SaveToFile(Filename: string);
var
  fs: TFileStream;
begin
  fs:=TFileStream.Create(Filename, fmCreate);
  SaveToStream(fs);
  fs.Free;
end;

procedure TCharacter.LoadFromFile(Filename: string);
var
  fs: TFileStream;
begin
  fs:=TFileStream.Create(Filename, fmOpenRead);
  LoadFromStream(fs);
  fs.Free;
end;

function TCharacter.GetSection(ssec: longword; copy: integer): TMemoryStream;
var
  ms: TMemoryStream;
begin
  Result:=nil;
//if ssec=$AAAAAAAA then Result:=SectionAAAAAAAA;
  if ssec=$55555555 then Result:=Section55555555;
  if ssec=$40A40A40 then Result:=Section40A40A40;
//if ssec=$DE0DE0DE then Result:=SectionDE0DE0DE;
//if ssec=$41392521 then Result:=Section41392521;
//if ssec=$3A5A3A5A then Result:=Section3A5A3A5A;
  if result=nil then exit;
  if copy>0 then begin { нужна не сама секция, а ее копия (для модификации) }
    ms:=TMemoryStream.Create;
    Result.Seek(0,0);
    ms.LoadFromStream(Result);
    Result:=ms;
  end;
  Result.Seek(0, 0);
end;

procedure TCharacter.SaveToStream(Stream: TStream);
var
  b0000: byte;
  c: char;
  sig: longword;
  ssec, ssiz, scrc, skey : longword;
  s0 : TMemoryStream;
  tmp0: longword;
  s: string;

function StreamCRC2: longword;
var
  res: longword;

procedure addbyte(t : longword);
begin
  inc(res,(t) and $000000FF shr 0 );
  res:=res shl 1;
end;

procedure adddword(t : longword);
begin
  inc(res,(t) and $000000FF shr 0 );
  res:=res shl 1;
  inc(res,(t) and $0000FF00 shr 8 );
  res:=res shl 1;
  inc(res,(t) and $00FF0000 shr 16);
  res:=res shl 1;
  inc(res,(t) and $FF000000 shr 24);
  res:=res shl 1;
end;

procedure adddword2(t : longword);
begin
  inc(res,(t) and $000000FF shr 0 );
  res:=res shl 1;
  inc(res,(t) and $0000FF00 shr 8 );
  res:=res shl 1;
  inc(res,(t) and $00FF0000 shr 16);
  res:=res shl 1;
  inc(res,(t) and $FF000000 shr 24);
end;

begin
  res:=0;
  adddword(MonstersKills      );
  adddword(PlayersKills       );
  adddword(Frags              );
  adddword(Deaths             );
  adddword(Money              );
  addbyte(Body                );
  addbyte(Reaction            );
  addbyte(Mind                );
  addbyte(Spirit              );
  adddword(Spells             );
  adddword(ActiveSpell      );
  adddword(ExpFireBlade       );
  adddword(ExpWaterAxe        );
  adddword(ExpAirBludgeon     );
  adddword(ExpEarthPike       );
  adddword2(ExpAstralShooting );
  result:=res;
end;

procedure WriteSignature;
var
  key: longword;
begin
  key:=skey shl 16;
  with Stream do begin
    Write(ssec, 4);
    Write(ssiz, 4);
    Write(key, 4);
    Write(scrc, 4);
  end;
end;

function StreamCRC: longword; // not a CRC, only checksum =)
var
  i: integer;
  a: byte;
begin
  s0.Seek(0, 0);
  result:=0;
  for i:=0 to s0.size-1 do begin
    s0.Read(a, 1);
    result:=result shl 1 + a;
  end;
end;

procedure CryptSection; 
var
  key, oldkey, b00: longword;
  i: longint;
begin
  s0.Seek(0, 0);
  key:=skey;
  oldkey:=key;
  key:=key and $FFFF;
  key:=(key shl $10) or key;
  for i:=0 to s0.Size-1 do begin
    s0.Read(b00, 1);
    b00:=(key shr $10) xor b00;
    s0.Seek(-1, 1);
    s0.Write(b00, 1);
    key:=key shl 1;
    if (i and $F) = $F then
      key:=key or (oldkey and $FFFF);
  end;
end;

begin
  b0000:=0;
  sig:=$04507989;
  with Stream do begin
    Seek(0, 0);
    Size:=0;
    Write(sig, 4);

    ssec:=$AAAAAAAA; // основные характеристики - _всегда_ первая
//    s0:=GetSection(ssec, 1);
    s0:=TMemoryStream.Create;
    s0.Size:=$34;
    FillChar(s0.Memory^, $34, 0);
    s0.Seek(0, 0);
    s0.Write(Id1, 4);
    s0.Write(Id2, 4);
    s0.Write(HatId, 4);
    c:='|';
    s:=ConvertAnsiToOem(Nick);
    s0.Write(s[1], length(s));
    if Clan<>'' then begin
      s0.Write(c, 1);
      s:=ConvertAnsiToOem(Clan);
      s0.Write(s[1], length(s));
    end;
    c:=#0;
    s0.Write(c, 1);
    s0.Seek($2C, 0);
    s0.Write(Sex, 1);
    s0.Write(Picture, 1);
    s0.Write(UnknownValue1, 1);
    s0.Write(UnknownValue2, 1);
    s0.Write(UnknownValue3, 1);

    ssiz:=s0.Size;
//    skey:=$0291;
    skey:=Random($8000); // для отладки обнулить
    scrc:=StreamCRC;
    CryptSection;
//    skey:=$529C0291;
    s0.Seek(0, 0);
    WriteSignature;
    CopyFrom(s0, ssiz);
    s0.Free;

    ssec:=$55555555; // память
    s0:=GetSection(ssec, 1);
    ssiz:=s0.Size;
    skey:=Random($8000); // для отладки обнулить
    scrc:=StreamCRC;
    CryptSection;
    s0.Seek(0, 0);
    WriteSignature;
    CopyFrom(s0, ssiz);
    s0.Free; 

    ssec:=$40A40A40; // горячие клавиши
    s0:=GetSection(ssec, 1);
    ssiz:=s0.Size;
    skey:=Random($8000); // для отладки обнулить
    scrc:=StreamCRC;
    CryptSection;
    s0.Seek(0, 0);
    WriteSignature;
    CopyFrom(s0, ssiz);
    s0.Free;

    ssec:=$DE0DE0DE; // вещи на себе
    s0:=TMemoryStream.Create;
    Dress.SaveToStream(s0);
    ssiz:=s0.Size;
    skey:=Random($8000); // для отладки обнулить
    scrc:=StreamCRC;
    CryptSection;
    s0.Seek(0, 0);
    WriteSignature;
    CopyFrom(s0, ssiz);
    s0.Free;

    ssec:=$41392521; // дополнительные характеристики
    s0:=TMemoryStream.Create;
    skey:=Random($8000); // для отладки обнулить

    if (skey and    $1) <> 0 then s0.Write(b0000, 1);
    tmp0:=MonstersKills xor $01529251;
    s0.Write(tmp0, 4);
    if (skey and    $2) <> 0 then s0.Write(b0000, 1);
    tmp0:=PlayersKills - MonstersKills*5 - $13141516;
    s0.Write(tmp0, 4);
    if (skey and    $4) <> 0 then s0.Write(b0000, 1);
    tmp0:=Frags - PlayersKills*7 - $00ABCDEF;
    s0.Write(tmp0, 4);
    if (skey and    $8) <> 0 then s0.Write(b0000, 1);
    tmp0:=Deaths xor $17FF12AA;
    s0.Write(tmp0, 4);
    if (skey and   $10) <> 0 then s0.Write(b0000, 1);
    tmp0:=Money - MonstersKills*3 + $21524542;
    s0.Write(tmp0, 4);
    if (skey and   $20) <> 0 then s0.Write(b0000, 1);
    tmp0:=$FF and (Body - Money*$11 - MonstersKills*$13);
    s0.Write(tmp0, 1);
    if (skey and   $40) <> 0 then s0.Write(b0000, 1);
    tmp0:=$FF and (Reaction - Body*3);
    s0.Write(tmp0, 1);
    if (skey and   $80) <> 0 then s0.Write(b0000, 1);
    tmp0:=$FF and (Mind - Body - Reaction*5);
    s0.Write(tmp0, 1);
    if (skey and  $100) <> 0 then s0.Write(b0000, 1);
    tmp0:=$FF and (Spirit - Body*7 - Mind*9);
    s0.Write(tmp0, 1);
    if (skey and $4000) <> 0 then s0.Write(b0000, 1);
    tmp0:=Spells + $10121974;
    s0.Write(tmp0, 4);
    if (skey and $2000) <> 0 then s0.Write(b0000, 1);
    tmp0:=ActiveSpell;
    s0.Write(tmp0, 4);
    if (skey and  $200) <> 0 then s0.Write(b0000, 1);
    tmp0:=ExpFireBlade xor $DADEDADE;
    s0.Write(tmp0, 4);
    if (skey and  $400) <> 0 then s0.Write(b0000, 1);
    tmp0:=ExpWaterAxe + ExpFireBlade*$771;
    s0.Write(tmp0, 4);
    if (skey and  $800) <> 0 then s0.Write(b0000, 1);
    tmp0:=ExpAirBludgeon + ExpWaterAxe*$771;
    s0.Write(tmp0, 4);
    if (skey and $1000) <> 0 then s0.Write(b0000, 1);
    tmp0:=ExpEarthPike + ExpAirBludgeon*$771;
    s0.Write(tmp0, 4);
    if (skey and $2000) <> 0 then s0.Write(b0000, 1);
    tmp0:=ExpAstralShooting + ExpEarthPike*$771;
    s0.Write(tmp0, 4);

    ssiz:=s0.Size;
    scrc:=StreamCRC2;
    s0.Seek(0, 0);
    WriteSignature;
    CopyFrom(s0, ssiz);
    s0.Free;

    ssec:=$3A5A3A5A; // вещи в мешке
    s0:=TMemoryStream.Create;
    Bag.SaveToStream(s0);
    ssiz:=s0.Size;
    skey:=Random($8000); // для отладки обнулить
    scrc:=StreamCRC;
    CryptSection;
    s0.Seek(0, 0);
    WriteSignature;
    CopyFrom(s0, ssiz);
    s0.Free;
  end;
end;

procedure TCharacter.LoadFromStream(Stream: TStream);
var
  sig, skey, ssig, ssiz, scrc : longword;
  s0 : TMemoryStream;
  b0 : byte;
  lw0 : longword;
  c: char;
  s: string;

procedure DecryptSection;
var
  i, key, oldkey, b00: longword;
begin
  if s0.size=0 then exit;
  s0.Seek(0, 0);
  key:=skey;
  oldkey:=key;
  key:=key and $FFFF;
  key:=(key shl $10) or key;
  for i:=0 to s0.Size-1 do begin
    s0.Read(b00, 1);
    b00:=(key shr $10) xor b00;
    s0.Seek(-1, 1);
    s0.Write(b00, 1);
    key:=key shl 1;
    if (i and $F) = $F then
      key:=key or (oldkey and $FFFF);
  end;
end;

procedure ReadSection;
begin
  s0.Clear;
  if ssiz<>0 then
    s0.CopyFrom(Stream, ssiz);
end;

begin
  with Stream do begin
    Seek(0, 0);
    Read(sig, 4);
    if sig<>$04507989 then begin
      Error('неверная сигнатура файла персонажа');
      exit;
    end;
    while Position<Size do begin
      Read(ssig, 4);
      Read(ssiz, 4);
      Read(skey, 4); skey:=skey shr $10;
      Read(scrc, 4);    
      s0:=GetSection(ssig, 0);
      case ssig of
        $41392521: begin // дополнительные характеристики
          if skey and $0001 > 0 then Read(b0, 1);
          Read(lw0, 4);
          MonstersKills:=lw0 xor $01529251;
          if skey and $0002 > 0 then Read(b0, 1);
          Read(lw0, 4);
          PlayersKills:=lw0 + MonstersKills * 5 + $13141516;
          if skey and $0004 > 0 then Read(b0, 1);
          Read(lw0, 4);
          Frags:=lw0 + PlayersKills * 7 + $00ABCDEF;
          if skey and $0008 > 0 then Read(b0, 1);
          Read(lw0, 4);
          Deaths:=lw0 xor $17FF12AA;
          if skey and $0010 > 0 then Read(b0, 1);
          Read(lw0, 4);
          Money:=lw0 + MonstersKills * 3 - $21524542;
          if skey and $0020 > 0 then Read(b0, 1);
          Read(b0, 1);
          Body:=(b0 + Money * $11 + MonstersKills * $13) and $FF;
          if skey and $0040 > 0 then Read(b0, 1);
          Read(b0, 1);
          Reaction:=(b0 + Body * 3) and $FF;
          if skey and $0080 > 0 then Read(b0, 1);
          Read(b0, 1);
          Mind:=(b0 + Body + Reaction * 5) and $FF;
          if skey and $0100 > 0 then Read(b0, 1);
          Read(b0, 1);
          Spirit:=(b0 + Body * 7 + Mind * 9) and $FF;
          if skey and $4000 > 0 then Read(b0, 1);
          Read(lw0, 4);
          Spells:=lw0 - $10121974;
          if skey and $2000 > 0 then Read(b0, 1);
          Read(lw0, 4);
          ActiveSpell:=lw0;
          if skey and $0200 > 0 then Read(b0, 1);
          Read(lw0, 4);
          ExpFireBlade:=lw0 xor $DADEDADE;
          if skey and $0400 > 0 then Read(b0, 1);
          Read(lw0, 4);
          ExpWaterAxe:=lw0 - ExpFireBlade * $771;
          if skey and $0800 > 0 then Read(b0, 1);
          Read(lw0, 4);
          ExpAirBludgeon:=lw0 - ExpWaterAxe * $771;
          if skey and $1000 > 0 then Read(b0, 1);
          Read(lw0, 4);
          ExpEarthPike:=lw0 - ExpAirBludgeon * $771;
          if skey and $2000 > 0 then Read(b0, 1);
          Read(lw0, 4);
          ExpAstralShooting:=lw0 - ExpEarthPike * $771;
//        TestCRC2
        end;
        $AAAAAAAA: begin // основные характеристики
          s0:=TMemoryStream.Create;
          ReadSection;
          DecryptSection;
          s0.Seek(0, 0);
          s0.Read(Id1, 4);
          s0.Read(Id2, 4);
          s0.Read(HatId, 4);
          s:='';
          s0.Read(c, 1);
          if (c<>'|')and(c<>#0) then
          repeat
            s:=s+c;
            s0.Read(c, 1);
          until (c='|')or(c=#0);
          Nick:=ConvertOemToAnsi(s);
          if c='|' then begin
            s:='';
            s0.Read(c, 1);
            if (c<>#0) then
            repeat
              s:=s+c;
              s0.Read(c, 1);
            until (c=#0);
            Clan:=ConvertOemToAnsi(s);
          end else Clan:='';
          s0.Seek($2C, 0);
          s0.Read(Sex, 1);
          s0.Read(Picture, 1);
          s0.Read(UnknownValue1, 1);
          s0.Read(UnknownValue2, 1);
          s0.Read(UnknownValue3, 1);
          s0.Free;
//        TestCRC;
        end;
        $3A5A3A5A: begin // вещи в мешке
          s0:=TMemoryStream.Create;
          ReadSection;
          DecryptSection;
          Bag.LoadFromStream(s0);
          s0.Free;
//        TestCRC;
        end;
        $DE0DE0DE: begin // вещи на себе
          s0:=TMemoryStream.Create;
          ReadSection;
          DecryptSection;
          Dress.LoadFromStream(s0);
          s0.Free;
//        TestCRC;
        end;
        $40A40A40: begin // хз
          ReadSection;
          DecryptSection;
//        TestCRC;
        end;
        $55555555: begin // хз
          ReadSection;
          DecryptSection;
//        TestCRC;
        end;
        else Error('неверная сигнатура секции');
      end;
    end;
  end;
end;

initialization
  randomize;

end.
