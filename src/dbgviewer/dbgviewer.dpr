program dbgviewer;

uses
  Forms,
  main in 'main.pas' {Form1},
  PubFunc in 'PubFunc.pas',
  Wait in 'Wait.pas' {frmWait};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TfrmWait, frmWait);
  Application.Run;
end.
