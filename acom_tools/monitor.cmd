@echo Monitor is running...
@echo off
:a
C:\Perl\bin\perl.exe C:\Allods2\PlScript\pmon.pl 2>> pmon_out.log
sleep 30
if exist C:\Allods2\record.txt copy /Y C:\Allods2\record.txt D:\Apache2\htdocs\pub_info\record.txt >nul
goto a
