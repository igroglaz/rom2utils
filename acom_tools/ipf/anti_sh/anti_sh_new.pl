#!/usr/bin/perl
#
# Changelog
# 1.16
# *исправлена ошибка, приводящая к закрытию скрипта при определенном кидании карты
# + добавлена возможность регулировать количество UCRM
# 1.15 
# +Сделал вывод в файл, а не на экран
# 1.14
# *исправлены пара багов
# +добавлен способ бана при СХ через таверну
# 1.13
# *поправлены пара багов, связанных со сменой карты
# +добавлен счетчик числа косячных убийств
# 1.12
# + Добавлен бан за быстрые перезаходы
# 1.11
# * Оптимизровал код, пара баг-фиксов
# * Убрал кучу багов, теперь не банит лишних людей
# + Теперь ники читеров отображаются всегда
# 1.10
# +Добавил бан за выход из игры из здания
# +
# 1.09
# +Добавлен бан при смерти СХ.
# 1.08
# *Вернулись на старый способ подтягивание логов сервера
# 1.07
# +Добавлен сброс сервера по ошибке "MsgBox: Shop Error, Amount > 1000", приводящей к повисанию сервера
# 1.06
# +Немного поправлен формат логов
# +Добавлено отлавливание СХ через пакет join the game
# 1.05
# * Исправлен баг, при котором не банились логи СХ, если на карте ничего не происходило
# 1.04
# * Исправлен баг, при котором лог сервера не начинает считываться с начала
# 1.03:
# * НЕ Исправлена ошибка с неподцепляемым логом сервера
# 1.02:
# +Добавлено отображение даты и времени внчалае каждого сообщения
# +Добавлено отображение вывода старта работы скрита и его версии
# *Исправлена ошибка, приводящая к некорректным банам, в случае если не удавалось открыть лог-файл сервера




#
#
# Настройки
#$PATH_TO_ALLODS = "";
#$PATH_TO_LOGS   = "";
#$PATH_TO_OUT = "anti_sh.log";
$PATH_TO_ALLODS = "C:\\Allods2\\";
$PATH_TO_LOGS   = "C:\\Allods2\\logs\\";
$PATH_TO_OUT = "C:\\Allods2\\logs\\anti_sh.log";
sub path_to_ban {$tmp_log=shift(@_); $tmp_log =~ m/^(.)/; return  ">".$PATH_TO_ALLODS."Chr\\$1\\".$tmp_log.".ban\n";} # номер логина, > - открыт на запись  
sub path_to_srvmgr {return $PATH_TO_LOGS."srvmgr".shift(@_).".log";}# $num_of_serv - номер сервера
sub path_to_server {return $PATH_TO_LOGS."srvmgr".shift(@_).".log";}# $num_of_serv - номер сервера
$_START_STRING = -500 * 70; # 500 строк, 80 символов в строке, считаем, что так
$_START_STRING_SRV = 200; # 200 символов
$time_of_sleeping = 15; # Изначально 15 секунд время сна
$first_server = 1; # Номер первого сервера
$last_server = 16; # Номер последнего сервера
$max_enter = 1; # Сколько раз разрешается зайти в здание подряд
$max_exit = 2; # Сколько раз разрешается выйти
$max_join = 2; # Сколько раз разрешается выйти
$max_reenter = 4; # Сколько раз разрешается быстро войти\выйти из магаза
$max_reenter_break = 2; #Сколько можно прервать
$max_UCRM = 2; # На который раз надпись "Unit can't return to map - no free place" считается применением СХ.
# --------------------------------------
#
#
$version = "1.16";
#

## URL Encode\Decode
#sub URLEncode {
#    my $theURL = $_[0];
#    $theURL =~ s/([\W])/”%” . uc(sprint OUT_FILEf(”%2.2x”,ord($1)))/eg;
#    return $theURL;
#}

#sub URLDecode {
#    my $theURL = $_[0];
#    $theURL =~ tr/+/ /;
#    $theURL =~ s/%([a-fA-F0-9]{2,2})/chr(hex($1))/eg;
#    $theURL =~ s/<!–(.|\n)*–>//g;
#    return $theURL;
#}

######
open (OUT_FILE, ">>".$PATH_TO_OUT);

