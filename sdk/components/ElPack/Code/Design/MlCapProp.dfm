object MlCapEditDialog: TMlCapEditDialog
  Left = 301
  Top = 164
  Caption = 'Multiline caption/text editor'
  ClientHeight = 231
  ClientWidth = 420
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 204
    Width = 420
    Height = 27
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 170
    ExplicitWidth = 348
    object Load: TSpeedButton
      Left = 8
      Top = 1
      Width = 23
      Height = 22
      Hint = 'Load from file'
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000010000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        88888888888888888888000000000008888800333333333088880B0333333333
        08880FB03333333330880BFB0333333333080FBFB000000000000BFBFBFBFB08
        88880FBFBFBFBF0888880BFB0000000888888000888888880008888888888888
        8008888888880888080888888888800088888888888888888888}
      ParentShowHint = False
      ShowHint = True
      OnClick = LoadClick
    end
    object Save: TSpeedButton
      Left = 32
      Top = 1
      Width = 23
      Height = 22
      Hint = 'Save to file'
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000010000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        8888880000000000000880330000008803088033000000880308803300000088
        0308803300000000030880333333333333088033000000003308803088888888
        0308803088888888030880308888888803088030888888880308803088888888
        0008803088888888080880000000000000088888888888888888}
      ParentShowHint = False
      ShowHint = True
      OnClick = SaveClick
    end
    object OkButton: TButton
      Left = 97
      Top = 0
      Width = 74
      Height = 25
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Ok'
      ModalResult = 1
      TabOrder = 0
    end
    object CancelButton: TButton
      Left = 176
      Top = 0
      Width = 76
      Height = 25
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 420
    Height = 204
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    TabOrder = 0
    ExplicitWidth = 348
    ExplicitHeight = 170
    object LineCounter: TLabel
      Left = 4
      Top = 4
      Width = 34
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Align = alTop
      Caption = '0 Lines'
    end
  end
  object OpenDialog: TOpenDialog
    Filter = '*.*'
    FilterIndex = -1
    Left = 352
    Top = 216
  end
  object SaveDialog: TSaveDialog
    Filter = '*.*'
    Left = 384
    Top = 216
  end
end