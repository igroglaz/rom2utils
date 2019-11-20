<?
$GAME_DIR="/Games/Allods/Acc";
//if($_SERVER["REQUEST_METHOD"] == 'POST' && empty($_POST['login']) == FALSE && empty($_POST['password']) == FALSE)
if($_SERVER["REQUEST_METHOD"] == 'POST')
{
$submit = $_POST['submit'];
$login = trim($_POST['login']);
$password = trim($_POST['password']);
$newpassword = trim($_POST['newpassword']);

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
       $banstring = "01.01.2001 01:01:01;-1,АВТОБАН ДО СМЕНЫ ПАРОЛЯ";
       $charbase = "C:\\Games\\Allods\\Chr";
       $ban_path = "$charbase\\".$login[0]."\\".$login.".ban";
       if (file_exists($ban_path)){
            $fp = @fopen($ban_path, "r");
          $buffer = fgets($fp);
          fclose ($fp);
          $buf_len = strlen($buffer);
          $test = 1;
          for ($i=0; $i < $buf_len; $i++) {
	     if ($buffer[$i]!=$banstring[$i])
             {
                  $test = 0;
                  break;
             }
          }
          if ($test == 1)
          {
             unlink($ban_path);
             $old_ban_path = "c:\\games\\allods\\Chr\\OLDBAN\\".$login.".ban";
             if (file_exists($old_ban_path )){
               copy ( $old_ban_path , $ban_path);
               unlink("$old_ban_path");
	       }
        }
        }
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

    case "Разблокировать":
        $lgn = $_POST['login'];
	$lgn = trim($lgn);
	$file = "c:/Games/Allods/Chr/".$lgn[0]."/".$lgn.".lgn" ;
	$shell = "unlock.exe c:/Games/Allods/Chr/".$lgn[0]."/".$lgn.".lgn" ;
	exec($shell, $out, $return);
	echo htmlspecialchars(convert_cyr_string(implode("\n",$out),'d','w'));
	if(!$return)
	{
	print "<font color=green><b>Логин успешно разблокирован</b></font>";
	}
	else
	{
	print "<font color=red><b>Ошибка при разблокировке логина</b></font>";
	}
	echo "<br>";
	print $file;
	break;
  }
}


?>
<form method="POST">
Логин<br>
<input name="login"><br>
Пароль<br>
<input name="password"><br>
Новый пароль<br>
<input name="newpassword"><br>
<input name="submit" type="submit" value="Создать">
<input name="submit" type="submit" value="Изменить">
<input name="submit" type="submit" value="Удалить">
<input name="submit" type="submit" value="Разблокировать">
<p>
Форма управление hat2:
<input name="submit" type="submit" value="Перегрузить">
</form>