<?
$GAME_DIR="/Games/Allods/Acc";

if($REQUEST_METHOD=="POST" && $login && $password)
{
  switch($submit)
  {
    case "Перегрузить":
     $fp=fopen("$GAME_DIR/Shutdown", "w");
     fclose($fp);
     //sleep(1);
     print "hat2 успешно перезапущен<BR>";
    break;
    case "Создать":
      $fp=fopen("$GAME_DIR/CreateAccount.Name", "w");
      fwrite($fp, "$login\n");
      fwrite($fp, "$password\n");
      fclose($fp);

sleep(5);
	$FN = trim("$GAME_DIR/Bad_CreateAccount.name");
	if (file_exists($FN)) {
		print "<font color=red>ОШИБКА! Аккаунт <b> $login </b> сушествует.<BR></font>";
	$FN2 = eregi_replace("/","\\",$FN);
	unlink("$FN2");
	} else {
	print "Аккаунт  <b> $login </b> создан.<BR>";
	}

      break;

    case "Изменить":
      $fp=fopen("$GAME_DIR/ChangePassword.".$login, "w");
      fwrite($fp, "$login\n");
      fwrite($fp, "$password\n");
      fwrite($fp, "$newpassword\n");
      fclose($fp);

sleep(5);
	$FN = trim("$GAME_DIR/Bad_ChangePassword.".$login);
	if (file_exists($FN)) {
		print "<font color=red>ОШИБКА! Смена пароля для аккаунта  <b> $login </b>  провалилась.<BR>";
	$FN2 = eregi_replace("/","\\",$FN);
	unlink("$FN2");
	} else {
	print "Успешно сменен пароль на аккаунте  <b> $login </b> .<BR></font>";
	}
      break;

    case "Удалить":
      $fp=fopen("$GAME_DIR/RemoveAccount.name", "w");
      fwrite($fp, "$login\n");
      fwrite($fp, "$password\n");
      fclose($fp);

sleep(5);
	$FN = trim("$GAME_DIR/Bad_RemoveAccount.name");
	if (file_exists($FN)) {
		print "<font color=red>ОШИБКА! Удаление аккаунта  <b> $login </b>  провалилось.<BR></font>";
	$FN2 = eregi_replace("/","\\",$FN);
	unlink("$FN2");
	} else {
	print "Аккаунт  <b> $login </b>  был удален.<BR>";
	}
      break;
  }
}


?>
<form method="post">
Логин<br>
<input name="login"><br>
Пароль<br>
<input name="password"><br>
Новый пароль<br>
<input name="newpassword"><br>
<input name="submit" type="submit" value="Создать">
<input name="submit" type="submit" value="Изменить">
<input name="submit" type="submit" value="Удалить">
<p>
Форма управление hat2:
<input name="submit" type="submit" value="Перегрузить">
</form>