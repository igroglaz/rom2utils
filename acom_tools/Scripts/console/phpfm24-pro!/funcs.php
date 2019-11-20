<?
function dirsize($dir)
{
	if(!is_dir($dir)) return 0;
	$size=0;
	$dh=opendir($dir);
	while ($file=readdir($dh))
	{
		if($file!="." && $file!="..")
		{
			$fullpath=$dir."/".$file;
			if(!is_dir($fullpath))
			{
				$size=$size+@filesize($fullpath);
			}else
			{
				$size=$size+dirsize($fullpath);
			}
		}
	}
	closedir($dh);
	return $size;
}

function removedir($dir)
{
	$dh=opendir($dir);
	while ($file=readdir($dh))
	{
		if($file!="." && $file!="..")
		{
			$fullpath=$dir."/".$file;
			if(!is_dir($fullpath))
			{
				unlink($fullpath);
			}else
			{
				removedir($fullpath);
			}
		}
	}
	closedir($dh);
	if(rmdir($dir))
	{
		return true;
	}else
	{
		return false;
	}
}

function chmoddir($dir)
{
	$dh=opendir($dir);
	while ($file=readdir($dh))
	{
		if($file!="." && $file!="..")
		{
			$fullpath=$dir."/".$file;
			if(!is_dir($fullpath))
			{
				chmod($fullpath);
			}else
			{
				chmoddir($fullpath);
			}
		}
	}
	closedir($dh);
	if(chmod($dir))
	{
		return true;
	}else
	{
		return false;
	}
}

function copydir($dir,$dir2)
{
	global $php_errormsg;
	if(!is_dir($dir2) && !mkdir($dir2,0777)) return false;
	@chmod($dir2,0777);
	$dir=realpath($dir);
	$dir2=realpath($dir2);
	//**
	if(strlen($dir2)>strlen($dir) && substr($dir2,0,strlen($dir))==$dir)
	{
		rmdir($dir2);
		return false; //защита от копирования внутрь себя
	}
	//**
	$dh=opendir($dir);
	while (($file=readdir($dh))!==false)
	{
		if($file!="." && $file!="..")
		{
			$fullpath=$dir."/".$file;
			$fullpath2=$dir2."/".$file;
			if(!is_dir($fullpath))
			{
				copy($fullpath,$fullpath2);
			}else
			{
				copydir($fullpath,$fullpath2);
			}
		}
	}
	closedir($dh);
	if(file_exists($dir2))
	{
		return true;
	}else
	{
		return false;
	}
}

function writefile($file, $content)
{
	if(is_readable($file) && is_writable($file))
	{
		$f=fopen($file,"wb+"); //обязательно wb , просто w портит бинарные файлы.
		fputs($f,$content);
		fclose($f);
		return true;
	}else
	{
		return false;
	}
}

function file2string($file)
{
	global $filecontent;
	if(file_exists($file) && is_readable($file))
	{
		$f=fopen($file,"rb");
		$filecontent=fread($f,filesize($file));
		fclose($f);
		return $filecontent;
	}else
	{
		return false;
	}
}

function mkfile($name, $chmod=0777)
{
	$f=fopen($name,"w");
	fwrite ($f,"",0);
	@chmod($f, $chmod);
	fclose($f);
	if(file_exists($name))
	{
		return true;
	}else
	{
		return false;
	}
}

function declesion($int, $expressions)
{
	settype($int, "integer");
	$count=$int % 100;
	if($count>=5 && $count<=20)
	{
		$result=$int." ".$expressions[2];
	}else
	{
		$count=$count % 10;
		if($count==1)
		{
			$result=$int." ".$expressions[0];
		}elseif($count>=2 && $count<=4)
		{
			$result=$int." ".$expressions[1];
		}else
		{
			$result=$int." ".$expressions[2];
		}
	}
	return $result;
}

