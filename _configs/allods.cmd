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
start /BELOWNORMAL server.cmd 11
sleep 5
rem start /LOW itemlog.cmd
rem sleep 5

rem Check CPU usage and restart if necessary
:check_cpu
for /L %%i in (1,1,11) do (
    set "process_name=a2serv%%i.exe"
    for /f "tokens=2 delims==" %%p in ('wmic path Win32_PerfFormattedData_PerfProc_Process where "Name='%%~ni'" get PercentProcessorTime /value') do (
        if %%p gtr 20 (
            echo "[%date%, %time%] CPU usage of %%~ni is above 20%%, restarting..." >> C:\Allods2\allods2.log
            taskkill /f /im %%~ni
            sleep 5
        )
    )
)
rem Wait for 1 minute before checking again
sleep 60
goto check_cpu