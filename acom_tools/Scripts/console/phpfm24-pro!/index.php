<?
//ip access
$ip = getenv('REMOTE_ADDR');
$ips = explode('.', $ip);
if($ips['0'] == '62' && $ips['1'] == '65')
{
        if(gethostbyname('pservers.mooo.com') != $ip) die('no access!');
}
//таймер, счётчик времени генерирования скрипта
class Timer_phpfm
{
	var $startTime;
	var $endTime;
	function start() { $this->startTime = gettimeofday(); }
	function stop() { $this->endTime = gettimeofday(); }
	function elapsed() { return (($this->endTime["sec"] - $this->startTime["sec"]) * 1000000 + ($this->endTime["usec"] - $this->startTime["usec"])) / 1000000; }
}
$timer = new Timer_phpfm();
$timer->start();
//**
session_name('fmsid');
session_start();
//**
$phpfm['php_self']=basename(__FILE__);
$phpfm['rootpath']=dirname(__FILE__);
//**
$phpfm['version']="2.4 pro";
$phpfm['copyright']="phpFM version ".$phpfm['version'];
//**
//функции
//**
include($phpfm['rootpath']."/config.php");
include($phpfm['rootpath']."/accounts.php");
include($phpfm['rootpath']."/funcs.php");
include($phpfm['rootpath'].'/description.php');
//**
include($phpfm['rootpath']."/check.php");
//**
if(!isset($_SESSION['started']))
{
	$_SESSION['use_gzip']=true;
	$_SESSION['show_time']=true;
	$_SESSION['thumbnails']=false;
	$_SESSION['show_dirsize']=false;
	$_SESSION['drives']=get_disk_drives();
}
//**
$_SESSION['started']=true;
//**
if(isset($_POST['submitted']))
{
	if(isset($_POST['show_dirsize'])) $_SESSION['show_dirsize']=true;
	else $_SESSION['show_dirsize']=false;
	//**
	if(isset($_POST['show_time'])) $_SESSION['show_time']=true;
	else $_SESSION['show_time']=false;
	//**
	if(isset($_POST['use_gzip'])) $_SESSION['use_gzip']=true;
	else $_SESSION['use_gzip']=false;
	//**
	if(isset($_POST['thumbnails'])) $_SESSION['thumbnails']=true;
	else $_SESSION['thumbnails']=false;
}
//**
if(isset($_SESSION['logined_as_user']))
{
	$phpfm['user-settings']=DumpSettings($_SESSION['user_name'], chr(0));
	$phpfm['user-name']=$_SESSION['user_name'];
	$phpfm['user-space']=GetDiskSpace();
	$phpfm['user-space-left']=floor($phpfm['user-settings']['quota']-$phpfm['user-space']);
}else if(isset($_SESSION['logined']))
{
	$phpfm['user-space-left']=999999; #т.е. очень много
	$phpfm['user-settings']['eval']=1;
	$phpfm['user-name']='Админ';
	$phpfm['user-settings']['dir']=':любая:';
	$phpfm['user-settings']['quota']=999999;
}
//**
$phpfm['gzip']=false;
if (phpversion() >= "4.0.4pl1" && extension_loaded("zlib") && $_SESSION['use_gzip']==true)
{
	@ob_end_clean();
	@ob_start("ob_gzhandler");
	$phpfm['gzip']=true;
}
//**
if(!isset($_GET['act'])) $_GET['act']="open";
switch($_GET['act'])
{
case "open":
	if(!isset($_GET['dir']))
	{
		if(isset($_SESSION['logined'])) $phpfm['dir']=(isset($_SESSION['path'])) ? ($_SESSION['path']) : (realpath(".."));
		else $phpfm['dir']=(isset($_SESSION['path'])) ? ($_SESSION['path']) : (realpath($phpfm['user-settings']['dir']));
	}else
	{
		$phpfm['dir']=$_SESSION['path']."/".$_GET['dir'];
	}
	//**
	if(isset($_GET['absolute_dir'])) $phpfm['dir']=$_GET['absolute_dir'];
	//**
	break;
case "rename":
	CheckAccess($_SESSION['path']."/".$_GET['file2']);
	//**
	if(empty($_GET['file2'])) { echo "<script language=javascript>alert('Имя файла не должно быть пустым !');</script>";break; }
	//**
	if(!@rename($_SESSION['path']."/".$_GET['file2'],$_SESSION['path']."/".$_GET['file'])) echo "<script language=javascript>alert('Ошибка при переименовании: ".$php_errormsg."');</script>";
	break;
case "delete":
	if(isset($_GET['dir']))
	{
		//и тут тоже все записано...
		CheckAccess($_SESSION['path']."/".$_GET['dir']);
		//**
		$phpfm['dirsize']=dirsize($_SESSION['path']."/".$_GET['dir']);
		//**
		if(@removedir($_SESSION['path']."/".$_GET['dir'])) EditDiskSpace(-$phpfm['dirsize']);
		else echo "<script language=javascript>alert('Папка не может быть удалена.\\nСкорее всего в папке содержатся файлы, доступ к которым запрещен\\nПроверьте права файлов внутри папки.\\n Они должны иметь права на запись.');</script>";
	}else if(isset($_GET['file']))
	{
		CheckAccess($_SESSION['path']."/".$_GET['file']);
		//**
		$phpfm['filesize']=filesize($_SESSION['path']."/".$_GET['file']);
		//**
		if(@unlink($_SESSION['path']."/".$_GET['file'])) EditDiskSpace(-$phpfm['filesize']);
		else echo "<script language=javascript>alert('Ошибка при удалении файла: ".$php_errormsg."');</script>";
	}else
	{
		$phpfm['parts']=explode(':',$_GET['all']);
		$phpfm['success']=true;
		foreach($phpfm['parts'] as $k=>$v)
		{
			if(empty($v)) continue;
			CheckAccess($_SESSION['path']."/".$v);
			//**
			$phpfm['filesize']=unisize($_SESSION['path']."/".$v,false,false,true);
			//**
			if(@deleteall($_SESSION['path']."/".$v) && $phpfm['success']) EditDiskSpace(-$phpfm['filesize']);
			else $phpfm['success']=false;
		}
		if(!$phpfm['success']) echo "<script language=javascript>alert('Элементы не могут быть удалены.\\nСкорее всего в папке содержатся файлы, доступ к которым запрещен\\nПроверьте права файлов внутри папки.\\n Они должны иметь права на запись.');</script>";
	}
	break;
case "download":
	//**
	$phpfm['shortname']=$_GET['file'];
	$phpfm['file']=$_SESSION['path']."/".$_GET['file'];
	//у нас все ходы записаны...
	CheckAccess($phpfm['file']);
	//**
	header("Content-Type: application/force-download\r\n"); 
	header("Content-Transfer-Encoding: binary\r\n");
	header("Content-Length: ".filesize($phpfm['file']));
	header("Content-Disposition: attachment; filename=".$phpfm['shortname']."\r\n");
	readfile($phpfm['file']);
	//**
	die();
	exit;
	break;
case "properties":
	if(isset($_GET['file']))
	{
		$phpfm['file']=$_GET['file'];
		//**
		$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['file']);
		//**
		$phpfm['pathinfo']=pathinfo($phpfm['full path']);
		$phpfm['extension']=$phpfm['pathinfo']['extension'];
		//**
		$phpfm['filesize']=filesize($phpfm['full path']);
		//**
		if($phpfm['filesize']<=1024) $phpfm['filesize']=$phpfm['filesize']." байт";
		else if($phpfm['filesize']>1024 && $phpfm['filesize']<=(1024*1024)) $phpfm['filesize']=round(($phpfm['filesize']/1024),1)." Кб (".$phpfm['filesize']." байт)";
		else $phpfm['filesize']=round(($phpfm['filesize']/(1024*1024)),1)." Мб (".$phpfm['filesize']." байт)";
		//**
		@$phpfm['perms']=display_perms(fileperms($phpfm['full path']));
		//**
		$phpfm['vals']=explode_by_size(substr($phpfm['perms'],1,9),(1/(1024*1024)));
		?>
		<html>
		<head>
		<title>Свойства: <?=$phpfm['file']?></title>
		<style type='text/css'>
		<!--
		BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; }
		TD { text-align:left; vertical-align:top; }
		.comment2 { font-family: Verdana; font-size: 7pt; color:black }
		input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
		-->
		</style>
		</head>
		<body onload='do_chmod("owner");do_chmod("group");do_chmod("other");'>
		<table width=380 border=0 cellspacing=2 cellpadding=2 align=right style='position:absolute;top:0;left:0;'>
		<form action='index.php' target='_blank' onsubmit='window.close()' method=get>
		<tr>
		<td width=100 valign=top><img src='images/file-<?=$phpfm['extension']?>.png'></td>
		<td width=280 valign=top><input type=text name='file' value='<?=$phpfm['file']?>' style='width:280'><input type=hidden name=act value=rename><input type=hidden name='file2' value='<?=$phpfm['file']?>'><input type=submit value='переименовать' style='display:none;visibility:hidden;'></td>
		</tr>
		</form>
		<tr>
		<td width=380 colspan=2><hr size=1></td>
		</tr>
		<tr>
		<td width=100 valign=top>Тип файла:</td>
		<td width=280 valign=top><?if(isset($desc[$phpfm['extension']])) { echo $desc[$phpfm['extension']]; }else{ echo "Описание отсутствует"; }?></td>
		</tr>
		<tr>
		<td width=380 colspan=2><hr size=1></td>
		</tr>
		<tr>
		<td width=100 valign=top>Размещение:</td>
		<td width=280 valign=top><?=$_SESSION['path']?></td>
		</tr>
		<tr>
		<td width=100 valign=top>Размер:</td>
		<td width=280 valign=top><?=$phpfm['filesize']?></td>
		</tr>
		<tr>
		<td width=380 colspan=2><hr size=1></td>
		</tr>
		<tr>
		<td width=100 valign=top>Создан:</td>
		<td width=280 valign=top><?=date("d.m.Y г., h:i:s",@filectime($phpfm['full path']))?></td>
		</tr>
		<tr>
		<td width=100 valign=top>Изменен:</td>
		<td width=280 valign=top><?=date("d.m.Y г., h:i:s",@filemtime($phpfm['full path']))?></td>
		</tr>
		<tr>
		<td width=100 valign=top>Открыт:</td>
		<td width=280 valign=top><?=date("d.m.Y г., h:i:s",@fileatime($phpfm['full path']))?></td>
		</tr>
		<tr>
		<td width=380 colspan=2><hr size=1></td>
		</tr>
		<tr>
		<td colspan=2>
		<script language="JavaScript">
		<!--
		/* chmod helper, Version 1.0
		 * by Dan Kaplan <design@abledesign.com>
		 * Last Modified: May 24, 2001
		 * --------------------------------------------------------------------
		 * Inspired by 'Chmod Calculator' by Peter Crouch:
		 * http://wsabstract.com/script/script2/chmodcal.shtml
		 *
		 * USE THIS LIBRARY AT YOUR OWN RISK; no warranties are expressed or
		 * implied. You may modify the file however you see fit, so long as
		 * you retain this header information and any credits to other sources
		 * throughout the file.  If you make any modifications or improvements,
		 * please send them via email to Dan Kaplan <design@abledesign.com>.
		 * --------------------------------------------------------------------
		*/
		
		function do_chmod(user) {
		var field4 = user + "4";
		var field2 = user + "2";
		var field1 = user + "1";
		var total = "t_" + user;
		var symbolic = "sym_" + user;
		var number = 0;
		var sym_string = "";
	
		if (document.chmod[field4].checked == true) { number += 4; }
		if (document.chmod[field2].checked == true) { number += 2; }
		if (document.chmod[field1].checked == true) { number += 1; }
	
		if (document.chmod[field4].checked == true) {
			sym_string += "r";
		} else {
			sym_string += "-";
		}
		if (document.chmod[field2].checked == true) {
			sym_string += "w";
		} else {
			sym_string += "-";
		}
		if (document.chmod[field1].checked == true) {
			sym_string += "x";
		} else {
			sym_string += "-";
		}
	
		if (number == 0) { number = "0"; }
		document.chmod[total].value = number;
		document.chmod[symbolic].value = sym_string;	
		document.chmod.t_total.value = "0" + document.chmod.t_owner.value + document.chmod.t_group.value + document.chmod.t_other.value;
		
		if(!document.chmod.sym_owner.value) f1="---";
		else f1=document.chmod.sym_owner.value;
		
		if(!document.chmod.sym_group.value) f2="---";
		else f2=document.chmod.sym_group.value;
		
		if(!document.chmod.sym_other.value) f3="---";
		else f3=document.chmod.sym_other.value;
		
		document.chmod.sym_total.value = "-" + f1 + f2 + f3;
		}
		//-->
		</script>
		
		<form name="chmod" action='index.php' method=get>
		<input type=hidden name=act value=formenu>
		<input type=hidden name=subact value=chmod>
		<input type=hidden name=content value='<?=$phpfm['file']?>'>
		<input type=hidden name=type value=file>
		<table cellpadding="0" cellspacing="0" border="0" bgcolor="#03075D">
		<tr><td width="100%" valign="top">
		<table width="100%" cellpadding="5" cellspacing="2" border="0">
			<tr bgcolor="#bcbcbc">
				<td align="left"><b>Права</b></td>
				<td align="center"><b>Владелец</b></td>
				<td align="center"><b>Группа</b></td>
				<td align="center"><b>Прочее</b></td>
				<td align="center"><b>Сумма</b></td>
			</tr><tr bgcolor="#dddddd">
				<td align="left" nowrap><b>Чтение</b></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="owner4" value="4" onclick="do_chmod('owner')"<?if($phpfm['vals'][0]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="group4" value="4" onclick="do_chmod('group')"<?if($phpfm['vals'][3]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="other4" value="4" onclick="do_chmod('other')"<?if($phpfm['vals'][6]!="-") echo " checked";?>></td>
				<td bgcolor="#dddddd">&nbsp;</td>
			</tr><tr bgcolor="#dddddd">
				<td align="left" nowrap><b>Запись</b></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="owner2" value="2" onclick="do_chmod('owner')"<?if($phpfm['vals'][1]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="group2" value="2" onclick="do_chmod('group')"<?if($phpfm['vals'][4]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="other2" value="2" onclick="do_chmod('other')"<?if($phpfm['vals'][7]!="-") echo " checked";?>></td>
				<td bgcolor="#dddddd">&nbsp;</td>
			</tr><tr bgcolor="#dddddd">
				<td align="left" nowrap><b>Выполнение</b></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="owner1" value="1" onclick="do_chmod('owner')"<?if($phpfm['vals'][2]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="group1" value="1" onclick="do_chmod('group')"<?if($phpfm['vals'][5]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="other1" value="1" onclick="do_chmod('other')"<?if($phpfm['vals'][8]!="-") echo " checked";?>></td>
				<td bgcolor="#dddddd">&nbsp;</td>
			</tr><tr bgcolor="#dddddd">
				<td align="right" nowrap>Цифровой:</td>
				<td align="center"><input type="text" id="t_owner" value="0" size="1" style='width:100%'></td>
				<td align="center"><input type="text" id="t_group" value="0" size="1" style='width:100%'></td>
				<td align="center"><input type="text" id="t_other" value="0" size="1" style='width:100%'></td>
				<td align="left"><input type="text" name="t_total" value="0" size="3" style='width:100%'></td>
			</tr><tr bgcolor="#dddddd">
				<td align="right" nowrap>Символьный:</td>
				<td align="center"><input type="text" id="sym_owner" value="" size="3" style='width:100%'></td>
				<td align="center"><input type="text" id="sym_group" value="" size="3" style='width:100%'></td>
				<td align="center"><input type="text" id="sym_other" value="" size="3" style='width:100%'></td>
				<td align="left"><input type="text" id="sym_total" value="<?=$phpfm['perms']?>" size="10" style='width:80px;'></td>
			</tr><tr bgcolor="#dddddd"><td colspan="5" align="center">
				<font face="Arial" size="1">Provided free by <a href="http://abledesign.com/programs/" target="_blank">AbleDesign</a>, inspired by <a href="http://wsabstract.com/script/script2/chmodcal.shtml" target="_blank">Chmod Calculator</a></font>
			</td></tr>
			</table></td></tr></table>
			<div align=center><input type=submit value='сменить права'></div>
			</form>
		</td>
		</tr>
		</table>
		</body>
		</html>
<?
	}else
	{
		$phpfm['file']=$_GET['dir'];
		//**
		$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['file']);
		//**
		$phpfm['stats']=dirstats($phpfm['full path']);
		//**
		if($phpfm['stats']['size']<=1024) $phpfm['stats']['size']=$phpfm['stats']['size']." байт";
		else if($phpfm['stats']['size']>1024 && $phpfm['stats']['size']<=(1024*1024)) $phpfm['stats']['size']=round(($phpfm['stats']['size']/1024),1)." Кб (".$phpfm['stats']['size']." байт)";
		else $phpfm['stats']['size']=round(($phpfm['stats']['size']/(1024*1024)),1)." Мб (".$phpfm['stats']['size']." байт)";
		//**
		@$phpfm['perms']=display_perms(fileperms($phpfm['full path']));
		//**
		$phpfm['vals']=explode_by_size(substr($phpfm['perms'],1,9),(1/(1024*1024)));
		?>
		<html>
		<head>
		<title>Свойства: <?=$phpfm['file']?></title>
		<style type='text/css'>
		<!--
		BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; }
		TD { text-align:left; vertical-align:top; }
		.comment2 { font-family: Verdana; font-size: 7pt; color:black }
		input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
		-->
		</style>
		</head>
		<body onload='do_chmod("owner");do_chmod("group");do_chmod("other");'>
		<table width=380 border=0 cellspacing=2 cellpadding=2 align=right style='position:absolute;top:0;left:0;'>
		<form action='index.php' target='_blank' onsubmit='window.close()' method=get>
		<tr>
		<td width=100 valign=top><img src='images/folder.png'></td>
		<td width=280 valign=top><input type=text name='file' value='<?=$phpfm['file']?>' style='width:280'><input type=hidden name=act value=rename><input type=hidden name='file2' value='<?=$phpfm['file']?>'><input type=submit value='переименовать' style='display:none;visibility:hidden;'></td>
		</tr>
		</form>
		<tr>
		<td width=380 colspan=2><hr size=1></td>
		</tr>
		<tr>
		<td width=100 valign=top>Тип:</td>
		<td width=280 valign=top>Папка с файлами</td>
		</tr>
		<tr>
		<td width=100 valign=top>Размещение:</td>
		<td width=280 valign=top><?=$_SESSION['path']?></td>
		</tr>
		<tr>
		<td width=100 valign=top>Размер:</td>
		<td width=280 valign=top><?=$phpfm['stats']['size']?></td>
		</tr>
		<tr>
		<td width=100 valign=top>Содержит:</td>
		<td width=280 valign=top><?echo declesion($phpfm['stats']['files'],array("файл","файла","файлов"));echo " и ";echo declesion($phpfm['stats']['dirs'],array("папку","папки","папок"));?></td>
		</tr>
		<tr>
		<td width=380 colspan=2><hr size=1></td>
		</tr>
		<tr>
		<td width=100 valign=top>Создана:</td>
		<td width=280 valign=top><?=date("d.m.Y г., h:i:s",@filectime($phpfm['full path']))?></td>
		</tr>
		<tr>
		<td width=100 valign=top>Изменена:</td>
		<td width=280 valign=top><?=date("d.m.Y г., h:i:s",@filemtime($phpfm['full path']))?></td>
		</tr>
		<tr>
		<td width=100 valign=top>Открыта:</td>
		<td width=280 valign=top><?=date("d.m.Y г., h:i:s",@fileatime($phpfm['full path']))?></td>
		</tr>
		<tr>
		<td width=380 colspan=2><hr size=1></td>
		</tr>
		<tr>
		<td colspan=2>
		<script language="JavaScript">
		<!--
		/* chmod helper, Version 1.0
		 * by Dan Kaplan <design@abledesign.com>
		 * Last Modified: May 24, 2001
		 * --------------------------------------------------------------------
		 * Inspired by 'Chmod Calculator' by Peter Crouch:
		 * http://wsabstract.com/script/script2/chmodcal.shtml
		 *
		 * USE THIS LIBRARY AT YOUR OWN RISK; no warranties are expressed or
		 * implied. You may modify the file however you see fit, so long as
		 * you retain this header information and any credits to other sources
		 * throughout the file.  If you make any modifications or improvements,
		 * please send them via email to Dan Kaplan <design@abledesign.com>.
		 * --------------------------------------------------------------------
		*/
		
		function do_chmod(user) {
		var field4 = user + "4";
		var field2 = user + "2";
		var field1 = user + "1";
		var total = "t_" + user;
		var symbolic = "sym_" + user;
		var number = 0;
		var sym_string = "";
	
		if (document.chmod[field4].checked == true) { number += 4; }
		if (document.chmod[field2].checked == true) { number += 2; }
		if (document.chmod[field1].checked == true) { number += 1; }
	
		if (document.chmod[field4].checked == true) {
			sym_string += "r";
		} else {
			sym_string += "-";
		}
		if (document.chmod[field2].checked == true) {
			sym_string += "w";
		} else {
			sym_string += "-";
		}
		if (document.chmod[field1].checked == true) {
			sym_string += "x";
		} else {
			sym_string += "-";
		}
	
		if (number == 0) { number = "0"; }
		document.chmod[total].value = number;
		document.chmod[symbolic].value = sym_string;	
		document.chmod.t_total.value = "0" + document.chmod.t_owner.value + document.chmod.t_group.value + document.chmod.t_other.value;
		
		if(!document.chmod.sym_owner.value) f1="---";
		else f1=document.chmod.sym_owner.value;
		
		if(!document.chmod.sym_group.value) f2="---";
		else f2=document.chmod.sym_group.value;
		
		if(!document.chmod.sym_other.value) f3="---";
		else f3=document.chmod.sym_other.value;
		
		document.chmod.sym_total.value = "d" + f1 + f2 + f3;
		}
		//-->
		</script>
		
		<form name="chmod" action='index.php' method=get>
		<input type=hidden name=act value=formenu>
		<input type=hidden name=subact value=chmod>
		<input type=hidden name=content value='<?=$phpfm['file']?>'>
		<input type=hidden name=type value=dir>
		<table cellpadding="0" cellspacing="0" border="0" bgcolor="#03075D">
		<tr><td width="100%" valign="top">
		<table width="100%" cellpadding="5" cellspacing="2" border="0">
			<tr bgcolor="#bcbcbc">
				<td align="left"><b>Права</b></td>
				<td align="center"><b>Владелец</b></td>
				<td align="center"><b>Группа</b></td>
				<td align="center"><b>Прочее</b></td>
				<td align="center"><b>Сумма</b></td>
			</tr><tr bgcolor="#dddddd">
				<td align="left" nowrap><b>Чтение</b></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="owner4" value="4" onclick="do_chmod('owner')"<?if($phpfm['vals'][0]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="group4" value="4" onclick="do_chmod('group')"<?if($phpfm['vals'][3]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="other4" value="4" onclick="do_chmod('other')"<?if($phpfm['vals'][6]!="-") echo " checked";?>></td>
				<td bgcolor="#dddddd">&nbsp;</td>
			</tr><tr bgcolor="#dddddd">
				<td align="left" nowrap><b>Запись</b></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="owner2" value="2" onclick="do_chmod('owner')"<?if($phpfm['vals'][1]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="group2" value="2" onclick="do_chmod('group')"<?if($phpfm['vals'][4]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="other2" value="2" onclick="do_chmod('other')"<?if($phpfm['vals'][7]!="-") echo " checked";?>></td>
				<td bgcolor="#dddddd">&nbsp;</td>
			</tr><tr bgcolor="#dddddd">
				<td align="left" nowrap><b>Выполнение</b></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="owner1" value="1" onclick="do_chmod('owner')"<?if($phpfm['vals'][2]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="group1" value="1" onclick="do_chmod('group')"<?if($phpfm['vals'][5]!="-") echo " checked";?>></td>
				<td align="center" bgcolor="#ffffff" valign=center style='vertical-align:center;text-align:center;'><input type="checkbox" id="other1" value="1" onclick="do_chmod('other')"<?if($phpfm['vals'][8]!="-") echo " checked";?>></td>
				<td bgcolor="#dddddd">&nbsp;</td>
			</tr><tr bgcolor="#dddddd">
				<td align="right" nowrap>Цифровой:</td>
				<td align="center"><input type="text" id="t_owner" value="0" size="1" style='width:100%'></td>
				<td align="center"><input type="text" id="t_group" value="0" size="1" style='width:100%'></td>
				<td align="center"><input type="text" id="t_other" value="0" size="1" style='width:100%'></td>
				<td align="left"><input type="text" name="t_total" value="0" size="3" style='width:100%'></td>
			</tr><tr bgcolor="#dddddd">
				<td align="right" nowrap>Символьный:</td>
				<td align="center"><input type="text" id="sym_owner" value="" size="3" style='width:100%'></td>
				<td align="center"><input type="text" id="sym_group" value="" size="3" style='width:100%'></td>
				<td align="center"><input type="text" id="sym_other" value="" size="3" style='width:100%'></td>
				<td align="left"><input type="text" id="sym_total" value="<?=$phpfm['perms']?>" size="10" style='width:80px;'></td>
			</tr><tr bgcolor="#dddddd"><td colspan="5" align="center">
				<font face="Arial" size="1">Provided free by <a href="http://abledesign.com/programs/" target="_blank">AbleDesign</a>, inspired by <a href="http://wsabstract.com/script/script2/chmodcal.shtml" target="_blank">Chmod Calculator</a></font>
			</td></tr>
			</table></td></tr></table>
			<div align=center><input type=submit value='сменить права'></div>
			</form>
		</td>
		</tr>
		</table>
		</body>
		</html>
<?
	}
	die();
	exit;
	break;
case "edit":
	if($phpfm['user-space-left'] <= 0)
	{
		echo "<script language=JavaScript>";
		echo "alert('Вы израсходовали лимит дискового пространства.\\nЭта функция недоступна. Удалите лишние файлы и попробуйте еще раз.');";
		echo "window.close();</script>";
		break;
	}
	//**
	$phpfm['file']=$_GET['file'];
	//**
	$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['file']);
	//**
	//защита от черезчур хитрых пользователей
	//**
	CheckAccess($phpfm['full path']);
	//**
	$phpfm['extension']=pathinfo($phpfm['full path']);
	//**
	switch(strtolower(@$phpfm['extension']['extension']))
	{
		case "jpg";
		case "png":
		case "jpeg":
		case "gif":
		case "jpe":
		Header("Location: ".$phpfm['php_self']."?act=viewpicture&file=".rawurlencode($phpfm['file']));
		die();
		exit;
		break;
	}
	//**
	if(filesize($phpfm['full path'])>200*1024) $phpfm['buffer']="Выбран слишком большой файл для редактирования:\nвозможно, это архив или системный файл";
	else if(!$phpfm['buffer']=@file2string($phpfm['full path'])) $phpfm['buffer']="Файл либо пустой, либо у вас нет прав на чтение этого файла";
	//**
	$phpfm['buffer']=stripslashes(addslashes(htmlspecialchars($phpfm['buffer'])));
	//**
	$phpfm['readable']=is_readable($phpfm['full path']);
	$phpfm['writable']=is_writable($phpfm['full path']);
	//**
	$phpfm['str readable']=($phpfm['readable']) ? ("<span style='color:green'>Есть права на чтение</span>") : ("<span style='color:red'>Нет прав на чтение</span>");
	$phpfm['str writable']=($phpfm['writable']) ? ("<span style='color:green'>Есть права на запись</span>") : ("<span style='color:red'>Нет прав на запись</span>");
	?>
	<html>
	<head>
	<title> <?=$phpfm['file']?> - PhpFM - редактор текста </title>
	<style type='text/css'>
	<!--
	BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; }
	TD { text-align:left; vertical-align:top; }
	.comment2 { font-family: Verdana; font-size: 7pt; color:black }
	#multitext { background-color: #F7F6F3; color:#000000; font-family:Courier new, Verdana, Arial; font-size:8pt }
	.halfbutton {border-style : outset;width : 95px; height:20 px;}
	input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
	A {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt; cursor: default; display: block; border: 0px solid #000000;}
	A:hover {COLOR: white; TEXT-DECORATION: none; background-color: #00126C;}
	-->
	</style>
	<script language=javascript>
	<!--
	var submitted = 0;
	var changed = 0;
	//-->
	</script>
	</head>
	<body onbeforeunload="if(!submitted && changed) { return 'Файл был изменен, продолжить закрытие окна?'; }">
	<h3 align=center style='align:center'> Редактирование файла <?=$phpfm['file']?> </h3> <div align=center style='align:center'>( <?=$phpfm['str readable']?> | <?=$phpfm['str writable']?> )<br><a href='index.php?act=edit-conf&subact=open&file=<?=rawurlencode($phpfm['file'])?>' target=_blank>отредактировать с помощью config-editor</a></div> 
	<br><br><form action='index.php?act=apply' name='editfile' id='editfile' method=post onsubmit="submitted=1;">
	<input type=hidden name='file' value='<?=$phpfm['file']?>'>
	<textarea id=multitext name='content' style='width:600;height:310' wrap=off<?if(!$phpfm['writable']) echo " readonly";?> onkeydown='changed=1;'><?=$phpfm['buffer']?></textarea>
	<br><div align=center style='align:center'><input type=submit value='применить' class=halfbutton<?if(!$phpfm['writable']) echo " disabled";?>>&nbsp;<input type=reset value='сбросить' class=halfbutton></div>
	</form>
	</body></html>
<?	//**
	die();
	exit;
	break;
case "viewpicture":
	$phpfm['file']=$_GET['file'];
	//**
	$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['file']);
	//даже чужие картинки невозможно просмотреть
	CheckAccess($phpfm['full path']);
	//**
	$phpfm['extension']=pathinfo($phpfm['full path']);
	//**
	switch(strtolower($phpfm['extension']['extension']))
	{
	case "jpg":
	case "jpeg":
	case "jpe":
		Header("Content-type: image/jpeg");
		break;
	case "gif":
		Header("Content-type: image/gif");
		break;
	case "png":
		Header("Content-type: image/png");
		break;
	}
	readfile($phpfm['full path']);
	//**
	die();
	exit;
	break;
case "thumb":
	@$phpfm['file']=$_GET['file'];
	//**
	$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['file']);
	//даже чужие картинки невозможно просмотреть
	CheckAccess($phpfm['full path']);
	//**
	$phpfm['extension']=pathinfo($phpfm['full path']);
	//**
	switch(@strtolower($phpfm['extension']['extension']))
	{
	case "jpg":
	case "jpeg":
	case "jpe":
	case "gif":
	case "png":
		CreateThumb($phpfm['full path']);
		//**
		break;
	default:
		Header("Location: images/file-".$phpfm['extension']['extension'].".png");
		break;
	}
	break;
case "apply":
	if($phpfm['user-space-left'] <= 0)
	{
		echo "<script language=JavaScript>";
		echo "alert('Вы израсходовали лимит дискового пространства.\\nЭта функция недоступна. Удалите лишние файлы и попробуйте еще раз.');";
		echo ";</script>";
		break;
	}
	//**
	$phpfm['file']=$_POST['file'];
	$phpfm['full path']=$_SESSION['path']."/".$phpfm['file'];
	//**
	$phpfm['content']=$_POST['content'];
	//**
	if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) $phpfm['content']=stripslashes($phpfm['content']);
	//**
	//ограничение пользователей
	//**
	EditDiskSpace(strlen($phpfm['content'])-file2string($phpfm['full path']));
	CheckAccess($phpfm['full path']);
	CheckByFilter($phpfm['content']);
	//**
	echo "<html><head><script language=javascript>";
	if(!@writefile($phpfm['full path'],$phpfm['content'])) echo "alert(\"Файл '".$phpfm['file']."' не может быть отредактирован. Причина: ".$php_errormsg."\");";
	echo "window.close();</script></head></html>";
	//**
	die();
	exit;
	break;
case "edit-conf":
	if($phpfm['user-space-left'] <= 0)
	{
		echo "<script language=JavaScript>";
		echo "alert('Вы израсходовали лимит дискового пространства.\\nЭта функция недоступна. Удалите лишние файлы и попробуйте еще раз.');";
		echo "window.close();</script>";
		break;
	}
	//**
	$phpfm['file']=$_GET['file'];
	//**
	$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['file']);
	//**
	//защита от черезчур хитрых пользователей
	//**
	CheckAccess($phpfm['full path']);
	//**
	?>
	<html>
	<head>
	<title> <?=$phpfm['file']?> - PhpFM - config editor </title>
	<style type='text/css'>
	<!--
	BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; }
	TD { text-align:left; vertical-align:top; }
	.comment2 { font-family: Verdana; font-size: 7pt; color:black }
	#multitext { background-color: #F7F6F3; color:#000000; font-family:Courier new, Verdana, Arial; font-size:8pt }
	.halfbutton {border-style : outset;width : 95px; height:20 px;}
	input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
	A {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt; cursor: default; display: block; border: 0px solid #000000;}
	A:hover {COLOR: white; TEXT-DECORATION: none; background-color: #00126C;}
	-->
	</style>
	</head>
	<body>
	<h3 align=center style='align:center'>PhpFM config editor</h3>
	<h4 align=center style='align:center'><a href='javascript:window.close()'>закрыть</a></h4>
	<?
	switch($_GET['subact'])
	{
	case "open":
		?>
		<form action='index.php?act=edit-conf&subact=apply&file=<?=$phpfm['file']?>' method=post>
		<table align=center cellspacing=2 cellpadding=2 border=1 width='75%'>
		<tr>
		<td colspan=2 style='text-align:center'><b>Конфигурация</b></td>
<?		
		$phpfm['content']=file2string($phpfm['full path']);
		//**
		//пре-обработка
		//**
		$phpfm['content']=str_replace(array("<?php","<?","?>","//**"),"",$phpfm['content']);
		$phpfm['content']=preg_replace("/\/\*(.*)\*\//sU", "\n", $phpfm['content']);
		$phpfm['content']=preg_replace("/(\r?\n)+/s","\n",$phpfm['content']);
		//**
		$phpfm['part']=explode(";",$phpfm['content']);
		
		foreach($phpfm['part'] as $key=>$value)
		{
			$phpfm['part'][$key]=explode("\n",$value);
			//**
			foreach($phpfm['part'][$key] as $kay=>$velue) if(empty($velue) || trim($velue)=="") unset($phpfm['part'][$key][$kay]);
		}
		//**
		//следующая часть - еще одна обработка, "пересобирание" кусков
		//**
		foreach($phpfm['part'] as $key=>$value)
		{
			foreach($value as $kay=>$velue)
			{
				$velue=trim($velue);
				//**
				if(trim(substr($velue,0,2))=="//")
				{
					if(!isset($phpfm['part'][$key-1]['desc'])) $phpfm['part'][$key-1]['desc']=trim(substr($velue,2,strlen($velue)));
					else $phpfm['part'][$key-1]['desc'].="<br>".trim(substr($velue,2,strlen($velue)));
				}else if(trim(substr($velue,0,1))=="#") #такие комментарии тоже встречаются
				{
					if(!isset($phpfm['part'][$key-1]['desc'])) $phpfm['part'][$key-1]['desc']=trim(substr($velue,1,strlen($velue)));
					else $phpfm['part'][$key-1]['desc'].="<br>".trim(substr($velue,1,strlen($velue)));
				}else
				{
					if(!isset($phpfm['part'][$key]['exp'])) $phpfm['part'][$key]['exp']=$velue;
					else $phpfm['part'][$key]['exp'].="\n".$velue;
				}
				//**
				unset($phpfm['part'][$key][$kay]);
			}
		}
		//**
		//вроде бы данные приведены к какому-то порядку
		//**
		?>
		</tr>
<?		
		if(isset($phpfm['part'][-1]['desc']))
		{
			?>
			<tr>
			<td colspan=2 style='text-align:center'><b><?=$phpfm['part'][-1]['desc']?></b></td>
			<input type=hidden name='desc' value='<?=str_replace("'","char_apostrof",$phpfm['part'][-1]['desc'])?>'>
			</tr>
<?			unset($phpfm['part'][-1]);
		}
		//**
		$phpfm['scounter']=0;
		//**
		foreach($phpfm['part'] as $key=>$value)
		{
			if(isset($value['desc']) && !isset($value['exp']))
			{
				?>
				<tr>
				<td colspan=2 style='text-align:center'><b><?=$value['desc']?></b></td>
				</tr>
<?				continue;
			}else if(isset($value['exp']) && !isset($value['desc']))
			{
				$phpfm['spart']=explode("=",$value['exp'],2);
				//**
				$value['desc']=trim($phpfm['spart'][0]);
			}else if(!isset($value['exp']) && !isset($value['desc']))
			{
				continue; //то есть если массив пустой
			}else
			{
				$phpfm['spart']=explode("=",$value['exp'],2);
			}
			//**
			$phpfm['spart'][0]=trim($phpfm['spart'][0]);
			$phpfm['spart'][1]=trim($phpfm['spart'][1]);
			//**
			if(substr($phpfm['spart'][1],0,1)=="'" && substr($phpfm['spart'][1],strlen($phpfm['spart'][1])-1,1)=="'")
			{
				//условие выглядит внушительно, хотя на самом-то деле ничего особенного в нем нет :)
				$phpfm['str_type']="string1";
				$phpfm['val']=substr($phpfm['spart'][1],1,strlen($phpfm['spart'][1])-2);
			}else if(substr($phpfm['spart'][1],0,1)=='"' && substr($phpfm['spart'][1],strlen($phpfm['spart'][1])-1,1)=='"')
			{
				$phpfm['str_type']="string2";
				$phpfm['val']=substr($phpfm['spart'][1],1,strlen($phpfm['spart'][1])-2);
			}else if(is_numeric($phpfm['spart'][1]))
			{
				$phpfm['str_type']="integer";
				$phpfm['val']=$phpfm['spart'][1];
			}else if(substr($phpfm['spart'][1],0,5)=="array")
			{
				$phpfm['str_type']="array";
				eval("\$phpfm['val']=".$phpfm['spart'][1].";"); //самый лучший способ загнать массив в привычный вид, это поручить его PHP :)
			}else if(strtolower($phpfm['spart'][1])=="true" or strtolower($phpfm['spart'][1])=="false")
			{
				$phpfm['str_type']="bool";
				$phpfm['val']=(strtolower($phpfm['spart'][1])=="true") ? (true) : (false);
			}else
			{
				$phpfm['str_type']="unknown";
				$phpfm['val']=$phpfm['spart'][1];
			}
			//**
			$phpfm['scounter']++;
			?>
			<tr>
			<td><?=$value['desc']?></td>
			<td style='text-align:right;' width=400>
			<input type=hidden name='<?=$phpfm['scounter']?>' value='name=<?=str_replace(array("'","="),array("char_apostrof","char_ravno"),$phpfm['spart'][0])?>;type=<?=$phpfm['str_type']?>;desc=<?=str_replace("'","char_apostrof",$value['desc'])?>'>
<?			
			switch($phpfm['str_type'])
			{
			case "string1":
				//chr(92) - это слэш (\), просто в строках 2 слэша подряд изобразить довольно проблематично
				?><input type=text name='<?=$phpfm['scounter']?>_input' value='<?=str_replace(array("\'",chr(92).chr(92)),array("&#39;",chr(92)),htmlspecialchars($phpfm['val']))?>' style='width:400'>
<?				break;
			case "string2":
				?><input type=text name='<?=$phpfm['scounter']?>_input' value='<?=str_replace(array(chr(92).'&quot;',chr(92).chr(92),'\$'),array('&quot;',chr(92),'$'),htmlspecialchars($phpfm['val']))?>' style='width:400'>
<?				break;
			case "integer":
				?><input type=text name='<?=$phpfm['scounter']?>_input' value='<?=$phpfm['val']?>' style='width:400'>
<?				break;
			case "bool":
				?><select name='<?=$phpfm['scounter']?>_input' style='width:400'><option value='yes'<?if($phpfm['val']==true) echo " selected";?>>да</option><option value='no'<?if($phpfm['val']==false) echo " selected";?>>нет</option></select>
<?				break;
			case "array":
				//массив - отдельная песня...
				//**
				$phpfm['sscounter']=0;
				//**
				foreach($phpfm['val'] as $kiy=>$vylue)
				{
					$phpfm['sscounter']++;
					?><input type=text name='<?=$phpfm['scounter']?>_input[name_<?=$phpfm['sscounter']?>]' value='<?=str_replace("'","&#39;",htmlspecialchars($kiy))?>' style='width:100'>&nbsp;=>&nbsp;
					<input type=text name='<?=$phpfm['scounter']?>_input[value_<?=$phpfm['sscounter']?>]' value='<?=str_replace("'","&#39;",htmlspecialchars($vylue))?>' style='width:270'><br>
<?				}
				//**
				$phpfm['sscounter']++;
				?><hr><div align=center style='align:center'><b>добавить элементы</b></div><hr><input type=text name='<?=$phpfm['scounter']?>_input[name_<?=$phpfm['sscounter']?>]' value='' style='width:100'>&nbsp;=>&nbsp;
					<input type=text name='<?=$phpfm['scounter']?>_input[value_<?=$phpfm['sscounter']?>]' value='' style='width:270'><br><?$phpfm['sscounter']++;?>
					<input type=text name='<?=$phpfm['scounter']?>_input[name_<?=$phpfm['sscounter']?>]' value='' style='width:100'>&nbsp;=>&nbsp;
					<input type=text name='<?=$phpfm['scounter']?>_input[value_<?=$phpfm['sscounter']?>]' value='' style='width:270'><br><?$phpfm['sscounter']++;?>
					<input type=text name='<?=$phpfm['scounter']?>_input[name_<?=$phpfm['sscounter']?>]' value='' style='width:100'>&nbsp;=>&nbsp;
					<input type=text name='<?=$phpfm['scounter']?>_input[value_<?=$phpfm['sscounter']?>]' value='' style='width:270'>
					<input type=hidden name='<?=$phpfm['scounter']?>_counter' value='<?=$phpfm['sscounter']?>'>
<?				break;
			case "unknown":
				?>
				тип переменной неизвестен и в следующий раз записан не будет
<?				break;
				?>
<?			} //switch
?>
			</td>
			</tr>
<?		} //foreach
?>
		</table>
		<div align=center style='align:center'><input type=submit value='применить' class=halfbutton>&nbsp;<input type=reset value='сбросить' class=halfbutton></div>
<?		break;
	case "apply":
		//**
		$phpfm['begin']="<?\n";
		//**
		if(isset($_POST['desc']))
		{
			if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) $_POST['desc']=stripslashes($_POST['desc']);
			$_POST['desc']=str_replace("char_apostrof","'",$_POST['desc']);
			//**
			$phpfm['begin'].="//".$_POST['desc']."\n\n";
			unset($_POST['desc']);
		}
		//**
		//обработка
		//**
		foreach($_POST as $key=>$value)
		{
			if(!is_numeric($key)) //то есть если это не описание, а данные
			{
				if(get_magic_quotes_runtime() || get_magic_quotes_gpc() && !is_array($value)) $value=stripslashes($value);
				$phpfm['other'][$key]=$value;
			}else
			{
				$phpfm['part']=explode(";",$value);
				//**
				foreach($phpfm['part'] as $kay=>$velue)
				{
					$phpfm['part']=explode("=",$velue);
					//**
					if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) $phpfm['part'][1]=stripslashes($phpfm['part'][1]);
					$phpfm['descs'][$key][$phpfm['part'][0]]=str_replace(array("char_apostrof","char_ravno"),array("'","="),$phpfm['part'][1]);
				}
			}
		}
		//**
		$phpfm['middle']="";
		//**
		//исполнительная часть
		//**
		foreach($phpfm['descs'] as $key=>$value)
		{
			$phpfm['content']=$phpfm['other'][$key."_input"];
			//**
			switch($value['type'])
			{
			case "string1":
				$phpfm['middle'].="\n".$value['name']."='".str_replace(array("'",chr(92)),array("\'",chr(92).chr(92)),$phpfm['content'])."'; //".$value['desc'];
				break;
			case "string2":
				$phpfm['middle'].="\n".$value['name'].'="'.str_replace(array('"',chr(92),'$'),array('\"',chr(92).chr(92),'\$'),$phpfm['content']).'"; //'.$value['desc'];
				break;
			case "integer":
				$phpfm['middle'].="\n".$value['name']."=".intval($phpfm['content'])."; //".$value['desc'];
				break;
			case "bool":
				$phpfm['tmpvar']=($phpfm['content']=="yes") ? ("true") : ("false");
				$phpfm['middle'].="\n".$value['name']."=".$phpfm['tmpvar']."; //".$value['desc'];
				break;
			case "array": //а вот и добрались до массива :)
				$phpfm['begin_array']="\n".$value['name']."=array\n(";
				$phpfm['tmpvar']=array();
				//**
				foreach($phpfm['content'] as $kay=>$velue)
				{
					if(substr($kay,0,5)=="name_") $phpfm['tmpvar_name']=$velue;
					else if(substr($kay,0,6)=="value_") $phpfm['tmpvar'][$phpfm['tmpvar_name']]=$velue;
				}
				//**
				foreach($phpfm['tmpvar'] as $kay=>$velue)
				{
					if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) { $kay=stripslashes($kay); $velue=stripslashes($velue); }
					//**
					if(!empty($velue) && trim($velue)!="") $phpfm['begin_array'].="\n\t'".str_replace(array(chr(92),"'"), array(chr(92).chr(92),"\'"),$kay)."'\t=>\t'".str_replace(array("'",chr(92)), array("\'",chr(92).chr(92)),$velue)."',";
				}
				//**
				$phpfm['begin_array'].="\n); //".$value['desc'];
				$phpfm['middle'].=$phpfm['begin_array'];
				//**
				break;
			}
		}
		//**
		$phpfm['end']="\n/* edited by Config Editor (in PhpFM) */\n?>";
		//**
		$phpfm['2write']=$phpfm['begin'].$phpfm['middle'].$phpfm['end'];
		if(writefile($phpfm['full path'], $phpfm['2write'])) echo "Изменения в конфигурацию успешно изменены.<br><a href='".$phpfm['php_self']."?act=edit-conf&subact=open&file=".$phpfm['file']."'>далее</a>";
		else echo "Изменения в конфигурацию не могут быть внесены. Проверьте права файла (должны быть 0666).<br><a href='".$phpfm['php_self']."?act=edit-conf&subact=open&file=".$phpfm['file']."'>вернуться назад</a>";
		break;
	}
	?>
	</body></html>
<?	die();
	exit;
	break;
case "copy":
	if($phpfm['user-space-left'] <= 0)
	{
		echo "<script language=JavaScript>";
		echo "alert('Вы израсходовали лимит дискового пространства.\\nЭта функция недоступна. Удалите лишние файлы и попробуйте еще раз.');";
		echo ";</script>";
		break;
	}
	if(isset($_GET['file'])) $_SESSION['copied_type']="file";
	else if(isset($_GET['dir'])) $_SESSION['copied_type']="dir";
	else $_SESSION['copied_type']='all';
	//**
	$_SESSION['copied_name']=$_GET[$_SESSION['copied_type']];
	//**
	if($_SESSION['copied_type']!='all')
	{
		$_SESSION['copied_full_name']=realpath($_SESSION['path']."/".$_SESSION['copied_name']);
		//даже скопировать файл не дадут :)
		CheckAccess(realpath($_SESSION['path']."/".$_SESSION['copied_name']));
	}else
	{
		$phpfm['parts']=explode(':',$_SESSION['copied_name']);
		$_SESSION['copied_full_name']=array();
		foreach($phpfm['parts'] as $k=>$v)
		{	
			$_SESSION['copied_full_name'][$k]=realpath($_SESSION['path']."/".$v);
			//даже скопировать файл не дадут :)
			CheckAccess(realpath($_SESSION['path']."/".$v));
		}
	}
	//**
	$_SESSION['copied']=true;
	break;
case "cancel_copy":
	unset($_SESSION['copied']);
	break;
case "paste":
	echo "<script language=JavaScript>";
	if($phpfm['user-space-left'] <= 0)
	{
		echo "alert('Вы израсходовали лимит дискового пространства.\\nЭта функция недоступна. Удалите лишние файлы и попробуйте еще раз.');";
		echo ";</script>";
		break;
	}
	//**
	if($_SESSION['copied_type']=="dir")
	{
		CheckAccess(realpath($_SESSION['path']."/".$_SESSION['copied_name']));
		//**
		if(!@copydir($_SESSION['copied_full_name'],$_SESSION['path']."/".$_SESSION['copied_name'])) echo "alert('Скопировать папку не удалось.\\nПроверьте, есть ли права на запись у файлов и папок внутри этой папки.')";
		//ограничение пользователей
		EditDiskSpace(dirsize($_SESSION['path']."/".$_SESSION['copied_name']));
	}else if($_SESSION['copied_type']=='file')
	{
		CheckAccess(realpath($_SESSION['path']."/".$_SESSION['copied_name']));
		//**
		if(@copy($_SESSION['copied_full_name'],$_SESSION['path']."/".$_SESSION['copied_name'])) echo "alert('Не удалось скопировать файл: ".$php_errormsg."')";
		//ограничение пользователей
		EditDiskSpace(filesize($_SESSION['path']."/".$_SESSION['copied_name']));
	}else
	{
		$phpfm['parts']=explode(':',$_SESSION['copied_name']);
		$phpfm['success']=true;
		foreach($phpfm['parts'] as $k=>$v)
		{
			if(empty($v)) continue;
			//echo 'alert("'.$v.'");';
			CheckAccess(realpath($_SESSION['path']."/".$v));
			//**
			if(copyall($_SESSION['copied_full_name'][$k],$_SESSION['path']."/".$v) && $phpfm['success']); 
			else $phpfm['success']=false;//ограничение пользователей
			EditDiskSpace(unisize($_SESSION['path']."/".$v,false,false,true));
		}
		if(!$phpfm['success']) echo "alert('Скопировать не удалось.\\nВозможно, элементы, в которую вы вставляете папку, не имеет прав на запись.\\nТакже могут возникнуть проблемы с PHP, работающим в режиме safe mode, но обычно таких проблем не возникает')";
	}
	//**
	unset($_SESSION["copied"]);
	//**
	echo ";</script>";
	break;
case "formenu":
	switch($_GET['subact'])
	{
	case "mkdir":
		$_GET['dir']=str_replace(array('..',':','\\','/'),'',$_GET['dir']);
		$phpfm['tomk']=$_SESSION['path']."/".$_GET['dir'];
		//CheckAccess($_SESSION['path']);
		//это уже проверяется в начале
		//**
		if(@mkdir($phpfm['tomk'],0777)) { @chmod($phpfm['tomk'],0777);  }
		else { echo "<script language=javascript>alert('"; echo "Создать папку ".$phpfm['tomk']." не удалось: ".$php_errormsg; echo "');</script>"; }
		//**
		break;
	case "mkfile":
		$_GET['file']=str_replace(array('..',':','\\','/'),'',$_GET['file']);
		$phpfm['tomk']=($_SESSION['path']."/".$_GET['file']);
		//CheckAccess($_SESSION['path']);
		//**
		if(!@mkfile($phpfm['tomk'])) { echo "<script language=javascript>alert('"; echo "Создать файл не удалось: ".$php_errormsg; echo "');</script>"; }
		break;
	case "chmod":
		$phpfm['content']=$_GET['content'];
		$phpfm['mod']=intval($_GET['t_total']);
		//**
		$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['content']);
		//**
		//даже chmod запрещен для не тех директорий...
		//**
		CheckAccess($phpfm['full path']);
		//**
		echo "<script language=javascript>alert('";
		//**
		if($_GET['type']=="dir")
		{
			if(@chmoddir($phpfm['full path'], $phpfm['mod'])) echo "Права \'".$phpfm['content']."\' успешно изменены";
			else echo "Права папки \'".$phpfm['content']."\' не могут быть изменены. Возможно, Windows-система, или хостинг по каким-то причинам запретил chmod.";
		}else
		{
			if(@chmod($phpfm['full path'], $phpfm['mod'])) echo "Права \'".$phpfm['content']."\' успешно изменены";
			else echo "Права файла сменить не удалось: ".$php_errormsg;
		}
		//**
		echo "');window.close();</script>";
		break;
	}
	break;
case "info":
	echo "<html><head></head><body>
	<style type='text/css'>
	<!--
	BODY { background-color: #D1DBF6; color:black; font-family: Tahoma; font-size: 11px; }
	-->
	</style>
	";
	
	if(isset($_GET['file']))
	{
		$_GET['file']=trim(str_replace('&nbsp;','',strip_tags($_GET['file'])));
		//echo '<pre>.'.htmlspecialchars($_GET['file']).'</pre>';
		//**
		$phpfm['fullpath']=realpath($_SESSION['path'].'/'.$_GET['file']);
		CheckAccess($phpfm['fullpath']);
		if(is_file($phpfm['fullpath']))
		{
			$phpfm['pathinfo']=pathinfo($phpfm['fullpath']);
			@$phpfm['ext']=strtolower($phpfm['pathinfo']['extension']);
			switch($phpfm['ext'])
			{
			case "jpeg":
			case "jpg":
			case "jpe":
			case "gif":
			case "png":
				$phpfm['im']=getimagesize($phpfm['fullpath']);
				echo '<table width=160 height=120 border=0 cellspacing=0 cellpadding=0 align=center><tr height=120><td style="vertical-align:middle; align:center; text-align:center;" width=160><img src="index.php?act=thumb&file='.$_GET['file'].'&info"></td></tr></table>';
				echo '<br><nobr><b>'.$_GET['file'].'</b></nobr>';
				echo '<br>'.(isset($desc[$phpfm['ext']]) ? $desc[$phpfm['ext']] : 'Файл "'.$phpfm['ext'].'"');
				echo '<br><br>Размеры: '.$phpfm['im'][0].' x '.$phpfm['im'][1];
				echo '<br><br>Размер: '.unisize($phpfm['fullpath']);
				echo '<br><br>Изменен: '.date('d.m.Y г, h:i:s',filemtime($phpfm['fullpath']));
				$h=250;
				break;
			default:
				echo '<nobr><b>'.$_GET['file'].'</b></nobr>';
				echo '<br>'.(isset($desc[$phpfm['ext']]) ? $desc[$phpfm['ext']] : 'Файл "'.$phpfm['ext'].'"');
				echo '<br><br>Изменен: '.date('d.m.Y г, h:i:s',filemtime($phpfm['fullpath']));
				echo '<br><br>Размер: '.unisize($phpfm['fullpath']);
				$h=80;
				break;
			}
		}else
		{
			echo '<nobr><b>'.$_GET['file'].'</b></nobr>';
			echo "<br>Папка с файлами";
			echo '<br><br>Изменена: '.date('d.m.Y г, h:i:s',filemtime($phpfm['fullpath']));
			if($_SESSION['show_dirsize']) echo '<br><br>Размер: '.unisize($phpfm['fullpath']);
			$h=60+($_SESSION['show_dirsize'] ? 20 : 0);
		}
	}else
	{
		echo '<b>PhpFM</b><br>Файловый менеджер';
		if(!empty($_SESSION['logined']))
		{
			echo '<br><br>Свободно: '.unisize('','',disk_free_space($_SESSION['path']));
			echo '<br><br>Полный объем: '.unisize('','',disk_total_space($_SESSION['path']));
		}
		$h=80;
	}
	$timer->stop();
	echo "\n\n<!-- ".$timer->elapsed()." -->";
	echo '<script language="javascript"><!--
	parent.document.getElementById("info").height='.$h.';
	//-->
	</script>';
	echo "\n\n</body></html>";
	die();
	break;
case "search":
	if(isset($_POST['string']) && (get_magic_quotes_runtime() || get_magic_quotes_gpc())) $_POST['string']=stripslashes($_POST['string']);
	?>
	<html>
	<head>
	<title> PhpFM - поиск </title>
	<style type='text/css'>
	<!--
	BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; }
	TD { text-align:left; vertical-align:top; }
	.comment { font-family: Tahoma; font-size: 8pt; color:black }
	A.black2 {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt; }
	#multitext { background-color: #F7F6F3; color:#000000; font-family:Courier new, Verdana, Arial; font-size:8pt }
	.halfbutton {border-style : outset;width : 150px; height:20 px;}
	input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
	A {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt; cursor: default; border: 0px solid #000000;}
	A:hover {COLOR: gray;}
	-->
	</style>
	</head>
	<body>
	<h3 align=center style='align:center'> PhpFM - поиск файлов и папок в текущей директории </h3>
	<form action='index.php?act=search&apply' method=post>
	<table width=500 border=0 cellpadding=3>
	<tr>
	<td width=150>Искать в:</td>
	<td width=350><select name='by' class=halfbutton><option value=1<?if(isset($_POST['by']) && $_POST['by']==1) echo " selected";?>>Файлах и папках</option><option value=2<?if(isset($_POST['by']) && $_POST['by']==2) echo " selected";?>>Папках</option><option value=3<?if(isset($_POST['by']) && $_POST['by']==3) echo " selected";?>>Файлах</option></select></td>
	</tr>
	<tr>
	<td width=150>Регулярные выражения (PRCE):</td>
	<td width=350><select name='regs' class=halfbutton><option value='0'<?if(isset($_POST['regs']) && $_POST['regs']==0) echo " selected";?>>нет</option><option value=1<?if(isset($_POST['regs']) && $_POST['regs']==1) echo " selected";?>>да</option></select></td>
	</tr>
	<tr>
	<td width=500 colspan=2><input type=checkbox name='recursive' id='recursive' value=1<?if(isset($_GET['apply']) && isset($_POST['recursive'])) echo " checked";elseif(!isset($_GET['apply'])) echo " checked";?>> <label for=recursive>Искать во вложенных папках</label></td>
	</tr>
	<tr>
	<td width=150>Часть имени файла или имя файла целиком:</td>
	<td width=350><input type=text name='string' class=halfbutton value='<?if(!isset($_POST['string'])) echo "поиск...' onclick='this.value=\"\";";else echo str_replace("'","&#39;",$_POST['string']);?>'></td>
	</tr>
	<tr>
	<td colspan=2 width=500 style='text-align:center;'><div align=center><input type=submit value='искать' class=halfbutton>&nbsp;<input type=reset value='сбросить' class=halfbutton></div></td>
	</tr>
	</table>
	</form>
	</body></html>
<?	if(isset($_GET['apply']))
	{
		?>
		<table border=0 width=491 cellspacing=0 cellpadding=0>
<?
		$phpfm['result']=search_file($_SESSION['path'],$_POST['string'],intval($_POST['by']),(isset($_POST['recursive']) ? true : false),intval($_POST['regs']));
		//**
		foreach($phpfm['result']['dirs'] as $value)
		{
			$phpfm['value1']=($_POST['regs']==1 ? preg_replace("/".$_POST['string']."/is","<b>\</b>",basename($value)) : str_replace($_POST['string'],"<b>".$_POST['string']."</b>",basename($value)));
			?>
			<tr>
			<td width=323 valign=top><a class=black2 href='index.php?absolute_dir=<?=rawurlencode($value)?>&act=open' title='Открыть эту папку' target=_blank><img src='images/folder.png' alt='Папка' width=16 height=16 border=0>&nbsp;<?=$phpfm['value1']?></a></td>
			<td width=69 valign=top class=comment2 style='text-align:right;'>&nbsp;</td>
			<td width=99 valign=top class=comment2>&nbsp;Папка</td>
			</tr>
<?	
		}
		foreach($phpfm['result']['files'] as $value)
		{
			$phpfm['value1']=($_POST['regs']==1 ? preg_replace("/".$_POST['string']."/is","<b>\</b>",basename($value)) : str_replace($_POST['string'],"<b>".$_POST['string']."</b>",basename($value)));
			$phpfm['pathinfo']=pathinfo($value);
			@$phpfm['ext']=$phpfm['pathinfo']['extension'];
			?>
			<tr>
			<td width=323 valign=top><a class=black2 href='index.php?absolute_dir=<?=rawurlencode(dirname($value))?>&act=open' title='Открыть папку, содержащую данный файл' target=_blank><img src='images/file-<?=$phpfm['ext']?>.png' alt='Файл' width=16 height=16 border=0>&nbsp;<?=$phpfm['value1']?></a></td>
			<td width=69 valign=top class=comment2 style='text-align:right;'><?=round(filesize($value)/1024,1)."&nbsp;Кб&nbsp;&nbsp;"?></td>
			<td width=99 valign=top class=comment2>&nbsp;Файл</td>
			</tr>
<?	
		}
		?>
		</table>
		<div align=center>[поиск занял <?$timer->stop(); echo round($timer->elapsed(),5)." сек";?>]
<?	}		
	die();
	exit;
	break;
case "additional":
	if(isset($_GET['subact'])) $phpfm['subact']=$_GET['subact'];
	else $phpfm['subact']=false;
	?>
	<html>
	<head>
	<title>Дополнительные возможности PhpFM (их довольно много)</title>
	<style type='text/css'>
	<!--
	BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; }
	TD { text-align:left; vertical-align:top; }
	.comment2 { font-family: Verdana; font-size: 7pt; color:black }
	#multitext { background-color: #FFFFFF; color:#000000; font-family:Courier new, Verdana, Arial; font-size:8pt }
	.halfbutton {border-style : outset;width : 95px; height:20 px;}
	input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
	-->
	</style>
	</head>
	<body>
	В данном разделе можно выполнить самые разные операции - от создания папки и изменения прав на файл или рекурсивного изменения прав на папку (все файлы внутри папки будут иметь указанные права), до выполнения PHP-кода и декодирования md5 - зашированных паролей.
	<form action='index.php?act=additional&subact=mk' method=post>
	<h3>Создать новый файл или папку</h3>
	<select name='type'><option value='file'>Файл</option><option value='dir'>Папку</option></select>
	<input type=text name='text' value='название' style='width:400'>
	<input type=submit value='создать' class=halfbutton>
	<br>При попытке создания существующего файла, его содержимое стирается без предупреждения !
	<?//**
	if($phpfm['subact']=="mk")
	{
		CheckAccess(realpath($_SESSION['path']."/".$_POST['text']));
		echo "<hr>";
		switch($_POST['type'])
		{
		case "file":
			if(mkfile(realpath($_SESSION['path']."/".$_POST['text']))) echo "Файл <b>".$_POST['text']."</b> успешно создан";
			else echo "Файл не может быть создан. Проверьте права на запись в папку <b>".$_SESSION['path']."</b>";
			break;
		case "dir":
			if(@mkdir(realpath($_SESSION['path']."/".$_POST['text']),0777)) { @chmod($_SESSION['path']."/".$_POST['text'],0777); echo "Папка <b>".$_POST['text']."</b> успешно создана"; }
			else echo "Создать папку не удалось: ".$php_errormsg;
			break;
		}
		echo "<hr>";
	}?>
	</form>
	<form action='index.php?act=additional&subact=chmod' method=post>
	<h3>CHMOD (сменить права)</h3>
	<select name='type'><option value='file'>Просто</option><option value='dir' style='background-color:yellow;'>Рекурсивно</option></select>
	<input type=text name='text' value='название' style='width:300'>
	<input type=text name='mod' value='права' style='width:100'>
	<input type=submit value='chmod' class=halfbutton>
	<br>Для смены прав файлов и папок не нужен доступ через shell.
	<br><b>Рекурсивный метод</b> - данные права получает не только папка, которой вы хотите сменить права, но и вложенные в неё файлы и подпапки.
	<?//**
	if($phpfm['subact']=="chmod")
	{
		echo "<hr>";
		$phpfm['content']=$_POST['text'];
		$phpfm['mod']=intval($_POST['mod']);
		//**
		$phpfm['full path']=realpath($_SESSION['path']."/".$phpfm['content']);
		//**
		//даже chmod запрещен для не тех директорий...
		//**
		CheckAccess($phpfm['full path']);
		//**
		if($_POST['type']=="dir")
		{
			if(@chmoddir($phpfm['full path'], $phpfm['mod'])) echo "<br><br>Права <b>".$phpfm['content']."</b> успешно изменены";
			else echo "<br><br>Права папки <b>".$phpfm['content']."</b> не могут быть изменены. Возможно, вы попытались сменить права файлу, или указанная вами папка не существует. Также PHP может работать в таком режиме, когда chmod недоступен. Попробуйте исползовать обычный метод вместо рекурсивного.";
		}else
		{
			if(@chmod($phpfm['full path'], $phpfm['mod'])) echo "<br><br>Права <b>".$phpfm['content']."</b> успешно изменены";
			else echo "<br><br>Права файла сменить не удалось: ".$php_errormsg;
		}
		echo "<hr>";
	}?>
	</form>
	<form action='index.php?act=additional&subact=upload' method=post enctype='multipart/form-data'>
	<h3>Закачать файлы на сервер</h3>
	<input type=file name='files[]' style='width:300'><input type=file name='files[]' style='width:300'>
	<br><input type=file name='files[]' style='width:300'><input type=file name='files[]' style='width:300'>
	<br><input type=file name='files[]' style='width:300'><input type=file name='files[]' style='width:300'>
	<br><input type=checkbox name='folder' value='yes' id='folder'> <label for="folder">Закачать как папку (читайте <b>readme.txt</b> для подробностей)</label>
	<br><br>Файлы, которые вы закачаете, не должны превышать <b><?=ini_get('upload_max_filesize')?></b>. Все файлы перезаписываются без разрешения !!! 
	<br><input type=submit value='закачать' class=halfbutton<?if($phpfm['user-space-left'] <= 0) echo " disabled";?>>
	<?//**
	if($phpfm['subact']=="upload" && $phpfm['user-space-left'] >= 0)
	{
		echo "<hr>";
		if(!isset($_POST['folder']))
		{
			foreach($_FILES['files']['name'] as $i=>$value)
			{
				$value=str_replace(array('..',':','\\','/'),'',$value);
				$phpfm['file'][$i]=$_SESSION['path']."/".basename($value);
				@CheckByFilter(file2string($_FILES['files']['tmp_name'][$i]));
				if(!@move_uploaded_file($_FILES['files']['tmp_name'][$i],$phpfm['file'][$i])) $phpfm['failed']=$i;
				//**
				//можно закачать стока всего...
				//**
				@$phpfm['length']+=strlen(file2string($phpfm['file'][$i]));
			}
			EditDiskSpace($phpfm['length']);
			//**
			if(isset($phpfm['failed']) && !file_exists($phpfm['file'][$phpfm['failed']])) echo "<br><br>Файл закачать не удалось: ".$php_errormsg;
			else echo "<br><br>Все файлы успешно закачаны на сервер";
		}else
		{
			$phpfm['file']=$phpfm['rootpath']."/tempfile.dat";
			$phpfm['tempfile']=$_FILES['files']['tmp_name'][0];
			//**
			if(!file_exists($phpfm['tempfile'])) echo "<br><br>Извините, но файл закачать не удалось. Попробуйте заново и убедитесь, что фрагмент не превышает максимальный размер закачиваемого файла (<b>".ini_get('upload_max_filesize')."</b>), и у вас есть права на запись в папку с PhpFM. Также не забудьте, что фрагмент должен быть в первом поле и только один.";
			//**
			if(!file_exists($phpfm['file'])) mkfile($phpfm['file']);
			if(!@writefile($phpfm['file'],file2string($phpfm['file']).file2string($phpfm['tempfile']))) echo "<br><br>Не удалось произвести запись в файл <b>".$phpfm['tempfile']."</b>. Возможно, ваш хостинг не позволяет создавать файлы большого размера, или у вас нет прав на запись этот файл. В любом случае удалите указанный файл и попробуйте закачать директорию еще раз.";
			else echo "<br><br>Фрагмент успешно закачан. Нажмите на кнопку ниже, если вы закачали последний фрагмент.<br>Не забудьте удалить все фрагменты из вашей папки с PhpFM по окончанию закачки.";
			//**
			echo "</form><form action='".$phpfm['php_self']."?act=additional&subact=upload_dir' method=post><input type=submit value='нажмите сюда, если все фрагменты папки закачаны на сервер'>";
		}
		echo "<hr>";
	}
	//**
	//непосредственно превращение файла в папку с файлами
	//**
	if($phpfm['subact']=="upload_dir"  && $phpfm['user-space-left'] >= 0)
	{
		echo "<hr>";
		$phpfm['file']=realpath($phpfm['rootpath']."/tempfile.dat");
		//**
		$phpfm['part']=explode("\n", file2string($phpfm['file']),2);
		$phpfm['5conf']=$phpfm['part'][0];
		//**
		$phpfm['4conf']=explode(";", $phpfm['5conf']);
		//**
		foreach($phpfm['4conf'] as $key=>$value)
		{
			$phpfm['part1']=explode("=",$value);
			$phpfm['conf'][trim($phpfm['part1'][0])]=trim($phpfm['part1'][1]);
		}
		//**
		if($phpfm['conf']['gzip']==1) $phpfm['part'][1]=trim(gzuncompress(trim($phpfm['part'][1])));
		$phpfm['part']=explode("\n",$phpfm['part'][1]);
		//**
		$phpfm['dir_upload']=$_SESSION['path']."/".$phpfm['conf']['name'];
		//**
		if(!is_dir($phpfm['dir_upload']) && @!mkdir($phpfm['dir_upload'],0777))
		{
			echo "<br><br>Папку <b>".$phpfm['dir_upload']."</b> создать не удалось. Причина: ".$php_errormsg;
		}else
		{
			@chmod($phpfm['dir_upload'], 0777); //для safe mode
			//**
			foreach($phpfm['part'] as $key=>$value)
			{
				$phpfm['part1']=explode(" ",$value,2);
				//**
				if(!empty($phpfm['part1'][0]) && !empty($phpfm['part'][1])) $phpfm['files_upload'][trim($phpfm['part1'][0])]=base64_decode(trim($phpfm['part1'][1]));
			}
			//**
			//непосредственно создание файлов и их наполнение
			//**
			foreach($phpfm['files_upload'] as $key=>$value)
			{
				CheckByFilter($value);
				mkfile(realpath($phpfm['dir_upload']."/".$key));
				writefile(realpath($phpfm['dir_upload']."/".$key),$value);
			}
			//**
			unlink($phpfm['file']); //удаляем временный файл
			echo "<br><br>Процесс закачки папки окончен. Если вы не видите никаких предупреждений от PHP, то папка была закачана успешно. В противном случае проверьте права папки, в которую вы закачиваете эту папку. Если не помогает, попробуйте вручную создать нужную вам папку и поставить ей права 0777 (на разных хостингах по-разному) и попробуйте еще раз.";
		}
		echo "<hr>";
	}
	?>
	</form>
	<form action='index.php?act=additional&subact=eval' method=post>
	<h3>Выполнить PHP-код</h3>
	<input type=checkbox name='alternate' value='yes' id='alternate'> <label for='alternate'>Альтернативный вариант (Необходимо иметь права на запись в папку с PhpFM)</label>
	<br><textarea name='text' id=multitext style='width:600;height:100'><?=((isset($_POST['text']) && $phpfm['subact']=='eval') ? ((get_magic_quotes_runtime() || get_magic_quotes_gpc()) ? (stripslashes($_POST['text'])) : ($_POST['text'])) : ("/* выполнить PHP-код: если стоит обычный режим, то <? и ?> писать не нужно, если выбран альтернативный вариант, наоборот, нужно */\n// phpinfo();"))?></textarea>
	<input type=submit value='выполнить' class=halfbutton<?if($phpfm['user-settings']['eval']!=1) echo " disabled";?>>
	<?//**
	if($phpfm['subact']=="eval" && $phpfm['user-settings']['eval']==1)
	{
		$phpfm['content']=$_POST['text'];
		//**
		if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) $phpfm['content']=stripslashes($phpfm['content']);
		//**
		echo "<hr><pre>";
		//**
		CheckByFilter($phpfm['content']);
		//**
		if(isset($_POST['alternate']))
		{
			mkfile($phpfm['rootpath']."/eval.php");
			writefile($phpfm['rootpath']."/eval.php",$phpfm['content']);
			include($phpfm['rootpath']."/eval.php");
			unlink($phpfm['rootpath']."/eval.php");
		}else
		{
			eval($phpfm['content']);
		}
		echo "</pre><hr>";
	}?>
	</form>
	<form action='index.php?act=additional&subact=shell' method=post>
	<h3>SHELL (команды операционной системы)</h3>
	<br><textarea name='text' id=multitext style='width:600;height:100'><?=((isset($_POST['text']) && $phpfm['subact']=='shell') ? ((get_magic_quotes_runtime() || get_magic_quotes_gpc()) ? (stripslashes($_POST['text'])) : ($_POST['text'])) : ("'команды операционной системы\nls 'в Unix\ndir 'в Windows"))?>