function dirstats($dir, $stats=array('files'=>0,'dirs'=>0,'size'=>0))
{
	$dirst=0;
	$filez=0;
	$size=0;
	settype($stats['files'], "integer");
	settype($stats['dirs'], "integer");
	settype($stats['size'], "integer");
	$realpath=realpath($dir);
	if(!$dh=opendir($dir)) return array('files'=>0,'dirs'=>0,'size'=>0);
	while ($file=readdir($dh))
	{
		if($file!="." && $file!="..")
		{
			$fullpath=$realpath."/".$file;
			if(is_dir($fullpath))
			{
				$dirst++;
				$dirs[]=$fullpath;
			}else
			{
				$filez++;
				$size+=filesize($fullpath);
			}
		}
	}
	closedir($dh);
	
	if(isset($dirs))
	{
		foreach($dirs as $key=>$value)
		{
			$stats2=dirstats($value, $stats);
			$dirst+=$stats2['dirs'];
			$filez+=$stats2['files'];
			$size+=$stats2['size'];
		}
	}
	$filez+=$stats['files'];
	$dirst+=$stats['dirs'];
	$size+=$stats['size'];
	return array('files'=>$filez,'dirs'=>$dirst,'size'=>$size,);
}

function dircontent($dir, $size=2, $gzip=true)
{
	if(!@$dh=opendir($dir)) return false;
	//**
	$now=0;
	$return="size=".$size.";gzip=".$gzip.";name=".basename($dir)."\n";
	$return1="";
	//**
	while(($file=readdir($dh))!==false)
	{
		$fullpath=realpath($dir."/".$file);
		//**
		if(!is_dir($fullpath))$return1.=$file." ".base64_encode(file2string($fullpath))."\n";
		else $dirs[]=$file;
	}
	//**
	if($gzip) $return1=gzcompress($return1,9);
	return array(explode_by_size($return.$return1,$size));
}

function explode_by_size($data,$size,$parts=false) //size указывается в мегабайтах
{
	$size*=1024*1024; //все-таки мегабайты...
	if(strlen($data)<=$size)
	{
		return array($data);
	}else
	{
		if(!$parts) $parts=ceil(strlen($data)/$size); //получаем количество фрагментов
		//**
		$counter=0;
		$part=array();
		//**
		while($counter<$parts)
		{
			$counter++;
			//**
			$start=($counter-1)*$size;
			//**
			$part[]=substr($data,$start,$size);
		}
		//**
		return $part;
	}
}

/**
 * @return array
 * @param string $dir
 * @param string $match
 * @param int $type
 * @param bool $recursive
 * @param int $regs
 * @desc Функция ищет файлы в папке $dir по шаблону $match, если $type=1, то поиск производится по файлам и папкам, если 2, то только по папкам, если 3, то только по файлам, если $regs=1, то используются perl-регулярные выражения, если $recursive=true, то поиск ведется еще и по подкаталогам
*/
function search_file($dir,$match,$type,$recursive=false,$regs=0)
{
	$dir=realpath($dir);
	if(!is_dir($dir)) return false;
	//**
	$return=array('files'=>array(),'dirs'=>array());
	@$dh=opendir($dir);
	while((@$file=readdir($dh))!==false)
	{
		if($file!="." && $file!="..")
		{
			$fullpath=$dir."/".$file;
			if(is_dir($fullpath) && ($type==1 || $type==2))
			{
				if($recursive)
				{
					$result=search_file($fullpath,$match,$type,$recursive,$regs);
					foreach($result['dirs'] as $value) $return['dirs'][]=$value;
					foreach($result['files'] as $value) $return['files'][]=$value;
				}
				if($regs!=1 && substr_count($file,$match)>0) $return['dirs'][]=$fullpath;
				else if($regs==1 && preg_match("/".$match."/isU",$file)) $return['dirs'][]=$fullpath;
			}else if($type==1 or $type==3)
			{
				if($regs!=1 && substr_count($file,$match)>0) $return['files'][]=$fullpath;
				else if($regs==1 && preg_match("/".$match."/isU",$file)) $return['files'][]=$fullpath;
			}
		}
	}
	@closedir($dh);
	return $return;
}

//function is from © RemView - http://php.spb.ru/remview/

