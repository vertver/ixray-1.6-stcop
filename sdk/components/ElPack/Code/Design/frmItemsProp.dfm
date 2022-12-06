object ItemsPropDlg: TItemsPropDlg
  Left = 290
  Top = 164
  Caption = 'ElTree Items Editor'
  ClientHeight = 261
  ClientWidth = 373
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ItemsGB: TGroupBox
    Left = 0
    Top = 0
    Width = 373
    Height = 224
    Align = alClient
    Caption = 'Items'
    TabOrder = 0
    DesignSize = (
      373
      224)
    object Panel2: TPanel
      Left = 200
      Top = 15
      Width = 171
      Height = 207
      Align = alRight
      BevelOuter = bvNone
      BorderWidth = 3
      TabOrder = 0
      object NewItemBtn: TButton
        Left = 10
        Top = 0
        Width = 73
        Height = 25
        Caption = '&New Item'
        TabOrder = 0
        OnClick = NewItemBtnClick
      end
      object SubitemBtn: TButton
        Left = 10
        Top = 32
        Width = 73
        Height = 25
        Caption = 'New &Subitem'
        Enabled = False
        TabOrder = 1
        OnClick = SubitemBtnClick
      end
      object DeleteBtn: TButton
        Left = 10
        Top = 96
        Width = 73
        Height = 25
        Caption = '&Delete'
        Enabled = False
        TabOrder = 2
        OnClick = DeleteBtnClick
      end
      object SaveBtn: TButton
        Left = 90
        Top = 167
        Width = 73
        Height = 25
        Caption = 'S&ave'
        TabOrder = 3
        OnClick = SaveBtnClick
      end
      object LoadBtn: TButton
        Left = 10
        Top = 167
        Width = 73
        Height = 25
        Caption = '&Load'
        TabOrder = 4
        OnClick = LoadBtnClick
      end
      object EditBtn: TButton
        Left = 10
        Top = 128
        Width = 73
        Height = 25
        Caption = '&Edit ...'
        Enabled = False
        TabOrder = 5
        OnClick = EditBtnClick
      end
      object MoveRightBtn: TButton
        Left = 90
        Top = 32
        Width = 73
        Height = 25
        Caption = '   &>>'
        Enabled = False
        TabOrder = 6
        OnClick = MoveRightBtnClick
      end
      object MoveDownBtn: TButton
        Left = 90
        Top = 96
        Width = 73
        Height = 25
        Caption = 'Move D&own'
        Enabled = False
        TabOrder = 7
        OnClick = MoveDownBtnClick
      end
      object MoveUpBtn: TButton
        Left = 90
        Top = 64
        Width = 73
        Height = 25
        Caption = 'Move &Up'
        Enabled = False
        TabOrder = 8
        OnClick = MoveUpBtnClick
      end
      object DuplicateBtn: TButton
        Left = 10
        Top = 64
        Width = 73
        Height = 25
        Caption = 'Du&plicate'
        Enabled = False
        TabOrder = 9
        OnClick = DuplicateBtnClick
      end
      object MoveLeftBtn: TButton
        Left = 90
        Top = 0
        Width = 73
        Height = 25
        Caption = '&<<   '
        Enabled = False
        TabOrder = 10
        OnClick = MoveLeftBtnClick
      end
    end
    object Tree: TElTree
      Left = 3
      Top = 15
      Width = 201
      Height = 192
      Cursor = crDefault
      Hint = ''
      LeftPosition = 0
      DragCursor = crDrag
      Anchors = [akLeft, akTop, akRight, akBottom]
      DockOrientation = doNoOrient
      DefaultSectionWidth = 120
      BorderSides = [ebsLeft, ebsRight, ebsTop, ebsBottom]
      ExplorerEditMode = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      HeaderHeight = 19
      HeaderSections.Data = {F4FFFFFF00000000}
      HeaderFont.Charset = DEFAULT_CHARSET
      HeaderFont.Color = clWindowText
      HeaderFont.Height = -11
      HeaderFont.Name = 'MS Sans Serif'
      HeaderFont.Style = []
      HorzScrollBarStyles.ShowTrackHint = False
      HorzScrollBarStyles.Width = 17
      HorzScrollBarStyles.ButtonSize = 19
      IgnoreEnabled = False
      IncrementalSearch = False
      KeepSelectionWithinLevel = False
      LineBorderActiveColor = clBlack
      LineBorderInactiveColor = clBlack
      LineHeight = 17
      OwnerDrawMask = '~~@~~'
      ParentFont = False
      ScrollbarOpposite = False
      ShowCheckboxes = True
      ShowEmptyImages = True
      ShowLeafButton = False
      ShowLines = False
      StoragePath = '\Tree'
      TabOrder = 1
      TabStop = True
      VertScrollBarStyles.ShowTrackHint = True
      VertScrollBarStyles.Width = 17
      VertScrollBarStyles.ButtonSize = 19
      VirtualityLevel = vlNone
      OnItemFocused = TreeItemFocused
      LinkStyle = [fsUnderline]
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 224
    Width = 373
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object OKBtn: TButton
      Left = 73
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
      OnClick = OKBtnClick
    end
    object CancelBtn: TButton
      Left = 153
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object ApplyBtn: TButton
      Left = 233
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Apply'
      TabOrder = 2
      OnClick = OKBtnClick
    end
  end
  object OpenDlg: TOpenDialog
    DefaultExt = '*.bwt'
    Filter = 'ElTree Items file (*.elt)|*.elt|All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofShareAware]
    Title = 'ElTree Items load dialog'
    Top = 256
  end
  object SaveDlg: TSaveDialog
    DefaultExt = '*.bwt'
    Filter = 'ElTree Items file (*.elt)|*.elt|All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist]
    Title = 'ElTree ITems save dialog'
    Left = 32
    Top = 256
  end
end