</textarea>
	<input type=submit value='выполнить'<?if($phpfm['user-settings']['eval']!=1) echo " disabled";?>>
	<?//**
	if($phpfm['subact']=="shell" && $phpfm['user-settings']['eval']==1)
	{
		$phpfm['content']=$_POST['text'];
		//**
		if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) $phpfm['content']=stripslashes($phpfm['content']);
		//**
		echo "<hr><pre>";
		exec($phpfm['content'],$phpfm['answer'],$phpfm['return']);
		echo htmlspecialchars(convert_cyr_string(implode("\n",$phpfm['answer']),'d','w'));
		//**
		echo "</pre><hr>";
		if(!$phpfm['return']) echo "Запрос увенчался успехом";
		else echo "Запрос не увенчаля успехом";
	}?>
	</form>
	<form action='index.php?act=additional&subact=convert' method=post>
	<h3>Универсальные конвертации</h3>
	<select name='type'>
	<option>---шифрование---</option>
	<option>&nbsp;</option>
	<option>&nbsp;MD5</option>
	<option value='md5'>&nbsp;&nbsp;-зашифровать</option>
	<option value='de_md5'>&nbsp;&nbsp;-расшифровать (короткий пароль)</option>
	<option>&nbsp;Base64</option>
	<option value='base64'>&nbsp;&nbsp;-зашифровать</option>
	<option value='base64_chunk'>&nbsp;&nbsp;-зашифровать и разбить по строкам</option>
	<option value='base64_decode'>&nbsp;&nbsp;-расшифровать</option>
	<option>&nbsp;</option>
	<option>---время---</option>
	<option>&nbsp;</option>
	<option>&nbsp;Unix time</option>
	<option value='time'>&nbsp;&nbsp;-текущее время</option>
	<option value='2unixtime'>&nbsp;&nbsp;-преобразовать в Unix time</option>
	<option value='fromunixtime'>&nbsp;&nbsp;-преобразовать из Unix time</option>
	<option>&nbsp;Даты</option>
	<option value='dayofweek'>&nbsp;&nbsp;-текущий день недели</option>
	<option value='month'>&nbsp;&nbsp;-текущий месяц ;)</option>
	<option value='year'>&nbsp;&nbsp;-текущий год :D</option>
	<option value='d_m_y'>&nbsp;&nbsp;-дата в формате ДД.ММ.ГГГГ</option>
	<option value='d_m_y_h_i_s'>&nbsp;&nbsp;-дата в формате ДД.ММ.ГГГГ г., ЧЧ:ММ:СС</option>
	<option>&nbsp;</option>
	<option>---русский текст---</option>
	<option>&nbsp;</option>
	<option>&nbsp;Транслит</option>
	<option value='2translit'>&nbsp;&nbsp;-преобразовать текст в транслит</option>
	<option value='fromtranslit'>&nbsp;&nbsp;-преобразовать текст из транслита</option>
	<option>&nbsp;Кодировки</option>
	<option value='koi2win'>&nbsp;&nbsp;-из KOI-8 в WIN</option>
	<option value='win2koi'>&nbsp;&nbsp;-из WIN в KOI-8</option>
	<option value='dos2win'>&nbsp;&nbsp;-из DOS в WIN</option>
	<option value='win2dos'>&nbsp;&nbsp;-из WIN в DOS</option>
	<option value='mac2win'>&nbsp;&nbsp;-из MAC в WIN</option>
	<option value='win2mac'>&nbsp;&nbsp;-из WIN в MAC</option>
	<option value='iso2win'>&nbsp;&nbsp;-из ISO в WIN</option>
	<option value='win2iso'>&nbsp;&nbsp;-из WIN в ISO</option>
	</select>
	<?
	if($phpfm['subact']=="convert")
	{
		@$phpfm['content']=$_POST['text'];
		if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) $phpfm['content']=stripslashes($phpfm['content']);
		//**
	?><br><textarea name='text' id=multitext style='width:600;height:100'><?=$phpfm['content']?></textarea>
