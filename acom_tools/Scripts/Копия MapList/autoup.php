<?php
//Map List Auto Update Done By:
/*
 _____
|  ___| __ ___  __ _ _ __ ___   ___ _ __
| |_ | '__/ _ \/ _` | '_ ` _ \ / _ \ '__|
|  _|| | |  __/ (_| | | | | | |  __/ |
|_|  |_|  \___|\__,_|_| |_| |_|\___|_|

*/
//
//Огромное спасибо Вадиму aka Installero за помощь с хтмл.
//
//
//
//
//
//-------->
$ts=microtime();
$max_server=15;
//
$pathto = 'http://forum.allods.net/a2/maps/';
$hatlocation = 'C:\Games\Allods\hat.cfg'; //Path to hat cfg
function getmapname($map, $servernum)
{
        $mapspath = 'C:\Games\Allods\server'."$servernum".'\maps\\';
        $maps = "$mapspath"."$map";
        $name = '';
        global  $name;
        @$content = file_get_contents("$maps");
        $dirty = substr("$content", '108', '35');
        $symb = chr('0');
        $clean = explode($symb, "$dirty"); //type 1
        //Strange symbols list...    173
        //type 1 '' standart...
        //type 2 '' Furry lands...
        //type 3 '' Nival stronghold
        //type 4 '' Golden lands
        //type 5 'ў' Arrow
        //type 6 'Ђ' Mount
        //type 7 'я0' In and out
        //type 8 '&#65533;' Lord of the rings
        //type 9 'Дђ' Three sides
        //type 10 '' Dark lands
        //type 11 '' Grave
        //type 12 '' Rodion
/*        if(strpos($clean[0], '') > 0) { $clean = explode('', "$dirty"); } //type 2
        if(strpos($clean[0], '') > 0) { $clean = explode('', "$dirty"); } //type 3
        if(strpos($clean[0], '') > 0) { $clean = explode('', "$dirty"); } //type 4
        if(strpos($clean[0], 'ў') > 0) { $clean = explode('ў', "$dirty"); } //type 5
        if(strpos($clean[0], 'Ђ') > 0) { $clean = explode('Ђ', "$dirty"); } //type 6
        if(strpos($clean[0], 'я0') > 0) { $clean = explode('я0', "$dirty"); } //type 7
        if(strpos($clean[0], '&#65533;') > 0) { $clean = explode('&#65533;', "$dirty"); } //type 8
        if(strpos($clean[0], 'Дђ') > 0) { $clean = explode('Дђ', "$dirty"); } //type 9
        if(strpos($clean[0], '') > 0) { $clean = explode('', "$dirty"); } //type 10
        if(strpos($clean[0], '') > 0) { $clean = explode('', "$dirty"); } //type 11
        if(strpos($clean[0], '') > 0) { $clean = explode('', "$dirty"); } //type 12
*/
        $name = $clean[0];
        $name = trim($name);
        $b = strlen($name);
        $len = $b - 1;
        for($i = 0; $b > $i; $i++)
        {
                $ord = ord($name[$i]);
                //
                if ($ord == '32' || $ord == '46') { } //Точка или пробел
                elseif ($ord > '32' && $ord < '63') { } //Цифра и знаки типа ; ...
                elseif ($ord > '96' && $ord < '123') { } //a-z
                elseif ($ord > '64' && $ord < '91') { } //A-Z
                else {
                        $name[$i] = '';
                        }
//              elseif(ord($name[$i]) < '65' || ord($name[$i]) > '90' || ord($name[$i]) < '97' || ord($name[$i]) > '122'
        }
        if($name != '') { }
        else { $name = $map; if(!$map) { $name = 'Empty map requested.'; } }
        return $name;
}
//getmapname("map.alm", "1");
//print $server['server1']['map'];
//Get server ids...
$hat = file("$hatlocation");
foreach($hat as $line)
{
        $pos = strpos($line, 'ID');
        if ($pos === false){ }
        else
        {
                //Выполняються действия если такая строка найдена.
                $clean = explode('=', $line);
                $clean[1] = trim($clean[1]);
                if($clean[1] >= 1000)
                { }
                else
                {
                        if($clean[1] > max_serve) { }
                        else
                        {
                                $server['server'.$clean[1]]['id'] = $clean[1];
                        }
                }
        }
}
//Go get their names...
reset($server);
reset($hat);
$many = count($server);
$i = 1;

