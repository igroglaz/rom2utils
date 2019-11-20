<?php
#
#
#	Скрипт для разлочивания логинов (lgn)
#
#
#
#
#

#------------ Конфигурация -------------------
$root="C:\Allods2\Chr\\";          // путь к папке Chr
#---------------------------------------------
error_reporting(E_ALL);
$tot='0';
$error='';



if ($FV=@opendir("$root")){     //открываем каталог
while(($nfile=readdir($FV)) !==false){          //берем текущее имя файла
	if ($nfile=="." or $nfile==".."){continue;}  //пропускаем его, если точки
	if (is_dir($root.'\\'.$nfile) !==false) {   //если каталог, то
	//	echo 'DIR ';                         //печатаем имя каталога
     //   echo "$nfile<br>\n";

 if ($FX=@opendir("$root".'\\'.$nfile)){   //открываем его

    		while(($pfile=readdir($FX)) !==false){  //берем текущее имя файла из каталога
				if ($pfile=="." or $pfile==".."){continue;}  //пропускаем его, если точки
				$pathf=$root.'\\'.$nfile.'\\'.$pfile;
				if (is_dir($pathf) !==true){ //если не каталог, то смотрим расширение файла
     				if (stripos($pfile,".lgn") !==false){
    // 					echo "$pfile<br>";   //если расширение lgn то печать

  #------------тело-----------
 $error='';

$path=$pathf;  //путь к логину на диске



if(empty($error)){					//если ошибок нет, то
if(!file_exists($path)){            //если файла не существует
	$error = "lgn_file_not_exists"; // то ошибка
}else{                              //иначе
    $fp = fopen($path,"r");         //открываем файл для чтения
    if($fp){                        //если получилось открыть, то
        $contentfile=fread($fp,filesize($path));  //читаем содержимое файла в бинарном режиме

    	fclose($fp);                //закрываем файл


		if(ord($contentfile[456])<>0){      //если логин заблокирован , то
		$errloc='was locked!';

		$contentfile[456] = chr (0);  //обнуляем флаг залоченности логина (байт со смещением 456=0).

	      $fd = fopen($path,"w");   //открываем файл на запись
      if($fd){
    	fwrite($fd,$contentfile);       //пишем файл
   	fclose($fd);
   	          }else{
  $error = "cannot_open_file";
   }
        					echo "$pfile<br>";   //печать
		 $tot=($tot+1);
		}else{
			$errloc='was unlock';
		}

}
}
}
if(empty($error)){
//	echo "ok";
//	echo ($errloc);
}else{
	echo $error;
   echo 'dddddd';
	die();
}

#----------------------------




     					}
     			}
     		}
		closedir ($FX);
		}else{
		echo 'unable to open cat';
		}


    }
	}
	closedir ($FV);
	}else{
		echo 'unable to open cat';
		}

echo "total $tot";

die();



?>