<input type=submit value='конвертировать'>
<?		$phpfm['hieroglyph']="<br><br>Если вам нужно раскодировать письмо, пришедшее в непонятной кодировке, или вам нужны более широкие возможности для работы с русским текстом, воспользуйтесь программой &quot;Hieroglyph&quot; - <a href='http://www.adelaida.net/hieroglyph/index.html'>adelaida.net/hieroglyph/index.html</a><pre>";
		//**
		echo "<hr><pre>";
		//**
		switch($_POST['type'])
		{
		case "md5":
			echo md5($phpfm['content']);
			break;
		case "de_md5":
				?></form><form action='demd5.php' method=post target=blank>
В пароле не больше <select name='limit'><option value='1'>1</option><option value='2'>2</option><option value='3'>3</option><option value='4'>4</option><option value='5' style='background-color:red'>5</option></select> символов.
Диапазон поиска пароля - <select name='koef'><option value='26'>латинские буквы в нижнем регистре (hello)</option><option value='52'>латинские буквы в нижнем и верхнем регистре (heLLo)</option><option value='62'>предыдущее + цифры (h3LL0)</option><option value='97'>предыдущее + спецсимволы (h3$L0)</option><option value='163'>предыдущее + русские буквы (Г$%aL)</option></select>

Для более длинных паролей лучше использовать программу <a href='http://forum.dklab.ru/download.php?id=95'>mdcrack</a> (оффициальный сайт - 
<a href='http://mdcrack.df.ru'>mdcrack.df.ru</a>).Может раскодировать пароль из 8 символов за 7 суток непрерывной работы.

Введите пароль, который нужно раскодировать и нажмите "конвертировать"
<br><input type=text name=hash value='пароль'><input type=submit value='конвертировать'>
<?			break;
		case "base64":
			echo base64_encode($phpfm['content']);
			break;
		case "base64_chunk":
			echo chunk_split(base64_encode($phpfm['content']));
			break;
		case "base64_decode":
			echo base64_decode($phpfm['content']);
			break;
		case "time":
			echo time();
			break;
		case "2unixtime":
			echo strtotime($phpfm['content']);
			break;
		case "fromunixtime":
			echo "Несколько вариантов:\n";
			echo date("d.m.Y", $phpfm['content'])."\n";
			echo date("d.m.Y | H:i:s", $phpfm['content'])."\n";
			echo date("d.m.Y г., H:i:s", $phpfm['content'])."\n";
			echo date("Y m d", $phpfm['content'])."\n";
			echo date("d/m/Y", $phpfm['content'])."\n";
			echo date("d F, Y ", $phpfm['content'])."\n";
			echo date("l, d F, Y", $phpfm['content'])."\n";
			echo date("r", $phpfm['content'])."\n";
			break;
		case "dayofweek":
			$phpfm['weeks']=array("Воскресенье","Понедельник","Вторник","Среда","Четверг","Пятница","Суббота");
			echo $phpfm['weeks'][date("w")];
			break;
		case "month":
			$phpfm['months']=array("","Январь","Февраль","Март","Апрель","Май","Июнь","Июль","Август","Сентябрь","Октябрь","Декабрь");
			echo $phpfm['months'][date("n")];
			break;
		case "year":
			echo date("Y");
			break;
		case "d_m_y":
			echo date("d.m.Y");
			break;
		case "d_m_y_h_i_s":
			echo date("d.m.Y г., H:i:s");
			break;
		case "2translit":
			echo "</pre>";
			$phpfm['translit']=array('а'=>'a','б'=>'b','в'=>'v','г'=>'g','д'=>'d','е'=>'e','ё'=>'e','ж'=>'j','з'=>'z','и'=>'i','й'=>'y','к'=>'k','л'=>'l','м'=>'m','н'=>'n','о'=>'o','п'=>'p','р'=>'r','с'=>'s','т'=>'t','у'=>'u','ф'=>'f','х'=>'h','ц'=>'ts','ч'=>'ch','ш'=>'sh','щ'=>'sch','ъ'=>"'",'ы'=>'i','ь'=>"'",'э'=>'е','ю'=>'yu','я'=>'ya','А'=>'A','Б'=>'B','В'=>'V','Г'=>'G','Д'=>'D','Е'=>'E','Ё'=>'E','Ж'=>'J','З'=>'Z','И'=>'I','Й'=>'Y','К'=>'K','Л'=>'L','М'=>'M','Н'=>'N','О'=>'O','П'=>'P','Р'=>'R','С'=>'S','Т'=>'T','У'=>'U','Ф'=>'F','Х'=>'H','Ц'=>'TS','Ч'=>'CH','Ш'=>'SH','Щ'=>'SCH','Ъ'=>"'",'Ы'=>'I','Ь'=>"'",'Э'=>'Е','Ю'=>'YU','Я'=>'YA',);
			echo str_replace(array_keys($phpfm['translit']),array_values($phpfm['translit']),$phpfm['content']);
			echo "<br><br>Если вас не устраивает качество транслитерации, воспользуйтесь программой &quot;Hieroglyph&quot; - <a href='http://www.adelaida.net/hieroglyph/index.html'>adelaida.net/hieroglyph/index.html</a><pre>";
			break;
		case "fromtranslit":
			echo "</pre>";
			$phpfm['untranslit']=array('ja'=>'я','ya'=>'я','yo'=>'ё','oo'=>'у','ch'=>'ч','sch'=>'щ','sh'=>'ш','ts'=>'ц','c'=>'ц','yu'=>'ю','a'=>'а','b'=>'б','v'=>'в','g'=>'г','d'=>'д','e'=>'е','j'=>'ж','z'=>'з','i'=>'и','y'=>'й','k'=>'к','l'=>'л','m'=>'м','n'=>'н','o'=>'o','p'=>'п','r'=>'р','s'=>'с','t'=>'т','u'=>'у','f'=>'ф','h'=>'х',"'"=>'ь','JA'=>'Я','YA'=>'Я','YO'=>'Ё','OO'=>'У','CH'=>'Ч','SCH'=>'Щ','SH'=>'Ш','TS'=>'Ц','C'=>'Ц','YU'=>'Ю','A'=>'А','B'=>'Б','V'=>'В','G'=>'Г','D'=>'Д','E'=>'Е','J'=>'Ж','Z'=>'З','I'=>'И','Y'=>'Й','K'=>'К','L'=>'Л','M'=>'М','N'=>'Н','O'=>'O','P'=>'П','R'=>'Р','S'=>'С','T'=>'Т','U'=>'У','F'=>'Ф','H'=>'Х',);
			echo str_replace(array_keys($phpfm['untranslit']),array_values($phpfm['untranslit']),$phpfm['content']);
			echo "<br><br>Если вас не устраивает качество детранслитерации, воспользуйтесь программой &quot;Hieroglyph&quot; - <a href='http://www.adelaida.net/hieroglyph/index.html'>adelaida.net/hieroglyph/index.html</a><pre>";
			break;
		case "koi2win":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'k','w');
			echo $phpfm['hieroglyph'];
			break;
		case "win2koi":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'w','k');
			echo $phpfm['hieroglyph'];
			break;
		case "dos2win":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'d','w');
			echo $phpfm['hieroglyph'];
			break;
		case "win2dos":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'w','d');
			echo $phpfm['hieroglyph'];
			break;
		case "mac2win":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'m','w');
			echo $phpfm['hieroglyph'];
			break;
		case "win2mac":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'w','m');
			echo $phpfm['hieroglyph'];
			break;
		case "iso2win":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'i','w');
			echo $phpfm['hieroglyph'];
			break;
		case "win2iso":
			echo "</pre>";
			echo convert_cyr_string($phpfm['content'],'w','i');
			echo $phpfm['hieroglyph'];
			break;
		}
		//**
		echo "</pre><hr>";
	}else
	{
		?><br><textarea name='text' id=multitext style='width:600;height:100'></textarea>
<input type=submit value='конвертировать'>
<?	}?>
	</form>
	<form action='index.php?act=additional&subact=prepare2upload' method=post>
	<h3>Подготовка закачки директории</h3>
	<select name=gzip style='width:250'><option value=1>Сжать GZip'ом (крайне желательно)</option><option value=0>Не сжимать GZip'ом</option></select>
	&nbsp;<input type=text name=maxsize value='размер фрагментов'>
	<input type=text name=text value='название папки' style='width:500'>&nbsp;<input type=submit value='подготовить' class=halfbutton>
	<br><b>Размер фрагментов</b> - размер фрагментов папки, получаемых после обработки. Указывается в мегабайтах. <i>Не пишите числа меньше нуля или равные нулю - это не имеет смысла !</i>. По умолчанию - <b><?=ini_get('upload_max_filesize')?></b>
	<?
	if($phpfm['subact']=="prepare2upload")
	{
		@$phpfm['content']=$_POST['text'];
		//**
		if(empty($_POST['maxsize']) or @$_POST['maxsize']<=0 or !is_numeric($_POST['maxsize']))  $phpfm['maxsize']=intval(ini_get('upload_max_filesize'));
		else $phpfm['maxsize']=abs(intval($_POST['maxsize']));
		//**
		if(get_magic_quotes_runtime() || get_magic_quotes_gpc()) $phpfm['content']=stripslashes($phpfm['content']);
		//**
		$phpfm['gzip']=$_POST['gzip'];
		//**
		$phpfm['dircontent']=dircontent(realpath($_SESSION['path']."/".$phpfm['content']),$phpfm['maxsize'],$phpfm['gzip']);
		//**
		$phpfm['files']=0;
		//**
		foreach($phpfm['dircontent'][0] as $key=>$value)
		{
			$phpfm['files']++;
			mkfile($phpfm['rootpath']."/dir_upload".$key.".dat");
			writefile($phpfm['rootpath']."/dir_upload".$key.".dat",$value);
		}
		//**
		echo "<hr>";
		echo "Если вы не видите никаких предупреждений, значит файлы подготовлены. Закачайте ".declesion($phpfm['files'],array("фрагмент","фрагмента","фрагментов"))." с именем <b>dir_upload<i>N</i>.dat</b> на сервер (при закачке укажите &quot;закачать как папку&quot;, закачивайте по одному файлу, и не забудьте указать количество фрагментов), которые лежат в папке с phpFM, и удалите их.";
		echo "<hr>";
	}
	?>
	</form>
	</body>
	</html>
