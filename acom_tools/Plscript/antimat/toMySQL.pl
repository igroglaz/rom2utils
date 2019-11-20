#!/usr/bin/perl
use Lingua::RU::Antimat;
use pvdlib;
use mysql;
#-----------------------------------------------------------------------------
# Настройка

#логин к MySQL
$login = 'root';

#пароль - при регистрации был выбран 1
$pass = '1';

$host = 'localhost';


# База Данных
$database = 'allods';

$PATH_TO_ALLODS = "";
$PATH_TO_LOGS   = "";

$MIN_LEN_SOOB = 4; #минимальная длина не матного сообщения
$MIN_KOLVO_SOOB = 10;  #< 10 сообщений одна градация, > 10 другая градация
$MIN_GRAN = 50; # <50% - нет изменения рейтинга >50% рейтинг ухудшается
$BALL_DO_10 = 1; #кол-во очков, начисляемых если сообщений было < 10 и много мата
@arrayboard = (-1,15,25,100); #начинается на -1, заканчивается на 100, в порядке возрастания
@arrayball = (-1,0,1); #на 1 элемент меньше чем в предыдущем массиве

#$PATH_TO_ALLODS = "C:\\Games\\Allods\\";
#$PATH_TO_LOGS   = "logs\\";
sub path_to_server {return $PATH_TO_ALLODS.$PATH_TO_LOGS."server".shift(@_).".log";}# $num_of_serv - номер сервера
$first_server = 1; # Номер первого сервера
$last_server = 15; # Номер последнего сервера
#-----------------------------------------------------------------------------

$dbh = Mysql->Connect($host, $database, $login, $pass);

$mat=Lingua::RU::Antimat->new; #Включаем систему антимат

sub cur_date { # Возвращает текущую дату
 return sprintf("\[%02d.%02d.%04d %02d:%02d:%02d\] ", sub {($_[3], $_[4]+1, $_[5]+1900, $_[2], $_[1], $_[0])}->(localtime));
}
@namearray = ();
@player2login = ();

