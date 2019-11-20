@echo off
mkdir C:\Allods2\Server%1
fsutil hardlink create C:\Allods2\Server%1\graphics.res C:\Allods2\Server\graphics.res
fsutil hardlink create C:\Allods2\Server%1\main.res C:\Allods2\Server\main.res
fsutil hardlink create C:\Allods2\Server%1\famehall.dat C:\Allods2\Server\famehall.dat
fsutil hardlink create C:\Allods2\Server%1\main.res C:\Allods2\Server\main.res
fsutil hardlink create C:\Allods2\Server%1\movies.res C:\Allods2\Server\movies.res
fsutil hardlink create C:\Allods2\Server%1\patch.res C:\Allods2\Server\patch.res
fsutil hardlink create C:\Allods2\Server%1\scenario.dll C:\Allods2\Server\scenario.dll
fsutil hardlink create C:\Allods2\Server%1\srvmgr.dll C:\Allods2\Server\srvmgr.dll
fsutil hardlink create C:\Allods2\Server%1\scenario.res C:\Allods2\Server\scenario.res
fsutil hardlink create C:\Allods2\Server%1\SMACKW32.DLL C:\Allods2\Server\SMACKW32.DLL
fsutil hardlink create C:\Allods2\Server%1\world.res C:\Allods2\Server\world.res
fsutil hardlink create C:\Allods2\Server%1\sfx.res C:\Allods2\Server\sfx.res
fsutil hardlink create C:\Allods2\Server%1\a2serv%1.exe C:\Allods2\Server\a2server.exe
copy C:\Allods2\Server\server.cfg C:\Allods2\Server%1\server.cfg
mkdir C:\Allods2\Server%1\maps
echo log2=C:\Allods2\logs\srvmgr%1.log > C:\Allods2\Server%1\srvmgr.cfg
echo mode=0 >> C:\Allods2\Server%1\srvmgr.cfg
echo text=srvmgr%1 >> C:\Allods2\Server%1\srvmgr.cfg
echo class=srvmgrclass%1  >> C:\Allods2\Server%1\srvmgr.cfg
echo log_buildings=YES   >> C:\Allods2\Server%1\srvmgr.cfg