<?	die();
	exit;
	break;
case "administration":
	if(!isset($_SESSION['logined']))
	{
		echo "<script language=javascript>alert('Этот раздел не предназначен для вас.\\nЗапомните, все ваши действия записываются и могут быть просмотрены администратором.');window.close();</script>";
		die();
		exit;
		break;
	}
	?>
	<html>
	<head>
	<title>Администраторская часть PhpFM <?=$phpfm['version']?></title>
	<style type='text/css'>
	<!--
	BODY,table { background-color:#F7F6F3; color:black; font-family: Verdana; font-size: 8pt; text-align:justify; }
	TD { text-align:justify; vertical-align:center; }
	.comment2 { font-family: Verdana; font-size: 7pt; color:black }
	#multitext { background-color: #FFFFFF; color:#000000; font-family:Courier new, Verdana, Arial; font-size:8pt }
	.halfbutton {border-style : outset;width : 95px; height:20 px;}
	input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
	A {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt; cursor: default; display: block; border: 0px solid #000000;}
	A:hover {COLOR: white; TEXT-DECORATION: none; background-color: #00126C;}
	-->
	</style>
	</head>
	<body>
	<h3 align=center style='align:center;'>Добро пожаловать в админку PhpFM</h3>
	<h6 align=center style='align:center;'>вся сессия администратора и этот раздел в частности защищены по-максимуму</h6>
	<!--навигация-->
	<a href='index.php?act=administration&subact=user-control'><b>-&gt;&nbsp;</b>Управление пользователями</a>
	<a href='index.php?act=administration&subact=protection'><b>-&gt;&nbsp;</b>Защита от других файловых менеджеров и потенциально опасных скриптов</a>
	<a href='index.php?act=administration&subact=stats'><b>-&gt;&nbsp;</b>Статистика использования дискового пространства</a>
	<a href='index.php?act=administration&subact=help'><b>-&gt;&nbsp;</b>Справка</a>
	<!--действия-->
	<?
	if(isset($_GET['subact']))
	{
		switch($_GET['subact'])
		{
		case "user-control":
			?>
			<hr>
			<h5>Управление пользователями</h5>
			<table width=600 cellspacing=2 cellpadding=2 border=1>
			<form action='index.php?act=administration&subact=apply' method=post>
			<tr>
			<td width=100><b><i>Логин:</i></b></td>
			<td width=100><b><i>Пароль:</i></b></td>
			<td width=150><b><i>Папка:</i></b></td>
			<td width=150><b class=comment2><i>Использовано места:</i></b></td>
			<td width=100><b><i>Eval, SHELL:</i></b></td>
			</tr>
<?			foreach($phpfm['users'] as $key=>$value)
			{
				$phpfm['settings'][$key]=DumpSettings($key, chr(0));
				$phpfm['settings'][$key]['used']=ceil(file2string($phpfm['rootpath']."/counters/space_".$key)/(1024*1024));
			}
			//**
			foreach($phpfm['settings'] as $key=>$value)
			{
				?>
				<tr>
				<td width=100><input type=text name='name[]' value='<?=$key?>' style='width:90'></td>
				<td width=100><input type=text name='pass[]' value='<?=$value['pass']?>' style='width:90'></td>
				<td width=150><input type=text name='dir[]' value='<?=$value['dir']?>' style='width:140'></td>
				<td width=150><nobr><b><?=$value['used']?> Мб</b> из <input type=text name='quota[]' value='<?=$value['quota']?>' style='width:40'>&nbsp;Мб</b></nobr></td>
				<td width=100><select name='eval[]' style='width:90'><option value='1' style='background-color:green;color:white'<?if($value['eval']==1) echo " selected";?>>Есть</option><option value='0' style='background-color:red;color:white'<?if($value['eval']==0) echo " selected";?>>Нет</option></select></td>
				</tr>
<?			}
			?>
			<tr>
			<td colspan=5><hr></td>
			</tr>
			<tr>
				<td width=100><input type=text name='name[]' value='' style='width:90'></td>
				<td width=100><input type=text name='pass[]' value='' style='width:90'></td>
				<td width=150><input type=text name='dir[]' value='' style='width:140'></td>
				<td width=150><input type=text name='quota[]' value='квота' style='width:140'></td>
				<td width=100><select name='eval[]' style='width:90'><option value='1' style='background-color:green;color:white'>Есть</option><option value='0' style='background-color:red;color:white'>Нет</option></select></td>
			</tr>
			<tr>
			<td colspan=5><input type=submit value='изменить' class=halfbutton>&nbsp;<input type=reset value='сбросить' class=halfbutton></td>
			</tr>
			</form>
			</table>
			<br><i>Чтобы удалить пользователя, сотрите его логин (остальное можно оставить).</i>
			<br><br><i>Чтобы добавить нового пользователя, заполните пустые ячейки (самый последняя строка)</i>
<?			break;
		case "apply":
			?><hr>
<?			$phpfm['2write1']='<?'.		"\n".
			'$phpfm[\'users\'] = array'."\n".
			'('.						"\n";
			$phpfm['2write3']=');'."\n".'?>';
			$phpfm['2write2']="";
			//**
			foreach($_POST['name'] as $key=>$value)
			{
				if(empty($value) || trim($value)=="") continue;
				//**
				$phpfm['2write2'].="\t'".$value."' => 'pass=".$_POST['pass'][$key]."'.chr(0).'dir=".$_POST['dir'][$key]."'.chr(0).'quota=".intval($_POST['quota'][$key])."'.chr(0).'eval=".intval($_POST['eval'][$key])."',\n";
			}
			//**
			$phpfm['2write']=$phpfm['2write1'].$phpfm['2write2'].$phpfm['2write3'];
			$phpfm['file2write']=realpath($phpfm['rootpath']."/accounts.pl");
			//**
			if(@writefile($phpfm['file2write'],$phpfm['2write'])) echo "Настройки пользователей успешно изменены";
			else echo "Настройки пользователей не могут быть изменены. Проверьте права на файл accounts.pl и поставьте 0666";
			//**
			break;
		case "protection":
			?>
			<hr>
			<h5>Защита от других файловых менеджеров и потенциально опасных скриптов</h5>
			<form action='index.php?act=administration&subact=set-protection' method=post>
			Есть несколько степеней защиты (в порядке увеличения безопасности):
			<br><br><b>1)</b> <input type=checkbox name='v1' value='1' id='v1'<?if($phpfm['filter'][0]==1) echo " checked";?>>&nbsp;<label for='v1'>Проверять наличие ключевых слов в описании скрипта и комментариях к нему (желательно)</label>
			<br><b>2)</b> <input type=checkbox name='v2' value='2' id='v2'<?if($phpfm['filter'][1]==1) echo " checked";?>>&nbsp;<label for='v2'>Проверять наличие присущих только файловым менеджерам функций (желательно)</label>
			<br><b>3)</b> <input type=checkbox name='v3' value='3' id='v3'<?if($phpfm['filter'][2]==1) echo " checked";?>>&nbsp;<label for='v3'>Проверять наличие SHELL-функций *</label>
			<br><b>4)</b> <input type=checkbox name='v4' value='4' id='v4'<?if($phpfm['filter'][3]==1) echo " checked";?>>&nbsp;<label for='v4'>Проверять наличие функций исполнения PHP-кода *</label>
			<br><b>5)</b> <input type=checkbox name='v5' value='5' id='v5'<?if($phpfm['filter'][4]==1) echo " checked";?>>&nbsp;<label for='v5'>Проверять наличие любых файловых функций **</label>
			<br><b>6)</b> <input type=checkbox name='v6' value='6' id='v6'<?if($phpfm['filter'][5]==1) echo " checked";?>>&nbsp;<label for='v6'>Запрещать или требовать проверки любых файлов ***</label>
			<br><br>* Функции обращения к системе (SHELL) и выполнения PHP-кода потенциально могут навредить работе системы.
			<br>** Это начинает попахивать маразмом. Каждый второй PHP-скрипт работает с файловой системой. И ничего страшного в этом нет. Если нет другой защиты, то это нормальный вариант...
			<br>*** Неплохая мысль. Для небольшого количества пользователей в принципе сойдет. А когда их станет 50...
			<br><br><input type=submit value='изменить' class=halfbutton> (запрещает закачку или редактирование таких скриптов)
			</form>
