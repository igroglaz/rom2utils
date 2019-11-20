<?$maplist="http://hat.allods.net:8030/Scripts/MapList/mapstable.txt";
if ($fp = @fopen($monitor, "r"))
{fpassthru($fp);}
else{print("Error reading maplist");}?>
