object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 181
  ClientWidth = 288
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 12
    Top = 98
    Width = 161
    Height = 68
    Caption = 'Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 68
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Edit1: TEdit
    Left = 12
    Top = 16
    Width = 245
    Height = 76
    Hint = 
      'Timer is in seconds'#13#10#8593','#8595' - adjust interval'#13#10'Enter - start/stop t' +
      'imer, stop alarm'#13#10'Esc - minimize app, stop alarm'
    AutoSelect = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 68
    Font.Name = 'Tahoma'
    Font.Style = []
    NumbersOnly = True
    ParentFont = False
    TabOrder = 0
    Text = '1'
    OnKeyPress = Edit1KeyPress
  end
  object UpDown1: TUpDown
    Left = 257
    Top = 16
    Width = 17
    Height = 76
    Associate = Edit1
    Min = 1
    Max = 32766
    Position = 1
    TabOrder = 1
    Thousands = False
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 96
  end
  object Timer2: TTimer
    Enabled = False
    OnTimer = Timer2Timer
    Left = 156
  end
end
