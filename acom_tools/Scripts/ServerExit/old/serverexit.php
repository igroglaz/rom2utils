<?php
if(isset($_REQUEST['n']) == FALSE || $_REQUEST['n'] == '')
{
for($i = '1'; $i != '15'; $i++)
{
print '<a href="http://hat.allods.net:8030/Scripts/ServerExit/serverexit.php?n='.$i.'">'.$i.' - serv</a><br>';
}
}
elseif($_REQUEST['n'] < '15' && $_REQUEST['n'] > '0') { 
print ('C:\\Games\\Allods\\Scripts\\ServerExit\\sexit.exe '.$_REQUEST['n']);
system('C:\\Games\\Allods\\Scripts\\ServerExit\\sexit.exe 7', $retval);
echo "<pre>$retval</pre>"; }
else { die('error'); }
?>