function display_perms($mode) 
{
//	global $_SERVER;
//	if ($_SERVER['COMSPEC']) return "<i>Windows-система</i>";
	/* Determine Type */
	if( $mode & 0x1000 )
	$type='p'; /* FIFO pipe */
	else if( $mode & 0x2000 )
	$type='c'; /* Character special */
	else if( $mode & 0x4000 )
	$type='d'; /* Directory */
	else if( $mode & 0x6000 )
	$type='b'; /* Block special */
	else if( $mode & 0x8000 )
	$type='-'; /* Regular */
	else if( $mode & 0xA000 )
	$type='l'; /* Symbolic Link */
	else if( $mode & 0xC000 )
	$type='s'; /* Socket */
	else
	$type='u'; /* UNKNOWN */
	
	/* Determine permissions */ 
	$owner["read"] = ($mode & 00400) ? 'r' : '-';
	$owner["write"] = ($mode & 00200) ? 'w' : '-';
	$owner["execute"] = ($mode & 00100) ? 'x' : '-';
	$group["read"] = ($mode & 00040) ? 'r' : '-';
	$group["write"] = ($mode & 00020) ? 'w' : '-';
	$group["execute"] = ($mode & 00010) ? 'x' : '-';
	$world["read"] = ($mode & 00004) ? 'r' : '-';
	$world["write"] = ($mode & 00002) ? 'w' : '-';
	$world["execute"] = ($mode & 00001) ? 'x' : '-';
	
	/* Adjust for SUID, SGID and sticky bit */ 
	if( $mode & 0x800 ) 
	$owner["execute"] = ($owner['execute']=='x') ? 's' : 'S'; 
	if( $mode & 0x400 ) 
	$group["execute"] = ($group['execute']=='x') ? 's' : 'S'; 
	if( $mode & 0x200 ) 
	$world["execute"] = ($world['execute']=='x') ? 't' : 'T'; 
	
	$s=sprintf("%1s", $type); 
	$s.=sprintf("%1s%1s%1s", $owner['read'], $owner['write'], $owner['execute']); 
	$s.=sprintf("%1s%1s%1s", $group['read'], $group['write'], $group['execute']); 
	$s.=sprintf("%1s%1s%1s", $world['read'], $world['write'], $world['execute']); 
	return trim($s);
}

//**
//administration and user-control functions
//**

function DumpSettings($user, $sep, $sep2="=")
{
	global $phpfm;
	//**
	if(!isset($phpfm['users'][$user])) return false;
	//**
	$part=explode($sep, $phpfm['users'][$user]);
	//**
	$return=array();
	//**
	foreach($part as $key=>$value)
	{
		$part=explode($sep2, $value, 2);
		$return[$part[0]]=$part[1];
	}
	//**
	return $return;
}

function UserPassIsValid($user, $pass)
{
	if(!$info=DumpSettings($user, chr(0))) return false;
	//**
	if($info['pass']!=$pass) return false;
	return true;
}

function CheckPath()
{
	CheckAccess($_SESSION['path']);
}

function CreateCounter($login)
{
	global $phpfm;
	//**
	$phpfm['user-settings']=DumpSettings($login, chr(0));
	//**
	$path=realpath($phpfm['user-settings']['dir']);
	//**
	@$dirsize=dirsize($path);
	//**
	@mkfile($phpfm['rootpath']."/counters/space_".$login);
	@writefile($phpfm['rootpath']."/counters/space_".$login, $dirsize);
}

function GetDiskSpace()
{
	global $phpfm;
	//**
	if(isset($_SESSION['logined'])) return 999999; #вообще говоря лимита для администратора формально нету, но это сделано, чтоб не глючило
	//**
	return (file2string($phpfm['rootpath']."/counters/space_".$phpfm['user-name']))/(1024*1024);
}

function EditDiskSpace($size)
{
	global $phpfm;
	//**
	if(isset($_SESSION['logined'])) return false; #администратору ограничения ни к чему
	//**
	@writefile($phpfm['rootpath']."/counters/space_".$phpfm['user-name'], file2string($phpfm['rootpath']."/counters/space_".$phpfm['user-name'])+$size);
}

