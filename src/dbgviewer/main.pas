unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, DB, ADODB, StdCtrls, ExtCtrls, Grids, DBGrids, dxExEdtr,
  dxCntner, dxTL, dxDBCtrl, dxDBGrid, dxEditor, dxEdLib, cxGraphics,
  cxControls, dxStatusBar;

type
  TForm1 = class(TForm)
    ADOConnection1: TADOConnection;
    DataSource1: TDataSource;
    ADOQuery1: TADOQuery;
    Panel1: TPanel;
    Label2: TLabel;
    Button2: TButton;
    dxEdit2: TdxEdit;
    Label3: TLabel;
    dxEdit1: TdxEdit;
    Label4: TLabel;
    dxEdit3: TdxEdit;
    dxDBGrid1: TdxDBGrid;
    Image1: TImage;
    Splitter1: TSplitter;
    Panel2: TPanel;
    ListBox1: TListBox;
    dxStatusBar1: TdxStatusBar;
    procedure dxDBGrid1BackgroundDrawEvent(Sender: TObject;
      ACanvas: TCanvas; ARect: TRect);
    procedure Button2Click(Sender: TObject);
    procedure ListBox1DblClick(Sender: TObject);
    procedure ADOQuery1FilterRecord(DataSet: TDataSet;
      var Accept: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

//{$EXTERNALSYM save_to_db}
//function save_to_db(address: PChar; port: SmallInt; table: Integer): Integer; stdcall;

implementation

uses Wait, dxDemoUtils;

{$R *.dfm}

//const DBGVIEWER_PATH = 'dbgviewer.dll';

const CMD_PLAYER_ACCOUNTS = $01;
const CMD_GAME_CITY_INFO = $02;
const CMD_PLAYER_SOLDIER_RACE = $03;
const CMD_EXE_SOLDIER = $04;
const CMD_RESEARCH_CONFIGURE = $05;
const CMD_ARMY_TABLE = $06;
const CMD_MARKET = $07;
const CMD_OFFICER_MEDAL = $08;
const CMD_CONSTRUCTION_BUILD = $09;
const CMD_PRODUCTION_ACTIVITY = $0A;
const CMD_CONSORTIA_INFO = $0B;
const CMD_CONSORTIA_BANK = $0C;
const CMD_CONSORTIA_UNION = $0D;
const CMD_PLAYER_OFFICER_ITEM_TABLE = $0E;
const CMD_PLAYER_ITEM_STORAGE_TABLE = $0F;
const CMD_SOLDIER_PARAM_TABLE = $10;
//const CMD_RACE_GENIUS_TABLE = $11;
const CMD_PLAYER_OFFICER_TABLE = $12;
const CMD_PLAYER_SOLDIER_TABLE = $13;
const CMD_PLAYER_RESOURCE = $14;
const CMD_CONSTRUCTION_CONFIGURE = $15;
const CMD_PLAYER_CONSTRUCTION = $16;
const CMD_PLAYER_TASK = $17;
const CMD_CONSORTIA_PERSONNEL = $18;
const CMD_PRIZE_VENDUE = $19;
const CMD_PRIZE_DISTRIBUTE = $1A;
const CMD_CELL_PRICE = $1B;
const CMD_CITY_ATTRIBUTE = $1C;
const CMD_CREATE_CONSORTIA = $1D;
const CMD_CONSORTIA_FIGHT = $1E;
const CMD_CONTRIBUTE_CONFIGURE = $1F;
const CMD_PRODUCTION_AMOUNT = $20;
const CMD_TASK_BASE_TABLE = $21;
const CMD_PRODUCE_CONFIGURE = $22;
const CMD_CHAT_ROOM_TABLE = $23;
const CMD_PLAYER_CHAT_ROOM_TABLE = $24;
const CMD_PLAYER_CITY = $25;
const CMD_ALLOT_OFFICER = $26;
const CMD_OFFICER_ITEM_ALLOT = $27;
const CMD_OFFICER_TECH = $28;
const CMD_WEAPON_ATTRIBUTE = $29;
const CMD_DEFEND_ATTRIBUTE = $2A;
const CMD_OFFICER_JOB_ATTRIBUTE = $2B;
const CMD_OFFICER_RACE_ATTRIBUTE = $2C;
const CMD_ARMY_OUT_TABLE = $2D;
const CMD_NPC = $2E;
const CMD_MAP_INFO_TABLE = $2F;
const CMD_RESEARCH_ITEM = $30;
const CMD_COUNT_RESOURCE = $31;
const CMD_ECTYPE_BASE_TABLE = $32;
const CMD_ECTYPE_NPC_INFO_TABLE = $33;
const CMD_PLAYER_CAPTIVE_HERO = $34;
const CMD_EQUIP_ATTRIBUTE = $35;
const CMD_WAR_NOTE = $36;
const CMD_PULLULATE_NOTE = $37;
const CMD_MAIL_TABLE = $38;
const CMD_FIREND_TABLE = $39;
const CMD_NEWS_TABLE = $3A;
const CMD_CONSORTIA_OFFICIAL = $3B;
const CMD_LOAD_MATERIAL = $3C;
const CMD_TREASURY = $3D;
const CMD_CARD_CONFIG = $3E;
const CMD_PUBLIC_BUILD_PARAM_TABLE = $3F;
const CMD_PUBLIC_BUILD_TABLE = $40;
const CMD_PLAYER_PRODUCE_STATE = $41;
const CMD_CHARGE_SVR_TIMER = $42;
const CMD_JOB_PARAM = $43;
const CMD_BIN_CONFIG = $44;

const CMD_EQUIP_ANTAGONISM = $45;
const CMD_BATTTLE_FIELD = $46;
const CMD_CONSORTIA_COMPARE = $47;
const CMD_PLAYER_COMPARE = $48;
const CMD_COUNCIL = $49;
const CMD_CANDIDATE = $4A;
const CMD_BALLOT = $4B;
const CMD_COUNTRY = $4C;
const CMD_RESEARCH_ACTIVITY = $4D;
const CMD_LEVY_SOLDIER_ACTIVITY = $4E;


//function save_to_db; external DBGVIEWER_PATH name 'save_to_db';

procedure PaintImageTiled(ACanvas: TCanvas; ARect: TRect; ABitmap: TBitmap);
var
  X, Y: Integer;
begin
  X := ARect.Left;
  repeat
    Y := ARect.Top;
    repeat
      ACanvas.Draw(X, Y, ABitmap);
      Inc(Y, ABitmap.Height);
    until Y >= ARect.Bottom;
    Inc(X, ABitmap.Width);
  until X >= ARect.Right
end;

procedure TForm1.dxDBGrid1BackgroundDrawEvent(Sender: TObject;
  ACanvas: TCanvas; ARect: TRect);
var
  CustomDrawRgn: TdxCustomDrawRegion;
begin
  CustomDrawRgn := TdxCustomDrawRegion.Create(ACanvas.Handle, ARect);
  try
    PaintImageTiled(ACanvas, ARect, Image1.Picture.Bitmap);
  finally
    CustomDrawRgn.Free;
  end;

end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  if (dxEdit2.Text = '') or (dxEdit1.Text = '') or (dxEdit3.Text = '') then
  begin
    ShowMessage('请输入服务器地址、账号、密码后再连接');
    Exit;
  end;
  SendMessage(frmWait.Handle, WM_SHOW_DLG, 0, 0);

  try
    ADOConnection1.Close;
    ADOConnection1.ConnectionString := 'Provider=SQLOLEDB.1;Password=' + dxEdit3.Text +
      ';Persist Security Info=True;User ID=' + dxEdit1.Text +
      ';Initial Catalog=GSGameDB;Data Source=' + dxEdit2.Text +
      ';Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;' +
      'Workstation ID=WIND-9ZPNCPHUAU;Use Encryption for Data=False;Tag with column collation when possible=False';
    ADOConnection1.Open;
  except
    SendMessage(frmWait.Handle, WM_HIDE_DLG, 0, 0);
    ShowMessage('连接数据库失败');
    Exit;
  end;

  ListBox1.Enabled := True;
  dxEdit2.Enabled := False;
  dxEdit1.Enabled := False;
  dxEdit3.Enabled := False;
  Button2.Enabled := False;
  SendMessage(frmWait.Handle, WM_HIDE_DLG, 0, 0);

end;

procedure TForm1.ListBox1DblClick(Sender: TObject);
var Cmd: Integer;
begin
//  Cmd := 0;
  SendMessage(frmWait.Handle, WM_SHOW_DLG, 0, 0);
{  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerAccounts' then Cmd := CMD_PLAYER_ACCOUNTS;
  if ListBox1.Items[ListBox1.ItemIndex] = 'GameCityInfo' then Cmd := CMD_GAME_CITY_INFO;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerSoldierRace' then Cmd := CMD_PLAYER_SOLDIER_RACE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ExeSoldier' then Cmd := CMD_EXE_SOLDIER;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ProductionAmount' then Cmd := CMD_PRODUCTION_AMOUNT;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConstructionConfigure' then Cmd := CMD_CONSTRUCTION_CONFIGURE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ResearchConfigure' then Cmd := CMD_RESEARCH_CONFIGURE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerConstruction' then Cmd := CMD_PLAYER_CONSTRUCTION;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerResource' then Cmd := CMD_PLAYER_RESOURCE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ArmyTable' then Cmd := CMD_ARMY_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'Market' then Cmd := CMD_MARKET;
  if ListBox1.Items[ListBox1.ItemIndex] = 'OfficerMedal' then Cmd := CMD_OFFICER_MEDAL;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConstructionBuild' then Cmd := CMD_CONSTRUCTION_BUILD;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ProductionActivity' then Cmd := CMD_PRODUCTION_ACTIVITY;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConsortiaInfo' then Cmd := CMD_CONSORTIA_INFO;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConsortiaBank' then Cmd := CMD_CONSORTIA_BANK;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConsortiaUnion' then Cmd := CMD_CONSORTIA_UNION;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerOfficerItemTable' then Cmd := CMD_PLAYER_OFFICER_ITEM_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerItemStorageTable' then Cmd := CMD_PLAYER_ITEM_STORAGE_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'SoldierParamTable' then Cmd := CMD_SOLDIER_PARAM_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerOfficerTable' then Cmd := CMD_PLAYER_OFFICER_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerTask' then Cmd := CMD_PLAYER_TASK;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerSoldierTable' then Cmd := CMD_PLAYER_SOLDIER_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConsortiaPersonnel' then Cmd := CMD_CONSORTIA_PERSONNEL;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PrizeVendue' then Cmd := CMD_PRIZE_VENDUE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PrizeDistribute' then Cmd := CMD_PRIZE_DISTRIBUTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'CellPrice' then Cmd := CMD_CELL_PRICE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'CityAttribute' then Cmd := CMD_CITY_ATTRIBUTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'CreateConsortia' then Cmd := CMD_CREATE_CONSORTIA;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConsortiaFight' then Cmd := CMD_CONSORTIA_FIGHT;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ContributeConfigure' then Cmd := CMD_CONTRIBUTE_CONFIGURE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'TaskBaseTable' then Cmd := CMD_TASK_BASE_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ProduceConfigure' then Cmd := CMD_PRODUCE_CONFIGURE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ChatRoomTable' then Cmd := CMD_CHAT_ROOM_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerChatRoomTable' then Cmd := CMD_PLAYER_CHAT_ROOM_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerCity' then Cmd := CMD_PLAYER_CITY;
  if ListBox1.Items[ListBox1.ItemIndex] = 'AllotOfficer' then Cmd := CMD_ALLOT_OFFICER;
  if ListBox1.Items[ListBox1.ItemIndex] = 'OfficerItemAllot' then Cmd := CMD_OFFICER_ITEM_ALLOT;
  if ListBox1.Items[ListBox1.ItemIndex] = 'OfficerRaceAttribute' then Cmd := CMD_OFFICER_RACE_ATTRIBUTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'OfficerJobAttribute' then Cmd := CMD_OFFICER_JOB_ATTRIBUTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'WeaponAttribute' then Cmd := CMD_WEAPON_ATTRIBUTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'DefendAttribute' then Cmd := CMD_DEFEND_ATTRIBUTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ArmyOutTable' then Cmd := CMD_ARMY_OUT_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'OfficerTech' then Cmd := CMD_OFFICER_TECH;
  if ListBox1.Items[ListBox1.ItemIndex] = 'NPC' then Cmd := CMD_NPC;
  if ListBox1.Items[ListBox1.ItemIndex] = 'MapInfoTable' then Cmd := CMD_MAP_INFO_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ResearchItem' then Cmd := CMD_RESEARCH_ITEM;
  if ListBox1.Items[ListBox1.ItemIndex] = 'CountResource' then Cmd := CMD_COUNT_RESOURCE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'EctypeBaseTable' then Cmd := CMD_ECTYPE_BASE_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'EctypeNPCInfoTable' then Cmd := CMD_ECTYPE_NPC_INFO_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerCaptiveHero' then Cmd := CMD_PLAYER_CAPTIVE_HERO;
  if ListBox1.Items[ListBox1.ItemIndex] = 'EquipAttribute' then Cmd := CMD_EQUIP_ATTRIBUTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'WarNote' then Cmd := CMD_WAR_NOTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PullulateNote' then Cmd := CMD_PULLULATE_NOTE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'MailTable' then Cmd := CMD_MAIL_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'FirendTable' then Cmd := CMD_FIREND_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'NewsTable' then Cmd := CMD_NEWS_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConsortiaOfficial' then Cmd := CMD_CONSORTIA_OFFICIAL;
  if ListBox1.Items[ListBox1.ItemIndex] = 'LoadMaterial' then Cmd := CMD_LOAD_MATERIAL;
  if ListBox1.Items[ListBox1.ItemIndex] = 'Treasury' then Cmd := CMD_TREASURY;
  if ListBox1.Items[ListBox1.ItemIndex] = 'CardConfig' then Cmd := CMD_CARD_CONFIG;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PublicBuildParamTable' then Cmd := CMD_PUBLIC_BUILD_PARAM_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PublicBuildTable' then Cmd := CMD_PUBLIC_BUILD_TABLE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerProduceState' then Cmd := CMD_PLAYER_PRODUCE_STATE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ChargeSvrTimer' then Cmd := CMD_CHARGE_SVR_TIMER;
  if ListBox1.Items[ListBox1.ItemIndex] = 'JobParam' then Cmd := CMD_JOB_PARAM;
  if ListBox1.Items[ListBox1.ItemIndex] = 'BinConfig' then Cmd := CMD_BIN_CONFIG;
  if ListBox1.Items[ListBox1.ItemIndex] = 'EquipAntagonism' then Cmd := CMD_EQUIP_ANTAGONISM;
  if ListBox1.Items[ListBox1.ItemIndex] = 'BatttleField' then Cmd := CMD_BATTTLE_FIELD;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ConsortiaCompare' then Cmd := CMD_CONSORTIA_COMPARE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'PlayerCompare' then Cmd := CMD_PLAYER_COMPARE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'Council' then Cmd := CMD_COUNCIL;
  if ListBox1.Items[ListBox1.ItemIndex] = 'Candidate' then Cmd := CMD_CANDIDATE;
  if ListBox1.Items[ListBox1.ItemIndex] = 'Ballot' then Cmd := CMD_BALLOT;
  if ListBox1.Items[ListBox1.ItemIndex] = 'Country' then Cmd := CMD_COUNTRY;
  if ListBox1.Items[ListBox1.ItemIndex] = 'ResearchActivity' then Cmd := CMD_RESEARCH_ACTIVITY;
  if ListBox1.Items[ListBox1.ItemIndex] = 'LevySoldierActivity' then Cmd := CMD_LEVY_SOLDIER_ACTIVITY;

  if save_to_db(PChar(dxEdit2.Text), 8899, Cmd) = 0 then
    ShowMessage('查询数据表：' + ListBox1.Items[ListBox1.ItemIndex] + '失败')
  else begin  }
    dxDBGrid1.Filter.Clear;
    dxDBGrid1.Bands.Clear;
    dxDBGrid1.Bands.Add;

    ADOQuery1.Close;
    ADOQuery1.SQL.Clear;
    ADOQuery1.SQL.Add('select * from ' + ListBox1.Items[ListBox1.ItemIndex]);
    ADOQuery1.Open;

    for Cmd := 0 to ADOQuery1.FieldCount - 1 do
    begin
      with dxDBGrid1.CreateColumn(TdxDBGridMaskColumn) as TdxDBGridMaskColumn do
        FieldName := ADOQuery1.Fields[Cmd].FieldName;

      if Cmd = 0 then dxDBGrid1.KeyField := ADOQuery1.Fields[Cmd].FieldName;
    end;
//  end;

  dxStatusBar1.Panels[1].Text := '当前共有 ' + IntToStr(ADOQuery1.RecordCount) + ' 条数据';
  SendMessage(frmWait.Handle, WM_HIDE_DLG, 0, 0);
end;

procedure TForm1.ADOQuery1FilterRecord(DataSet: TDataSet;
  var Accept: Boolean);
begin
  dxStatusBar1.Panels[1].Text := '当前共有 ' + IntToStr(ADOQuery1.RecordCount) + ' 条数据';
end;

end.

