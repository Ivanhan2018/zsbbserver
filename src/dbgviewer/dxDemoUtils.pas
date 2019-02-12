unit dxDemoUtils;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus;

type
  { TdxCustomDrawRegion }

(*
  TdxCustomDrawRegion
  Use TdxCustomDrawRegion to set and then restore clip region.
  For instance:
  var
    CustomDrawRgn: TdxCustomDrawRegion;
  begin
    //Set new Clip region, intersect new clip rect with the old one
    CustomDrawRgn := TdxCustomDrawRegion.Create(ARect);
    try
    ...
    finally
      //Destroy object and restore the old clip region
      CustomDrawRgn.Free;
    end;
*)

  TdxCustomDrawRegion = class
  private
    fOldClipRgn: HRgn;
    fCanvasHandle: THandle;
  protected
    procedure SaveClipRegion;
    procedure SetClipRegion(AClipRect: TRect);
    procedure RestoreClipRegion;
  public
    constructor Create(ACanvasHandle: THandle; AClipRect: TRect);
    destructor Destroy; override;
  end;

procedure ClearMenuItem(AMenuItem: TMenuItem);
function LightColor(AColor: TColor; M, D: Integer): TColor;
procedure LoadGraphicFilters(List: TStrings);
procedure ResetDefaultFontName(AWinControl: TWinControl);
procedure FillGrayGradientRect(ACanvas: TCanvas; const ARect: TRect; AEndColor: TColor);

implementation

type
  TControlCracker = class(TControl);

procedure ClearMenuItem(AMenuItem: TMenuItem);
begin
  while AMenuItem.Count > 0 do
    AMenuItem.Delete(0);
end;

function LightColor(AColor: TColor; M, D: Integer): TColor;
var
  ARGB: array [0..2] of Byte absolute Result;
  I: Integer;
begin
  Result := ColorToRGB(AColor);
  for I := 0 to 2 do
    ARGB[I] := ARGB[I] + MulDiv($FF - ARGB[I], M, D);
end;

procedure LoadGraphicFilters(List: TStrings);

  function ExtractFilter(const S: string; var Pos: Integer): string;
  var
    I: Integer;
  begin
    I := Pos;
    while (I <= Length(S)) and (S[I] <> '|') do
      Inc(I);
    if (I <= Length(S)) and (S[I] = '|') then Inc(I);
    while (I <= Length(S)) and (S[I] <> '|') do
      Inc(I);
    Result := Trim(Copy(S, Pos, I - Pos));
    if (I <= Length(S)) and (S[I] = '|') then Inc(I);
    Pos := I;
  end;

var
  I: Integer;
  S: string;
begin
  I := 1;
  S := GraphicFilter(TGraphic);
  while I <= Length(S) do
    List.Add(ExtractFilter(S, I));
end;

procedure ResetDefaultFontName(AWinControl: TWinControl);

  procedure ResetFontName(AParent: TWinControl; const AFontName: string);
  var
    AControl: TControl;
    I: Integer;
  begin
    with AParent do
      for I := 0 to ControlCount - 1 do
      begin
        AControl := Controls[I];
        TControlCracker(AControl).Font.Name := AFontName;
        if AControl is TWinControl then
          ResetFontName(AControl as TWinControl, AFontName);
      end;
  end;

var
  AFont: TFont;
begin
  AFont := TFont.Create;
  try
    TControlCracker(AWinControl).Font.Name := AFont.Name;
  finally
    AFont.Free;
  end;
  ResetFontName(AWinControl, TControlCracker(AWinControl).Font.Name);
end;

procedure FillGrayGradientRect(ACanvas: TCanvas; const ARect: TRect; AEndColor: TColor);
const
  HiColorByte = 255;
  N = 20;
var
  I, J, Delta, DeltaX, DeltaY, C: Integer;
  CRGB: array [0..3] of Byte absolute C;
  R: TRect;
begin
  C := ColorToRGB(AEndColor);
  Delta := (HiColorByte - CRGB[0]) div N;
  R := ARect;
  DeltaX := (R.Right - R.Left) div N;
  DeltaY := (R.Bottom - R.Top) div N;
  with ACanvas do
  begin
    for I := 0 to N - 1 do
    begin
      Brush.Color := C;
      FillRect(R);
      Dec(R.Right, DeltaX);
      Dec(R.Bottom, DeltaY);
      for J := 0 to 2 do
        CRGB[J] := CRGB[J] + Delta;
    end;
  end;
end;

{ TdxCustomDrawRegion }

constructor TdxCustomDrawRegion.Create(ACanvasHandle: THandle; AClipRect: TRect);
begin
  inherited Create;
  fCanvasHandle := ACanvasHandle;
  SaveClipRegion;
  SetClipRegion(AClipRect);
end;

destructor TdxCustomDrawRegion.Destroy;
begin
  RestoreClipRegion;
  inherited Destroy;
end;

procedure TdxCustomDrawRegion.SaveClipRegion;
begin
  fOldClipRgn := CreateRectRgn(0, 0, 0, 0);
  if (GetClipRgn(fCanvasHandle, fOldClipRgn) <> 1) then
  begin
    DeleteObject(fOldClipRgn);
    fOldClipRgn := 0;
  end;
end;

procedure TdxCustomDrawRegion.SetClipRegion(AClipRect: TRect);
begin
  with AClipRect do
    IntersectClipRect(fCanvasHandle, Left, Top, Right, Bottom);
end;

procedure TdxCustomDrawRegion.RestoreClipRegion;
begin
  SelectClipRgn(fCanvasHandle, fOldClipRgn);
  if (fOldClipRgn <> 0) then
    DeleteObject(fOldClipRgn);
end;

end.