@cur_pos_svr = (); # Текущие позиции в читаемых файлах server
for ($num_of_serv=$first_server; $num_of_serv <=$last_server; $num_of_serv++)
  {
	# Займемся файлом лога сервера
	open(file_srv, path_to_server($num_of_serv)) or (warn cur_date()."Unable to open file \"".path_to_server($num_of_serv)."\": $!"); 
	if  (defined $cur_pos_svr[$num_of_serv]) # Если сервер уже открыт
	  {
		seek file_srv, 0, 2; # 2 - конец файла
		$size_of_file = tell (file_srv);
		if ($cur_pos_svr[$num_of_serv]<=$size_of_file)
		  {
			seek file_srv, $cur_pos_svr[$num_of_serv], 0; # 0 - начало файла
		  }
			# Если вдруг окажется, что файл меньшего размера чем тот, который мы обрабатывали в прошлый раз - открываем файл сначала
		else
		  {
			seek file_srv, 0, 0;
                        undef $line_srv;
		  }
	  }
        else	#Сервер не открыт
	  {
		undef $line_srv;
	  }

	while ($line_srv=<file_srv>)
          {
		if ($line_srv =~ m/(\d{2})\.(\d{2})\.(\d{2}) (\d{2}:\d{2}:\d{2})/)
		  {
			$line_srv_crop = $line_srv;
			$line_srv_crop =~ s/(\d{2})\.(\d{2})\.(\d{2}) (\d{2}:\d{2}:\d{2}) //;

			$time_srv = "$3.$2.$1 $4";		#зачем надо?????
			$normal_time_srv = "$1.$2.20$3 $4";

			if ($line_srv =~ m/New player (.*) has logged in as (\w*)/)
			  {
				$name = $1;
				$login = $2;

				$player2login[$num_of_serv]{"$name"} = $login;
				@namearray = ($name, @namearray);
				$login2player[$num_of_serv]{"$login"} = $name;
				$loginmat[$num_of_serv]{"$login"} = 0;
				$loginall[$num_of_serv]{"$login"} = 0;
				$login_ochki[$num_of_serv]{"$login"} = 0;
			 	$login_otnosh[$num_of_serv]{"$login"} = 0;
			  }
			elsif ($line_srv_crop =~ m/(.*): (.*)/)
			  {
				$name = $1;
				$login = $player2login[$num_of_serv]{"$name"};
				if (defined $login)
				  {     
					$test =encoder($2,"dos","win");
					if ($mat->detect_slang($test))
					  {
						$loginmat[$num_of_serv]{"$login"}++;
						$loginall[$num_of_serv]{"$login"}++;						
					  }
					elsif (length($2)> $MIN_LEN_SOOB)
					  {
						$loginall[$num_of_serv]{"$login"}++;
					  }

				  }
			   }

			elsif ($line_srv =~ m/Player (.*) has left the game/)
			  {
				$name = $1;
				$login = $player2login[$num_of_serv]{"$name"};
				if (defined $login)
				  {     
#					print $loginmat[$num_of_serv]{"$login"};
#					print "/";
#					print $loginall[$num_of_serv]{"$login"};
#					print "   -   ";
#					print $name;
#					print "  =  ";
					
					#Подсчитываем отношение мат/не мат

					if ($loginall[$num_of_serv]{"$login"} != 0)
					  {
	                                        $login_otnosh[$num_of_serv]{"$login"} = int($loginmat[$num_of_serv]{"$login"}/$loginall[$num_of_serv]{"$login"}*100);

					  }
					else
					  {	
						$login_otnosh[$num_of_serv]{"$login"} = 0;
					  };
					#Вычисляем рейтинги
					$login_ochki[$num_of_serv]{"$login"} = 0;
					if ($loginall[$num_of_serv]{"$login"} < $MIN_KOLVO_SOOB)
					  {
						if ($login_otnosh[$num_of_serv]{"$login"} > $MIN_GRAN) 
						  {
							$login_ochki[$num_of_serv]{"$login"} = $BALL_DO_10;
						  }
					  }
					else
					  {
						for($i = 0; $i < $#arrayboard; $i++)
						  {
							if (($login_otnosh[$num_of_serv]{"$login"} > @arrayboard[$i]) && ($login_otnosh[$num_of_serv]{"$login"} <= @arrayboard[$i+1]))
							  {
								$login_ochki[$num_of_serv]{"$login"} = @arrayball[$i];
							  }
						  }						
					  }
#					print $login_otnosh[$num_of_serv]{"$login"};	
#					print "   -   ";
#					print $login_ochki[$num_of_serv]{"$login"};
#					print "\n";

					#  Запрос на добаление в БД

#					print "call InsertDATA(\'".$login."\',".$loginall[$num_of_serv]{"$login"}.",".$loginmat[$num_of_serv]{"$login"}.",".$login_ochki[$num_of_serv]{"$login"}.");";
#					print "\n";
                                        if ($loginall[$num_of_serv]{"$login"} != 0)
					  {
						$sth = $dbh->Query("call InsertDATA(\'".$login."\',".$loginall[$num_of_serv]{"$login"}.",".$loginmat[$num_of_serv]{"$login"}.",".$login_ochki[$num_of_serv]{"$login"}.");") or die $Mysql::db_errstr;
					  }
					delete $player2login[$num_of_serv]{"$name"};
					delete $login2player[$num_of_serv]{"$login"};
				  }
			   }
			elsif ($line_srv =~ m/Player (.*) has disconnected/)
			  {
				$name=$1;
				$login = $player2login[$num_of_serv]{"$name"};
			  }
			elsif ($line_srv =~ m/Player (.*) has joined the game/)
			  {
				$name = $1;
				$login =  $player2login[$num_of_serv]{"$name"};
			  }
		  }
	  }
        $cur_pos_svr[$num_of_serv] = tell (file_srv); # запоминаем текущую позицию
	close file_srv;
	foreach my $element (@namearray) 
	  { # $element �� ᨭ����
		$name = $element;       # ��।���� ������� $namearray[$i]
		$login = $player2login[$num_of_serv]{"$name"};
		if (defined $login)
		  {     			
			#Подсчитываем отношение мат/не мат
	
			if ($loginall[$num_of_serv]{"$login"} != 0)
			  {
		        	$login_otnosh[$num_of_serv]{"$login"} = int($loginmat[$num_of_serv]{"$login"}/$loginall[$num_of_serv]{"$login"}*100);
	                  }
			else
			  {	
				$login_otnosh[$num_of_serv]{"$login"} = 0;
			  };
			#Вычисляем рейтинги
			$login_ochki[$num_of_serv]{"$login"} = 0;
			if ($loginall[$num_of_serv]{"$login"} < $MIN_KOLVO_SOOB)
			  {
				if ($login_otnosh[$num_of_serv]{"$login"} > $MIN_GRAN) 
				  {
					$login_ochki[$num_of_serv]{"$login"} = $BALL_DO_10;
				  }
			  }
			else
			  {
				for($i = 0; $i < $#arrayboard; $i++)
				  {
					if (($login_otnosh[$num_of_serv]{"$login"} > @arrayboard[$i]) && ($login_otnosh[$num_of_serv]{"$login"} <= @arrayboard[$i+1]))
					  {
						$login_ochki[$num_of_serv]{"$login"} = @arrayball[$i];
					  }
				  }						


			  }
			#  Запрос на добаление в БД
	                if ($loginall[$num_of_serv]{"$login"} != 0)
			  {
	#			print $login." ".$loginall[$num_of_serv]{"$login"}." ".$loginmat[$num_of_serv]{"$login"}." ".$login_ochki[$num_of_serv]{"$login"}."\n";
				$sth = $dbh->Query("call InsertDATA(\'".$login."\',".$loginall[$num_of_serv]{"$login"}.",".$loginmat[$num_of_serv]{"$login"}.",".$login_ochki[$num_of_serv]{"$login"}.");") or die $Mysql::db_errstr;
			  }
			delete $player2login[$num_of_serv]{"$name"};
			delete $login2player[$num_of_serv]{"$login"};
		  }
	  }
	@namearray=();

  }