<?			break;
		case "set-protection":
			//**
			if(isset($_POST['v1'])) $phpfm['v1']=1;else $phpfm['v1']=0;
			if(isset($_POST['v2'])) $phpfm['v2']=1;else $phpfm['v2']=0;
			if(isset($_POST['v3'])) $phpfm['v3']=1;else $phpfm['v3']=0;
			if(isset($_POST['v4'])) $phpfm['v4']=1;else $phpfm['v4']=0;
			if(isset($_POST['v5'])) $phpfm['v5']=1;else $phpfm['v5']=0;
			if(isset($_POST['v6'])) $phpfm['v6']=1;else $phpfm['v6']=0;
			//**
			$phpfm['2write']="<?\n\$phpfm['filter']=array(".$phpfm['v1'].",".$phpfm['v2'].",".$phpfm['v3'].",".$phpfm['v4'].",".$phpfm['v5'].",".$phpfm['v6']."); //настройки фильтра, самому лучше не редактировать\n?>";
			$phpfm['file']=realpath($phpfm['rootpath']."/config.php");
			if(@writefile($phpfm['file'], $phpfm['2write'])) echo "<hr>Настройки фильтра успешно обновлены";
			else echo "<hr>Настройки фильтра не могут быть обновлены. Проверьте права файла config.php - они должны быть 0666";
			//**
			break;
		case "stats":
			?><hr>
			<h5>Статистика использования дискового пространства</h5>
