object ItemColDlg: TItemColDlg
  Left = 275
  Top = 160
  ActiveControl = StylesCB
  BorderStyle = bsDialog
  Caption = 'Item Properties Editor'
  ClientHeight = 347
  ClientWidth = 330
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 87
    Top = 316
    Width = 75
    Height = 25
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 167
    Top = 316
    Width = 75
    Height = 25
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 330
    Height = 305
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    ActivePage = TabSheet2
    Align = alTop
    TabOrder = 2
    object TabSheet1: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Text'
      object Label2: TLabel
        Left = 8
        Top = 4
        Width = 71
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Alignment = taCenter
        Caption = 'Item main text: '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 8
        Top = 84
        Width = 46
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Alignment = taCenter
        Caption = 'Item hint: '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label1: TLabel
        Left = 8
        Top = 168
        Width = 227
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Item columns text (multi-line text in run-time only):'
      end
      object TextEdit: TMemo
        Left = 8
        Top = 16
        Width = 305
        Height = 57
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        ScrollBars = ssBoth
        TabOrder = 0
        WordWrap = False
      end
      object HintEdit: TMemo
        Left = 8
        Top = 96
        Width = 305
        Height = 57
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        ScrollBars = ssBoth
        TabOrder = 1
        WordWrap = False
      end
      object ColTextMemo: TMemo
        Left = 8
        Top = 184
        Width = 305
        Height = 89
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        ScrollBars = ssBoth
        TabOrder = 2
        WordWrap = False
      end
    end
    object TabSheet2: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Styles'
      object Label3: TLabel
        Left = 184
        Top = 154
        Width = 59
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Item images:'
      end
      object Label6: TLabel
        Left = 184
        Top = 184
        Width = 44
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        AutoSize = False
        Caption = 'Normal:'
        Layout = tlCenter
      end
      object Label7: TLabel
        Left = 184
        Top = 208
        Width = 28
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        AutoSize = False
        Caption = 'State:'
        Layout = tlCenter
      end
      object Label10: TLabel
        Left = 231
        Top = 168
        Width = 17
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = '1st:'
      end
      object Label11: TLabel
        Left = 271
        Top = 168
        Width = 21
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = '2nd:'
      end
      object Label16: TLabel
        Left = 184
        Top = 232
        Width = 44
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        AutoSize = False
        Caption = 'Overlay:'
        Layout = tlCenter
      end
      object StylesCB: TCheckBox
        Left = 8
        Top = 8
        Width = 81
        Height = 17
        Hint = 
          'When checked, tree settings are used. when unchecked, item'#39's fon' +
          't styles are used.'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Owner styles'
        TabOrder = 0
        OnClick = StylesCBClick
      end
      object StylesGB: TGroupBox
        Left = 8
        Top = 32
        Width = 89
        Height = 105
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Item Styles'
        TabOrder = 1
        object BoldCB: TCheckBox
          Left = 8
          Top = 59
          Width = 49
          Height = 17
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Bold'
          TabOrder = 2
        end
        object ItCB: TCheckBox
          Left = 8
          Top = 16
          Width = 49
          Height = 17
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Italic'
          TabOrder = 0
        end
        object ULCB: TCheckBox
          Left = 8
          Top = 37
          Width = 73
          Height = 17
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Underlined'
          TabOrder = 1
        end
        object StrikeCB: TCheckBox
          Left = 8
          Top = 80
          Width = 65
          Height = 17
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Strikeout'
          TabOrder = 3
        end
      end
      object ColorsCB: TCheckBox
        Left = 104
        Top = 8
        Width = 81
        Height = 17
        Hint = 
          'When checked, tree colors are used. When unchecked, item'#39's own c' +
          'olors are used.'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Owner colors'
        TabOrder = 2
        OnClick = ColorsCBClick
      end
      object ColorsGB: TGroupBox
        Left = 104
        Top = 32
        Width = 209
        Height = 105
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Item Colors'
        TabOrder = 3
        object Label4: TLabel
          Left = 8
          Top = 16
          Width = 24
          Height = 13
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Font:'
        end
        object Label5: TLabel
          Left = 112
          Top = 16
          Width = 85
          Height = 13
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Text Background:'
        end
        object Label12: TLabel
          Left = 112
          Top = 56
          Width = 86
          Height = 13
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Row Background:'
        end
        object ColorCombo: TComboBox
          Left = 8
          Top = 32
          Width = 81
          Height = 21
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Ctl3D = True
          DropDownCount = 16
          ParentCtl3D = False
          TabOrder = 0
          Items.Strings = (
            'clAqua'
            'clBlack'
            'clBlue'
            'clDkGray'
            'clFuchsia'
            'clGray'
            'clGreen'
            'clLime'
            'clLtGray'
            'clMaroon'
            'clNavy'
            'clOlive'
            'clPurple'
            'clRed'
            'clSilver'
            'clTeal'
            'clWhite'
            'clYellow'
            'clActiveBorder'
            'clActiveCaption'
            'clAppWorkSpace'
            'clBackground'
            'clBtnFace'
            'clBtnHighlight'
            'clBtnShadow'
            'clBtnText'
            'clCaptionText'
            'clGrayText'
            'clHighlight'
            'clHighlightText'
            'clInactiveBorder'
            'clInactiveCaption'
            'clInactiveCaptionText'
            'clMenu'
            'clMenuText'
            'clScrollBar'
            'clWindow'
            'clWindowFrame'
            'clWindowText')
        end
        object BkColorCombo: TComboBox
          Left = 112
          Top = 32
          Width = 81
          Height = 21
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          DropDownCount = 16
          TabOrder = 1
          Items.Strings = (
            'clAqua'
            'clBlack'
            'clBlue'
            'clDkGray'
            'clFuchsia'
            'clGray'
            'clGreen'
            'clLime'
            'clLtGray'
            'clMaroon'
            'clNavy'
            'clOlive'
            'clPurple'
            'clRed'
            'clSilver'
            'clTeal'
            'clWhite'
            'clYellow'
            'clActiveBorder'
            'clActiveCaption'
            'clAppWorkSpace'
            'clBackground'
            'clBtnFace'
            'clBtnHighlight'
            'clBtnShadow'
            'clBtnText'
            'clCaptionText'
            'clGrayText'
            'clHighlight'
            'clHighlightText'
            'clInactiveBorder'
            'clInactiveCaption'
            'clInactiveCaptionText'
            'clMenu'
            'clMenuText'
            'clScrollBar'
            'clWindow'
            'clWindowFrame'
            'clWindowText')
        end
        object RowBkColorCombo: TComboBox
          Left = 112
          Top = 72
          Width = 81
          Height = 21
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          DropDownCount = 16
          TabOrder = 2
          Items.Strings = (
            'clAqua'
            'clBlack'
            'clBlue'
            'clDkGray'
            'clFuchsia'
            'clGray'
            'clGreen'
            'clLime'
            'clLtGray'
            'clMaroon'
            'clNavy'
            'clOlive'
            'clPurple'
            'clRed'
            'clSilver'
            'clTeal'
            'clWhite'
            'clYellow'
            'clActiveBorder'
            'clActiveCaption'
            'clAppWorkSpace'
            'clBackground'
            'clBtnFace'
            'clBtnHighlight'
            'clBtnShadow'
            'clBtnText'
            'clCaptionText'
            'clGrayText'
            'clHighlight'
            'clHighlightText'
            'clInactiveBorder'
            'clInactiveCaption'
            'clInactiveCaptionText'
            'clMenu'
            'clMenuText'
            'clScrollBar'
            'clWindow'
            'clWindowFrame'
            'clWindowText')
        end
        object UseBkColorCB: TCheckBox
          Left = 8
          Top = 56
          Width = 89
          Height = 17
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Use BkColors'
          TabOrder = 3
        end
      end
      object CBGroup: TGroupBox
        Left = 8
        Top = 170
        Width = 169
        Height = 89
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Enabled = False
        TabOrder = 5
        object Label8: TLabel
          Left = 8
          Top = 32
          Width = 33
          Height = 21
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Type:'
          Enabled = False
          Layout = tlCenter
        end
        object Label9: TLabel
          Left = 8
          Top = 64
          Width = 33
          Height = 21
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'State:'
          Enabled = False
          Layout = tlCenter
        end
        object CBTypeCombo: TComboBox
          Left = 48
          Top = 32
          Width = 113
          Height = 21
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Style = csDropDownList
          DropDownCount = 3
          Enabled = False
          TabOrder = 1
          OnChange = CBTypeComboChange
          Items.Strings = (
            '2-State CheckBox'
            '3-State CheckBox'
            'RadioButton')
        end
        object CBStateCombo: TComboBox
          Left = 48
          Top = 64
          Width = 113
          Height = 21
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Style = csDropDownList
          DropDownCount = 3
          Enabled = False
          TabOrder = 2
          Items.Strings = (
            'Unchecked'
            'Checked'
            'Grayed')
        end
        object CBEnabledCB: TCheckBox
          Left = 8
          Top = 8
          Width = 65
          Height = 17
          Margins.Left = 2
          Margins.Top = 2
          Margins.Right = 2
          Margins.Bottom = 2
          Caption = 'Enabled'
          Enabled = False
          TabOrder = 0
        end
      end
      object ShowChecksCB: TCheckBox
        Left = 8
        Top = 152
        Width = 168
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Show Checkbox/RadioButton'
        TabOrder = 4
        OnClick = ShowChecksCBClick
      end
      object StIndexEdit: TEdit
        Left = 231
        Top = 208
        Width = 32
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 7
      end
      object IndexEdit: TEdit
        Left = 231
        Top = 184
        Width = 32
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
      end
      object Index2Edit: TEdit
        Left = 271
        Top = 184
        Width = 33
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 9
      end
      object StIndex2Edit: TEdit
        Left = 271
        Top = 208
        Width = 33
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 10
      end
      object OvIndexEdit: TEdit
        Left = 231
        Top = 232
        Width = 32
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 8
      end
      object OvIndex2Edit: TEdit
        Left = 271
        Top = 232
        Width = 33
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 11
      end
    end
    object TabSheet3: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Misc'
      object Label14: TLabel
        Left = 184
        Top = 192
        Width = 22
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Tag:'
        Layout = tlCenter
      end
      object Label15: TLabel
        Left = 184
        Top = 152
        Width = 84
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Item'#39's border style'
      end
      object ForcedBtnsCB: TCheckBox
        Left = 8
        Top = 172
        Width = 97
        Height = 17
        Hint = 
          'When checked buttons will be displayed even when the item doesn'#39 +
          't have children'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Forced buttons'
        TabOrder = 6
      end
      object EnabledCB: TCheckBox
        Left = 8
        Top = 4
        Width = 65
        Height = 17
        Hint = 
          'When checked, the item is enabled. Disabled items don'#39't respond ' +
          'to mouse clicks and can'#39't be selected.'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Enabled'
        TabOrder = 0
      end
      object HiddenCB: TCheckBox
        Left = 8
        Top = 28
        Width = 65
        Height = 17
        Hint = 'When checked, the item is not visible'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Hidden'
        TabOrder = 1
      end
      object HtmlCB: TCheckBox
        Left = 8
        Top = 52
        Width = 97
        Height = 17
        Hint = 
          'When checked, text that starts with <html> is treated as HTML te' +
          'xt'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Contains HTML'
        TabOrder = 2
      end
      object TagEdit: TEdit
        Left = 200
        Top = 208
        Width = 33
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 16
      end
      object StrikeOutCB: TCheckBox
        Left = 184
        Top = 4
        Width = 97
        Height = 17
        Hint = 'When checked the item is striked out. '
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Striked out'
        TabOrder = 10
        OnClick = StrikeOutCBClick
      end
      object StrikeLineColorCB: TComboBox
        Left = 200
        Top = 22
        Width = 81
        Height = 21
        Hint = 'Color of strikeout line.'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        DropDownCount = 16
        Enabled = False
        TabOrder = 12
        Items.Strings = (
          'clAqua'
          'clBlack'
          'clBlue'
          'clDkGray'
          'clFuchsia'
          'clGray'
          'clGreen'
          'clLime'
          'clLtGray'
          'clMaroon'
          'clNavy'
          'clOlive'
          'clPurple'
          'clRed'
          'clSilver'
          'clTeal'
          'clWhite'
          'clYellow'
          'clActiveBorder'
          'clActiveCaption'
          'clAppWorkSpace'
          'clBackground'
          'clBtnFace'
          'clBtnHighlight'
          'clBtnShadow'
          'clBtnText'
          'clCaptionText'
          'clGrayText'
          'clHighlight'
          'clHighlightText'
          'clInactiveBorder'
          'clInactiveCaption'
          'clInactiveCaptionText'
          'clMenu'
          'clMenuText'
          'clScrollBar'
          'clWindow'
          'clWindowFrame'
          'clWindowText')
      end
      object HorZlineCB: TCheckBox
        Left = 8
        Top = 100
        Width = 121
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Draw horizontal line'
        TabOrder = 4
      end
      object AllowEditCB: TCheckBox
        Left = 8
        Top = 124
        Width = 97
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Allow edit'
        TabOrder = 5
      end
      object SuppressButtonsCB: TCheckBox
        Left = 8
        Top = 196
        Width = 121
        Height = 17
        Hint = 
          'When checked, no buttons will be displayed even when the item ha' +
          's children'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Suppressed buttons'
        TabOrder = 7
      end
      object MultilineCB: TCheckBox
        Left = 8
        Top = 76
        Width = 129
        Height = 17
        Hint = 'When checked, item has multi-line text. '
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Contains multi-line text'
        TabOrder = 3
      end
      object OwnHeightCB: TCheckBox
        Left = 184
        Top = 56
        Width = 97
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Own height'
        TabOrder = 9
        OnClick = OwnHeightCBClick
      end
      object HeightEdit: TEdit
        Left = 200
        Top = 74
        Width = 33
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 11
      end
      object IndentEdit: TEdit
        Left = 200
        Top = 124
        Width = 33
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 14
      end
      object IndentAdjustCB: TCheckBox
        Left = 184
        Top = 104
        Width = 129
        Height = 17
        Hint = 
          'When checked item indentation is adjusted accoring to adjustment' +
          ' value.'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Use indent adjustment'
        TabOrder = 13
        OnClick = IndentAdjustCBClick
      end
      object BorderStyleCombo: TComboBox
        Left = 200
        Top = 168
        Width = 81
        Height = 21
        Hint = 'Color of strikeout line.'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Style = csDropDownList
        DropDownCount = 16
        TabOrder = 15
        Items.Strings = (
          'None'
          'Raised'
          'Flat'
          'Sunken')
      end
      object SuppressLinesCB: TCheckBox
        Left = 8
        Top = 220
        Width = 121
        Height = 17
        Hint = 'When checked, no tree lines are drawn for the item'
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Suppressed lines'
        TabOrder = 8
      end
    end
  end
end