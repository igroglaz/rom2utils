cd C:\Allods2\
echo "[%date%, %time%] Stopping Allods Server %1" >> allods2.log
echo "[%date%, %time%] Write to server's chat %1" >> allods2.log

FOR /L %%i IN (30, -5, 11) DO (
  start cmd /c msg.cmd %1 "Server will restart in %%i minutes"
  sleep 300
)

FOR /L %%i IN (10, -1, 2) DO (
  start cmd /c msg.cmd %1 "Server will restart in %%i minutes"
  sleep 60
)

FOR /L %%i IN (60, -10, 1) DO (
  start cmd /c msg.cmd %1 "Server will restart in %%i seconds"
  sleep 10
)

echo "[%date%, %time%] Kill server process %1" >> allods2.log
wmic process where ExecutablePath="C:\\Allods2\\Server%1\\a2serv%1.exe" delete
exit
