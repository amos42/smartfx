object frmMain: TfrmMain
  Left = 201
  Top = 113
  AutoScroll = False
  Caption = 'BMP '#48320#54872' '#54532#47196#44536#47016
  ClientHeight = 626
  ClientWidth = 697
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = #44404#47548
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 11
  object SpeedButton1: TSpeedButton
    Left = 664
    Top = 16
    Width = 22
    Height = 22
    Caption = '...'
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Left = 664
    Top = 48
    Width = 22
    Height = 22
    Caption = '...'
    OnClick = SpeedButton2Click
  end
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 59
    Height = 11
    Caption = #49548#49828' '#54028#51068'  :'
  end
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 55
    Height = 11
    Caption = #52636#47141' '#54028#51068' :'
  end
  object Label3: TLabel
    Left = 8
    Top = 72
    Width = 65
    Height = 11
    Caption = #52636#47141' '#54028#51068' 2 :'
  end
  object SpeedButton3: TSpeedButton
    Left = 664
    Top = 72
    Width = 22
    Height = 22
    Caption = '...'
    OnClick = SpeedButton2Click
  end
  object btnConvert: TButton
    Left = 488
    Top = 352
    Width = 201
    Height = 73
    Caption = #48320' '#54872
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = #44404#47548
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = btnConvertClick
  end
  object Edit1: TEdit
    Left = 80
    Top = 16
    Width = 585
    Height = 19
    ImeName = #54620#44397#50612'('#54620#44544') (MS-IME98)'
    TabOrder = 1
  end
  object Edit2: TEdit
    Left = 80
    Top = 48
    Width = 585
    Height = 19
    ImeName = #54620#44397#50612'('#54620#44544') (MS-IME98)'
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 8
    Top = 104
    Width = 473
    Height = 329
    TabOrder = 3
    object Image1: TImage
      Left = 8
      Top = 8
      Width = 457
      Height = 313
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 609
    Width = 697
    Height = 17
    Panels = <>
    SimplePanel = False
  end
  object RadioGroup1: TRadioGroup
    Left = 488
    Top = 104
    Width = 201
    Height = 145
    ItemIndex = 4
    Items.Strings = (
      '8bit pixel, 15bit Palette'
      '8bit pixel, 16bit Palette'
      '8bit pixel, 24bit Palette'
      '15bit pixel - 5:5:5'
      '16bit pixel - 5:6:5'
      '24bit pixel'
      '32bit pixel (with alpha)')
    TabOrder = 5
  end
  object RadioGroup2: TRadioGroup
    Left = 488
    Top = 256
    Width = 201
    Height = 81
    ItemIndex = 2
    Items.Strings = (
      'Text'
      'Binary'
      'Binary (Header)'
      'Binary (Header, Mask)')
    TabOrder = 6
    OnClick = RadioGroup2Click
  end
  object Edit3: TEdit
    Left = 80
    Top = 72
    Width = 585
    Height = 19
    ImeName = #54620#44397#50612'('#54620#44544') (MS-IME98)'
    TabOrder = 7
  end
  object Memo1: TMemo
    Left = 8
    Top = 440
    Width = 681
    Height = 161
    ImeName = 'Microsoft Office IME 2007'
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 8
  end
  object OpenPictureDialog1: TOpenPictureDialog
    DefaultExt = 'bmp'
    Filter = 'Bitmaps (*.bmp)|*.bmp|All Files (*.*)|*.*'
    FilterIndex = 0
    Left = 56
    Top = 144
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'h'
    Filter = 'C Header File|*.h|All Files (*.*)|*.*'
    Left = 160
    Top = 144
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'bmp'
    Filter = 'Bitmaps (*.bmp)|*.bmp|All Files (*.*)|*.*'
    FilterIndex = 0
    Left = 232
    Top = 144
  end
end
