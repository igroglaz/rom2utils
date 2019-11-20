cd C:\
cd Allods2\
cd Server%1
del /q time.info
:a
del /q time.info
del /q chr\*.sck
del /q chr\info.map
del /q ctl\*
start /BELOWNORMAL /w a2serv%1.exe -cfg"C:\Allods2\Server%1\server.cfg" --cfg2="srvmgr.cfg"
sleep 15
goto a