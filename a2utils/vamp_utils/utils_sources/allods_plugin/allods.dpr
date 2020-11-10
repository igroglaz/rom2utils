library allods;

uses kol, data_classes_archive, data_classes_imagecomplex, data_bmpheader;

const
  BlockSize=20000;
  FT_ARCHIVE=$0001;
  FT_IMAGECOMPLEX = $0008;
  INFO_1='Загружается информация об архиве';

var
  FData,FTemp,FMem: PStream;

function Expand(DB: Byte; var DBL,DBR: Byte): Boolean;
var
  fB: Byte;
begin
  fB:=DB;
  DBL:=fB shr 6;
  DBR:=fB and $3F;
end;

{Дополняет строку до требуемой длины указанным символом}
function BuildStr(s: string; c: char; n: LongInt): string;
var
  t: string;
begin
  if Length(s)=n then
  begin
    BuildStr:=s;
    Exit;
  end;
  t:=s;
  While Length(t)<n do t:=c+t;
  BuildStr:=t;
end;

function MakeTree(Dir: string): Boolean;
begin
  Result := True;
  if Length(Dir) = 0 then Exit;
  if Dir[Length(Dir)]='\' then Delete(Dir,Length(Dir),1);
  if (Length(Dir) < 3) or DirectoryExists(Dir) or (ExtractFilePath(Dir) = Dir) then Exit; // avoid 'xyz:\' problem.
  Result := MakeTree(ExtractFilePath(Dir)) and CreateDir(Dir);
end;

procedure GetPluginInfo(var Title,Version,GameName,FilterGroupName,Filters,Extentions,Comments: PChar);
begin
  Title:=PChar('Allods Plugin');
  Version:=PChar('v1.01');
  GameName:=PChar('Allods');
  FilterGroupName:=PChar('Allods (*.res;*.16a;*.256)');
  Filters:=PChar('*.res;*.16a;*.256');
  Extentions:=PChar('RES 16A 256');
  Comments:=PChar('Распаковывает ресурсные архивы RES и конвертирует изображения форматов 16A и 256');
end;

function GetFileInfo(fName: PChar): Word;
var
  fStr,Extention: string;
begin
  fStr:=string(fName);
  Extention:=UpperCase(Copy(fStr,DelimiterLast(fStr,'.')+1,3));
  if Extention='RES' then Result:=FT_ARCHIVE;
  if Extention='16A' then Result:=FT_IMAGECOMPLEX;
  if Extention='256' then Result:=FT_IMAGECOMPLEX;
end;

//Возвращаем ID плагина
function GetPluginID: Integer;
begin
  Result:=$0007;
end;

function GetFileAutoload(fName: PChar): Boolean;
var
  fStr,Extention: string;
begin
  fStr:=string(fName);
  Extention:=UpperCase(Copy(fStr,DelimiterLast(fStr,'.')+1,3));
  if Extention='RES' then Result:=False;
  if Extention='16A' then Result:=True;
  if Extention='256' then Result:=True;
end;

procedure GetArchiveContent(fName: PChar; var fArchive: TArchive; fForm,fLabel,fProgressBar: PControl);
var
  TotalFiles,i,j,fTemp,SavePosition: LongInt;
  Extention: string;
  fPChar: PChar;
  Found: Boolean;
  Buffer: array[0..16]of char;
{
  В данном случае флаг Compressed используется для определения директории
  Если Compressed значит - директория
}
  procedure BuildLevels(fFrom,fTo,fCur: Integer);
  var
    i: Integer;
  begin
    for i:=fFrom to fTo do
    begin
      fArchive.Items[i].Reserved_3:=fCur;
      if fArchive.Items[i].Reserved_2=1 then BuildLevels(fArchive.Items[i].Offset,fArchive.Items[i].Offset+fArchive.Items[i].SizeCompressed-1,fCur+1);
    end;
  end;

  procedure BuildPath(fAddValue: string; fFrom, fTo: Integer);
  var
    i: Integer;
  begin
    for i:=fFrom to fTo do
    begin
      fArchive.Items[i].Name:=fAddValue+'\'+fArchive.Items[i].Name;
      if fArchive.Items[i].Reserved_2=1 then BuildPath(fArchive.Items[i].Name,fArchive.Items[i].Offset,fArchive.Items[i].Offset+fArchive.Items[i].SizeCompressed-1);
    end;
  end;

