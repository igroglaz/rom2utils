<?
 $monitor="/Games/Allods/PlScript/monitoradm.txt";
?>
<table cellspacing=0 cellpadding=3 border=1 width =100%>
<header><br></header>
<tr align=center valign=middle>
 <td>№</td><td>Имя сервера</td><td>Карта</td><td>Сложность</td><td>Размер</td><td>Тип игры</td><td>Количество игроков</td>
</tr>
<tr></tr>
<?
 if ($fp = @fopen($monitor, "r"))
   {
    fpassthru($fp);
   }
   else
   {
     print("Ошибка определения состояния хэта");
   }

?>
</table>
