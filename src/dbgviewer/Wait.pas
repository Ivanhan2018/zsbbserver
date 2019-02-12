unit Wait;

interface

uses
  Messages, Forms, Classes, Controls, StdCtrls, ComCtrls;

const
  WM_HIDE_DLG = $0400 + $0101;
  WM_SHOW_DLG = $0400 + $0102;

  type
  TfrmWait = class(TForm)
    TabControl1: TTabControl;
    Label1: TLabel;
  private
    procedure WMHideDlg(var Message: TMessage); message WM_HIDE_DLG;
    procedure WMShowDlg(var Message: TMessage); message WM_SHOW_DLG;
   { Private declarations }
  public
    { Public declarations }
  end;

var frmWait: TfrmWait;

implementation

{$R *.dfm}

procedure TfrmWait.WMHideDlg(var Message: TMessage);
begin
  frmWait.Close;
end;

procedure TfrmWait.WMShowDlg(var Message: TMessage);
begin
  frmWait.Show;
  Application.ProcessMessages;
end;

end.

