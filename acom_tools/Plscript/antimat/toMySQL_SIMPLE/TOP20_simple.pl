use DBD::mysql::SimpleMySQL qw/:all/;

#-----------------------------------------------------------------------------
#				Параметры
$port = '3306';
#логин к MySQL
$login = 'root';

#пароль - при регистрации был выбран 1
$pass = '1';

$host = '127.0.0.1';

#так как по умолчанию есть БД test, на неё и ссылаюсь, что бы Login и Пароль
# можно было ввести
$database = 'allods';


#-----------------------------------------------------------------------------
        my %dbinfo = (
                user            => $login,
                pass            => $pass,
                database        => $database,
                host            => $host,
                port            => $port

        );
$dbh = dbconnect(\%dbinfo);


#$dbh = Mysql->Connect($host, $database, $login, $pass);

# Подсчитываем отношение матных сообщение/ко всем сообщениям
$TextQuery = "UPDATE reitingmat
		SET Otnosh = SoobMat/SoobAll*100
		Where SoobAll <> 0;";
$sth = $dbh->do("use ".$database);
$sth = $dbh->do($TextQuery);


#Очищаем TOP20 по количеству матных сообщений
$TextQuery = "Delete From TOP20_MS;";
$sth = $dbh->do($TextQuery);

#Формируем TOP20 по количеству матных сообщений
$TextQuery = "Insert into TOP20_MS (select login, SoobMat
				     from reitingmat
				     order by SoobMat desc,
				     Otnosh desc
				     Limit 20)";
$sth = $dbh->do($TextQuery) or die $Mysql::db_errstr;


#Очищаем TOP20 по отношению
$TextQuery = "Delete From TOP20_OT;";
$sth = $dbh->do($TextQuery);

#Формируем TOP20 по количеству матных сообщений
$TextQuery = "Insert into TOP20_OT (select login, Otnosh
				     from reitingmat
				     order by Otnosh desc, 
				     SoobMat desc
				     Limit 20)";
$sth = $dbh->do($TextQuery) or die $Mysql::db_errstr;

# Обнуляем историю
$TextQuery = "UPDATE reitingmat
		SET SoobMat = 0,
		SoobAll = 0;";
$sth = $dbh->do($TextQuery);
