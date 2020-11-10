<?php
#
#
#	Скрипт получения хеша пароля по указанному игровому логину (lgn)
#
#
#
#
#
error_reporting(0);

#------------ Конфигурация -------------------
$CONF['root']="C:\\Allods2\\Chr\\";          // путь к папке Chr
$CONF['hash']="52ae2e470e19189edbc5dcc3634ef9162f31066b";
#---------------------------------------------

if($CONF['hash']==$_GET['hash']){
	$login = $_GET['login'];
	$error = "";

	if(!is_numeric($login))
		die("gm");

	if(empty($login)){
		$error ="empty_login_or_password";
	}


	$path=$CONF['root'].'\\'.$login[0].'\\'.$login.'.lgn';

	if(empty($error)){
		$newpath = "deleted_logins/".$login[0]."/".$login.".".time().".lgn";
		if(!is_dir("deleted_logins")) mkdir("deleted_logins");
		if(!is_dir("deleted_logins/".$login[0])) mkdir("deleted_logins/".$login[0]);
		copy($path, $newpath);

		@unlink($path);
		if(!file_exists($path)) $error='ok';
		else $error='unknown';
	}

}else{
	$error="wrong_hash";

}

echo $error;

?>