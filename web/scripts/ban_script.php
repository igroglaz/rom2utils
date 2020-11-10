<?php
error_reporting(0);

#
#
#	Бан-скрипт (ver 1.0)
#
#

#---------- configuration ----------------#
$CONF['root']="C:\\Allods2\\Chr\\";
$CONF['hash']='9a7817cz8002cnzxX1yP990zaubbIzaE0';
#-----------------------------------------#


function log_ban($action, $hash_sequence, $description) {
	$fp = fopen('logs/ban.log', 'a+');
	$content  = date('Y.m.d H:i:s');
	$content .= "\t" . $action . "\n";
	$content .= 'Совпадение хеш: ' . $hash_sequence . "\n";
	$content .= 'Описание: ' . $description . "\n------------\n";
	fwrite($fp, $content);
}



if($_GET['hash']==$CONF['hash']){
	//авторизировано



	#------------------- Чтение возможного бан-файла ------------------
	if($_GET['action']=='read_banfile'){
		$LOG = $_GET['log_ban'];
		if(!empty($LOG)){
			$str='';
        	$dir=substr($LOG,0,1);
     		$path=$CONF['root'].'/'.$dir[0].'/'.$LOG.".ban";
      		$fp=fopen($path,"r");
      		if($fp){

      				while(!feof($fp)){
      		     		$str.=fgets($fp,1000);

                    }
      		}else{
      			$str="no_banfile";
      		}
      		fclose($fp);
      		echo $str;

		}

	}
   	#------------------------------------------------------------------


   	#---------------- Запись в бан-файл -------------------------------
   	if($_GET['action']=='write_banfile'){
   		$LOG = $_GET['log_ban'];
   		if(!empty($LOG)){
   			$str='';
        	$dir=substr($LOG,0,1);
     		$path=$CONF['root'].'/'.$dir[0].'/'.$LOG.".ban";
     		$fp=fopen($path,"w");
     		if($fp){
     			$_GET['ban_str']=str_replace("_"," ",$_GET['ban_str']);
     			fwrite($fp,$_GET['ban_str']);
     			log_ban('Запись в бан-файл "' . $LOG . '"', 1, 'Успешно записано');
     			echo "ok";
     		}else{
     			log_ban('Запись в бан-файл "' . $LOG . '"', 1, 'Не удалось записать');
     			echo "cannot_write_banfile";
     		}
   		}
   	}
   	#------------------------------------------------------------------

   	#---------------- Удаление в бан-файла ----------------------------
   	if($_GET['action']=='delete_banfile'){
   		$LOG = $_GET['log_ban'];
   		if(!empty($LOG)){
   			$str='';
        	$dir=substr($LOG,0,1);
     		$path=$CONF['root'].'/'.$dir[0].'/'.$LOG.".ban";
     		unlink($path);
     		log_ban('Удаление бан-файла "' . $LOG . '"', 1, 'Успешно удалено');
     		echo "ok";
   		}else{
   			log_ban('Удаление бан-файла "' . $LOG . '"', 1, 'empty_log');
   			echo "empty_log";
   		}
   	}
   	#------------------------------------------------------------------
} else {
	log_ban($_GET['action'] . '- "' . $_GET['log_ban'] . '"', 0, 'Неверный хеш');
	echo "cannot_write_banfile";
}


?>