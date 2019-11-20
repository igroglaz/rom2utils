@echo off

FOR /L %%i IN (1,1,16) DO CALL :obyav %%i
sleep 2
goto :eof


:obyav
pushd
echo Say to Server%1
if exist C:\Allods2\obyavlenie.txt copy /Y C:\Allods2\obyavlenie.txt C:\Allods2\Server%1\ctl\say.8 >nul

popd