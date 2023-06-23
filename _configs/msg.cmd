@echo off
set repeat=1
set delay=100
FOR /L %%i IN (1, 1, %repeat%) DO (
	echo %2 > C:\Allods2\Server%1\ctl\say.10
	sleep -m %delay%
)
exit
