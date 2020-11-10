unit data_classes_imagecomplex;

interface

uses kol;

type

  TImageSimple=class
  private
    fBitmap: PBitmap;
    fName: string;
    function GetWidth: Integer;
    function GetHeight: Integer;
    function GetPixelFormat: TPixelFormat;
  protected
  public
    constructor Create;
    procedure LoadImageFromStream(fStream: PStream);
    property Name: string read fName write fName;
    property Width: Integer read GetWidth;
    property Height: Integer read GetHeight;
    property PixelFormat: TPixelFormat read GetPixelFormat;
    property Bitmap: PBitmap read fBitmap;
  end;

  TImageComplex=class
  private
    fItems: PList;
    function GetItem(Index: Integer): TImageSimple;
    procedure SetItem(Index: Integer; Value: TImageSimple);
  protected
  public
    constructor Create;
    procedure Add;
    function Count: Integer;
    procedure Clear;
    property Items[Index: Integer]: TImageSimple read GetItem write SetItem;
  end;

implementation

constructor TImageSimple.Create;
begin
  fBitmap:=NewBitmap(1,1);
end;

function TImageSimple.GetWidth: Integer;
begin
  Result:=fBitmap.Width;
end;

function TImageSimple.GetHeight: Integer;
begin
  Result:=fBitmap.Height;
end;

function TImageSimple.GetPixelFormat: TPixelFormat;
begin
  Result:=fBitmap.PixelFormat;
end;

constructor TImageComplex.Create;
begin
  fItems:=NewList;
end;

function TImageComplex.GetItem(Index: Integer): TImageSimple;
begin
  Result:=fItems.Items[Index];
end;

procedure TImageComplex.SetItem(Index: Integer; Value: TImageSimple);
begin
  fItems.Items[Index]:=Value;
end;

procedure TImageComplex.Add;
begin
  fItems.Add(TImageSimple.Create);
end;

function TImageComplex.Count;
begin
  Result:=fItems.Count;
end;

procedure TImageComplex.Clear;
var
  i: Integer;
begin
  if Count>0 then
  for i:=1 to Count do
  begin
    Items[i-1].fBitmap.Free;
    TImageSimple(Items[i-1]).Free;
  end;
  fItems.Clear;
end;

procedure TImageSimple.LoadImageFromStream(fStream: PStream);
begin
  fBitmap.LoadFromStream(fStream);
end;

end.
