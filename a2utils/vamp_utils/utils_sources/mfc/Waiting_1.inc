{ KOL MCK } // Do not remove this line!

procedure NewForm3( var Result: PForm3; AParent: PControl );
begin

  New( Result, Create );
  Result.Form := NewForm( AParent, 'Пожалуйста, подождите...' ).SetPosition( 371, 305 ).SetSize( 326, 116 );
  Result.Form.Add2AutoFree( Result );
    Result.Form.Visible := False;
    Result.Form.Style := Result.Form.Style and not (WS_MINIMIZEBOX or WS_MAXIMIZEBOX);
    DeleteMenu( GetSystemMenu( Result.Form.GetWindowHandle, False ), SC_CLOSE, MF_BYCOMMAND );
      Result.Form.OnClose := Result.KOLForm1Close;
    // Result.Label1.TabOrder = -1
    Result.Label1 := NewLabel( Result.Form, '' ).SetPosition( 0, 16 ).SetSize( 313, 0 );
    Result.Label1.Font.FontStyle := [ fsBold ];
    Result.Label1.Font.FontHeight := -11;
    Result.Label1.TextAlign := taCenter;
    Result.Label1.VerticalAlign := vaCenter;
    // Result.ProgressBar1.TabOrder = 0
    Result.ProgressBar1 := NewProgressBarEx( Result.Form, [ pboSmooth ] ).SetPosition( 8, 56 );
    Result.ProgressBar1.Progress := 50;
    Result.Form.CenterOnParent.CanResize := False;

end;

