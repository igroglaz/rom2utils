<?
$FLAGSDIR="/Games/Allods/flags";
if($REQUEST_METHOD=="POST")
{
  switch($submit)
  {
    case "Обновить список аккаунтов":

      $fp=fopen("$FLAGSDIR/accounts.sem","w");
      fclose($fp);
	print "<font color=red>Список должен быть готов в течении одной минуты</font>";
      break;
  }
}


?>
<form method="post">
<input name="submit" type="submit" value="Обновить список аккаунтов">
</form>
</table>