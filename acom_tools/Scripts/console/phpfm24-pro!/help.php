<html>
<head>
<title>Справка</title>
<style type='text/css'>
<!--
BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; }
TD { text-align:justify; vertical-align:middle; }
.comment2 { font-family: Verdana; font-size: 7pt; color:black }
#multitext { background-color: #F7F6F3; color:#000000; font-family:Courier new, Verdana, Arial; font-size:8pt }
.halfbutton {border-style : outset;width : 95px; height:20 px;}
input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
A {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt;}
A:hover {COLOR: white; TEXT-DECORATION: none;}
-->
</style>
</head>
<body>
<table width=377 border=0 cellpadding=0 cellspacing=0 style='position:absolute; top:3;left:3;'>
<tr>
<td width='100%' height=20 style='background-color:darkblue;color:white;font-weight:bold;' colspan=2>Справочная система</td>
</tr>
<tr>
<td width='100%' colspan=2>
<br>
<?
switch ($_SERVER['QUERY_STRING'])
{
case "dirsize":
	echo "<b>Показывать размер папок</b><br><br>";
	echo "Эта опция включает и выключает отображение размеров папок. Учтите, что включение этой опции может серьезно замедлить работу файлового менеджера PhpFM.";
	break;
case 'thumbnails':
	echo "<b>Режим &laquo;эскизы страниц&raquo;</b><br><br>";
	echo "Если эта опция включена, вместо файлов, содержащих картинки, будут показываться их уменьшенные изображения. Учтите, что PhpFM использует библиотеку GD, которая может отсутствовать ".((extension_loaded("gd") || extension_loaded("gd2")) ? " (у вас она есть, так что вам не о чем беспокоиться)" : "(у вас её нет, рекомендуется её установить)").", и она использует очень много ресурсов, так что не злоупотребляйте этой возможностью (вы можете заметить значительное замедление при просмотре больших изображений).";
	break;
case "show_time":
	echo "<b>Показывать статистику</b><br><br>";
	echo "Если эта опция включена, вам будет показываться статистика, в которой написано время генерирования скрипта и о том, включено ли Gzip-сжатие.";
	break;
case "use_gzip":
	echo "<b>Использовать Gzip-сжатие</b><br><br>";
	echo "Если возможно, содержимое страницы сжимается с помощью алгоритма сжатия Gzip, и передается в сжатом виде браузеру. Таким образом, вы тратите примерно в 5 раз меньше траффика, чем если бы сжатия не происходило. Это особенно актуально при работе с большими директориями (несколько тысяч файлов), когда коэффициент сжатия может достигнуть 95 %, или даже больше.";
	break;
}
?>
<br><br>
</td>
</tr>
<tr height=20>
<td width='100%' height=20 style='background-color:darkblue;color:white;font-weight:bold;'>Copyright &copy; 2004 PhpFM</td>
<td height=20 style='background-color:darkblue;color:white;font-weight:bold;'><a href='javascript:window.close();' style='color:white;'>[закрыть]</a>
</tr>
</table>
</body>
</html>