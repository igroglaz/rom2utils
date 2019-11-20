use mysql;

#-----------------------------------------------------------------------------
#				Параметры
#логин к MySQL
$login = 'antimat';

#пароль - при регистрации был выбран 1
$pass = 'acommat';

$host = 'localhost';

#так как по умолчанию есть БД test, на неё и ссылаюсь, что бы Login и Пароль
# можно было ввести
$database = 'autoban';

#Промежуточные вычисления, если = 1 то при вводе данных в таблицу будет вычислятся
#поле отношение, если <> 1, то поле можно вычислить принудительно
$PromezhVuch = 1;

#-----------------------------------------------------------------------------
$dbh = Mysql->Connect($host, $database, $login, $pass);

# Если нет базы данных, то создаем её;
$sth = $dbh->Query("Create DATABASE if not exists allods") or die $Mysql::db_errstr;

$dbh->SelectDB(allods);


$TextQuery1 = "Create TABLE if not exists reitingmat (login varchar(50) primary key,
SoobAll bigint Unsigned default 0 ,
SoobMat bigint Unsigned default 0,
Otnosh float Unsigned default 0,
Ochki float Unsigned default 0);";
#print $TextQuery1;

#Создаем таблицу если её нет
$sth = $dbh->Query($TextQuery1) or die $Mysql::db_errstr;

#топ 20 по количеству матных сообщений
$TextQuery2 = "Create TABLE if not exists TOP20_MS (login varchar(50) primary key, matsoob bigint Unsigned);";

#Создаем таблицу если её нет
$sth = $dbh->Query($TextQuery2) or die $Mysql::db_errstr;

#топ 20 по отношению матных сообщений ко всем сообщениям
$TextQuery3 = "Create TABLE if not exists TOP20_OT (login varchar(50) primary key, otnosh float);";

#Создаем таблицу если её нет
$sth = $dbh->Query($TextQuery3) or die $Mysql::db_errstr;


# Удаляем если процедура есть - для отладки, что бы не удалчть ручками,
# имеет смысл закоментить
$StoredProcedText = "DROP PROCEDURE IF EXISTS InsertDATA;";
$sth = $dbh->Query($StoredProcedText) or die $Mysql::db_errstr;

#Создаем хранимую процедуру
$StoredProcedText01 =
"create procedure InsertDATA (IN log varchar(50),IN allSoob bigint Unsigned,
IN mat bigint Unsigned,IN och float)
Begin
Declare SA bigint unsigned;
Declare SM bigint unsigned;
Declare O bigint;

  Select rm.SoobAll, rm.SoobMat, rm.Ochki 
		into SA, SM, O
		From reitingmat rm
		Where rm.login=log;

 IF SA IS NOT NULL THEN
      UPDATE reitingmat
	SET SoobAll = SA + allSoob,	
	 SoobMat = SM + mat,
	 Ochki = if((O + och)>0,(O + och),0)";

$StoredProcedText03 = "Where login = log;
 ELSE
     INSERT INTO reitingmat 	
     VALUES (log, allSoob, mat, ";
$StoredProcedText05 = ", if(och>0,och,0));
 END IF;

end";


$StoredProcedText02 = "";
$StoredProcedText04 = "0";

if ($PromezhVuch != 0)
  {
	$StoredProcedText02 = ", 
	 Otnosh = (SM + mat)/(SA + allSoob)*100 ";
	$StoredProcedText04 = "(mat/allSoob)*100";

  }; 
#print $StoredProcedText01.$StoredProcedText02.$StoredProcedText03.$StoredProcedText04.$StoredProcedText05;
$sth = $dbh->Query($StoredProcedText01.$StoredProcedText02.$StoredProcedText03.$StoredProcedText04.$StoredProcedText05) or die $Mysql::db_errstr;