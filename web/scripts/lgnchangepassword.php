<?php
#
#
#	Скрипт для смены паролей игровых логинов (lgn)
#
#
#
#
#
error_reporting(0);

function log_chpass($login, $hash_sequence, $description) {
	$fp = fopen('logs/chpass.log', 'a+');
	$content  = date('Y.m.d H:i:s');
	$content .= "\t" . $login . "\n";
	$content .= 'Совпадение хеш: ' . $hash_sequence . "\n";
	$content .= $description . "\n------------\n";
	fwrite($fp, $content);
}

#------------ Конфигурация -------------------
$CONF['root']="C:\\Allods2\\Chr\\";          // путь к папке Chr
$CONF['hash']="52ae2e470e19189edbc5dcc3634ef9162f31066b";
#---------------------------------------------

$description = '';

if($CONF['hash']==$_GET['hash']){
	$hash_sequence = 1;
	$login = $_GET['login'];
	$password = $_GET['password'];
	$error = "";

	if(!is_numeric($login))
		die("gm");


	if(empty($login)||empty($password)){
		$error ="empty_login_or_password";
	}

	if(strlen($password)>15){
		$error="password_is_too_long";
	}

	$dir=substr($login,0,1);
	$path=$CONF['root'].'\\'.$dir[0].'\\'.$login.'.lgn';

	if(empty($error)){
		if(!file_exists($path)){
			$error = "lgn_file_not_exists";
		}else{
			$fp = fopen($path,"r");
			if($fp){
				$contentfile=fread($fp,filesize($path));

				fclose($fp);


				//Находим старый пароль
				$content=substr($contentfile,4,15);  //пароль + нолики
				for($i=strlen($password); $i<15; $i++){
					$password.=chr(0);
					$description = sha1(str_replace(chr(0),'',$content)) . '->' . sha1(str_replace(chr(0),'',$password));
				}




				if(ord($contentfile[456])==0){
					$contentfile = substr($contentfile, 0, 4) . $password . substr($contentfile,19);
					$fd = fopen($path,"w");
					if($fd){
						fwrite($fd,$contentfile);
						fclose($fd);
					}
				}else{
					$error = "login_locked";
				}





			}else{
				$error = "cannot_open_file";
			}
		}
	}

}else{
	$error="wrong_hash";
	$hash_sequence = 0;
}

$lpath = $CONF['root'].$login[0]."\\".$login.".ban";
$lban = trim(file_get_contents($lpath));

if($lban == "01.01.2012 00:00:00;-1,chpass")
{
	@unlink($lpath);
}

if(empty($error)){
	log_chpass($login, $hash_sequence, $description);
	echo "ok";


}else{
	echo $error;
	log_chpass($login, $hash_sequence, 'Ошибка: ' . $error);
}

?>