<?			foreach($phpfm['users'] as $key=>$value)
			{
				$phpfm['settings'][$key]=DumpSettings($key, chr(0));
				$phpfm['settings'][$key]['used']=file2string($phpfm['rootpath']."/counters/space_".$key)/(1024*1024);
			}
			//**
			?>
			<table width=600 cellspacing=0 cellpadding=2 border=0>
			<tr>
			<td width=90><b>Логин:</b></td>
			<td width=510><b>Использование дискового пространства:</b></td>
			</tr>
<?			foreach($phpfm['settings'] as $key=>$value)
			{
				?>
				<tr>
				<td width=90><?=$key?></td>
				<td width=510><img src='images/admin/red.png' width='<?$value['percent']=round(($value['used']/$value['quota'])*100);echo $value['percent']*5;?>' height=10><img src='images/admin/white.png' width='<?=(500-$value['percent']*5)?>' height=10></td>
				</tr>
<?			}?>
			</table>
<?			break;
		case "help":
			?><hr>
			<h5>Справка. Часто задаваемые вопросы</h5>
			1. <b>Управление пользователями</b>
			<br><br>
			<a href='#1'><b>&nbsp;-&gt;&nbsp;</b>Как создать нового пользователя ?</a>
			<a href='#2'><b>&nbsp;-&gt;&nbsp;</b>Как удалить пользователя ?</a>
			<a href='#3'><b>&nbsp;-&gt;&nbsp;</b>Как отредактировать настройки пользователя ?</a>
			<a href='#4'><b>&nbsp;-&gt;&nbsp;</b>Некоторые советы по использованию</a>
			<br>
			2. <b>Защита от файловых менеджеров и потенциально опасных скриптов</b>
			<br><br>
			<a href='#5'><b>&nbsp;-&gt;&nbsp;</b>Что делает этот фильтр ?</a>
			<a href='#6'><b>&nbsp;-&gt;&nbsp;</b>Как работает этот фильтр ?</a>
			<a href='#7'><b>&nbsp;-&gt;&nbsp;</b>Какие фильтры лучше ставить ?</a>
			<a href='#8'><b>&nbsp;-&gt;&nbsp;</b>Какова вероятность ложного срабатывания фильтра ?</a>
			<br>
			3. <b>Статистика использования дискового пространства</b>
			<br><br>
			<a href='#9'><b>&nbsp;-&gt;&nbsp;</b>Как узнать, сколько процентов квоты израсходовал пользователь ?</a>
			<hr>
			<a name=1></a><h6>Как создать нового пользователя</h6>
			В таблице, в самой последней строке после горизонтальной черты есть пустые поля. Заполните их, и после введения изменений новый пользователь создастся автоматически
			<a name=2></a><h6>Как удалить пользователя</h6>
			Сотрите логин пользователя, которого вы хотите удалить. После внесения изменений этот пользователь перестанет сущестсвовать.
			<a name=3></a><h6>Как отредактировать настройки пользователя</h6>
			В таблице есть ячейки. Это поля ввода. Изменяя их, вы изменяете настройки пользователя. Не забудьте после внесения изменений нажать кнопку "изменить" в конце таблицы.
			<a name=4></a><h6>Некоторые советы по использованию</h6>
			<b>1)</b> Не делайте слишком простой пароль для пользователей - их будет легко взломать
			<br><b>2)</b> НИКОГДА НЕ СОЗДАВАЙТЕ ПОЛЬЗОВАТЕЛЯ С ЛОГИНОМ, КАК У ВАС ! (например если ваш логин - "<b>admin</b>", не создавайте пользователя "<b>admin</b>")
			<br><b>3)</b> Структурируйте папки пользователей. Например сделайте папку <b>/home/users/</b> для пользователей, создавайте там папки для пользователей
			<br><b>4)</b> Проследите за тем, чтобы ни один пользователь не имел доступа к папке с PhpFM (иначе он может туда зайти и сделать непоправимое)
			<br><b>5)</b> Особенность дисковой квоты состоит в том, что ограничения наступают только после <b>превышения</b> квоты (то есть если у пользователя заполнено 312 Мб из 300, он может только удалять файлы и папки)
			<br><b>6)</b> Лучше всего запрещать доступ к Eval и SHELL всем пользователям, которые могут вызвать у вас подозрения. Например простая команда "<b>unlink(</b><i>__FILE__</i><b>)</b>" удалит этот файл
			<br><b>7)</b> Обратите внимание на 1 и 2 пункт.
			<hr>
			<a name=5></a><h6>Что делает этот фильтр</h6>
			Этот фильтр проверяет все файлы на наличие потенциально опасного кода (например кода, присущего всем файловым менеджерам), и если при редактировании или при закачке этого файла обнаруживается такой код, файловый менеджер не дает закачать / внести изменения в этот файл.
			<a name=6></a><h6>Как работает этот фильтр</h6>
			Он сначала проверяет, какие фильтры выбраны, а потом по регулярному выражению проверяет наличие определенных функций или комментариев к этому скрипту. Если он находит что-то похожее, он аварийно завершает работу файлового менеджера и выдает соотвествующее сообщение. Тем самым не дает самому себе сохранить данные или продолжить закачку файлов.
			<a name=7></a><h6>Какие фильтры лучше ставить</h6>
			Обычно первых двух фильтров достаточно. Если вы не очень доверяете своим пользователям, можете также включить 3 и 4 фильтр. 5 и 6 режимы включать <b>не рекомендуется</b>, потому что вероятность срабатывания этих фильтров излишне высока. Но зато они обеспечивают большую безопасность.
			<a name=8></a><h6>Какова вероятность ложного срабатывания фильтра</h6>
			В зависимости от степени фильтрации:
			<br><b>1-2</b> - 5-7 %
			<br><b>3-4</b> - > 10 %
			<br><b>5</b> - <b style='color:red'>&gt; 40</b> %
			<br><b>6</b> - <b style='color:red'>100</b> %
			<hr>
			<a name=9></a><h6>Как узнать, сколько процентов квоты израсходовал пользователь</h6>
			Пока что никак. Только визуально. Если очень нужно, нажмите "свойства" красной части полоски и поделите ширину на 5.