foreach($hat as $line)
{
        $pos = strpos($line, 'Name=');
        if ($pos === false){ }
        else
        {
                if($i > $max_server) { }
                else
                {
                $clean = explode('=', $line);
                $clean[1] = trim($clean[1]);
                $server['server'.$i]['name'] = $clean[1];
                $i++;
                }

        }

}
//Main part...
$agrr = 0;
for($i = 1; $many > $agrr; $i++)
{
$succ = '1';
$serverconfloc = 'C:\Games\Allods\server'."$i".'\server.cfg';
@$serverconf = file("$serverconfloc") OR $succ = '';
if($succ == 1)
{
$agrr++;
foreach($serverconf as $line)
{
        $pos = strpos($line, 'maps');
        if ($pos === false){  }
        else
        {
                if($i > $max_server) { }
                else
                {
                $clean = explode('\\', $line);
                $clean[1] = trim($clean[1]);
                $pos = strpos($clean[0], ';');
                if($pos === false)
                {
                        $cleann = explode('=', $clean[1]);
                        $cleann[1] = trim($cleann[1]);
                        $cleann[0] = trim($cleann[0]);
                        $server['server'.$i]['maps'][$cleann[0]]['name'] = getmapname($cleann[0], $i);
                        $server['server'.$i]['maps'][$cleann[0]]['alm'] = explode('.', $cleann[0]);
                        $server['server'.$i]['maps'][$cleann[0]]['alm'] = $server['server'.$i]['maps'][$cleann[0]]['alm']['0'];
                        $server['server'.$i]['maps'][$cleann[0]]['time'] = $cleann[1];
                } else { }

                }

        }

}
}
else { }
}

$i = 1;
//echo '<pre>'; print_r($server); echo '</pre>';
$fp = fopen('mapstable.txt', 'w');
$output = '<TABLE borderColor=#808000 align=center border=1>
              <TBODY>
<TR align=middle>
                <TD width=180><STRONG>Server Name</STRONG></TD>
                <TD align=middle width=244><STRONG>Название карты</STRONG></TD>
                <TD align=middle width=128><STRONG>Время (в
              часах)</STRONG></TD></TR>
';
fwrite($fp, $output);
$aggr = 0;
for($i = 1; $many > $aggr; $i++)
{
$output = '<TR align=middle>
                <TD vAlign=top width=180 rowSpan=';
fwrite($fp, $output);
//Count how many maps...
$keyz = array_keys( $server['server'.$i]['maps']);
$manymaps = count($keyz);
$manyrows = $manymaps;
$output = "$manyrows";
fwrite($fp, $output);
//
$output = '><I><B>';
fwrite($fp, $output);
$output = $server['server'.$i]['name'];
fwrite($fp, $output);
$output = '</B></I></TD>';
fwrite($fp, $output);
$mapi = 0;

fwrite($fp, $output);
//print "$manymaps - manymaps <br>"; //Needed only for debugging...
for($ii = 0; $manymaps > $ii; $ii++)
{

        $output = '<TD align=middle width=244>';
        fwrite($fp, $output);
        $output = "<a href='$pathto".$server['server'.$i]['maps'][$keyz[$mapi]]['alm'].".rar'>".$server['server'.$i]['maps'][$keyz[$mapi]]['name']."</a>";
        if($output == '') { $output = '-'."$manymaps $i $ii"; }
        fwrite($fp, $output);
        $output = '</TD>';
        fwrite($fp, $output);
        $output = '<TD align=middle width=128>';
        fwrite($fp, $output);
        $output = $server['server'.$i]['maps'][$keyz[$mapi]]['time'];
        if($output == '') { $output = '-'; }
        fwrite($fp, $output);
        $output = '</TD></TR>';
        fwrite($fp, $output);
        $mapi++;
}
//$a = array_keys( $server['server1'][maps]);
//echo '<pre>'; print_r($a); echo '</pre>';
//$b = count($a);
//print "$b";
$aggr++;
}
$output = '</TBODY></TABLE>';
fwrite($fp, $output);
fclose($fp);
include('mapstable.txt');
//
$te=microtime();
$ts=explode(' ',$ts);
$te=explode(' ',$te);
print '<center>Page Generated In: ';
print($te[0]+$te[1]-$ts[0]-$ts[1]);
print ' sec</center>';
//For debugging only...
//echo '<pre>'; print_r($keyz); echo '</pre>';
//echo '<pre>'; print_r($server); echo '</pre>';
//
?>