begin
  fForm.Show;
  fLabel.Caption:=INFO_1+'(1/1)';
  fProgressBar.Progress:=0;
  fForm.Update;
  FData:=NewReadFileStream(fName);
  FData.Seek($10,spBegin);
  FData.Read(SavePosition,4);
  TotalFiles:=(FData.Size-SavePosition) div 32;
  fArchive.TotalFiles:=TotalFiles;
  fArchive.ArchiveFullFileName:=string(fName);
  FData.Seek(SavePosition,spBegin);
  for i:=1 to TotalFiles do
  begin
    fArchive.Add('',0,0,0);
    FData.Read(fTemp,4);
    fArchive.Items[i-1].Reserved_1:=fTemp;
    FData.Read(fTemp,4);
    fArchive.Items[i-1].Offset:=fTemp;
    FData.Read(fTemp,4);
    fArchive.Items[i-1].SizeCompressed:=fTemp;
    fArchive.Items[i-1].SizeReal:=fTemp;
    FData.Read(fTemp,4);
    fArchive.Items[i-1].Reserved_2:=fTemp;
    FillChar(Buffer,17,#$00);
    FData.Read(Buffer,16);
    fPChar:=Buffer;
    fArchive.Items[i-1].Name:=string(fPChar);
    fArchive.Items[i-1].Reserved_3:=0;
    fProgressBar.Progress:=Round(Int(100*(i/TotalFiles)));
  end;
  for i:=1 to TotalFiles do
  begin
    if (fArchive.Items[i-1].Reserved_2=1)and(fArchive.Items[i-1].Reserved_3=0) then
    begin
      fArchive.Items[i-1].Reserved_3:=1;
      BuildLevels(fArchive.Items[i-1].Offset,fArchive.Items[i-1].Offset+fArchive.Items[i-1].SizeCompressed-1,2);
    end;
  end;
  for i:=1 to TotalFiles do
  begin
    if (fArchive.Items[i-1].Reserved_2=1)and(fArchive.Items[i-1].Reserved_3=1) then
    begin
      BuildPath(fArchive.Items[i-1].Name,fArchive.Items[i-1].Offset,fArchive.Items[i-1].Offset+fArchive.Items[i-1].SizeCompressed-1);
    end;
  end;
  for i:=1 to TotalFiles do fArchive.Items[i-1].Name:=fArchive.Items[i-1].Path+'\'+fArchive.Items[i-1].Name;
  for i:=TotalFiles downto 1 do
  if fArchive.Items[i-1].Reserved_2=1 then
  begin
    Dec(TotalFiles);
    fArchive.TotalFiles:=TotalFiles;
    fArchive.Delete(i-1);
  end;
  for i:=1 to TotalFiles do
  begin
    fArchive.Items[i-1].Name:=Copy(fArchive.Items[i-1].Name,2,Length(fArchive.Items[i-1].Name)-1);
  end;
  for i:=1 to TotalFiles do
  begin
    Extention:=UpperCase(Copy(fArchive.Items[i-1].Name,DelimiterLast(fArchive.Items[i-1].Name,'.')+1,3));
    if fArchive.CountFilters=0 then
    begin
      fArchive.AddFilter('*.'+LowerCase(Extention),Extention);
      fArchive.Items[i-1].FilterIndex:=fArchive.CountFilters-1;
    end
    else
    begin
      Found:=False;
      for j:=1 to fArchive.CountFilters do if fArchive.Filters[j-1].Extention=Extention then
      begin
        Found:=True;
        fArchive.Items[i-1].FilterIndex:=j-1;
        Break;
      end;
      if not Found then
      begin
        fArchive.AddFilter('*.'+LowerCase(Extention),Extention);
        fArchive.Items[i-1].FilterIndex:=fArchive.CountFilters-1;
      end;
    end;
  end;
  fArchive.FlagUseFilters:=True;
  FData.Free;
  fForm.Hide;
end;

procedure UnpackArchive(fName: PChar; fArchive: TArchive; var fForm,fLabel,fProgressBar: PControl);
var
  fBasePath,fFilePath: string;
  i: Integer;
begin
  if fArchive.TotalFiles=0 then Exit;
  fForm.Show;
  fLabel.Caption:='Извлекаем файлы...';
  fProgressBar.Progress:=0;
  fForm.Update;
  fBasePath:=fName;
  if not(fBasePath[Length(fBasePath)]='\') then fBasePath:=fBasePath+'\';
  FData:=NewReadFileStream(fArchive.ArchiveFullFileName);
  for i:=1 to fArchive.TotalFiles do
  begin
    if fArchive.Items[i-1].FlagExtract then
    begin
      fFilePath:=fBasePath+ExtractFilePath(fArchive.Items[i-1].Name);
      if not (DirectoryExists(fFilePath)) then MakeTree(fFilePath);
      FTemp:=NewWriteFileStream(fBasePath+fArchive.Items[i-1].Name);
      FData.Seek(fArchive.Items[i-1].Offset,spBegin);
      Stream2Stream(FTemp,FData,fArchive.Items[i-1].SizeCompressed);
      FTemp.Free;
    end;
    fProgressBar.Progress:=Round(Int(100*(i/fArchive.TotalFiles)));
  end;
  fForm.Hide;
  FData.Free;
end;

procedure GetImageComplex(fName, fPalName: PChar; var fImageComplex: TImageComplex; fForm,fLabel,fProgressBar: PControl);
var
  i,j,x,y,z,SavePosition: LongInt;
  TotalSprites,SS: Word;
  FreeBuffer,Buffer: array[1..1024]of byte;
  fHeight,fWidth,fLeft,fTop,fRight,fBottom,fDataSize,fMaxWidth,fMaxHeight: LongInt;
  Add,DByte,DByteL,DByteR,SL,SR: Byte;
  fOnlyName,fStr,Extention: string;
begin
  fForm.Show;
  fLabel.Caption:='Загружаем базу изображений...';
  fProgressBar.Progress:=0;
  fForm.Update;
  //Инициализируем заголовок
  BmpID[1]:='B';
  BmpID[2]:='M';
  BMPHeader._C1:=$00000436;
  BMPHeader._C2:=$00000028;
  BMPHeader._C3:=$0001;
  BMPHeader._C4:=$0008;
  BMPHeader._H2:=$00;
  BMPHeader._H3:=$00;
  FData:=NewReadFileStream(fName);
  fStr:=string(fName);
  fOnlyName:=ExtractFileName(fStr);
  fOnlyName:=Copy(fOnlyName,1,Pos('.',fOnlyName)-1);
  Extention:=UpperCase(Copy(fStr,DelimiterLast(fStr,'.')+1,3));
  FData.Seek(FData.Size-4,spBegin);
  FData.Read(TotalSprites,2);
  //Загружаем палитру
  FData.Seek(0,spBegin);
  for i:=0 to 255 do
  begin
    FData.Read(BMPPalette[i].R,1);
    FData.Read(BMPPalette[i].G,1);
    FData.Read(BMPPalette[i].B,1);
    FData.Read(BMPPalette[i].F,1);
  end;
  BMPPalette[0].R:=$FF;
  BMPPalette[0].G:=$00;
  BMPPalette[0].B:=$FF;
  SavePosition:=FData.Position;
  //Определяем максимальный размер изображений
  fMaxWidth:=0;
  fMaxHeight:=0;
  for i:=1 to TotalSprites do
  begin
    FData.Seek(SavePosition,spBegin);
    FData.Read(fWidth,4);
    FData.Read(fHeight,4);
    if fWidth>fMaxWidth then fMaxWidth:=fWidth;
    if fHeight>fMaxHeight then fMaxHeight:=fHeight;
    FData.Read(fDataSize,4);
    SavePosition:=SavePosition+fDataSize+12;
  end;
  //Добавляем пустые записи в базу изображений
  for i:=1 to TotalSprites do fImageComplex.Add;
  //Конвертируем спрайты
  if Extention='16A' then
  begin
    FData.Seek($400,spBegin);
    SavePosition:=FData.Position;
    for i:=1 to TotalSprites do
    begin
      FData.Seek(SavePosition,spBegin);
      FData.Read(fWidth,4);
      FData.Read(fHeight,4);
      FData.Read(fDataSize,4);
      SavePosition:=SavePosition+fDataSize+12;
      //Создаем потоки и определяем некоторые параметры
      FTemp:=NewMemoryStream; //Основной поток для BMP
      FMem:=NewMemoryStream;  //Временный поток
      fLeft:=(fMaxWidth-fWidth) div 2;
      fRight:=fMaxWidth-fWidth-fLeft;
      fTop:=(fMaxHeight-fHeight) div 2;
      fBottom:=fMaxHeight-fHeight-fTop;
      //Определяем и пишем заголовки
      BMPHeader.x:=fMaxWidth;
      BMPHeader.y:=fMaxHeight;
      BMPheader.DataSize1:=BMPHeader.x*BMPHeader.y;
      BMPheader.DataSize2:=BMPHeader.x*BMPHeader.y;
      BMPHeader.FullSize:=BMPHeader.DataSize1+$436;
      BMPHeader._H3:=BMPHeader.FullSize-$36;
      FTemp.Write(bmpID,2);
      FTemp.Write(BMPHeader,SizeOf(BMPHeader));
      FTemp.Write(BMPPalette,1024);
      Add:=Round(Int((4*(1-(Frac(fMaxWidth/4))))));
      if Add=4 then Add:=0;
      FillChar(Buffer,1024,$00);
      FillChar(FreeBuffer,1024,$00);
      //Конвертируем
      if fTop>0 then for z:=1 to fTop do FMem.Write(Buffer,fMaxWidth+Add);
      y:=0;
      repeat
        if fLeft>0 then fMem.Write(FreeBuffer,fLeft);
        x:=0;
        repeat
          FData.Read(DByteL,1);
          FData.Read(DByteR,1);
          case DByteR of
          $00:begin
                FData.Read(Buffer,DByteL*2);
                for z:=1 to DByteL do
                begin
                  SL:=Buffer[z*2] shr 4;
                  SR:=Buffer[z*2] and $0F;
                  SS:=0;
                  SS:=SR;
                  SS:=SS shl 8;
                  SS:=SS+Buffer[z*2-1];
                  SS:=SS shr 1;
                  Buffer[z]:=SS;
                end;
                FMem.Write(Buffer,DByteL);
                x:=x+DByteL;
              end;
          $40:begin
                for z:=1 to DByteL do FMem.Write(FreeBuffer,fLeft+fRight+Add+fWidth);
                y:=y+DByteL;
                x:=fWidth;
              end;
          $80:begin
                FMem.Write(FreeBuffer,DByteL);
                x:=x+DByteL;
              end;
          end;
        until x=fWidth;
        if DByteR<>$40 then
        begin
          if fRight>0 then fMem.Write(FreeBuffer,fRight);
          if Add>0 then FMem.Write(FreeBuffer,Add);
          Inc(y);
        end;
      until y=fHeight;
      if fBottom>0 then for z:=1 to fBottom do FMem.Write(Buffer,fMaxWidth+Add);
      for y:=1 to fMaxHeight do
      begin
        FMem.Seek((fMaxHeight-y)*(fMaxWidth+Add),spBegin);
        Stream2Stream(FTemp,FMem,fMaxWidth+Add);
      end;
      FTemp.Seek(0,spBegin);
      fImageComplex.Items[i-1].LoadImageFromStream(FTemp);
      fImageComplex.Items[i-1].Name:=fOnlyName+BuildStr(Int2Str(i),'0',3)+'.bmp';
      FMem.Free;
      FTemp.Free;
      fProgressBar.Progress:=Round(Int(100*(i/TotalSprites)));
    end;
  end else
  if Extention='256' then
  begin
    FData.Seek($400,spBegin);
    SavePosition:=FData.Position;
    for i:=1 to TotalSprites do
    begin
      FData.Seek(SavePosition,spBegin);
      FData.Read(fWidth,4);
      FData.Read(fHeight,4);
      FData.Read(fDataSize,4);
      SavePosition:=SavePosition+fDataSize+12;
      //Создаем потоки и определяем некоторые параметры
      FTemp:=NewMemoryStream; //Основной поток для BMP
      FMem:=NewMemoryStream;  //Временный поток
      fLeft:=(fMaxWidth-fWidth) div 2;
      fRight:=fMaxWidth-fWidth-fLeft;
      fTop:=(fMaxHeight-fHeight) div 2;
      fBottom:=fMaxHeight-fHeight-fTop;
      //Определяем и пишем заголовки
      BMPHeader.x:=fMaxWidth;
      BMPHeader.y:=fMaxHeight;
      BMPheader.DataSize1:=BMPHeader.x*BMPHeader.y;
      BMPheader.DataSize2:=BMPHeader.x*BMPHeader.y;
      BMPHeader.FullSize:=BMPHeader.DataSize1+$436;
      BMPHeader._H3:=BMPHeader.FullSize-$36;
      FTemp.Write(bmpID,2);
      FTemp.Write(BMPHeader,SizeOf(BMPHeader));
      FTemp.Write(BMPPalette,1024);
      Add:=Round(Int((4*(1-(Frac(fMaxWidth/4))))));
      if Add=4 then Add:=0;
      FillChar(Buffer,1024,$00);
      FillChar(FreeBuffer,1024,$00);
      if fTop>0 then for z:=1 to fTop do FMem.Write(Buffer,fMaxWidth+Add);
      y:=0;
      repeat
        x:=0;
        if fLeft>0 then fMem.Write(FreeBuffer,fLeft);
        repeat
          FData.Read(DByte,1);
          Expand(DByte,DByteL,DByteR);
          case DByteL of
          $0:begin
               Stream2Stream(FMem,FData,DByteR);
               x:=x+DByteR;
             end;
          $1:begin
               FMem.Write(Buffer,fWidth+fRight+Add);
               if DByteR>1 then for z:=1 to DByteR-1 do FMem.Write(Buffer,fMaxWidth+Add);
               Inc(y,DByteR);
               x:=fWidth;
             end;
          $2:begin
               FMem.Write(Buffer,DByteR);
               x:=x+DByteR;
             end;
          end;
        until x>=fWidth;
        if DByteL<>1 then if fRight>0 then fMem.Write(Buffer,fRight);
        if DByteL<>1 then if Add>0 then fMem.Write(Buffer,Add);
        if DByteL<>1 then Inc(y);
      until y>=fHeight;
      if fBottom>0 then for j:=1 to fBottom do fMem.Write(Buffer,fMaxWidth+Add);
      for y:=1 to fMaxHeight do
      begin
        FMem.Seek((fMaxHeight-y)*(fMaxWidth+Add),spBegin);
        Stream2Stream(FTemp,FMem,fMaxWidth+Add);
      end;
      FTemp.Seek(0,spBegin);
      fImageComplex.Items[i-1].LoadImageFromStream(FTemp);
      fImageComplex.Items[i-1].Name:=fOnlyName+BuildStr(Int2Str(i),'0',3)+'.bmp';
      FMem.Free;
      FTemp.Free;
      fProgressBar.Progress:=Round(Int(100*(i/TotalSprites)));
    end;
  end;
  FData.Free;
  fForm.Hide;
end;

exports
  GetPluginInfo,
  GetFileInfo,
  GetArchiveContent,
  GetFileAutoload,
  UnpackArchive,
  GetPluginID,
  GetImageComplex;

begin
end.
