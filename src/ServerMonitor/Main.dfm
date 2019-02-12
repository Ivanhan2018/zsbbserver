object Form1: TForm1
  Left = 639
  Top = 255
  Width = 248
  Height = 323
  Caption = #26381#21153#22120#30417#25511#22120
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 12
  object ListBox1: TListBox
    Left = 0
    Top = 0
    Width = 240
    Height = 277
    Align = alClient
    ItemHeight = 12
    TabOrder = 0
  end
  object ListBox2: TListBox
    Left = 64
    Top = 56
    Width = 121
    Height = 97
    ItemHeight = 12
    Items.Strings = (
      '192.168.1.35'
      '3000')
    TabOrder = 1
    Visible = False
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = Timer1Timer
    Left = 88
    Top = 104
  end
  object MainMenu1: TMainMenu
    Left = 48
    Top = 104
    object N1: TMenuItem
      Caption = #25991#20214
      object N2: TMenuItem
        Caption = #28155#21152#39033#30446
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = #21024#38500#39033#30446
        OnClick = N3Click
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object N4: TMenuItem
        Caption = #24320#22987#30417#25511
        OnClick = N4Click
      end
      object N5: TMenuItem
        Caption = #20572#27490#30417#25511
        Enabled = False
        OnClick = N5Click
      end
      object N9: TMenuItem
        Caption = #20572#27490#25152#26377#31243#24207
        OnClick = N9Click
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object N8: TMenuItem
        Caption = #36864#20986
      end
    end
  end
end
