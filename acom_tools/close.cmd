@echo off
FOR /L %%I IN (49000,1,50000) DO call :a %%I
@echo on
goto exit

:a
netsh firewall delete portopening TCP %1 
echo Processing port %1

:exit