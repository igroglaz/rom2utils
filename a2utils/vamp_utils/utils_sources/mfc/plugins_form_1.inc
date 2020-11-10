{ KOL MCK } // Do not remove this line!

procedure NewForm2( var Result: PForm2; AParent: PControl );
begin

  New( Result, Create );
  Result.Form := NewForm( AParent, 'Загруженые плагины...' ).SetPosition( 278, 311 ).SetSize( 626, 254 );
  Result.Form.Add2AutoFree( Result );
    Result.Form.Visible := False;
    Result.Form.Style := Result.Form.Style and not WS_MAXIMIZEBOX;
      Result.Form.OnClose := Result.KOLForm1Close;
    // Result.ListView1.TabOrder = 0
    Result.ListView1 := NewListView( Result.Form, lvsDetail, [ lvoAutoArrange, lvoRowSelect ], nil, nil, nil ).SetPosition( 0, 0 ).SetSize( 617, 193 );
    Result.ListView1.Color := clWindow;
    // Result.Button1.TabOrder = 1
    Result.Button1 := NewButton( Result.Form, 'Закрыть' ).SetPosition( 552, 200 );
    Result.Button1.Font.FontHeight := -11;
      Result.Button1.OnClick := Result.Button1Click;
    Result.Form.CenterOnParent.CanResize := False;
    Result.KOLForm1FormCreate( Result );

end;

