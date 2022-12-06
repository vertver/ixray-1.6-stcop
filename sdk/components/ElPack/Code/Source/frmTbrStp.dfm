object frmToolbarSetup: TfrmToolbarSetup
  Left = 349
  Top = 293
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Customize Toolbar'
  ClientHeight = 259
  ClientWidth = 497
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlSections: TPanel
    Left = 0
    Top = 0
    Width = 497
    Height = 259
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object lblAvailable: TElLabel
      Left = 8
      Top = 9
      Width = 116
      Height = 13
      Caption = 'A&vailable toolbar buttons'
      Transparent = False
    end
    object lblVisible: TElLabel
      Left = 250
      Top = 9
      Width = 107
      Height = 13
      Caption = '&Current toolbar buttons'
      Transparent = False
    end
    object TextOptionsLabel: TElLabel
      Left = 8
      Top = 208
      Width = 58
      Height = 13
      Caption = 'Te&xt options'
      Transparent = False
    end
    object IconOptionsLabel: TElLabel
      Left = 8
      Top = 236
      Width = 61
      Height = 13
      Caption = 'Ico&n options:'
      Transparent = False
    end
    object btnAdd: TElPopupButton
      Left = 166
      Top = 82
      Width = 75
      Height = 23
      ImageIndex = 0
      DrawDefaultFrame = False
      NumGlyphs = 1
      ShowFocus = False
      Caption = '&Add ->'
      TabStop = False
      TabOrder = 1
      Color = clBtnFace
      ParentColor = False
      OnClick = btnAddClick
      DockOrientation = doNoOrient
      DoubleBuffered = False
    end
    object btnDelete: TElPopupButton
      Left = 166
      Top = 111
      Width = 75
      Height = 23
      ImageIndex = 0
      DrawDefaultFrame = False
      NumGlyphs = 1
      ShowFocus = False
      Caption = '<- &Remove'
      Enabled = False
      TabStop = False
      TabOrder = 2
      Color = clBtnFace
      ParentColor = False
      OnClick = btnDeleteClick
      DockOrientation = doNoOrient
      DoubleBuffered = False
    end
    object btnUp: TElPopupButton
      Left = 415
      Top = 82
      Width = 75
      Height = 23
      ImageIndex = 0
      DrawDefaultFrame = False
      NumGlyphs = 1
      ShowFocus = False
      Caption = 'Move &Up'
      Enabled = False
      TabStop = False
      TabOrder = 4
      Color = clBtnFace
      ParentColor = False
      OnClick = btnUpClick
      DockOrientation = doNoOrient
      DoubleBuffered = False
    end
    object btnDown: TElPopupButton
      Left = 415
      Top = 119
      Width = 75
      Height = 23
      ImageIndex = 0
      DrawDefaultFrame = False
      NumGlyphs = 1
      ShowFocus = False
      Caption = 'Move &Down'
      Enabled = False
      TabStop = False
      TabOrder = 5
      Color = clBtnFace
      ParentColor = False
      OnClick = btnDownClick
      DockOrientation = doNoOrient
      DoubleBuffered = False
    end
    object btnOk: TElPopupButton
      Left = 415
      Top = 9
      Width = 75
      Height = 23
      ImageIndex = 0
      DrawDefaultFrame = False
      Default = True
      ModalResult = 1
      NumGlyphs = 1
      ShowFocus = False
      Caption = 'OK'
      TabOrder = 8
      Color = clBtnFace
      ParentColor = False
      DockOrientation = doNoOrient
      DoubleBuffered = False
    end
    object btnCancel: TElPopupButton
      Left = 415
      Top = 38
      Width = 75
      Height = 23
      ImageIndex = 0
      DrawDefaultFrame = False
      Cancel = True
      ModalResult = 2
      NumGlyphs = 1
      ShowFocus = False
      Caption = 'Cancel'
      TabOrder = 9
      Color = clBtnFace
      ParentColor = False
      DockOrientation = doNoOrient
      DoubleBuffered = False
    end
    object lbxAvailable: TElListBox
      Left = 6
      Top = 24
      Width = 151
      Height = 172
      AllowGrayed = False
      ItemHeight = 13
      ItemIndex = -1
      TopIndex = 0
      ActiveBorderType = fbtFlat
      BorderSides = [ebsLeft, ebsRight, ebsTop, ebsBottom]
      Flat = True
      HorizontalScroll = False
      LineBorderActiveColor = clBlack
      LineBorderInactiveColor = clBlack
      SelectedFont.Charset = DEFAULT_CHARSET
      SelectedFont.Color = clHighlightText
      SelectedFont.Height = -11
      SelectedFont.Name = 'MS Sans Serif'
      SelectedFont.Style = []
      OnClick = FormShow
      OnDblClick = btnAddClick
      OnDragDrop = lbxAvailableDragDrop
      OnDragOver = lbxAvailableDragOver
      OnEnter = lbxAvailableEnter
      OnExit = FormShow
      OnMouseDown = lbxAvailableMouseDown
      TabOrder = 0
    end
    object lbxVisible: TElListBox
      Left = 250
      Top = 24
      Width = 151
      Height = 172
      AllowGrayed = False
      ItemHeight = 13
      ItemIndex = -1
      TopIndex = 0
      ActiveBorderType = fbtFlat
      BorderSides = [ebsLeft, ebsRight, ebsTop, ebsBottom]
      Flat = True
      HorizontalScroll = False
      LineBorderActiveColor = clBlack
      LineBorderInactiveColor = clBlack
      SelectedFont.Charset = DEFAULT_CHARSET
      SelectedFont.Color = clHighlightText
      SelectedFont.Height = -11
      SelectedFont.Name = 'MS Sans Serif'
      SelectedFont.Style = []
      OnClick = FormShow
      OnDblClick = btnDeleteClick
      OnDragDrop = lbxVisibleDragDrop
      OnDragOver = lbxVisibleDragOver
      OnEnter = lbxVisibleEnter
      OnExit = FormShow
      OnMouseDown = lbxVisibleMouseDown
      TabOrder = 3
    end
    object TextOptionsCombo: TElComboBox
      Left = 88
      Top = 205
      Width = 153
      Height = 21
      VertScrollBarStyles.ShowTrackHint = False
      VertScrollBarStyles.Width = 13
      VertScrollBarStyles.ButtonSize = 13
      HorzScrollBarStyles.ShowTrackHint = False
      HorzScrollBarStyles.Width = 13
      HorzScrollBarStyles.ButtonSize = 13
      UseCustomScrollBars = True
      ActiveBorderType = fbtFlat
      Alignment = taLeftJustify
      BorderStyle = bsSingle
      BorderSides = [ebsLeft, ebsRight, ebsTop, ebsBottom]
      BtnFlat = True
      BtnArrowColor = clBlack
      AltBtnWidth = 13
      AutoCompletion = False
      Items.Strings = (
        'Show text labels'
        'No text labels')
      ListSelectedColor = clHighlight
      TopMargin = 2
      RTLContent = False
      Flat = True
      LineBorderActiveColor = clBlack
      LineBorderInactiveColor = clBlack
      ParentColor = False
      ReadOnly = True
      TabOrder = 6
      Transparent = False
      AltButtonShortcut = 0
      ButtonShortcut = 0
    end
    object IconOptionsCombo: TElComboBox
      Left = 88
      Top = 233
      Width = 153
      Height = 21
      VertScrollBarStyles.ShowTrackHint = False
      VertScrollBarStyles.Width = 13
      VertScrollBarStyles.ButtonSize = 13
      HorzScrollBarStyles.ShowTrackHint = False
      HorzScrollBarStyles.Width = 13
      HorzScrollBarStyles.ButtonSize = 13
      UseCustomScrollBars = True
      ActiveBorderType = fbtFlat
      Alignment = taLeftJustify
      BorderStyle = bsSingle
      BorderSides = [ebsLeft, ebsRight, ebsTop, ebsBottom]
      BtnFlat = True
      BtnArrowColor = clBlack
      AltBtnWidth = 13
      AutoCompletion = False
      Items.Strings = (
        'Small Icons'
        'Large Icons')
      ListSelectedColor = clHighlight
      TopMargin = 2
      RTLContent = False
      Flat = True
      LineBorderActiveColor = clBlack
      LineBorderInactiveColor = clBlack
      ParentColor = False
      ReadOnly = True
      TabOrder = 7
      Transparent = False
      AltButtonShortcut = 0
      ButtonShortcut = 0
    end
  end
end