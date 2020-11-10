unit data_classes_archive;

interface

uses kol;

type

  TArchiveFile=class
  private
    fName: string;
    fPath: string;
    fOffset: LongInt;
    fSizeCompressed: LongInt;
    fSizeReal: LongInt;
    fFlagExtract: Boolean;
    fFlagCompressed: Boolean;
    fFilterIndex: Integer;
    fReserved_1: LongInt;
    fReserved_2: LongInt;
    fReserved_3: LongInt;
    fReserved_4: LongInt;
  protected
  public
    constructor Create(oName: PChar; oOffset,oSizeCompressed,oSizeReal: LongInt);
    property Name: string read fName write fName;
    property Offset: LongInt read fOffset write fOffset;
    property SizeCompressed: LongInt read fSizeCompressed write fSizeCompressed;
    property SizeReal: LongInt read fSizeReal write fSizeReal;
    property FlagExtract: Boolean read fFlagExtract write fFlagExtract;
    property FilterIndex: Integer read fFilterIndex write fFilterIndex;
    property FlagCompressed: Boolean read fFlagCompressed write fFlagCompressed;
    property Reserved_1: LongInt read fReserved_1 write fReserved_1;
    property Reserved_2: LongInt read fReserved_2 write fReserved_2;
    property Reserved_3: LongInt read fReserved_3 write fReserved_3;
    property Reserved_4: LongInt read fReserved_4 write fReserved_4;
    property Path: string read fPath write fPath;
  end;

  TArchiveFilter=class
  private
    fFilter: string;
    fExtention: string;
  protected
  public
    constructor Create(oFilter,oExtention: string);
    property Filter: string read fFilter write fFilter;
    property Extention: string read fExtention write fExtention;
  end;

  TArchive=class
  private
    fList: PList;
    fFilters: PList;
    fTotalFiles: LongInt;
    fFlagUseFilters: Boolean;
    fArchiveFullFileName: string;
    fSavePosition: LongInt;
    function GetItem(Index: Integer): TArchiveFile;
    procedure SetItem(Index: Integer; Value: TArchiveFile);
    function GetFilter(Index: Integer): TArchiveFilter;
    procedure SetFilter(Index: Integer; Value: TArchiveFilter);
  protected
  public
    constructor Create;
    function Count: LongInt;
    function CountFilters: LongInt;
    procedure Clear;
    procedure ClearFilters;
    procedure Delete(Index: Integer);
    procedure Add(fName: PChar; fOffset,fSizeCompressed, fSizeReal: LongInt);
    procedure AddFilter(fFilter,fExtention: string);
    property Items[Index: Integer]: TArchiveFile read GetItem write SetItem;
    property Filters[Index: Integer]: TArchiveFilter read GetFilter write SetFilter;
    property TotalFiles: LongInt read fTotalFiles write fTotalFiles;
    property FlagUseFilters: Boolean read fFlagUseFilters write fFlagUseFilters;
    property ArchiveFullFileName: string read fArchiveFullFileName write fArchiveFullFileName;
    property SavePosition: LongInt read fSavePosition write fSavePosition;
  end;

implementation

constructor TArchiveFilter.Create;
begin
  Filter:=oFilter;
  Extention:=oExtention;
end;

constructor TArchive.Create;
begin
  fList:=NewList;
  fFilters:=NewList;
end;

procedure TArchive.Delete(Index: Integer);
begin
  TArchiveFile(Items[Index]).Free;
  fList.Delete(Index);
end;

function TArchive.GetFilter(Index: Integer): TArchiveFilter;
begin
  Result:=fFilters.Items[Index];
end;

procedure TArchive.SetFilter(Index: Integer; Value: TArchiveFilter);
begin
  fFilters.Items[Index]:=Value;
end;

function TArchive.GetItem;
begin
  Result:=fList.Items[Index];
end;

procedure TArchive.SetItem;
begin
  fList.Items[Index]:=Value;
end;

procedure TArchive.AddFilter(fFilter,fExtention: string);
begin
  fFilters.Add(TArchiveFilter.Create(fFilter,fExtention));
end;

procedure TArchive.Add;
begin
  fList.Add(TArchiveFile.Create(fName,fOffset,fSizeCompressed,fSizeReal));
end;

constructor TArchiveFile.Create;
begin
  Name:=oName;
  Offset:=oOffset;
  SizeCompressed:=oSizeCompressed;
  SizeReal:=oSizeReal;
end;

function TArchive.Count;
begin
  Result:=fList.Count;
end;

function TArchive.CountFilters;
begin
  Result:=fFilters.Count;
end;

procedure TArchive.Clear;
var
  i: Integer;
begin
  if Count>0 then
  for i:=1 to Count do TArchiveFile(Items[i-1]).Free;
  fList.Clear;
end;

procedure TArchive.ClearFilters;
var
  i: Integer;
begin
  if CountFilters>0 then
  begin
    for i:=1 to CountFilters do TArchiveFilter(Filters[i-1]).Free;
  end;
  fFilters.Clear;
end;

end.
