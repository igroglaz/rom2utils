cd C:\
cd Allods2\
del /q time.info
set reset_time="04:00"
echo "[%date%, %time%] Starting a deffered task to kill Allods Server # %1 at %reset_time%" >> allods2.log
schtasks /create /f /tn "KillAllodsServer%1" /sc daily /st %reset_time% /tr "C:\Allods2\stop_server.cmd %1"
cd Server%1
:a
del /q time.info
del /q chr\*.sck
del /q chr\info.map
del /q ctl\*

echo "[%date%, %time%] Allods Server # %1 is starting" >> ..\allods2.log
start /BELOWNORMAL /w a2serv%1.exe -cfg"C:\Allods2\Server%1\server.cfg" --cfg2="srvmgr.cfg"
echo "[%date%, %time%] Allods Server # %1 stopped" >> ..\allods2.log
sleep 15
goto a