function CheckAccess($path)
{
	global $phpfm;
	//**
	if(isset($_SESSION['logined'])) return true; #администратор может делать все, что захочется
	//**
	$realpath1=realpath($path);
	$realpath2=realpath($phpfm['user-settings']['dir']);
	$realpath3=realpath($phpfm['rootpath']);
	//**
	if(substr($realpath1,0,strlen($realpath2)) != substr($realpath2,0,strlen($realpath2)) || substr($realpath1,0,strlen($realpath3))==$realpath3)
	{
		echo "<h2 align=center style='align:center;color:red'>доступ к этому файлу или папке запрещен</h2><h3 align=center style='align:center'><a href='".$phpfm['php_self']."'>нажмите сюда чтобы попасть в свою директорию</a></h3>";
		$_SESSION['path']=$realpath2;
		die();
		exit;
	}
}

function CheckByFilter($string)
{
	global $phpfm;
	//**
	if(isset($_SESSION['logined'])) return true; #администратор - то может закачивать все, что угодно :). На то он и администратор
	//**
	$f=$phpfm['filter'];
	$string=preg_replace(array("/\s*\/\*(.*)\*\/\s*/isU","/(\/\/|#)(.*)(\r?\n|\n?\r)/isU"),"",$string); //удаление комментариев
	$string=str_replace(array("\r","\n","\t","'.'",'"."',"'.\"",'".\''),"",$string); //удаление концов строки, и штуковин типа "." и '.' , способные обмануть фильтр
	//**
	if($f[0]==1)
	{
		$reg="/(phpfm|phpremoteview|remoteview|remview|invision power file manager|file manager|file list|file system|file list|файловый менеджер|cистема управления каталогами)/i";
		if(preg_match($reg,$string)) { echo "<h2 align=center style='align:center;color:red'>использование файловых менеджеров запрещено</h2><h3 align=center style='align:center'>для подробностей свяжитесь с системным администратором</h3>"; die(); exit; }
	}
	//**
	if($f[1]==1)
	{
		$reg="/(opendir|readdir|closedir|popen\((\"|')ls|proc_open\((\"|')ls|system\((\"|')ls|exec\((\"|')ls)/i"; //эти функции редко где еще используются
		if(preg_match($reg,$string)) { echo "<h2 align=center style='align:center;color:red'>использование скриптов похожих на файловые менеджеры запрещено</h2><h3 align=center style='align:center'>для подробностей свяжитесь с системным администратором</h3>"; die(); exit; }
	}
	//**
	if($f[2]==1)
	{
		$reg="/(popen|exec|=\s{0,}`|`[a-z]{2}|passthru|proc_open|shell_exec|system\s{0,}\()/i"; //для system нужно смотреть только вызов функции system, а не слово "system", которое может встретиться в любом комментарии. В любом случае лучше такие вещи пропускать. Что же касается обатной кавычки - `, то она по-моему используется довольно редко
		if(preg_match($reg,$string)) { echo "<h2 align=center style='align:center;color:red'>использование команд SHELL запрещено</h2><h3 align=center style='align:center'>для подробностей свяжитесь с системным администратором</h3>"; die(); exit; }
	}
	//**
	if($f[3]==1)
	{
		$reg="/(eval|".preg_quote('$')."([a-z0-9а-я_]+)\s*\(|call_user_func|include\s*\(['\"]http)/i"; // запрещение использование "variable fuinctions", типа $function('index.php');
		if(preg_match($reg,$string)) { echo "<h2 align=center style='align:center;color:red'>использование потенциально опасных конструкций выполнения PHP-кода запрещены</h2><h3 align=center style='align:center'>для подробностей свяжитесь с системным администратором</h3>"; die(); exit; }
	}
	//**
	if($f[4]==1)
	{
		$reg="/(fopen|file\s{0,}\(|file_get_contents|copy|fread|fgets|fwrite|fputs|rename\s{0,}\(|unlink|chmod|parse_ini_file|mkdir|rmdir|chown|chgrp|flock|fpassthru|move_uploaded_file)/i"; //но все же по-моему это идиотизм...
		if(preg_match($reg,$string)) { echo "<h2 align=center style='align:center;color:red'>использование файловых функций запрещено</h2><h3 align=center style='align:center'>для подробностей свяжитесь с системным администратором</h3>"; die(); exit; }
	}
	//**
	if($f[5]==1)
	{
		echo "<h2 align=center style='align:center;color:red'>закачка и редактирование любых файлов запрещена</h2><h3 align=center style='align:center'>для подробностей свяжитесь с системным администратором</h3>"; die(); exit; //вот этого делать никому не советую. По-моему можно обойтись первыми двумя вариантами и поставить еще какую-нибудь защиту на сервер
	}
}

