<?php

echo "Starting Proxy Detection System<br><br>";

#Here you can call a function to check if the IP is blocked
#and if not then continue to the IF statement

if ($HTTP_X_FORWARDED_FOR)
{
echo "Warring: You might be using a proxy server to connect us<br><br>";
echo "Connected Via: " . $HTTP_VIA . " - " . $REMOTE_ADDR;
echo "<br>Your real IP: " . $HTTP_X_FORWARDED_FOR; 
}
else
{
echo "You are probably not using a proxy server<br>";
echo "Your IP: " . $REMOTE_ADDR;
}

?>