sub change_map # почистить все, что можно в случае смены или слета карты
{
	$temp_hash =\ $player2login[$num_of_serv]; # Почистить список имя - логин
	%{$$temp_hash} = ();
	$temp_hash =\ $login2player[$num_of_serv]; # Почистить список логин - имя
	%{$$temp_hash} = ();
	$temp_hash =\ $players_build[$num_of_serv]; # Почистить список логин - здание
	%{$$temp_hash} = ();
	$temp_hash =\ $players_build_time[$num_of_serv]; # Почистить список логин - время захода в здание
	%{$$temp_hash} = ();
	$temp_hash =\ $players_build_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
        %{$$temp_hash} = ();
	$temp_hash =\ $players_join_count [$num_of_serv]; # Почистить список логнин - количество заходов на карту подряд
	%{$$temp_hash} = ();
	$temp_hash =\ $players_join_time [$num_of_serv]; # Почистить список логин - время захода на карту
	%{$$temp_hash} = ();
	$temp_hash =\ $players_build_less_sek_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
	%{$$temp_hash} = ();
	$temp_hash =\ $players_build_less_sek_break_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
	%{$$temp_hash} = ();
	$temp_hash =\ $players_kill_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
	%{$$temp_hash} = ();
	undef $shop_login[$num_of_serv]; # Очистить индикатор того, что последний заход был в магаз
	undef $srv_kill[$num_of_serv]; # Очистить индиктор того, что необходимо сбросить сервер
	undef $UCRM[$num_of_serv]; # Очистить счетчик события "Unit can't return to map - no free place"
        undef $old_time_srv[$num_of_serv];
        undef $less_ban_login[$num_of_serv];
}

sub server_reload # почистить все, что можно в случае заново открытых серверов
{
        my $type = $_[0];
        if ($type == 1) # server
        {
            $temp_hash =\ $player2login[$num_of_serv]; # Почистить список имя - логин
            %{$$temp_hash} = ();
            $temp_hash =\ $login2player[$num_of_serv]; # Почистить список логин - имя
            %{$$temp_hash} = ();
            $temp_hash =\ $players_join_count [$num_of_serv]; # Почистить список логнин - количество заходов на карту подряд
            %{$$temp_hash} = ();
            $temp_hash =\ $players_join_time [$num_of_serv]; # Почистить список логин - время захода на карту
            %{$$temp_hash} = ();
	    $temp_hash =\ $players_kill_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
	    %{$$temp_hash} = ();
            undef $old_time_srv[$num_of_serv];
            undef $srv_allowed;
            undef $less_ban_login[$num_of_serv];
	    undef $UCRM[$num_of_serv]; # Очистить счетчик события "Unit can't return to map - no free place"
        }	
        else # SRVMGR
        {
            $temp_hash =\ $players_build[$num_of_serv]; # Почистить список логин - здание
            %{$$temp_hash} = ();
            $temp_hash =\ $players_build_time[$num_of_serv]; # Почистить список логин - время захода в здание
            %{$$temp_hash} = ();
            $temp_hash =\ $players_build_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
            %{$$temp_hash} = ();
            $temp_hash =\ $players_build_less_sek_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
            %{$$temp_hash} = ();
            $temp_hash =\ $players_build_less_sek_break_count [$num_of_serv]; # Почистить список логин - количество заходов в здание подряд
            %{$$temp_hash} = ();
	    undef $shop_login[$num_of_serv]; # Очистить индикатор того, что последний заход был в магаз
        }
	undef $srv_kill[$num_of_serv]; # Очистить индиктор того, что необходимо сбросить сервер
}



sub do_ban {
 	my($blogin, $breason, $btime, $btext, $bwarning)=@_;
#        print OUT_FILE "DO_BAN";
        for ($i=0; $i<$bun_numb; $i++) # А вдруг уже забанен
        {
                last if ($bans[$i] eq $blogin);
        }
        if ($i==$bun_numb) # Если лог еще не забанен
        {
            if (!defined $bwarning)
            {
                open (banfile, path_to_ban($blogin));

                $bans[$bun_numb]=$blogin;
                $bun_numb++;
                
                $btime =~ s/\.\d{3}$//;
                print banfile "$btime;-1,$breason";
                close banfile;
            }
            print OUT_FILE cur_date();
            if (defined $bwarning)
            {
                print OUT_FILE "WARNING (NO BAN, BECAUSE NOT CHECKED SERVER.LOG, ONLY DLL LOG): ";
            }
            print OUT_FILE $btext;
        }
}

sub server_kill { #Функция, убивающая сервак
#	print OUT_FILE cur_date();
	system ("taskkill /F /IM a2serv".shift(@_).".exe");
        undef $srv_kill[$num_of_serv];
}

sub cur_date { # Возвращает текущую дату
 return sprintf("\[%02d.%02d.%04d %02d:%02d:%02d\] ", sub {($_[3], $_[4]+1, $_[5]+1900, $_[2], $_[1], $_[0])}->(localtime));
}

