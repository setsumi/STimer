object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 206
  ClientWidth = 307
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  ShowHint = True
  OnContextPopup = FormContextPopup
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 12
    Top = 131
    Width = 161
    Height = 68
    Caption = 'Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 68
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = Label1Click
    OnContextPopup = FormContextPopup
  end
  object btnTopmost: TSpeedButton
    Left = 259
    Top = 135
    Width = 34
    Height = 57
    Hint = 'Always on Top (Ctrl-T)'
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'T'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -35
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = btnTopmostClick
  end
  object lblOverdue: TLabel
    Left = 134
    Top = 131
    Width = 165
    Height = 42
    Caption = 'lblOverdue'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 42
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    Visible = False
    OnClick = Label1Click
    OnContextPopup = FormContextPopup
  end
  object PageControl1: TPageControl
    Left = 8
    Top = 8
    Width = 289
    Height = 117
    ActivePage = TabSheet2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Style = tsFlatButtons
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Seconds'
      object Edit1: TEdit
        Left = 0
        Top = 3
        Width = 265
        Height = 76
        Hint = 
          'Ctrl+Tab - switch page'#13#10#8592','#8594','#8593','#8595' - adjust interval'#13#10'Enter - start' +
          '/stop timer, stop alarm'#13#10'Esc - minimize app, stop alarm'#13#10'Right C' +
          'lick - minimize app'
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
        Left = 265
        Top = 3
        Width = 16
        Height = 76
        Associate = Edit1
        Min = 1
        Max = 32766
        Position = 1
        TabOrder = 1
        Thousands = False
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Clock'
      ImageIndex = 1
      object DateTimePicker1: TDateTimePicker
        Left = 0
        Top = 3
        Width = 281
        Height = 76
        Date = 43830.000717592590000000
        Time = 43830.000717592590000000
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = 68
        Font.Name = 'Tahoma'
        Font.Style = []
        Kind = dtkTime
        ParentFont = False
        TabOrder = 0
        OnEnter = DateTimePicker1Enter
        OnKeyPress = Edit1KeyPress
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 176
    Top = 8
  end
  object Timer2: TTimer
    Enabled = False
    OnTimer = Timer2Timer
    Left = 212
    Top = 8
  end
  object Timer3: TTimer
    Enabled = False
    Interval = 1
    OnTimer = Timer3Timer
    Left = 248
    Top = 8
  end
  object tmrOverdue: TTimer
    Enabled = False
    OnTimer = tmrOverdueTimer
    Left = 196
    Top = 160
  end
end
