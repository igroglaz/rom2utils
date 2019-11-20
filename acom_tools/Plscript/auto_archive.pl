sub cur_date { # Возвращает текущую дату
 return sprintf("\[%02d.%02d.%04d %02d:%02d:%02d\] ", sub {($_[3], $_[4]+1, $_[5]+1900, $_[2], $_[1], $_[0])}->(localtime));
}
#Шаблон для имени
sub shablon {
	$Fname = $_[0];
	@fdate = localtime;
	$year = @fdate[5] +1900;
	$mounth = @fdate[4]+1;
	if ($mounth <= 9)
	  {
		$mounth = "0".$mounth;
	  }
	$day = @fdate[3];
	$hour = @fdate[2];
	$min = @fdate[1];
	$sec = @fdate[0];	

	#Изменять только эту строчку!!!!!
	#можно использовать элементы $Fname, $year, $mounth, $day, $hour, $min, $sec
	#в качестве разделителей можно использовать любые строчки
	#между любыми элементами должна стоять точка (.)

	$shablon = $Fname."_".$year.".".$mounth."."."$day"."_".$hour;
	
  return $shablon;
}

#Путь к 7z.exe
$PATH_7Z = '"C:\\Program Files\\7-Zip\\7z.exe"';
#print $PATH_7Z."\n";
#Путь к архивам - должен существовать!!!! если есть дожен оканчиваться на \\
# Например $PATH_TO_OUTPUT="С:\\arhive\\";
$PATH_TO_OUTPUT="d:\\Allods\\Archives\\";

#20 мегабайта в байтах=)	
$M20 = 20971520;

#Имя конфигурационного файла
$config = "d:\\Allods\\Archives\\config.txt";
#Имя буферного файла
$buffer = "C:\\Games\\Allods\\temp\\buffer.log";

open($fconfig, '<'.$config) or die "Ошибка открытия файла $config: $!\n";;  
seek ($fconfig, 0, 0);

$prevpos = tell($fconfig);
@conf = ();
while ($line_config = <$fconfig>)
  {
	#print $line_config;
	if ($line_config =~ m/"(.*)"=(.*)/) 
   	  {     
		$path = $1;
		$pos = $2;
		open($flog,$path) or (warn cur_date()."Unable to open file \"$path\": $!"); 
		seek $flog, 0, 2; # 2 - конец файла
		$size_of_file = tell ($flog);

		if ($pos <= $size_of_file)
		  {
			seek($flog,$pos,0);
		  }
		else
		  {
#			print "Its.work";
			seek($flog,0,0);
		  }
		undef $line_log;
		open ($fbuffer, '>'.$buffer);
		while ($line_log = <$flog>)
  		  {
			print $fbuffer $line_log;
		  }
		$bufsize = tell($fbuffer);
		close $fbuffer;
		$pos = tell($flog);
		@conf = (@conf,"\"$path\"=$pos\n");
		$prevpos = tell($fconfig);;
		seek($flog,0,2);
		$big = 0;
#		print tell($flog)."\n";
#		print $M20."\n";
		if (tell($flog)> $M20)
		  {
#			print "allOk \n";
			$big=1;
		  }
		close $flog;
		if ($big==1)
		  {
			print unlink $path; #удаляем файл если его размер больше 20 мегов
		  }

		if (($path =~ m/(.*)\\(.*)[.](.*)/)&& ($bufsize > 0))
		  {
			$fullpath = $1;
			$filename = $2;
			$filetype = $3;
			@date = localtime;
			@date[4] = @date[4]+1;
			@date[5] = @date[5] +1900;
			if (@date[4] < 10)
			  {
				@date[4] = "0".@date[4];
			  }			
			mkdir $PATH_TO_OUTPUT.$filename;
			mkdir $PATH_TO_OUTPUT.$filename."\\".@date[5];
			mkdir $PATH_TO_OUTPUT.$filename."\\".@date[5]."\\".@date[4];				
			$todir = $PATH_TO_OUTPUT.$filename."\\".@date[5]."\\".@date[4];
			$toname = shablon($filename);
			rename($buffer, $toname.".log");
#			print "$PATH_7Z"." a -t7z buffer.7z ". $toname.".log";
			system("$PATH_7Z"." a -t7z buffer.7z ". $toname.".log -mx9");
			#удаляем файл
 			unlink $toname.".log" if -e $toname.".log";
			rename("buffer.7z", $todir."\\".$toname.".7z");
		}
		
	  }
  }
close $fconfig;
open($fconfig, '>'.$config) or die "Ошибка открытия файла $config: $!\n";;  
foreach my $element (@conf) { # $element это синоним
   print $fconfig "$element";       # очередного элемента $conf[$i]
}