@players_build = (); 	# players => build. 0 - вышел 1 - вошел в магаз, 2 - вошел в таверну
@players_build_time = ();	# players => time. В какое время зашел в магаз?
@players_build_count = ();	# players => количество заходов
@players_build_less_sek_count = ();	# players => количество быстрых выходов входов
@players_build_less_sek_break_count = ();	# players => количество их прерыванией
@player2login = ();	# players => time. В какое время зашел в магаз?
@player2login = ();	# players => time. В какое время зашел в магаз?
@player2login = ();	# players => time. В какое время зашел в магаз?
@shop_login = (); # Следит за тем, что предыдущая строчка была
@srv_kill = (); # Убивать ли сервер
@UCRM = (); # Убивать ли сервер
@$less_ban_login= (); #кто СХ путем перезахода в здание

@cur_pos = (); # Текущие позиции в читаемых файлах srvmgr
@cur_pos_svr = (); # Текущие позиции в читаемых файлах server

@bans = (); # Список забанненых с момента старта программы логинов
$bun_numb=0;
@srv_allowed = ();

#	$test = "15.01.2008 2:57:40.654\n";
#	print OUT_FILE $test;
#	$test =~ s/\.(\d{2})(\d{2})/\.$2/;
#	$test =~ s/(?<=\D)(\d{1})([:,\.])/0$1$2/;
#	print OUT_FILE $test;

print OUT_FILE cur_date()."Anti_sh version $version succefully started\n";
close OUT_FILE;

while (1)
{
        open (OUT_FILE, ">>".$PATH_TO_OUT);
	for ($num_of_serv=$first_server; $num_of_serv <=$last_server; $num_of_serv++)
	{
#               undef $srv_allowed[$num_of_serv];
		undef $srv_kill[$num_of_serv];
#		print OUT_FILE "srvmgr: $cur_pos[$num_of_serv]\n";
#		print OUT_FILE "server: $cur_pos_svr[$num_of_serv]\n";
#		@players_build[$num_of_serv] = ();
#		@players_build_time[$num_of_serv] = ();
		open(file, path_to_srvmgr($num_of_serv)) or (warn cur_date()."Unable to open file \"".path_to_srvmgr($num_of_serv)."\": $!" and next) ; 
#		print OUT_FILE path_to_srvmgr($num_of_serv)."\n";
		#отсчитываем последние $start_string строк файла
		if (defined ($cur_pos[$num_of_serv])) # Если не первый раз открываем
		{
			seek file, 0, 2;# 2 - конец файла
			$size_of_file = tell (file);
	    		if ($cur_pos[$num_of_serv]<=$size_of_file)
			{
				seek file, $cur_pos[$num_of_serv], 0; # 0 - начало файла
			}
			# Если вдруг окажется, что файл меньшего размера чем тот, который мы обрабатывали в прошлый раз - открываем файл сначала
			else
			{
				#Перемещаемся на начало файла
				seek file, 0, 0; # 0 - начало файла
                                server_reload(2);
			}
		}
		else 
		{
			seek file, 0, 2; # 2 - конец файла
			$size_of_file = tell (file);
			if (($size_of_file + $_START_STRING) >= 0)
			{
				seek file, $_START_STRING, 2; # 2 - конец файла
			}
			else
			{
				seek file, 0, 0; # 0 - начало файла
                                server_reload(2);
				#$temp_hash =\ $player2login[$num_of_serv];
				#%{$$temp_hash} = ();
			}
			undef $srv_allowed[$num_of_serv];
		}

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
                                server_reload(1);
                                undef $line_srv;
#                               change_map();                               
			}
		}
                else 
                {
                    server_reload(1);
                    undef $line_srv;
                }

