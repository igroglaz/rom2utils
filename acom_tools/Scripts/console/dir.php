<?php
echo '<pre>';
$last_line = system('c:\\Progra~1\\Apache~1\\Apache\\apache -n "Apache" -k restart', $retval);
// Outputs all the result of shellcommand "ls", and returns
// the last output line into $last_line. Stores the return value
// of the shell command in $retval.
//$last_line = system('tasklist', $retval);
// Printing additional info

echo '</pre>
<hr />Last line of the output: ' . $last_line . '
<hr />Return value: ' . $retval;
?>