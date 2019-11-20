<?php
echo '<pre>';
$command = $_REQUEST['c'];
$ip = $_GET['ip'];

// Outputs all the result of shellcommand "ls", and returns
// the last output line into $last_line. Stores the return value
// of the shell command in $retval.
if(isset($ip))
{
$last_line = system('dump.cmd '.$ip, $retval);
echo '
</pre>
<hr />Last line of the output: ' . $last_line . '
<hr />Return value: ' . $retval;
echo '<hr />'.$ip;
}
else echo 'Enter ip';
?>