#	warn path_to_srvmgr($num_of_serv)."\n";
		while($line = <file>) # Считываем файл построчно
		{
                    #                       print OUT_FILE "SRVMGR: \n";
                    #	print OUT_FILE $line;
			#print OUT_FILE "SERVER: \n";
			if ($line =~  m/\[(.*)\].*/) #Подгоняем сервер под dll
			{
				$time=$1;
#				print OUT_FILE "time_SRMGR_BEFORE: $time\n";
				$time =~ s/\.(\d{2})(\d{2})/\.$2/;
				$time =~ s/((?<=[\D])\d{1}|^\d{1})([:,\.])/0$1$2/g;
				$time =~ s/\.\d{3}//;
				$time =~ s/(\d{2})\.(\d{2})\.(\d{2}) (.*)/$3\.$2\.$1 $4/;
				# $time = ($1=~ s/(\d{2})\.(\d{2})\.(\d{2})/$3\.$2\.$1/)." ". $2;
#				print OUT_FILE "time_SRMGR: $time\n";
				# Подогнали $time под формат логов сервера
				#
				$curr = tell(file_srv);
#				print OUT_FILE "Line: \n".$line;
#				$line_srv=<file_srv> if !defined $line_srv;
#                               if ($line_srv)
#                                {
#                                    do
                                PODGONKA:
                                while ($line_srv=<file_srv>)
                                {
#			print OUT_FILE "Line_srv: \n".$line_srv;
#					
                                            if ($line_srv =~ m/(\d{2})\.(\d{2})\.(\d{2}) (\d{2}:\d{2}:\d{2})/)
                                            {
                                                    $time_srv = "$3.$2.$1 $4";
                                                    $normal_time_srv = "$1.$2.20$3 $4";
#						$time = ($1=~ s/(\d{2})\.(\d{2})\.(\d{2})/$3\.$2\.$1/)." ". $2;
#						$time_srv = reverse $time_srv;
#						print OUT_FILE "time_SERVER: $time_srv\n";
                                                    last PODGONKA if $time lt $time_srv;

                                                    $srv_allowed[$num_of_serv] = 1; 

                                                    if ($line_srv =~ m/New player (.*) has logged in as (\w*)/)
                                                    {
                                                            $name = $1;
                                                            $login = $2;

#                                                            print OUT_FILE "name = $name, login = $login\n";
#                                                            $temp_hash =\ $player2login [$num_of_serv];
                                                            #if ($name eq "Перунас")
                                                            #{
                                                                #print OUT_FILE "AAA\n";

                                                                #}
                                                            #if ($login eq "100287")
                                                            #{
                                                                #print OUT_FILE "BBB; name = $name; login = $login;  \n";
                                                                #
#                                                          }
#
#                                        		    foreach $log (keys %{$$temp_hash})
#                                                           {
                                                            if (defined $login2player[$num_of_serv]{"$login"})
                                                            {
                                                                    print OUT_FILE cur_date()."WARNING: Server #$num_of_serv, player $name at login $login logged to game without left at time $normal_time_srv!\n";
                                                                    undef $players_join_count[$num_of_serv]{"$login"}; 
                                                                    undef $players_build[$num_of_serv]{"$login"}; 
                                                            }
#                                                           }
                                                            $player2login[$num_of_serv]{"$name"} = $login;
                                                            $login2player[$num_of_serv]{"$login"} = $name;
                                                    }
                                                    elsif ($line_srv =~ m/Player (.*) has left the game/)
                                                    {
                                                            $name = $1;
                                                            $login = $player2login[$num_of_serv]{"$name"};
                                                            if (defined $login)
                                                            {
                                                                if ($players_build_count[$num_of_serv]{"$login"} > 0)
                                                                {
                                                                        #ban
                                                                        $ban_text = "NO_BAN_BUG: Server# $num_of_serv, $login (name is $name) has left the game from building at time $normal_time_srv. Time of entering building ".$players_build_time[$num_of_serv]{"$login"}."\n";
                                                                        #do_ban ($login, "autoban: BUG, left the game from building", $normal_time_srv, $ban_text);
                                                                        #server_kill
                                                                        #$srv_kill[$num_of_serv] = 1;
                                                                }

                                                                if(exists $players_join_count[$num_of_serv]{"$login"})
                                                                {
                                                                        delete $players_join_count[$num_of_serv]{"$login"}; 
                                                                        delete $players_join_time[$num_of_serv]{"$login"}; 
                                                                }
                                                                if(exists $players_build[$num_of_serv]{"$login"})
                                                                {
                                                                        delete $players_build[$num_of_serv]{"$login"}; 
                                                                        delete $players_build_count[$num_of_serv]{"$login"}; 
                                                                        delete $players_build_time[$num_of_serv]{"$login"}; 
                                                                }
                                                                if(exists $players_build_less_sek_count[$num_of_serv]{"$login"})
                                                                {
                                                                        delete $players_build_less_sek_break_count[$num_of_serv]{"$login"}; 
                                                                        delete $players_build_less_sek_count[$num_of_serv]{"$login"}; 
                                                                }
                                                                if(exists $players_kill_count[$num_of_serv]{"$login"})
                                                                {
                                                                        delete $players_kill_count[$num_of_serv]{"$login"}; 
                                                                }
                                                                delete $player2login[$num_of_serv]{"$name"};
                                                                delete $login2player[$num_of_serv]{"$login"};
                                                            }
                                                    }
                                                    elsif ($line_srv =~ m/Player (.*) has disconnected/)
                                                    {
                                                            $name=$1;
                                                            $login = $player2login[$num_of_serv]{"$name"};
#                                                            if ($login eq "220756")
#                                                            {
#                                                                print OUT_FILE "AAAA!!\n";
#                                                            }
#                                                            if ($name eq "olg2004")
                                                            #                                                           {
#                                                                print OUT_FILE "BBBB!! login = $login\n";
                                                            #}
 
                                                            if (defined $login)
                                                            {
                                                                if(exists $players_join_count[$num_of_serv]{"$login"})
                                                                {
#								$players_build_time[$num_of_serv]{"$player2login{"$name"}"} = 
                                                                        undef $players_join_count[$num_of_serv]{"$login"}; 
                                                                        undef $players_join_time[$num_of_serv]{"$login"}; 
                                                                }
                                                                if(exists $players_build[$num_of_serv]{"$login"})
                                                                {
#								$players_build_time[$num_of_serv]{"$player2login{"$name"}"} = 
                                                                        undef $players_build[$num_of_serv]{"$login"}; 
                                                                        undef $players_build_count[$num_of_serv]{"$login"}; 
                                                                        undef $players_build_time[$num_of_serv]{"$login"}; 
                                                                }
                                                                if(exists $players_build_less_sek_count[$num_of_serv]{"$login"})
                                                                {
                                                                        undef $players_build_less_sek_break_count[$num_of_serv]{"$login"}; 
                                                                        undef $players_build_less_sek_count[$num_of_serv]{"$login"}; 
                                                                }
                                                                if(exists $players_kill_count[$num_of_serv]{"$login"})
                                                                {
                                                                        undef $players_kill_count[$num_of_serv]{"$login"}; 
                                                                }
                                                            }
                                                    }
                                                    elsif ($line_srv =~ m/Server started/)
                                                    {
                                                            change_map();
                                                    }
                                                    elsif ($line_srv =~ m/Player (.*) has joined the game/)
                                                    {
                                                        $name = $1;
                                                        $login =  $player2login[$num_of_serv]{"$name"};
                                                        if (defined $login)
                                                        {
                                                            if (!exists $players_join_count[$num_of_serv]{"$login"} || !defined $players_join_count[$num_of_serv]{"$login"} ) # Вошел в первый раз
                                                            {
                                                                    $players_join_count[$num_of_serv]{"$login"} = 1; # Запустим счетчик
                                                                    $players_join_time[$num_of_serv]{"$login"} = $normal_time_srv; # Запишем время первого входа
                                                            }
                                                            else
                                                            {
                                                                    if ($players_join_count[$num_of_serv]{"$login"} < $max_join)
                                                                    {
                                                                            $players_join_count[$num_of_serv]{"$login"}++; 
                                                                            print OUT_FILE cur_date()."WARNING: Server #$num_of_serv, player $name at login $login joined to game ".$players_join_count[$num_of_serv]{"$login"}." times last time at $normal_time_srv\n";
                                                                    }
                                                                    else
                                                                    {
                                                                            #ban
###убрал бан и киляние сервера за множественное присоединение к игре, которое читается СХ (Las)
###                                                                            $ban_text = "Speed hack: Server# $num_of_serv, $login (name is $name) at time $normal_time_srv by joining the game, time of previous join: ".$players_join_time[$num_of_serv]{"$login"}."\n";
###                                                                           do_ban ($login, "autoban: SpeedHack", $normal_time_srv, $ban_text);
###                                                                            #server_kill
###                                                                           $srv_kill[$num_of_serv] = 1;
                                                                    }
                                                            }
                                                        }
                                                    }

                                                    elsif ($line_srv =~ m/Warning! Player::RemoveUnit from NULL group!/)
                                                    {
                                                        if ($line_srv_old[$num_of_serv] =~ m/(\d{2})\.(\d{2})\.(\d{2}) (\d{2}:\d{2}:\d{2}) Player (.*) p?killed by/)
                                                        {
                                                               $old_time_srv = "$1.$2.20$3 $4";
                                                               $name = $5;
                                                               $login = $player2login[$num_of_serv]{"$name"};
                                                            
                                                               if (defined $login)
                                                               {
                                                                   if ($normal_time_srv eq $old_time_srv)
                                                                   {
                                                                        if (!defined $players_kill_count[$num_of_serv]{"$login"} ) # Убили однократно
                                                                        {
                                                                                $players_kill_count[$num_of_serv]{"$login"} = 1; # Запустим счетчик
                                                                        }
                                                                        else
                                                                        {
                                                                                if ($players_kill_count[$num_of_serv]{"$login"} < $max_kill)
                                                                                {
                                                                                        $players_kill_count[$num_of_serv]{"$login"}++; 
                                                                                        print OUT_FILE cur_date()."WARNING: Server #$num_of_serv, player $name at login $login was killed at time $normal_time_srv\n";
                                                                                }
                                                                                else
                                                                                {
                                                                                    #ban
### убрал бан и киляние сервера за слишком быстрое киляние игроков
###                                                                               $ban_text = "Speed hack: Server# $num_of_serv, $login (name is $name) was killed at time $normal_time_srv\n";
###                                                                                do_ban ($login, "autoban: SpeedHack", $normal_time_srv, $ban_text);
###                                                                                 #server_kill
###                                                                                 $srv_kill[$num_of_serv] = 1;
                                                                                }
                                                                         }
                                                                     }
                                                                }
                                                        }
                                                    }
                                                    elsif ($line_srv =~ m/Unit can't return to map - no free place/)
                                                    {
                                                        if (defined $UCRM[$num_of_serv])
                                                        {
                                                            if ($UCRM[$num_of_serv] >= $max_UCRM)
                                                            {
                                                                $login = $less_ban_login[$num_of_serv];
                                                                $name = $login2player[$num_of_serv]{"$login"};
                                                                if (defined $less_ban_login[$num_of_serv])
                                                                {
### Убрал бан за перезаход
###                                                               $ban_text = "Speed hack (by reenter): Server# $num_of_serv, $login (name is $name) at time $normal_time_srv\n";
###                                                                    do_ban ($login, "autoban: SpeedHack", $normal_time_srv, $ban_text);
###                                                                    #server_kill
###                                                                    $srv_kill[$num_of_serv] = 1;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                $UCRM[$num_of_serv]++;
                                                            }
                                                        }
                                                        else 
                                                        {
                                                                $UCRM[$num_of_serv]=1;
                                                        }
                                                    }
                                                $line_srv_old[$num_of_serv] = $line_srv;
                                                        
                                                           
                                            }
					$curr = tell(file_srv);
                                    } #while ($line_srv=<file_srv>);
#                                    PODGONKA: 
                                    #                               }
				seek file_srv, $curr, 0;
			}

			if ($line =~ m/\[(.*)\] player (\w*) entered (\w*) id=(\d*),.*/) # Игрок зашел или в магаз или в таверну
			{
				$time = $1;
				$login = $2;
				$building = $3;
				$id = $4;

                                $name = $login2player[$num_of_serv]{"$login"};
#		if ($players_build[$num_of_serv]{"$login"} eq undef || $players_build[$num_of_serv]{"$login"} eq "left") # Вошел в первый раз
				if (!exists $players_build[$num_of_serv]{"$login"} || !defined $players_build[$num_of_serv]{"$login"} ) # Вошел в первый раз
				{
					$players_build[$num_of_serv]{"$login"} = $building; # Запишем тот факт, что вошел и куда вошел
					$players_build_time[$num_of_serv]{"$login"} = $time; # Запишем то время, в которое вошел
					$players_build_count[$num_of_serv]{"$login"} = 1; # Запустим счетчик

					if ($building eq "shop") 
					{
						$shop_login[$num_of_serv] = $login;
					}
					else 
					{
						undef $shop_login[$num_of_serv];
					}
				}
				else
				{
#					print OUT_FILE "\n\n";
#					print OUT_FILE $line;
#					print OUT_FILE "time:".$time." login: $login\n";
#					print OUT_FILE "build: ".$players_build[$num_of_serv]{"$login"}."\n";
#					print OUT_FILE "build_time: ".$players_build_time[$num_of_serv]{"$login"}."\n";
#						if ($login == "80$656632529")	
#						{
#							print OUT_FILE "!!!!\n";
#							print OUT_FILE "max_enter: $max_enter\n";
#							print OUT_FILE "count :".$players_build_count[$num_of_serv]{"$login"}."\n";
#						}
                                    if ($players_build_count[$num_of_serv]{"$login"} < $max_enter)
                                    {
                                            $players_build_count[$num_of_serv]{"$login"}++; 
					    $players_build_time[$num_of_serv]{"$login"} = $time; # Запишем то время, в которое вошел
                                            print OUT_FILE cur_date()."WARNING: Server #$num_of_serv, player $name at login $login entered to $building ".$players_build_count[$num_of_serv]{"$login"}." times, last time at $time\n";
                                    }
                                    else
                                    {
                                            if ($players_build[$num_of_serv]{"$login"} eq $building) # СХ
                                            {
                                                    $ban_text =  "Speed hack: Server #$num_of_serv, $login (name is $name) at time $time by $building, time of previous enter: ".$players_build_time[$num_of_serv]{"$login"}."\n";
                                            }
                                            else 
                                            {
                                                    $ban_text =  "Speed hack: Server #$num_of_serv, $login (name is $name) entered ".$players_build[$num_of_serv]{"$login"}.", at time ". $players_build_time[$num_of_serv]{"$login"}." and then entered the $building at time $time\n";
                                            }
                                            if (!defined $srv_allowed[$num_of_serv] || !defined $login2player[$num_of_serv]{"$login"})
                                            {
###                                                    do_ban ($login, "autoban: SpeedHack", $time, $ban_text, 1);
                                            }
                                            else
                                            {
###убран бан и киляние серва за СХ
###                                                    #ban
###                                                   do_ban ($login, "autoban: SpeedHack", $time, $ban_text);
###                                                    #server_kill
###                                                    $srv_kill[$num_of_serv] = 1;
                                            }
                                            
                                    }
				}
			}
			elsif ($line =~ m/\[(.*)\] player (\w*) left building/)
			{
				$time = $1;
				$login = $2;

                                $name = $login2player[$num_of_serv]{"$login"};

#		print OUT_FILE $line;
#		print OUT_FILE "time:".$time." login: $login\n";
#		print OUT_FILE $players_build[$num_of_serv]{"$login"}."\n";
				
#		if ($players_build[$num_of_serv]{"$login"} eq "left") # Вышел не войдя!
				if (exists $players_build[$num_of_serv]{"$login"} && !defined $players_build[$num_of_serv]{"$login"}) # Вышел не войдя!
				{
			            #СХ
                                    if ($players_build_count[$num_of_serv]{"$login"} > -$max_exit)
                                    {
                                            $players_build_count[$num_of_serv]{"$login"}--; 
#                                           $players_build_count[$num_of_serv]{"$login"}--; 
                                            print OUT_FILE cur_date()."WARNING: Server #$num_of_serv, player $name at login $login left $building ".-$players_build_count[$num_of_serv]{"$login"}." times, last time at $time\n";
                                    }
                                    else
                                    {
                                            #ban
                                            $ban_text = "Speed hack (>2 left building): Server #$num_of_serv, $login (name is $name) at time $time, time of previous left: ".$players_build_time[$num_of_serv]{"$login"}."\n";
                                            if (!defined $srv_allowed[$num_of_serv] || !defined $login2player[$num_of_serv]{"$login"})
                                            {
###                                                    do_ban ($login, "autoban: SpeedHack", $time, $ban_text, 1);
                                            }
                                            else
                                            {
### Убрал бан за СХ
###                                                    do_ban ($login, "autoban: SpeedHack", $time, $ban_text);
###                                                    #server_kill
###                                                    $srv_kill[$num_of_serv] = 1;
                                            }
                                    }
				}
				else
				{
					#$players_build[$num_of_serv]{"$login"}="left";
                                        
                                        $time =~ m/.+ \d+:\d+:(\d+)\.(\d+)/;
                                        $tmp_time = $1 * 1000 + $2;
					$players_build_time[$num_of_serv]{"$login"} =~ m/.+ \d+:\d+:(\d+)\.(\d+)/;
                                        $tmp_build_time = $1 * 1000 + $2;
                                        if ($tmp_time - $tmp_build_time <= 1200 && $tmp_time - $tmp_build_time >= 0)
                                        {

                                           if (defined $players_build_less_sek_count[$num_of_serv]{"$login"})
                                           {
                                               if ($players_build_less_sek_count[$num_of_serv]{"$login"} < $max_reenter)
                                               {
                                                    $players_build_less_sek_count[$num_of_serv]{"$login"}++;
                                               }
                                               else
                                               {
                                                   #if ($players_build[$num_of_serv]{"$login"} eq "shop")
                                                   #{
                                                   #    $ban_text = "Speed hack (by reenter, shop): Server #$num_of_serv, $login (name is $name) at time $time\n";
                                                   #    do_ban ($login, "autoban: SpeedHack", $time, $ban_text);
                                                       #server_kill
                                                       #    $srv_kill[$num_of_serv] = 1;
                                                       # }
                                                   $less_ban_login[$num_of_serv] = $login;
                                               }
                                           }
                                           else 
                                           {
                                            $players_build_less_sek_count[$num_of_serv]{"$login"}=1;
                                           }
                                        }
                                        else # можно прерывать серию быстрых перезаходов несколькими мелкими
                                        {
                                            if (defined $players_build_less_sek_count[$num_of_serv]{"$login"})
                                            {
                                                if (defined $players_build_less_sek_break_count[$num_of_serv]{"$login"})
                                                {
                                                    if ($players_build_less_sek_break_count[$num_of_serv]{"$login"} >= $max_reenter_break)
                                                    {
                                                        undef $players_build_less_sek_count[$num_of_serv]{"$login"};
                                                        undef $players_build_less_sek_break_count[$num_of_serv]{"$login"};
                                                    }
                                                    else
                                                    {
                                                        $players_build_less_sek_break_count[$num_of_serv]{"$login"}++;
                                                    }
                                                }
                                                else
                                                {
                                                        $players_build_less_sek_break_count[$num_of_serv]{"$login"}=1;
                                                }

                                            }
                                        }



					undef $players_build[$num_of_serv]{"$login"};
					$players_build_time[$num_of_serv]{"$login"}=$time;
					$players_build_count[$num_of_serv]{"$login"}=-1; 

				}

				if ($login eq $shop_login[$num_of_serv])
				{
					undef $shop_login[$num_of_serv];
				}
			}
			elsif ($line =~ m/\[.*\] srvmgr started using .*/) # Сервер стартовал
			{
#			print OUT_FILE STDERR "TEST~\n";
			# Почистим все накопленные массивы
			change_map();
#			foreach $log (keys %{$$temp_hash})
#			{
#				print OUT_FILE STDERR "log: ".$log."build: ".$players_build[$num_of_serv]{$log}."\n";
#				delete $players_build[$num_of_serv]{$log};
#			}
#			$temp_hash =\ $players_build_time[$num_of_serv];
#			foreach $log (keys %{$$temp_hash})
#			{
#				delete $players_build_time[$num_of_serv]{$log};
#			}
			}
	            	elsif ($line =~ m/\[.*\] MsgBox: Shop Error, Amount > 1000.*/) # Сервер стартовал
			{
                            $time = $1;
                            print OUT_FILE cur_date()."Error: Server #$num_of_serv, error \"MsgBox: Shop Error, Amount > 1000\" at time $time\n";
                            $srv_kill[$num_of_serv] = 1;
			}
			elsif ($line =~ m/\[(.*)\] exception, saving characters.../) # Сервер "слетел"
			{
				$time = $1;
#			print OUT_FILE STDERR "shop_login = $shop_login[$num_of_serv]\n";
#			if ($num_of_serv == 2 && $shop_login[$num_of_serv] eq "253146")
#			{
#				print OUT_FILE STDERR $time."\n";
#			}
				if (defined $shop_login[$num_of_serv])		
				{
					$time =~ s/\.\d{3}$//;
					$shop_time = $players_build_time[$num_of_serv]{$shop_login[$num_of_serv]};
					$shop_time =~ s/\.\d{3}$//;

					if ($time eq $shop_time) #кидание карт, если время слета и захода в магаз совпало с точностью до секунды...
					{
						#ищем сообщника, который сидел в магазе
						$temp_hash =\ $players_build[$num_of_serv];
						$count=0;
						foreach $log (keys %{$$temp_hash})
						{
							next if !defined $players_build[$num_of_serv]{"$log"}; #Если не сидит в магазе
							next if "shop" ne $players_build[$num_of_serv]{"$log"}; #Или, если это тавернаа
							next if $shop_login[$num_of_serv] eq $log; # Того, кто сбросил карту не считаем, он 100% в магазине
							$count++;
							$for_ban = $log;
						}

						if ($count == 1) # Если ровно один человек сидит в магазе, кроме того, кто кидает
						{
                                                        #ban
                                                        $ban_text = "Server #$num_of_serv crashed: $for_ban was in shop at time $time, and entered shop at time ".$players_build_time[$num_of_serv]{"$for_ban"}."\n";
                                                        do_ban ($for_ban, "autoban: help for crash", $time, $ban_text);
						}

                                                #ban
                                                $ban_text = "Server #$num_of_serv crashed: $shop_login[$num_of_serv] at time $time\n";
                                                do_ban ($shop_login[$num_of_serv], "autoban: server crash", $time, $ban_text);
					}
				}
			}
		}
		server_kill($num_of_serv) if defined $srv_kill[$num_of_serv];

		$cur_pos[$num_of_serv] = tell (file); # Запомним позицию файла, что бы при следующем открытии брать отсюда
		$cur_pos_svr[$num_of_serv] = tell (file_srv); # Запомним позицию файла, что бы при следующем открытии брать отсюда

		close file;
		close file_srv;
	}
        close OUT_FILE;
	sleep $time_of_sleeping;
}
