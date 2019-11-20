cd C:\Allods2\
echo "[%date%, %time%] Stopping Allods Server %1" >> allods2.log
echo "[%date%, %time%] Write to server's chat %1" >> allods2.log
AAA %1
echo "[%date%, %time%] Kill server process %1" >> allods2.log
wmic process where ExecutablePath="C:\\Allods2\\Server%1\\a2serv%1.exe" delete
