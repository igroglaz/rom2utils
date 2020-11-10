<?php
        ############################
        #           _/\_           #
	#        .=|    |=.        #
	#       /   \  /   \       #
	#      '     \/     '      #
        #      |     /\     |      #
        #      ,    /  \    ,      #
        #       \  |_  _|  /       #
        #        '=-|--|-='        #
        #                          #
        ############################

	error_reporting(0);
	
	#------------------------------------ Конфигурация ------------------------------------#
	$CONF['root']							= "C:\\Allods2\\Acc";
	$CONF['hash']				= "52ae2e470e19189edbc5dcc3634ef9162f31066b";
	$CONF['char']							= "C:\\Allods2\\chr";
	#--------------------------------------------------------------------------------------#
	
	if($CONF['hash'] == $_GET['hash'])
	{
		$login = $_GET['login'];
		$password = $_GET['password'];
		$error = "";
		
		if(!is_numeric($login))
			die("gm");
		
		if(empty($login) || empty($password))
		{
				die("empty_login_or_password");
		}
		
		$dir = substr($login, 0, 1);
		$path = $CONF['root'].'\\createaccount.'.$login;
		$patha = $CONF['char'].'\\'.$login[0].'\\'.$login.'.lgn';
		$data = $login."\n".$password;
		
		if(file_exists($patha))
		{
			die("lgn_file_exists");
		}
		else
		{
			$fp = fopen($path, "w");
			if($fp)
			{
				fwrite($fp, $data);
				fclose($fp);
			}
			else
			{
				die("cannot_write_file ".$path);
			}
		}
	}
	else
	{
		die("wrong_hash");
	}
	
	echo "ok";
?>