<?			break;
		}
	}
	?>
	</body>
	</html>
<?	die();
	exit;
	break;
}
//**
$_SESSION['path']=(isset($phpfm['dir'])) ? (realpath($phpfm['dir'])) : ($_SESSION['path']);
//**
$phpfm['dir']=$_SESSION['path'];
//**
CheckPath();
?>
<html>
<head>
<title> PhpFM <?=$phpfm['version']?> - новое слово в online-файловых менеджерах </title>
<script language=javascript>
<!--
function OpenMenu3(filename,event) {
	closeMenu();
	var el, x, y, login, login2;
	el = document.getElementById("oMenu");
	x = event.clientX + document.documentElement.scrollLeft + document.body.scrollLeft - 3;
	y = event.clientY + document.documentElement.scrollTop + document.body.scrollTop;

	if (event.clientY + 127 > document.body.clientHeight) { y-=131 } else { y-=2 }
	event.returnValue=false;

	el.innerHTML =
	'<a href="index.php?die=.&act=open" class=black title="Обновить текущее окно"  onmouseover="window.status=this.title;return true;" onmouseout="window.status=\'\';return true;" >&nbsp;&nbsp;&nbsp;&nbsp;<b>Обновить</b></a>'+
	'<hr size=1>'+
<? if(isset($_SESSION['copied'])) { ?>	'<a href="index.php?act=paste" class=black title="Вставить <? echo (($_SESSION['copied_type']=="dir") ? ("папку") : ("файл"))." \'".$_SESSION['copied_name']."\'"?>"  onmouseover="window.status=this.title;return true;" onmouseout="window.status=\'\';return true;" >&nbsp;&nbsp;&nbsp;&nbsp;Вставить</a>'+
	'<a href="index.php?act=cancel_copy" class=black title="Отменить копирование"  onmouseover="window.status=this.title;return true;" onmouseout="window.status=\'\';return true;" >&nbsp;&nbsp;&nbsp;&nbsp;Отменить</a>'+<? }
else { ?> '<a class=black style=\'color:gray\'>&nbsp;&nbsp;&nbsp;&nbsp;Вставить</a>'+ <? } ?>
	'<hr size=1>'+
	'<a href="javascript:make_folder();" class=black title="Создать новую папку"  onmouseover="window.status=this.title;return true;" onmouseout="window.status=\'\';return true;" >&nbsp;&nbsp;&nbsp;&nbsp;Создать новую папку</a>'+
	'<a href="javascript:make_file();" class=black title="Создать новый файл"  onmouseover="window.status=this.title;return true;" onmouseout="window.status=\'\';return true;" >&nbsp;&nbsp;&nbsp;&nbsp;Создать новый файл</a>'+
	'<hr size=1>'+
	'<a href="#" onclick="window.open(\'index.php?dir=.&act=properties\',\'properties\',\'width=400,height=480,resizeable=0,menubar=0,location=0,scrollbars=0,toolbar=0,status=0\');return false;" class=black title="Посмотреть свойства этой папки"  onmouseover="window.status=this.title;return true;" onmouseout="window.status=\'\';return true;" >&nbsp;&nbsp;&nbsp;&nbsp;Свойства</a>'+
	'';

	el.style.left = x + "px";
	el.style.top  = y + "px";
	if(el.filters) el.filters["BlendTrans"].Apply();
	el.style.visibility = "visible";
	if(el.filters) el.filters["BlendTrans"].Play();
}
//-->
</script>
<script language=javascript src='main.js' type='text/javascript'></script>
<style type='text/css'>
<!--
BODY { background-color:white; }
TD { text-align:left; vertical-align:top; }
A.black {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt; cursor: default; display: block; border: 0px solid #000000;}
A.menu { color: #215DC6;position:relative;top:-3;left:2; font-family: Tahoma;}
A.black:hover {COLOR: white; TEXT-DECORATION: none; background-color: #00126C;}
A.black2 {COLOR: black; TEXT-DECORATION: none; font-family: Tahoma; font-size: 8pt; cursor: default;}
A.black2:hover {TEXT-DECORATION: none;}
.title { font-family: "Trebuchet MS"; font-size: 13 px; color:white }
.comment { font-family: Verdana; font-size: 8pt; color:black }
.comment2 { font-family: Tahoma; font-size: 8pt; color:black }
.halfbutton {border-style : outset;width : 95px; height:20 px;}
input,select,button {font-size : 8pt; font-family : Verdana, Arial, sans-serif;color : #000000;vertical-align:bottom}
.middle {  vertical-align:middle; text-align:left; align:left; }
.center { vertical-align: middle; text-align: center; align: center; }
-->
</style>
</head>
<body onmousedown='if(!menu_opened) return cm(false,event,"3");' oncontextmenu='return false;' onkeydown='kd(thelast_el,event);' onclick='closeMenu();menu_opened=false;if(bl) uncl(event);'>
<?
if(preg_match('/opera/i',$_SERVER['HTTP_USER_AGENT'])) echo '<script>alert("Дорогой пользователь браузера Opera! Подумайте пожалуйста над тем, чтобы сменить браузер (например Internet Explorer или Mozilla), иначе большинство функций данного файлового менеджера будет для Вас недоступными");</script>';
?>
<div id="oMenu" style='filter: BlendTrans(Duration=0.2); visibility:hidden;position:absolute;border-color: #9D9DA1 #000000 #000000 #9D9DA1;border-style: solid;border-width: 1px;background-color: white; line-height: 16px; width: 150;z-index: 100;'></div>
<table width=740 cellspacing=0 cellpadding=0 border=0 align=center>
<tr>
<form action='index.php' method=get name='menu1'>
<td width=740 height=84 background="images/roof.png" colspan=3 valign=top>
<table width=740 cellspacing=0 cellpadding=0 border=0>
<tr height=5><td height=5><img width=1 height=5 style='visibility:hidden;'></td></tr>
<tr height=25>
<td width=25 height=25>&nbsp;</td><td class=middle width=710><b class=title><?=basename($_SESSION['path'])?></b></td>
</tr>
<tr height=31>
<td colspan=2 height=31 class=middle>&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:history.back(1)'><img src='images/back.png' width=15 height=15 alt='Назад' border=0></a>
&nbsp;&nbsp;<a href='javascript:history.forward(1)'><img src='images/forward.png' width=15 height=15 alt='Вперед' border=0></a>
&nbsp;&nbsp;&nbsp;&nbsp;<a href='index.php?dir=..&act=open'><img src='images/up.png' width=12 height=16 alt='Вверх' border=0></a>
&nbsp;<a href='index.php?dir=.&act=open'><img src='images/reload.png' width=13 height=15 alt='Обновить' border=0></a>
<?
if($_SESSION['drives']!=false)
{
	$phpfm['curdrive']=substr($_SESSION['path'],0,1);
	echo "&nbsp;<select name='absolute_dir' onchange='window.location.href=&quot;".$phpfm['php_self']."?absolute_dir=&quot;+this.value' style='font-weight: bold;' onfocus='typing=true;' onblur='typing=false;'>";
	foreach($_SESSION['drives'] as $value) echo "<option value='".$value.":/'".($phpfm['curdrive']==$value ? " selected" : "").">".strtoupper($value).":</option>";
	echo "</select>";
}?></td>
</tr>
<tr height=21>
<td colspan=2><table width=740 height=21 cellspacing=0 cellpadding=0 border=0><tr height=21><td height=21 width=70>&nbsp;</td><td><input type=text name='absolute_dir' value='<?=$_SESSION['path']?>' style='font-family: Tahoma;width:567;height:21;' onfocus="typing=true;" onblur="typing=false;"><input type=submit value='переход' style='visibility:hidden;display:none'></td></tr></table></td>
</tr>
</table>
</td>
</form>
</tr>
<tr>
<td width=230 valign=top background='images/left_menu.png'>
<br><table width=199 border=0 cellspacing=0 cellpadding=0 class=comment align=center>
<form action='index.php' method=post>
<tr>
<td width=199 height=25 style='vertical-align:middle; background-image: url("images/top_left.png");'>&nbsp;&nbsp;<b style='color: #215DC6;'>Добро пожаловать</b></td>
</tr>
<tr>
<td width=199 style='text-align:justify; background-image: url("images/bkg_left.png");' nowrap=nowrap>
<div style='padding-left:10px;'>
<br>Добро пожаловать, <b><?=$phpfm['user-name']?></b> !
<br>Директория - <b><?=$phpfm['user-settings']['dir']?></b>
<br>Квота - <b><?=$phpfm['user-settings']['quota']?> Мб</b>
<br>Осталось места - <b><?=$phpfm['user-space-left']?> Мб</b>
<br>PHP-код и SHELL - <b><?if($phpfm['user-settings']['eval']==1) echo "Да";else echo "Нет";?></b></div>
<br><img src='images/btm_left.png' width=199 height=1></td>
</tr>
<tr>
<td><br></td>
</tr>
<tr>
<td width=199 height=25 style='vertical-align:middle; background-image: url("images/top_left.png");'>&nbsp;&nbsp;<b style='color: #215DC6;'>Инструкции</b></td>
</tr>
<tr>
<td width=199 style='text-align:justify; background-image: url("images/bkg_left.png");' nowrap=nowrap>
<div style='padding-left:10px;'>
<br><img src='images/menu/howtouse.png' width=16 height=16>&nbsp;<a href='FAQ.html' target=_blank class=menu>Как пользоваться PhpFM</a>
</div>
<br><img src='images/btm_left.png' width=199 height=1></td>
</tr>
<tr>
<td><br></td>
</tr>
<tr>
<td width=199 height=25 style='vertical-align:middle; background-image: url("images/top_left.png");'>&nbsp;&nbsp;<b style='color: #215DC6;'>Настройки</b></td>
</tr>
<tr>
<td width=199 style='text-align:justify; background-image: url("images/bkg_left.png");' nowrap=nowrap>
<div style='padding-left:10px;'>
<br>
<input type=checkbox name='show_dirsize' value='yes' id='show_dirsize'<?if($_SESSION['show_dirsize']==true) echo " checked";?>> <label for='show_dirsize'>Размер папок</label> [<a href='javascript:window_open("help.php?dirsize","help",400,300);' style='font-weight:bold;color:#215DC6;'>?</a>]
<br><input type=checkbox name='thumbnails' value='yes' id='thumbnails'<?if($_SESSION['thumbnails']==true) echo " checked";?>> <label for='thumbnails'>&laquo;Эскизы страниц&raquo;</label> [<a href='javascript:window_open("help.php?thumbnails","help",400,300);' style='font-weight:bold;color:#215DC6;'>?</a>]
<br><input type=checkbox name='show_time' value='yes' id='show_time'<?if($_SESSION['show_time']==true) echo " checked";?>> <label for='show_time'>Показывать статистику</label> [<a href='javascript:window_open("help.php?show_time","help",400,300);' style='font-weight:bold;color:#215DC6;'>?</a>]
<br><input type=checkbox name='use_gzip' id='use_gzip' value='yes'<?if($_SESSION['use_gzip']==true) echo " checked";?>> <label for='use_gzip'>Gzip-сжатие</label> [<a href='javascript:window_open("help.php?use_gzip","help",400,300);' style='font-weight:bold;color:#215DC6;'>?</a>]
<br><br>
<input type=submit class=halfbutton name=submitted value='изменить'>
</div>
<br><img src='images/btm_left.png' width=199 height=1></td>
</tr>
<tr>
<td><br></td>
</tr>
<tr>
<td width=199 height=25 style='vertical-align:middle; background-image: url("images/top_left.png");'>&nbsp;&nbsp;<b style='color: #215DC6;'>Дополнительно</b></td>
</tr>
<tr>
<td width=199 style='text-align:justify; background-image: url("images/bkg_left.png");' nowrap=nowrap>
<div style='padding-left:10px;'>
<br><img src='images/menu/more.png' width=16 height=16>&nbsp;<a href='' onclick='window_open("index.php?act=additional","additional",640,480); return false;' class=menu>Дополнительно</a>
<?if(isset($_SESSION['logined'])) { ?><br><img src='images/menu/admin.png' width=16 height=16>&nbsp;<a href='' onclick='window_open("index.php?act=administration","administration",640,480);return false;' class=menu>Администрирование</a><? } ?>
<br><img src='images/menu/search.png' width=16 height=16>&nbsp;<a href='' onclick='window_open("index.php?act=search","search",640,480); return false;' class=menu>Поиск</a>
</div>
<br><img src='images/btm_left.png' width=199 height=1></td>
</tr>
<tr>
<td><br><br></td>
</tr>
<tr>
<td width=199 height=25 style='vertical-align:middle; background-image: url("images/top_left.png");'>&nbsp;&nbsp;<b style='color: #215DC6;'>Подробно</b></td>
</tr>
<tr>
<td width=199 style='text-align:justify; background-image: url("images/bkg_left.png");' nowrap=nowrap>
<div style='padding-left:10px;'>
<br><iframe src="index.php?act=info" width=180 height=80 border=0 frameborder=0 scrolling='no' marginheight=0 marginwidth=0 id='info' style='background-color: #D1DBF6;'></iframe>
</div>
<br><img src='images/btm_left.png' width=199 height=1></td>
</tr>
<tr>
<td><br></td>
</tr>
</form>
</table>
</td>
<td width=510>
<table border=0 width=510 cellspacing=0 cellpadding=0>
<?
$phpfm['realpath']=realpath($phpfm['dir']);
//**
$phpfm['dirs']=array();
$phpfm['files']=array();
$phpfm['counter']=0;
//**
if(@$phpfm['dh']=opendir($phpfm['dir']))
{
	while(($phpfm['file']=readdir($phpfm['dh']))!==false)
	{
		if($phpfm['file']!="." && $phpfm['file']!="..")
		{
			$phpfm['fullpath']=$phpfm['realpath']."/".$phpfm['file'];
			if(is_dir($phpfm['fullpath'])) $phpfm['dirs'][$phpfm['file']]=$phpfm['fullpath'];
			else  $phpfm['files'][$phpfm['file']]=$phpfm['fullpath'];
			$phpfm['counter']++;
		}
	}
	//**
	if($_SESSION['thumbnails']==false)
	{
		?><tr><td colspan=3><img src='images/types.png' width=491 height=18></td></tr>
<?		
		$i=0;
		foreach($phpfm['dirs'] as $key=>$value)
		{
			$i++;
			?>
			<tr>
			<td width=340 valign=top><a class=black2 id='<?=$i?>' href='' ondblclick='window.location="index.php?dir=<?=rawurlencode($key)?>&act=open"' onclick='return false;' onmousedown='return cm(this,event,"");' onmouseover='bl=false;' onmouseout='bl=true;' title='Папка'><img src='images/folder.png' alt='Папка' width=16 height=16 border=0>&nbsp;<?=$key?></a></td>
			<td width=70 valign=top class=comment2 style='text-align:right;'><?if($_SESSION['show_dirsize']==true) echo unisize($value,'dir');else echo "&nbsp;";?>&nbsp;&nbsp;</td>
			<td width=100 valign=top class=comment2>&nbsp;Папка</td>
			</tr>
<?			
		}
		//**
		foreach($phpfm['files'] as $key=>$value)
		{
			$i++;
			$phpfm['pathinfo']=pathinfo($value);
			if(strlen(@$phpfm['extension']=$phpfm['pathinfo']['extension'])>10) $phpfm['extension']=substr($phpfm['extension'],0,7).'...';
			$phpfm['desc']=(isset($desc[$phpfm['extension']]) ? $desc[$phpfm['extension']] : 'Файл "'.$phpfm['extension'].'"');
			?>
			<tr>
			<td width=340 valign=top><a class=black2 id='<?=$i?>' href='' ondblclick='window_open("index.php?file=<?=rawurlencode($key)?>&act=edit","edit",640,480)' onclick='return false;'  onmousedown='return cm(this,event,"2");' onmouseover='bl=false;' onmouseout='bl=true;' title='Файл'><img src='images/file-<?=$phpfm['extension']?>.png' alt='Файл' width=16 height=16 border=0>&nbsp;<?=$key?></a></td>
			<td width=70 valign=top class=comment2 style='text-align:right;'><?=unisize($value,'file')?>&nbsp;&nbsp;</td>	
			<td width=100 valign=top class=comment2><?=$phpfm['desc']?></td>
			</tr>
<?			
		}
	}else
	{
		
		foreach($phpfm['dirs'] as $key=>$value)
		{
		?>
		<tr>
		<td width=475 valign=top colspan=5><a class=black2 href='' ondblclick='window.location="index.php?dir=<?=rawurlencode($key)?>&act=open"' onclick='return false;' onmousedown='return cm(this,event,"");' onmouseover='bl=false;' onmouseout='bl=true;'  title='Папка'><img src='images/folder.png' alt='Папка' width=16 height=16 border=0>&nbsp;<?=$key?></a></td>
		</tr>
<?		}
		echo "<tr>";
		$phpfm['tr']=0;
		//**
		foreach($phpfm['files'] as $key=>$value)
		{
			if($phpfm['tr']>=5) { echo "</tr><tr>"; $phpfm['tr']=0; }
			//**
			$phpfm['tr']++;
			?>
			<td width=95 class='center'><a class=black2 href='' onclick='return false;' ondblclick='window_open("index.php?file=<?=rawurlencode($key)?>&act=edit","edit",640,480)' onmousedown='return cm(this,event,"2");' onmouseover='bl=false;' onmouseout='bl=true;' title='<?=$key?>'>
			<table width=90 border=1><tr><td class='center' height=67>&nbsp;<img src='index.php?act=thumb&file=<?=rawurlencode($key)?>' border='0'>&nbsp;</td></tr></table>
			<?if(strlen($key)>16) echo substr($key,0,13)."...";else echo $key;?></a>
			</td>
<?		}
	}
}else
{
	?><tr><td>Не удалось открыть папку. <? if(!empty($php_errormsg)) echo 'Причина: '.$php_errormsg; echo '</td></tr>';
}
?></tr>
</table>
</td>
<td width=4 background='images/right_menu.png'>
</td></tr>
<tr>
<td colspan=3 width=740 height=29 background='images/bottom_menu.png'><span class=comment style='position:relative;top:8;left:5'>Объектов: <?=$phpfm['counter']?></span></td>
</tr></table>
<?
if($phpfm['gzip']) $phpfm['gzip_en']="включено";else $phpfm['gzip_en']="выключено";
$timer->stop();if($_SESSION['show_time']==true) echo "<div align=center style='align:center'>[ время генерирования скрипта : ".$timer->elapsed()." сек ] :: [ gzip-сжатие ".$phpfm['gzip_en']." ]</div>";?>
</body>
</html>
