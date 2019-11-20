#talk robot v2.0 (c)2011 Las Jackwolf
$rob_text = "C:\\Allods2\\PlScript\\talkrobot.txt";
$path1 = "C:\\Allods2\\Server";
$path2 = "\\ctl\\say.";
open($wtext, "<".$rob_text) or die "Can't open file $file1: $!\n"; #открываем файл на чтение
@spam=();
@spam=<$wtext>;
close($wtext);
$all=@spam; #получаем кол-во элементов в массиве
if ($all==0) #если элементов не найдено
{
die 'Nothing to talk';
}
$random=int rand $all;
$spam=@spam[$random];
chomp $spam; #удаление перевода строки
# $spam =~ s/(["'])/\\$1/g; #экранирование кавычек ' и "
 use POSIX qw(strftime);
$pr = strftime " %H:%M:%S, %A, %d-%b-%Y", localtime;
$lens=1024;
$spam1=substr($spam, 0, $lens); 
$spam2=substr($spam, $lens, $lens); 
$spam3=substr($spam, $lens*2, $lens); 

for ($s = 1; $s <= 7; $s++) {

	for ($i = 1; $i <= 5; $i++) {
	if ($i==1) {$line=" ";}
	if ($i==2) {$line=$pr;}
	if ($i==3) {$line=$spam1;}
	if ($i==4) {$line=$spam2;}
	if ($i==5) {$line=$spam3;}

	$tot_path="$path1"."$s"."$path2"."$i";
if (length($line)>0){
	open(FSAY,"+>".$tot_path);
	flock(FSAY,2); #блокируем только для себя на запись
	print(FSAY "$line"); #пишем строку спама в файл
	close(FSAY);
		}
	}
}

exit;