{ KOL MCK } // Do not remove this line!
resourcestring KOLForm1_Caption = 'Multi-Format Converter v1.02';
resourcestring KOLForm1_SimpleStatusText = ' ';
resourcestring KOLForm1_MainMenu1_N1 = 'Файл';
resourcestring KOLForm1_MainMenu1_N3 = 'Выход';
resourcestring KOLForm1_MainMenu1_N4 = 'Сервис';
resourcestring KOLForm1_MainMenu1_N5 = 'Автозагрузка содержимого архивов';
resourcestring KOLForm1_MainMenu1_N7 = 'Никогда';
resourcestring KOLForm1_MainMenu1_N8 = 'Автоопределение (реком.)';
resourcestring KOLForm1_MainMenu1_N9 = 'Всегда';
resourcestring KOLForm1_MainMenu1_N6 = 'Плагины...';
resourcestring KOLForm1_MainMenu1_N10 = 'Помощь';
resourcestring KOLForm1_MainMenu1_N11 = 'Справка...';
resourcestring KOLForm1_MainMenu1_N13 = 'О Программе...';
resourcestring KOLForm1_OpenSaveDialog1_Filter = 'Bitmaps (*.bmp)|*.bmp';
resourcestring KOLForm1_OpenSaveDialog1_DefExtension = 'bmp';
resourcestring KOLForm1_GroupBox4_Caption = ' Сборник изображений ';
resourcestring KOLForm1_Label8_Caption = 'Всего изображений:';
resourcestring KOLForm1_Label9_Caption = 'N/A';
resourcestring KOLForm1_GroupBox5_Caption = ' Действия ';
resourcestring KOLForm1_Button6_Caption = 'Сохранить все...';
resourcestring KOLForm1_Button7_Caption = 'Сохранить выделенное...';
resourcestring KOLForm1_Button8_Caption = 'Выделить все';
resourcestring KOLForm1_Button9_Caption = 'Инвертировать выделение';
resourcestring KOLForm1_GroupBox6_Caption = ' Просмотр ';
resourcestring KOLForm1_GroupBox1_Caption = ' Состав архива ';
resourcestring KOLForm1_Label7_Caption = 'Фильтр:';
resourcestring KOLForm1_ComboBox1_Item0 = 'ComboBox1';
resourcestring KOLForm1_GroupBox2_Caption = ' Действия ';
resourcestring KOLForm1_Button2_Caption = 'Выделить все';
resourcestring KOLForm1_Button3_Caption = 'Инвертировать выделение';
resourcestring KOLForm1_Button4_Caption = 'Извлечь все...';
resourcestring KOLForm1_Button5_Caption = 'Извлечь выделенное...';
resourcestring KOLForm1_GroupBox3_Caption = ' Информация об архиве ';
resourcestring KOLForm1_Label3_Caption = 'Размер (реальный):';
resourcestring KOLForm1_Label1_Caption = 'Всего файлов:';
resourcestring KOLForm1_Label2_Caption = 'Размер (в архиве):';
resourcestring KOLForm1_Label6_Caption = 'N/A';
resourcestring KOLForm1_Label4_Caption = 'N/A';
resourcestring KOLForm1_Label5_Caption = 'N/A';
resourcestring KOLForm1_Button1_Caption = 'Загрузить содержимое архива...';
const N1 = 0;
const N3 = 1;
const N4 = 2;
const N5 = 3;
const N7 = 4;
const N8 = 5;
const N9 = 6;
const N6 = 8;
const N10 = 9;
const N11 = 10;
const N13 = 12;


