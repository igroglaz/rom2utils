@echo off
set "restart_hour=03"
set "restart_minute=30"
set "num_servers=10"

:loop
REM Step 0: Kill existing processes
cd C:\Allods2\
start cmd /c kill.cmd
sleep 5

REM Step 1: Start allods.cmd
start cmd /c allods.cmd

REM Step 2: Wait for the set time
:wait
set "current_hour=%time:~0,2%"
set "current_minute=%time:~3,2%"
set /a "current_time=%current_hour%*60 + %current_minute%"
set /a "restart_time=%restart_hour%*60 + %restart_minute%"

if %current_time% lss %restart_time% (
    sleep 60
    goto wait
)

REM Step 3: Server restart notice
FOR /L %%j IN (30, -5, 11) DO (
  FOR /L %%i IN (1, 1, %num_servers%) DO (
    start cmd /c msg.cmd %%i "Server will restart in %%j minutes"
  )
  sleep 300
)

FOR /L %%j IN (10, -1, 2) DO (
  FOR /L %%i IN (1, 1, %num_servers%) DO (
    start cmd /c msg.cmd %%i "Server will restart in %%j minutes"
  )
  sleep 60
)

FOR /L %%j IN (60, -10, 1) DO (
  FOR /L %%i IN (1, 1, %num_servers%) DO (
    start cmd /c msg.cmd %%i "Server will restart in %%j seconds"
  )
  sleep 10
)

REM Repeat the loop
goto loop
