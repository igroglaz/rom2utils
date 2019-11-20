<?
if($REQUEST_METHOD=="POST" && $login)
{
        $login = $_POST['login'];  //а где эта строчка была раньше? оО
	$a = "/0";
//	$file = "C:/Allods2/Hat/Chr/".$login[0]."/".$login.".lgn"; 
//	print $file;
	$file = "D:/Games/Allods/Chr/".$login[0]."/".$login.".lgn" ;
        $fp = fopen($file, "rb+");
        if ($fp == 0)
        {
         echo "Не могу открыть файл ".$file;
         exit;
        }
        print $file;
        //fpassthru ($fp);
	//print "<br>";
	fseek($fp, 456);
	fwrite($fp, $a, 1);

	fclose($fp);
}
?>
<form method="post">
Логин<br>
<input name="login"><br>
<input name="submit" type="submit" value="Разблокировать">
</form>