procedure NewForm1( var Result: PForm1; AParent: PControl );
begin

  New( Result, Create );
  Result.Form := NewForm( AParent, KOLForm1_Caption ).SetPosition( 205, 134 ).SetSize( 710, 458 );
  Result.Form.Add2AutoFree( Result );
    Result.Form.Style := Result.Form.Style and not WS_MINIMIZEBOX;
    Result.Form.Border := 0;
    Result.Form.SimpleStatusText := PChar( KOLForm1_SimpleStatusText );
      Result.Form.OnClose := Result.KOLForm1Close;
      Result.Form.OnShow := Result.KOLForm1Show;
    Result.Form.MinWidth := 466;
    Result.Form.MinHeight := 400;

    Result.MainMenu1 := NewMenu( Result.Form, 0, [ 
      PChar( KOLForm1_MainMenu1_N1 ), '('
      , PChar( KOLForm1_MainMenu1_N3 ), ')'
      , PChar( KOLForm1_MainMenu1_N4 ), '('
      , PChar( KOLForm1_MainMenu1_N5 ), '('
      , PChar( '-!' + KOLForm1_MainMenu1_N7)
      , PChar( '+!' + KOLForm1_MainMenu1_N8)
      , PChar( '-!' + KOLForm1_MainMenu1_N9), ')', '-' + ''
      , PChar( KOLForm1_MainMenu1_N6 ), ')'
      , PChar( KOLForm1_MainMenu1_N10 ), '('
      , PChar( KOLForm1_MainMenu1_N11 ), '-' + ''
      , PChar( KOLForm1_MainMenu1_N13 ), ')', '' ], nil );
    Result.MainMenu1.AssignEvents( 1, [ Result.MainMenu1N3Menu ] );
    Result.MainMenu1.AssignEvents( 4, [ Result.KOLForm1N7Menu ] );
    Result.MainMenu1.AssignEvents( 5, [ Result.KOLForm1N8Menu ] );
    Result.MainMenu1.AssignEvents( 6, [ Result.KOLForm1N9Menu ] );
    Result.MainMenu1.AssignEvents( 8, [ Result.MainMenu1N6Menu ] );
    Result.MainMenu1.AssignEvents( 10, [ Result.KOLForm1N11Menu ] );
    Result.MainMenu1.ItemEnabled[ 11 ] := False;
    Result.MainMenu1.AssignEvents( 12, [ Result.KOLForm1N13Menu ] );

    Result.OpenDirDialog1 := NewOpenDirDialog( '', [  ] );
  Result.Form.Add2AutoFree( Result.OpenDirDialog1 );

    Result.OpenSaveDialog1 := NewOpenSaveDialog( '', '', [  ] );
      Result.OpenSaveDialog1.Filter := KOLForm1_OpenSaveDialog1_Filter;
      Result.OpenSaveDialog1.OpenDialog := FALSE;
      Result.OpenSaveDialog1.DefExtension := KOLForm1_OpenSaveDialog1_DefExtension;
  Result.Form.Add2AutoFree( Result.OpenSaveDialog1 );
    // Result.Panel3.TabOrder = 1
    Result.Panel3 := NewPanel( Result.Form, esNone ).SetAlign ( caClient );
    Result.Panel3.Color := clGray;
    // Result.PANEL_IMAGECOMPLEX.TabOrder = 0
      Result.PANEL_IMAGECOMPLEX := NewPanel( Result.Panel3, esNone ).SetPosition( 8, 8 ).SetSize( 521, 353 );
    Result.PANEL_IMAGECOMPLEX.Color := clBtnFace;
      Result.PANEL_IMAGECOMPLEX.Border := 2;
      Result.PANEL_IMAGECOMPLEX.Visible := False;
      Result.PANEL_IMAGECOMPLEX.OnShow := Result.PANEL_IMAGECOMPLEXShow;
      Result.PANEL_IMAGECOMPLEX.OnHide := Result.PANEL_IMAGECOMPLEXHide;
    // Result.Panel5.TabOrder = 0
        Result.Panel5 := NewPanel( Result.PANEL_IMAGECOMPLEX, esNone ).SetAlign ( caLeft ).SetSize( 197, 0 );
        Result.Panel5.Border := 0;
    // Result.GroupBox4.TabOrder = 0
          Result.GroupBox4 := NewGroupBox( Result.Panel5, KOLForm1_GroupBox4_Caption ).SetAlign ( caTop ).SetSize( 0, 185 );
    Result.GroupBox4.TabStop := FALSE;
    Result.GroupBox4.Font.FontHeight := -11;
          Result.GroupBox4.Border := 2;
          Result.GroupBox4.MarginTop := 16;
    // Result.Panel4.TabOrder = 0
            Result.Panel4 := NewPanel( Result.GroupBox4, esNone ).SetAlign ( caTop ).SetSize( 0, 23 );
    // Result.Label8.TabOrder = -1
              Result.Label8 := NewLabel( Result.Panel4, KOLForm1_Label8_Caption ).SetPosition( 0, 0 ).SetSize( 105, 17 );
    Result.Label8.VerticalAlign := vaCenter;
    // Result.Label9.TabOrder = -1
              Result.Label9 := NewLabel( Result.Panel4, KOLForm1_Label9_Caption ).SetPosition( 112, 0 ).SetSize( 73, 17 );
    Result.Label9.Font.Color := clNavy;
    Result.Label9.VerticalAlign := vaCenter;
    // Result.ListView2.TabOrder = 1
            Result.ListView2 := NewListView( Result.GroupBox4, lvsDetail, [ lvoMultiselect, lvoRowSelect ], nil, nil, nil ).SetAlign ( caClient );
    Result.ListView2.Color := clWindow;
      Result.ListView2.OnClick := Result.ListView2Click;
      Result.ListView2.OnLVStateChange := Result.ListView2LVStateChange;
    // Result.GroupBox5.TabOrder = 1
          Result.GroupBox5 := NewGroupBox( Result.Panel5, KOLForm1_GroupBox5_Caption ).SetAlign ( caClient );
    Result.GroupBox5.TabStop := FALSE;
    Result.GroupBox5.Font.FontHeight := -11;
          Result.GroupBox5.Border := 2;
          Result.GroupBox5.MarginTop := 16;
    // Result.Button6.TabOrder = 0
            Result.Button6 := NewButton( Result.GroupBox5, KOLForm1_Button6_Caption ).SetPosition( 8, 72 ).SetSize( 177, 0 );
      Result.Button6.OnClick := Result.Button6Click;
    // Result.Button7.TabOrder = 1
            Result.Button7 := NewButton( Result.GroupBox5, KOLForm1_Button7_Caption ).SetPosition( 8, 96 ).SetSize( 177, 0 );
      Result.Button7.OnClick := Result.Button7Click;
    // Result.Button8.TabOrder = 2
            Result.Button8 := NewButton( Result.GroupBox5, KOLForm1_Button8_Caption ).SetPosition( 8, 16 ).SetSize( 177, 0 );
      Result.Button8.OnClick := Result.Button8Click;
    // Result.Button9.TabOrder = 3
            Result.Button9 := NewButton( Result.GroupBox5, KOLForm1_Button9_Caption ).SetPosition( 8, 40 ).SetSize( 177, 0 );
      Result.Button9.OnClick := Result.Button9Click;
    // Result.GroupBox6.TabOrder = 1
        Result.GroupBox6 := NewGroupBox( Result.PANEL_IMAGECOMPLEX, KOLForm1_GroupBox6_Caption ).SetAlign ( caClient );
    Result.GroupBox6.TabStop := FALSE;
    Result.GroupBox6.Font.FontHeight := -11;
        Result.GroupBox6.MarginTop := 16;
    // Result.ScrollBox1.TabOrder = 0
          Result.ScrollBox1 := NewScrollBoxEx( Result.GroupBox6, esLowered ).SetAlign ( caClient );
          Result.ScrollBox1.Border := 0;
    // Result.PaintBox1.TabOrder = 0
            Result.PaintBox1 := NewPaintBox( Result.ScrollBox1 );
            Result.PaintBox1.Border := 1;
            Result.PaintBox1.Cursor := LoadCursor( 0, IDC_HAND );
            Result.PaintBox1.DoubleBuffered := True;
      Result.PaintBox1.OnClick := Result.PaintBox1Click;
      Result.PaintBox1.OnPaint := Result.PaintBox1Paint;
    // Result.Panel1.TabOrder = 2
    Result.Panel1 := NewPanel( Result.Form, esNone ).SetAlign ( caLeft ).SetSize( 169, 0 );
    // Result.SPCDriveComboBox1.TabOrder = 0
      Result.SPCDriveComboBox1 := NewTSPCDriveComboBox( Result.Panel1, [ coNoIntegralHeight ] ).SetAlign ( caTop );
    Result.SPCDriveComboBox1.Color := clWhite;
      Result.SPCDriveComboBox1.CurIndex := 0;
      Result.SPCDriveComboBox1.Top:=0;
      Result.SPCDriveComboBox1.Left:=0;
      Result.SPCDriveComboBox1.Width:=169;
      Result.SPCDriveComboBox1.Height:=22;
      Result.SPCDriveComboBox1.Color:=16777215;
      Result.SPCDriveComboBox1.Drive:='E';
    // Result.SPCFilterComboBox1.TabOrder = 1
      Result.SPCFilterComboBox1 := NewTSPCFilterComboBox( Result.Panel1, [ coNoIntegralHeight ] ).SetAlign ( caBottom );
    Result.SPCFilterComboBox1.Color := clWhite;
    Result.SPCFilterComboBox1.Font.FontHeight := -11;
      Result.SPCFilterComboBox1.OnChange := Result.SPCFilterComboBox1Change;
      Result.SPCFilterComboBox1.CurIndex := 0;
      Result.SPCFilterComboBox1.Text:='';
      Result.SPCFilterComboBox1.Color:=16777215;
      Result.SPCFilterComboBox1.Top:=390;
      Result.SPCFilterComboBox1.Left:=0;
      Result.SPCFilterComboBox1.Width:=169;
      Result.SPCFilterComboBox1.Height:=22;
    // Result.SPCFileListBox1.TabOrder = 2
      Result.SPCFileListBox1 := NewTSPCFileListBox( Result.Panel1, [ loNoIntegralHeight ] ).SetAlign ( caClient );
    Result.SPCFileListBox1.Color := clWindow;
      Result.SPCFileListBox1.OnSelChange := Result.SPCFileListBox1SelChange;
      Result.SPCFileListBox1.CurIndex := 0;
      Result.SPCFileListBox1.DoCase:=1;
      Result.SPCFileListBox1.IntegralHeight:=False;
      Result.SPCFileListBox1.Filters:='*.*';
      Result.SPCFileListBox1.Color:=-2147483643;
      Result.SPCFileListBox1.Top:=191;
      Result.SPCFileListBox1.Left:=0;
      Result.SPCFileListBox1.Width:=169;
      Result.SPCFileListBox1.Height:=199;
      Result.SPCFileListBox1.Path:='E:\Program Files\Borland\Custom Projects\Formats\Multi-format converter\';
      Result.SPCFileListBox1.ExecuteOnDblClk:=False;
      Result.SPCFileListBox1._SortBy:=sbName;
    // Result.SPCDirectoryListBox1.TabOrder = 3
      Result.SPCDirectoryListBox1 := NewTSPCDirectoryListBox( Result.Panel1, lvsIcon, [  ], nil, nil, nil ).SetAlign ( caTop ).SetSize( 0, 165 );
    Result.SPCDirectoryListBox1.Color := clWindow;
      Result.SPCDirectoryListBox1.OnChange := Result.SPCDirectoryListBox1Change;
      Result.SPCDirectoryListBox1.Top:=22;
      Result.SPCDirectoryListBox1.Left:=0;
      Result.SPCDirectoryListBox1.Width:=169;
      Result.SPCDirectoryListBox1.Height:=165;
      Result.SPCDirectoryListBox1.DoIndent:=True;
      Result.SPCDirectoryListBox1.IntegralHeight:=False;
      Result.SPCDirectoryListBox1.Path:='E:\Program Files\Morrowind\Data Files\';
    // Result.Splitter1.TabOrder = 4
      Result.Splitter1 := NewSplitter( Result.Panel1, 100, 116 ).SetAlign ( caTop );
    // Result.PANEL_ARCHIVE.TabOrder = 3
    Result.PANEL_ARCHIVE := NewPanel( Result.Form, esNone ).SetPosition( 457, 144 ).SetSize( 611, 385 );
    Result.PANEL_ARCHIVE.Border := 2;
    Result.PANEL_ARCHIVE.Visible := False;
      Result.PANEL_ARCHIVE.OnShow := Result.PANEL_ARCHIVEShow;
      Result.PANEL_ARCHIVE.OnHide := Result.PANEL_ARCHIVEHide;
    // Result.GroupBox1.TabOrder = 0
      Result.GroupBox1 := NewGroupBox( Result.PANEL_ARCHIVE, KOLForm1_GroupBox1_Caption ).SetAlign ( caClient );
    Result.GroupBox1.TabStop := FALSE;
    Result.GroupBox1.Font.FontHeight := -11;
      Result.GroupBox1.MarginTop := 16;
    // Result.ListView1.TabOrder = 0
        Result.ListView1 := NewListView( Result.GroupBox1, lvsDetail, [ lvoMultiselect, lvoRowSelect ], nil, nil, nil ).SetAlign ( caClient );
    Result.ListView1.Color := clWindow;
    // Result.PANEL_FILTERS.TabOrder = 1
        Result.PANEL_FILTERS := NewPanel( Result.GroupBox1, esNone ).SetAlign ( caTop ).SetSize( 0, 23 );
    // Result.Label7.TabOrder = -1
          Result.Label7 := NewLabel( Result.PANEL_FILTERS, KOLForm1_Label7_Caption ).SetPosition( 8, 0 ).SetSize( 46, 0 );
    Result.Label7.VerticalAlign := vaCenter;
    // Result.ComboBox1.TabOrder = 0
          Result.ComboBox1 := NewComboBox( Result.PANEL_FILTERS, [ coReadOnly, coNoIntegralHeight ] ).SetPosition( 56, 0 ).SetSize( 73, 0 );
    Result.ComboBox1.Color := clWindow;
      Result.ComboBox1.OnChange := Result.ComboBox1Change;
          Result.ComboBox1.Items[ 0 ] := KOLForm1_ComboBox1_Item0;
          Result.ComboBox1.CurIndex := 0;
    // Result.Panel2.TabOrder = 3
      Result.Panel2 := NewPanel( Result.PANEL_ARCHIVE, esNone ).SetAlign ( caLeft ).SetSize( 197, 0 );
      Result.Panel2.Border := 0;
    // Result.GroupBox2.TabOrder = 1
        Result.GroupBox2 := NewGroupBox( Result.Panel2, KOLForm1_GroupBox2_Caption ).SetAlign ( caClient );
    Result.GroupBox2.TabStop := FALSE;
    Result.GroupBox2.Font.FontHeight := -11;
        Result.GroupBox2.Border := 2;
        Result.GroupBox2.MarginTop := 16;
    // Result.Button2.TabOrder = 0
          Result.Button2 := NewButton( Result.GroupBox2, KOLForm1_Button2_Caption ).SetPosition( 8, 16 ).SetSize( 177, 0 );
      Result.Button2.OnClick := Result.Button2Click;
    // Result.Button3.TabOrder = 1
          Result.Button3 := NewButton( Result.GroupBox2, KOLForm1_Button3_Caption ).SetPosition( 8, 40 ).SetSize( 177, 0 );
      Result.Button3.OnClick := Result.Button3Click;
    // Result.Button4.TabOrder = 2
          Result.Button4 := NewButton( Result.GroupBox2, KOLForm1_Button4_Caption ).SetPosition( 8, 72 ).SetSize( 177, 0 );
          Result.Button4.Enabled := False;
      Result.Button4.OnClick := Result.Button4Click;
    // Result.Button5.TabOrder = 3
          Result.Button5 := NewButton( Result.GroupBox2, KOLForm1_Button5_Caption ).SetPosition( 8, 96 ).SetSize( 177, 0 );
          Result.Button5.Enabled := False;
      Result.Button5.OnClick := Result.Button5Click;
    // Result.GroupBox3.TabOrder = 2
        Result.GroupBox3 := NewGroupBox( Result.Panel2, KOLForm1_GroupBox3_Caption ).SetAlign ( caTop ).SetSize( 0, 97 );
    Result.GroupBox3.TabStop := FALSE;
    Result.GroupBox3.Font.FontHeight := -11;
        Result.GroupBox3.Border := 2;
        Result.GroupBox3.MarginTop := 16;
    // Result.Label3.TabOrder = -1
          Result.Label3 := NewLabel( Result.GroupBox3, KOLForm1_Label3_Caption ).SetPosition( 8, 72 ).SetSize( 105, 17 );
    // Result.Label1.TabOrder = -1
          Result.Label1 := NewLabel( Result.GroupBox3, KOLForm1_Label1_Caption ).SetPosition( 8, 40 ).SetSize( 105, 17 );
    // Result.Label2.TabOrder = -1
          Result.Label2 := NewLabel( Result.GroupBox3, KOLForm1_Label2_Caption ).SetPosition( 8, 56 ).SetSize( 105, 17 );
    // Result.Label6.TabOrder = -1
          Result.Label6 := NewLabel( Result.GroupBox3, KOLForm1_Label6_Caption ).SetPosition( 112, 72 ).SetSize( 0, 17 );
    Result.Label6.Font.Color := clNavy;
    // Result.Label4.TabOrder = -1
          Result.Label4 := NewLabel( Result.GroupBox3, KOLForm1_Label4_Caption ).SetPosition( 112, 40 ).SetSize( 65, 17 );
    Result.Label4.Font.Color := clNavy;
    // Result.Label5.TabOrder = -1
          Result.Label5 := NewLabel( Result.GroupBox3, KOLForm1_Label5_Caption ).SetPosition( 112, 56 ).SetSize( 65, 17 );
    Result.Label5.Font.Color := clNavy;
    // Result.Button1.TabOrder = 0
          Result.Button1 := NewButton( Result.GroupBox3, KOLForm1_Button1_Caption ).SetPosition( 8, 16 ).SetSize( 177, 0 );
      Result.Button1.OnClick := Result.Button1Click;
    Result.SPCDriveComboBox1.DirectoryListBox:=Result.SPCDirectoryListBox1;
    Result.SPCFilterComboBox1.FileListBox:=Result.SPCFileListBox1;
    Result.SPCDirectoryListBox1.FileListBox:=Result.SPCFileListBox1;
      Result.OpenDirDialog1.WndOwner := Result.Form.GetWindowHandle;
      Result.OpenSaveDialog1.WndOwner := Result.Form.GetWindowHandle;
    Result.Form.CenterOnParent;
    Result.KOLForm1FormCreate( Result );

end;

