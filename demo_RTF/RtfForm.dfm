object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Informacja'
  ClientHeight = 486
  ClientWidth = 750
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000000000388838883888388838800000000000
    0000003330000033300000000000000000000000083888000000000000000000
    0000000333333333000000000000000000000000000000000000000000000888
    B888B888B888B888B888B888B88008B8000000B888B888B888B8000088B00888
    B888B888B888B888B8880000B88008B888B888B888B888B888B888B888B00888
    B00000000000000000000008B88008B808777777777777777777778088B00888
    077777777777777777777770B88008B807777878787878787877777088B00888
    077787878787878787878770B88008B807787878787878787878777088B00888
    077787878787878787878770B88008B807787878888888888878777088B00888
    077787888888888887878770B88008B807787878888888888878777088B00888
    077787888888888887878770B88008B807787878787878787878777088B00888
    077787878787878787878770B88008B807777878787878787878777088B00888
    077777878787878787877770B88008B807777777777777777777777088B00888
    087777777777777777777780B88008B880000000000000000000000888B00888
    B888B888B888B888B888B888B88008B888B888B888B888B888B888B888B00888
    B888B888B888B888B888B888B88000000000000000000000000000000000F800
    003FF800003FF800003FFFC007FFFFC007FF8000000100000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000080000001}
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnResize = FormResize
  DesignSize = (
    750
    486)
  PixelsPerInch = 96
  TextHeight = 13
  object GoButton: TButton
    Left = 0
    Top = 447
    Width = 750
    Height = 39
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Dalej'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = GoButtonClick
  end
  object InstructionRichEdit: TRichEdit
    Left = 0
    Top = 0
    Width = 750
    Height = 449
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'InstructionRichEdit')
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
  end
end
