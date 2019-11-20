@echo off
echo SpamBot 1-16 is running...
:cycle
FOR /L %%i IN (1,1,16) DO CALL :spam %%i

sleep 900
goto :cycle
goto :eof


:spam
pushd
rem echo spam Server%1
if exist C:\Allods2\spam.txt copy /Y C:\Allods2\spam.txt C:\Allods2\Server%1\ctl\say.9 >nul
sleep 1
popd