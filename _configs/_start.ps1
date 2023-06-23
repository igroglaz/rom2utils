$restart_hour = 03
$restart_minute = 30
$num_servers = 10

while($true) {
    # Step 0: Kill existing processes
    Set-Location "C:\Allods2\"
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c kill.cmd"
    Start-Sleep -Seconds 5

    # Step 1: Start allods.cmd
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c allods.cmd"

    # Step 2: Wait for the set time
    do {
        $current_hour = (Get-Date).Hour
        $current_minute = (Get-Date).Minute
        $current_time = $current_hour*60 + $current_minute
        $restart_time = $restart_hour*60 + $restart_minute
        Start-Sleep -Seconds 60
    } until($current_time -ge $restart_time)

    # Step 3: Server restart notice
    for ($j = 30; $j -ge 11; $j -= 5) {
        1..$num_servers | ForEach-Object {
            $i = $_
            Start-Process -FilePath "cmd.exe" -ArgumentList "/c", "msg.cmd", "$i", "`"Server will restart in $j minutes`""
        }
        Start-Sleep -Seconds 300
    }

    for ($j = 10; $j -ge 2; $j -= 1) {
        1..$num_servers | ForEach-Object {
            $i = $_
            Start-Process -FilePath "cmd.exe" -ArgumentList "/c", "msg.cmd", "$i", "`"Server will restart in $j minutes`""
        }
        Start-Sleep -Seconds 60
    }

    for ($j = 60; $j -ge 1; $j -= 10) {
        1..$num_servers | ForEach-Object {
            $i = $_
            Start-Process -FilePath "cmd.exe" -ArgumentList "/c", "msg.cmd", "$i", "`"Server will restart in $j seconds`""
        }
        Start-Sleep -Seconds 10
    }
}
