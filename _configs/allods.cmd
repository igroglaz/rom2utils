TITLE a2_launcher
rem launch_servers
start /BELOWNORMAL hat2.cmd
sleep 15
start /BELOWNORMAL server.cmd 1
sleep 5
start /BELOWNORMAL server.cmd 2
sleep 5
start /BELOWNORMAL server.cmd 3
sleep 5
start /BELOWNORMAL server.cmd 4
sleep 5
start /BELOWNORMAL server.cmd 5
sleep 5
start /BELOWNORMAL server.cmd 6
sleep 5
start /BELOWNORMAL server.cmd 7
sleep 5
start /BELOWNORMAL server.cmd 8
sleep 5
start /BELOWNORMAL server.cmd 9
sleep 5
start /BELOWNORMAL server.cmd 10
sleep 5
rem start /LOW itemlog.cmd
rem sleep 5

@echo off
setlocal enabledelayedexpansion

rem Initializing high CPU counters for each server
for /L %%i in (1,1,10) do (
    set "highcpu_%%i=0"
)

echo.
echo Monitoring started...
echo.

rem Check CPU usage and restart if necessary
:check_cpu
for /L %%i in (1,1,10) do (
    set "cpu="
    for /f "tokens=2 delims==" %%a in ('wmic path Win32_PerfFormattedData_PerfProc_Process where "Name='a2serv%%i'" get PercentProcessorTime /value 2^>nul ^| findstr /r "[0-9]"') do (
        set "cpu=%%a"
    )
    if defined cpu (
        set "cpu=!cpu: =!"
        set /a cpunum=!cpu! 2>nul
        
        rem If CPU is above 20%
        if !cpunum! GTR 20 (
            set /a highcpu_%%i+=1
            echo [%date%, %time%] a2serv%%i CPU: !cpu!%% ^(!highcpu_%%i! checks^)
            
            rem Kill the process only if CPU is high for 3 checks in a row (3 minutes)
            if !highcpu_%%i! GEQ 3 (
                echo [%date%, %time%] CPU usage of a2serv%%i is !cpu!%% for !highcpu_%%i! consecutive checks, restarting...
                echo [%date%, %time%] CPU usage of a2serv%%i is !cpu!%% for !highcpu_%%i! consecutive checks, restarting... >> C:\Allods2\allods2.log
                taskkill /f /im a2serv%%i.exe >nul 2>&1
                sleep 5
                start /BELOWNORMAL server.cmd %%i
                set "highcpu_%%i=0"
            )
        ) else (
            rem CPU is normal - reset counter
            if !highcpu_%%i! GTR 0 (
                echo [%date%, %time%] a2serv%%i CPU normalized: !cpu!%%
            )
            set "highcpu_%%i=0"
        )
    )
)

rem Wait for 1 minute before checking again
sleep 60
goto check_cpu