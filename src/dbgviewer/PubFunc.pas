unit PubFunc;

interface

uses DB, DBTables, Forms;

const
  WM_HIDE_DLG = $0400 + $0101;
  WM_SHOW_DLG = $0400 + $0102;

function CustomExecSql(var AQuery: TQuery; ASQL: string; ShowWaitDlg: Boolean;
  OpenFlag: Boolean = True): Boolean;

procedure ShowForm(FmClass: TFormClass; ModalFlag: Boolean = False);

//function SimpleFind(APrompt: string; var Value: string): Boolean;

////////////////////////////////////////////////////////////////////////////////

implementation

uses Wait, Windows;

function CustomExecSql(var AQuery: TQuery; ASQL: string; ShowWaitDlg: Boolean;
  OpenFlag: Boolean): Boolean;
begin
  Result := AQuery <> nil;
  if not Result then
    Exit;
  with AQuery do
  try
    Close;
    SQL.Clear;
    SQL.Add(ASQL);
    Prepare;
    if ShowWaitDlg then
      SendMessage(frmWait.Handle, WM_SHOW_DLG, 0, 0);
    if OpenFlag then
      Open
    else
      ExecSQL;
    if ShowWaitDlg then
      SendMessage(frmWait.Handle, WM_HIDE_DLG, 0, 0);
  except
    Result := False;
  end;
end;

//打开窗体 Falg=False Show模式,True ShowModal模式

procedure ShowForm(FmClass: TFormClass; ModalFlag: Boolean);
var FmCount: Integer;
  FmTmp: TForm;
  Bool: Boolean;
begin
  FmTmp := nil;
  Bool := False;
  try
    //获得屏幕窗口
    for FmCount := 0 to Screen.FormCount - 1 do
    begin
      FmTmp := Screen.Forms[FmCount];
      Bool := FmTmp.ClassType = FmClass;
      //如果当前是目标窗口则结束
      if Bool then
        Break
    end;
    if not ModalFlag then
      SendMessage(frmWait.Handle, WM_SHOW_DLG, 0, 0);
    if Bool then
    begin
      //设置窗体风格
      if FmTmp.WindowState = wsMinimized then
        FmTmp.WindowState := wsNormal;
      FmTmp.BringToFront;
      FmTmp.Setfocus;
    end
    else
    begin
      //创建实例
      FmTmp := FmClass.Create(Application);
    end;
    if ModalFlag then
    begin
      //以模态方式打开
      FmTmp.ShowModal;
      FmTmp.Free;
    end else
      SendMessage(frmWait.Handle, WM_HIDE_DLG, 0, 0);
  except
    FmTmp.Free;
  end;
end;

{
function SimpleFind(APrompt: string; var Value: string): Boolean;
begin
  with TfrmSimpleFind.Create(Application) do
  try
    lb_Prompt.Caption := APrompt + ':';
    Result := ShowModal = ID_OK;
    if Result then
      Value := Edt_Value.Text;
  finally
    Free;
  end;
end;
}

end.

