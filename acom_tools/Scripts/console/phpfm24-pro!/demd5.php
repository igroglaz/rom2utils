<?
$tmp=explode(' ',microtime());
$time1=$tmp[0] + $tmp[1];
//**
$md5=$_POST['hash'];
$sizeof=$_POST['koef'];
switch($_POST['limit'])
{
case 1:
	$max=$sizeof;
	break;
case 2:
	$max=$sizeof*$sizeof;
	break;
case 3:
	$max=$sizeof*$sizeof*$sizeof;
	break;
case 4:
	$max=$sizeof*$sizeof*$sizeof*$sizeof;
	break;
case 5:
	$max=$sizeof*$sizeof*$sizeof*$sizeof*$sizeof;
	break;
}
echo "Декодирование md5 - метод прямого перебора. Вы можете довольно ощутимо сократить время подборки пароля, если укажете меньший диапазон (по умолчанию он включает 163 символа, которые можно набрать на клавиатуре, не применяя сочетания Alt+код... Например символ Alt+0169 (на NumPad) - ©, или на html - &copy;. У вас уже нет шансов остановить процесс декодирования, кроме как с помощью Alt+Ctrl+Delete (если Вы запускаете скрипт на своей машине), либо связавшись с администратором вашего хостинга, которому вы устраиваете такой тест на прочность, что может привести к &quot;падению&quot; сервера, если на нем не стоит никакой защиты.) Если вы увидели этот текст только после загрузки результата, то значит, на вашем хостинге стоит буферизация вывода, не позволяющая вывести текст прямо во время исполнения. Вроде бы все. Если пароль окажется длинный (например из 5 символов), вы успеете уснуть, пока увидите результаты декодирования. Но результат того стоит... Ну, вроде бы все - удачного поиска !!!<br><br>\n";
flush();
//**
$counter=0;
$charcounter=0;
$letters=1;
//**
$chars=array
( //ох*** массив символов, которые можно набрать на клавиатуре (спецсимволы типа "©" не в счет)
'0',
'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
0,1,2,3,4,5,6,7,8,9,
'~','`','-','\'','"','\\','!','@','#','$','%','^','&','*','(',')','-','_','+','=','|','/','?','>','<','[',']','{','}',';',':','№',' ','.',',',
'а','б','в','г','д','е','ё','ж','з','и','й','к','л','м','н','о','п','р','с','т','у','ф','х','ц','ч','ш','щ','ъ','ы','ь','э','ю','я',
'А','Б','В','Г','Д','Е','Ё','Ж','З','И','Й','К','Л','М','Н','О','П','Р','С','Т','У','Ф','Х','Ц','Ч','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я',
'0',
);
//**
set_time_limit(0);
//**
function sub_genword1($letter)
{
    global $letters,$charcounter,$sizeof;
    //**
    if($charcounter[$letter]>=$sizeof)
    {
        $charcounter[$letter]=1;
        sub_genword1($letter-1);
    }else
    {
        $charcounter[$letter]++;
    }
    //**
    if($letter==1 && $charcounter[1]==$sizeof) { $letters++; sub_genword2(); echo "Переход на ".$letters." букв(ы)<br>\n"; flush(); }
}
//**
function sub_genword2()
{
    global $letters,$charcounter;
    //**
    for($i=1;$i<=$letters;$i++)
    {
        if($i==$letters) $charcounter[$i]=0;
        else $charcounter[$i]=1;
    }
}
//**
function sub_genword3()
{
    global $letters,$charcounter,$chars,$sizeof;
    //**
    $result="";
    //**
    for($i=1;$i<=$letters;$i++)
    {
        $result.=$chars[$charcounter[$i]];
    }
    return $result;
}
//**
function genword()
{
    global $letters,$charcounter,$chars,$sizeof;
    //**
    if($letters<=1)
    {
        $charcounter++;
        if($charcounter>=$sizeof) { $letters=2; echo "Переход на 2 буквы<br>\n"; flush(); $charcounter=array(); $charcounter[1]=1;$charcounter[2]=0; return $chars[$sizeof]; }
        return $chars[$charcounter];
    }else
    {
        sub_genword1($letters);
    }
    //**
    return sub_genword3();
}
//**
function de_md5($md5,$limit=0)
{
	global $counter,$total, $time1;
    if(md5('')==$md5) return "Пустая строка !!!";
    //**
    while(true)
    {
		$counter++;
        $word=genword();
        if(md5($word)==$md5) { $tmp=explode(' ',microtime());
    $total=$tmp[0]+$tmp[1]-$time1; return $word; break; }
		if($limit!=0 && $counter>=$limit) { return "пароль, к сожалению, не найден";break; }
    }
}
//**
echo "<br>Пароль найден - вот он - &quot;".de_md5($md5,$max)."&quot; ! Сгенерился за ".$total.' , попробовал паролей: '.$counter;
exit;
?>