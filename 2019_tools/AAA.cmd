@echo off

FOR /L %%i IN (1,1,8) DO CALL :obyav %%i
sleep -m 100
goto :eof


:obyav
pushd
echo Say to Server%1
if exist C:\Allods2\aaa.txt copy /Y C:\Allods2\aaa.txt C:\Allods2\Server%1\ctl\say.8 >nul
sleep -m 100
echo Say to Server%1
if exist C:\Allods2\aaa.txt copy /Y C:\Allods2\aaa.txt C:\Allods2\Server%1\ctl\say.8 >nul
sleep -m 100
echo Say to Server%1
if exist C:\Allods2\aaa.txt copy /Y C:\Allods2\aaa.txt C:\Allods2\Server%1\ctl\say.8 >nul
sleep -m 100
echo Say to Server%1
if exist C:\Allods2\aaa.txt copy /Y C:\Allods2\aaa.txt C:\Allods2\Server%1\ctl\say.8 >nul
sleep -m 100
echo Say to Server%1
if exist C:\Allods2\aaa.txt copy /Y C:\Allods2\aaa.txt C:\Allods2\Server%1\ctl\say.8 >nul
sleep -m 100

popd