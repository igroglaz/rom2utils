{ KOL MCK } // Do not remove this line!

procedure NewForm4( var Result: PForm4; AParent: PControl );
begin

  New( Result, Create );
  Result.Form := NewForm( AParent, 'О Программе...' ).SetPosition( 257, 183 ).SetSize( 313, 200 );
  Result.Form.Add2AutoFree( Result );
    Result.Form.Visible := False;
      Result.Form.OnClose := Result.KOLForm1Close;
    // Result.Label4.TabOrder = -1
    Result.Label4 := NewLabel( Result.Form, 'Web:' ).SetPosition( 8, 120 ).SetSize( 41, 0 );
    Result.Label4.Font.FontHeight := -11;
    // Result.LabelEffect1.TabOrder = -1
    Result.LabelEffect1 := NewLabelEffect( Result.Form, 'Multi-format Converter v1.02', 1 ).SetAlign ( caTop ).SetSize( 0, 39 );
    Result.LabelEffect1.Ctl3D := False;
    Result.LabelEffect1.Font.Color := clNavy;
    Result.LabelEffect1.Font.FontStyle := [ fsBold ];
    Result.LabelEffect1.Font.FontHeight := -16;
    // Result.Label1.TabOrder = -1
    Result.Label1 := NewLabel( Result.Form, 'Автор:' ).SetPosition( 8, 48 ).SetSize( 41, 0 );
    Result.Label1.Font.FontHeight := -11;
    // Result.Label2.TabOrder = -1
    Result.Label2 := NewLabel( Result.Form, 'Дата:' ).SetPosition( 8, 72 ).SetSize( 41, 0 );
    Result.Label2.Font.FontHeight := -11;
    // Result.Label3.TabOrder = -1
    Result.Label3 := NewLabel( Result.Form, 'E-mail:' ).SetPosition( 8, 96 ).SetSize( 41, 0 );
    Result.Label3.Font.FontHeight := -11;
    // Result.Label8.TabOrder = -1
    Result.Label8 := NewLabel( Result.Form, 'http://extractor.far.ru' ).SetPosition( 56, 120 ).SetSize( 241, 17 );
    Result.Label8.Font.Color := clBlue;
    Result.Label8.Font.FontStyle := [ fsUnderline ];
    Result.Label8.Font.FontHeight := -11;
    Result.Label8.Cursor := LoadCursor( 0, IDC_HAND );
      Result.Label8.OnClick := Result.Label8Click;
    // Result.Label5.TabOrder = -1
    Result.Label5 := NewLabel( Result.Form, 'Михаил Бесчетнов' ).SetPosition( 56, 48 ).SetSize( 241, 0 );
    Result.Label5.Font.Color := clNavy;
    Result.Label5.Font.FontHeight := -11;
    // Result.Label6.TabOrder = -1
    Result.Label6 := NewLabel( Result.Form, '28 октября 2002г.' ).SetPosition( 56, 72 ).SetSize( 241, 0 );
    Result.Label6.Font.Color := clNavy;
    Result.Label6.Font.FontHeight := -11;
    // Result.Label7.TabOrder = -1
    Result.Label7 := NewLabel( Result.Form, 'collapse_forever@mail.ru' ).SetPosition( 56, 96 ).SetSize( 241, 17 );
    Result.Label7.Font.Color := clBlue;
    Result.Label7.Font.FontStyle := [ fsUnderline ];
    Result.Label7.Font.FontHeight := -11;
    Result.Label7.Cursor := LoadCursor( 0, IDC_HAND );
      Result.Label7.OnClick := Result.Label7Click;
    // Result.Button1.TabOrder = 0
    Result.Button1 := NewButton( Result.Form, 'Закрыть' ).SetPosition( 121, 144 );
    Result.Button1.Font.FontHeight := -11;
      Result.Button1.OnClick := Result.Button1Click;
    Result.Form.CenterOnParent;

end;