//**
//Misc
//**
function CreateThumb($fullpath,$width=80,$height=60)
{
	Header("Content-type: image/jpeg");
	if(isset($_GET['info']))
	{
		$width=160;
		$height=120;
	}
	//**
	$crc=crc32(file2string($fullpath));
	$cache=dirname($fullpath).'/_phpfm_thumbs/'.basename($fullpath).'.'.$crc.'.'.$width.'.'.$height.'.jpg';
	//**
	if(file_exists($cache))
	{
		readfile($cache);
		return true;
	}
	//**
	$w=getimagesize($fullpath);
	if($w[0]<=$width && $w[1]<=$height)
	{
		readfile($fullpath);
		return true;
	}
	//**
	$pathinfo=pathinfo($fullpath);
	//**
	@$pathinfo['extension']=strtolower($pathinfo['extension']);
	//**
	if($pathinfo['extension']=="jpg" or $pathinfo['extension']=="jpeg" or $pathinfo['extension']=="jpe")
	{
		@$image=imagecreatefromjpeg($fullpath);
	}elseif($pathinfo['extension']=="gif")
	{
		@$image=imagecreatefromgif($fullpath);
	}elseif($pathinfo['extension']=="png")
	{
		@$image=imagecreatefrompng($fullpath);
	}
	//**
	//вычисление пропорций
	//**
	$width2=round(($height/$w[1])*$w[0]);
	$height2=round(($width/$w[0])*$w[1]);
	if($width2>$width) $width2=$width;
	if($height2>$height) $height2=$height;
	//**
	if(function_exists("imagecreatetruecolor"))
	{
		$image2=imagecreatetruecolor($width2, $height2);
	}else
	{
		$image2=imagecreate($width2, $height2);
	}
	
	imagecopyresized($image2, $image, 0, 0, 0, 0, $width2, $height2, $w[0], $w[1]);
	if(!file_exists(dirname($cache)))
	{
		@mkdir(dirname($cache),0777);
		@chmod(dirname($cache),0777);
	}
	imagejpeg($image2,$cache);
	imagedestroy($image);
	imagedestroy($image2);
	
	readfile($cache);
	
	return true;
}
//**
function get_disk_drives()
{
	if(!getenv("COMSPEC")) return false;
	//**
	$disks="cdefghijklmnopqrstuvwxyz"; //in stupid windows diskette always exists but generates an error "please insert diskette" if it isn't inserted
	$len=strlen($disks);
	//**
	$result=array('a');
	//**
	for($i=0;$i<$len;$i++) if(@file_exists($disks[$i].":/")) $result[]=$disks[$i];
	return $result;
}
//**
function unisize($file,$type=false,$sz=false,$onlyint=false)
{
	if($type===false)
	{
		if(is_dir($file)) $sz=dirsize($file);
		else if(is_file($file)) $sz=filesize($file);
		else return '';
	}else if($sz===false)
	{
		if($type=='dir') $sz=dirsize($file);
		else if($type=='file') $sz=filesize($file);
	}
	//**
	if($onlyint) return $sz;
	//**
	if($sz<1024) return $sz.' байт';
	else if($sz<1024*1024) return round($sz/1024,2).' Кб';
	else if($sz<1024*1024*1024) return round($sz/1024/1024,2).' Мб';
	else if($sz<1024*1024*1024*1024) return round($sz/1024/1024/1024,2).' Гб';
	else return 'Больше ТЕРАБАЙТА!!!';
}
//**
function copyall($s,$d)
{
	if(is_dir($s)) return copydir($s,$d);
	else return copy($s,$d);
}
//**
function deleteall($s)
{
	if(is_dir($s)) return removedir($s);
	else return unlink($s);
}
?>