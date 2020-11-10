unit data_bmpheader;

interface

uses kol;

type
  TBMPPalette=array[0..255]of record
                                R,G,B,F: Byte;
                              end;
var
  BMPPalette: TBMPPalette;
  BMPID:array[1..2]of Char;
  BMPHeader:record
              FullSize: LongInt;
              _H1: LongInt;
              _C1: LongInt;
              _C2: LongInt;
              X: LongInt;
              Y: LongInt;
              _C3: Word;
              _C4: Word;
              _H2: LongInt;
              _H3: LongInt;
              DataSize1: LongInt;     //DataSize=X*Y+2*Y
              DataSize2: LongInt;     //DataSize=X*Y+2*Y
              _C5: array[1..8]of Byte;
            end;

  procedure LoadPalette(fName: string; Inverse: Boolean; var fBMPPalette: TBMPPalette);

implementation

procedure LoadPalette(fName: string; Inverse: Boolean; var fBMPPalette: TBMPPalette);
var
  F: PStream;
  i: Integer;
  fRGB: record
          R,G,B: Byte;
        end;
  fRGBF:record
          R,G,B,F: Byte;
        end;
begin
  case FileSize(fName) of
  768:begin
        F:=NewReadFileStream(fName);
        for i:=0 to 255 do
        begin
          F.Read(fRGB,3);
          if Inverse then
          begin
            fBMPPAlette[i].R:=fRGB.B;
            fBMPPAlette[i].B:=fRGB.R;
          end else
          begin
            fBMPPAlette[i].R:=fRGB.R;
            fBMPPAlette[i].B:=fRGB.B;
          end;
          fBMPPAlette[i].G:=fRGB.G;
          fBMPPAlette[i].F:=$00;
        end;
        F.Free;
      end;
  1024:begin
        F:=NewReadFileStream(fName);
        for i:=0 to 255 do
        begin
          F.Read(fRGBF,4);
          if Inverse then
          begin
            fBMPPAlette[i].R:=fRGBF.B;
            fBMPPAlette[i].B:=fRGBF.R;
          end else
          begin
            fBMPPAlette[i].R:=fRGBF.R;
            fBMPPAlette[i].B:=fRGBF.B;
          end;
          fBMPPAlette[i].G:=fRGBF.G;
          fBMPPAlette[i].F:=fRGBF.F;
        end;
        F.Free;
      end;
  else
  end;
end;

end.
