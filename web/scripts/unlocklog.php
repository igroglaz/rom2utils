<?php

error_reporting(0);
#
# 	Скрипт
#   Разблокировки повисших логинов
#


#====================== CONFIG ============================
$CONF['hash']="52ae2e470e19189edbc5dcc3634ef9162f31066b";
$CONF['root']="C:\\Allods2\\Chr";
$CONF['server_root']="D:\\Games\\Allods";
$CONF['timeout']=10*60; //Таймаут нахождения на хэте
#==========================================================
$badlogin=false;
$login = trim($_GET['login']);
if(!eregi("^([A-Za-z0-9_]{1,15})$",$login)){
	$return = "lgn_file_not_exists";
	$badlogin=true;
}

$dir=substr($login,0,1);
$path=$CONF['root'].'/'.$dir[0].'/'.$login.'.lgn';

if($_GET['hash']==$CONF['hash']){
    if((empty($login))||($badlogin)){
		$return ="empty_login";
	} else {

        if(!file_exists($path)){
			$return = "lgn_file_not_exists";
		}else{
			
        	//Проверка заблокирован или нет
			if($_GET['action']=='check'){
                $fp = fopen($path,"r");
    			if($fp){
        			$contentfile=fread($fp,filesize($path));
        			if(ord($contentfile[456])==0){
        				$return = "lgn_file_normal";
        			}else{
        				$return = "lgn_file_crashed";
        			}
        			fclose($fp);
        		}else{
        			$return = "lgn_file_error";
        		}
			}


			if($_GET['action']=='repair'){
                $fp = fopen($path,"r");
                $makerepair=false;
    			if($fp){
        			$contentfile=fread($fp,filesize($path));
        			//echo
        			if(ord($contentfile[456])!=0){
        				$contentfile[456]=chr(0);
        				$makerepair=true;
        			}else{
        				$return = "lgn_file_normal";
        			}
        			fclose($fp);

        			//Проверяем дату изменения файла
        			if($makerepair){
        				clearstatcache();
                        $nowdate = mktime();
                        $filedate = filemtime($path);
                        if(($nowdate-$filedate<$CONF['timeout'])||($filedate==false)){
                            $makerepair=false;
                            $return = "lgn_file_on_hat";
                        }

        			}

        			//Проверяем находится ли персонаж на сервере
        			if($makerepair){
                        //Получаем список серверов
        				if ($handle = opendir($CONF['server_root'])) {
    						while (false !== ($file = readdir($handle))) {
        						if ($file != "." && $file != "..") {
            						if(is_dir($CONF['server_root']."\\".$file)){
            							if(eregi("Server",$file)){
            								$dirs[]=$file;

            							}

            						}
       		 					}

    						}

    						closedir($handle);
						}

						//Пересматриваем все сервера

						for($i=0; $i<count($dirs); $i++){
							if(file_exists($CONF['server_root']."\\".$dirs[$i]."\\chr\\".$login)){
								$return = "lgn_file_on_server";
								$makerepair=false;
							}
						}

        			}

                    if($makerepair){
        				$fp1 = fopen($path,"w");
        				if($fp1){
        					fwrite($fp1,$contentfile);
        					fclose($fp1);
        					$return = "lgn_file_repaired";
        				}
        			} else {
						$return = "lgn_wtf";
					}
					
        		}else{
        			$return = "lgn_file_error";
        		}
			}


		}
	}
}


echo $return;
?>
