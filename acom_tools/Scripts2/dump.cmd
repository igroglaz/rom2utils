@Echo Off 
SET DAY=%DATE:~4,2%
SET MTH=%DATE:~7,2%
SET YR=%DATE:~10,4%
SET HR=%TIME:~0,2%
SET HR0=%TIME:~0,1%
IF "%HR0%"==" " SET HR=0%TIME:~1,1%
SET MIN=%TIME:~3,2%
SET SEC=%TIME:~6,2%
SET MYDATE=%DATE%-%HR%h%MIN%m%SEC%s
"C:\Program Files\Wireshark\tshark.exe" -nx -a duration:600 -i 2 host %1  > %1_%MYDATE%_dump.pcap
