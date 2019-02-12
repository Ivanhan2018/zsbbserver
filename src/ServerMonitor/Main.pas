unit Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Tlhelp32, ExtCtrls, ComCtrls, IniFiles, Menus;

type
  TProInfo = class
    PID: DWORD;
    MHD: HWND;
  end;

  TForm1 = class(TForm)
    Timer1: TTimer;
    MainMenu1: TMainMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    N6: TMenuItem;
    N7: TMenuItem;
    N8: TMenuItem;
    ListBox1: TListBox;
    ListBox2: TListBox;
    N9: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure N2Click(Sender: TObject);
    procedure N3Click(Sender: TObject);
    procedure N4Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure N5Click(Sender: TObject);
    procedure N9Click(Sender: TObject);
  private
    HashStrings: THashedStringList;
    { Private declarations }
  public
    { Public declarations }
  end;

{$EXTERNALSYM init}
function init(address: PChar; port: SmallInt): Integer; stdcall;
{$EXTERNALSYM fini}
procedure fini; stdcall;
{$EXTERNALSYM log}
function log(Msg: PChar; Len: Integer): Integer; stdcall;

var
  Form1: TForm1;

implementation

uses Unit2;

{$R *.dfm}

function init; external 'report.dll' name 'init';
procedure fini; external 'report.dll' name 'fini';
function log; external 'report.dll' name 'log';

function EnumWindowsProc(hw: Hwnd; lParam: DWORD): boolean; stdcall;
var
  dwPID: DWORD;
  hWndParent, hWndChild: HWND;
  Title: array[0..144] of Char;
  ExeName: string;
  pi: TProInfo;
begin
  Result := True;
  GetWindowThreadProcessId(hw, @dwPID);
  if dwPID = lParam then
  begin
    hWndChild := hw;
    hWndParent := GetParent(hWndChild);
    while (hWndParent <> 0) do
    begin
      hWndChild := hWndParent;
      hWndParent := GetParent(hWndChild);
    end;
    GetWindowText(hWndChild, Title, 144);
    ExeName := UpperCase(ExtractFileName(Title));

    pi := TProInfo.Create;
    pi.PID := dwPID;
    pi.MHD := hWndChild;
    Form1.HashStrings.AddObject(ExeName, pi);
    Result := False;
  end;
end;

procedure EnumProcess;
var
  ProcessList: THandle;
  Pe: TPROCESSENTRY32;
  ExeName: string;
  I: Integer;
begin
  Form1.HashStrings.BeginUpdate;
  for I := 0 to Form1.HashStrings.Count - 1 do
    Form1.HashStrings.Objects[i].Free;
  Form1.HashStrings.Clear;

  ProcessList := CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  Pe.dwSize := SizeOf(TPROCESSENTRY32);
  while Process32next(ProcessList, Pe) do
  begin
    ExeName := UpperCase(ExtractFileName(Pe.szexefile));
//      (ExeName = 'UM_WEB_SERVER.EXE') or
//      (ExeName = 'LOGSVC.EXE') or
//      (ExeName = 'LOGINSVC.EXE') or
//      (ExeName = 'DBGATEWAYSVC.EXE') or

    if (ExeName = 'TASKSERVER.EXE') or
      (ExeName = 'MANAGERSVC.EXE') or
      (ExeName = 'GMSERVER.EXE') or
      (ExeName = 'ECTYPESYSTEM.EXE') or
      (ExeName = 'COUNTRYMAPSVR.EXE') or
      (ExeName = 'DATASERVER.EXE') or
      (ExeName = 'CONSORTIASERVER.EXE') or
      (ExeName = 'CHATSERVER.EXE') or
      (ExeName = 'CHARGESERVER.EXE') or
      (ExeName = 'AGENTSVC.EXE') or
      (ExeName = 'GAMEHALL.EXE') or
      (ExeName = 'LOGSVC_GAME.EXE') then
    begin
      EnumWindows(@EnumWindowsProc, Pe.th32ProcessID);
    end;
  end;
  Form1.HashStrings.EndUpdate;
  CloseHandle(ProcessList);
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  HashStrings := THashedStringList.Create;
  ListBox1.Items.LoadFromFile('ServerMonitor.list');
  ListBox2.Items.LoadFromFile('ServerMonitor.address');
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  fini;
  HashStrings.Free;
  ListBox1.Items.SaveToFile('ServerMonitor.list');
