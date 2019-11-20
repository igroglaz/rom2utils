cd D:\
cd Games\Allods\
cd Server%1
del /q time.info
:a
del /q time.info
del /q chr\*.sck
del /q chr\info.map
del /q ctl\*
rem start /BELOWNORMAL "debug_serv%1" /w C:\Allods2\exescript\de_bug.exe --log="C:\Allods2\logs\de_bug%1.log" --cmd="a2serv%1.exe -cfg\"server.cfg\" --cfg2=\"srvmgr.cfg\"" --handler=6081F0 
start /BELOWNORMAL /w a2serv%1.exe -cfg"server.cfg" --cfg2="srvmgr.cfg"
sleep 15
goto a