end;

procedure TForm1.N2Click(Sender: TObject);
begin
  if (not N4.Enabled) then
  begin
    ShowMessage('正处于监控状态');
  end else
  begin
    with TForm2.Create(Self) do
    try
      if (ShowModal = mrOK) then
      begin
        ListBox1.Items.BeginUpdate;
        ListBox1.Items.Add(UpperCase(Edit1.Text));
        ListBox1.Items.EndUpdate;
      end;
    finally
      Free;
    end;
  end;
end;

procedure TForm1.N3Click(Sender: TObject);
begin
  if (not N4.Enabled) then
  begin
    ShowMessage('正处于监控状态');
  end else if (ListBox1.SelCount = 0) then
  begin
    ShowMessage('请选择要删除的项目');
  end else
  begin
    ListBox1.Items.BeginUpdate;
    ListBox1.DeleteSelected;
    ListBox1.Items.EndUpdate;
  end;
end;

procedure TForm1.N4Click(Sender: TObject);
var I: Integer;
  path: string;
begin
  if (ListBox1.Items.Count = 0) then
  begin
    ShowMessage('至少需要一个项目才能执行该功能');
  end
  else if init(PChar(ListBox2.Items[0]), SmallInt(StrToInt(ListBox2.Items[1]))) = 0 then
  begin
    ShowMessage('连接日志服务器失败');
  end else
  begin
    EnumProcess;

    for I := 0 to ListBox1.Items.Count - 1 do
    begin
      if HashStrings.IndexOf(ListBox1.Items[i]) = -1 then
      begin
        try
          StrToInt(ListBox1.Items[i]);
          path := ExtractFilePath(Application.ExeName) +
            'COUNTRYMAPSVR.EXE ' + ListBox1.Items[i];
        except
          path := ExtractFilePath(Application.ExeName) + ListBox1.Items[i];
        end;

        WinExec(PChar(path), SW_SHOW);
        Sleep(1000);
      end;
    end;

    Timer1.Enabled := True;
    N4.Enabled := False;
    N5.Enabled := True;
  end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var I: Integer;
  path, msg: string;
begin
  Timer1.Enabled := False;
  EnumProcess;
  for I := 0 to ListBox1.Items.Count - 1 do
  begin
    if HashStrings.IndexOf(ListBox1.Items[i]) = -1 then
    begin
      try
        StrToInt(ListBox1.Items[i]);
        path := ExtractFilePath(Application.ExeName) +
          'COUNTRYMAPSVR.EXE ' + ListBox1.Items[i];
      except
        path := ExtractFilePath(Application.ExeName) + ListBox1.Items[i];
      end;
      msg := '服务器(' + ListBox1.Items[i] + ')非法退出';
      log(PChar(msg), Length(msg));

      WinExec(PChar(path), SW_SHOW);
      Sleep(500);
    end;
  end;
  Timer1.Enabled := True;
end;

procedure TForm1.N5Click(Sender: TObject);
var I: Integer;
begin
  if (ListBox1.Items.Count = 0) then
  begin
    ShowMessage('至少需要一个项目才能执行该功能');
  end else
  begin
    Timer1.Enabled := False;
    fini;

    EnumProcess;
    for I := 0 to Form1.HashStrings.Count - 1 do
      PostMessage((HashStrings.Objects[i] as TProInfo).MHD, WM_CLOSE, 0, 0);

    N4.Enabled := True;
    N5.Enabled := False;
  end;
end;

procedure TForm1.N9Click(Sender: TObject);
var I: Integer;
begin
  EnumProcess;
  for I := 0 to Form1.HashStrings.Count - 1 do
    PostMessage((HashStrings.Objects[i] as TProInfo).MHD, WM_CLOSE, 0, 0);
end;

end.

