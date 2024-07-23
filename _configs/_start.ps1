$restart_hour = 03
$restart_minute = 30
$num_servers = 10

while ($true) {
    # Step 0: Kill existing processes
    Set-Location "C:\Allods2\"
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c kill.cmd"

    # Delay to close existing servers
    Start-Sleep -Seconds 11

    # Step 1: Randomize maps order
    for ($i = 2; $i -le $num_servers; $i++) {
        $filePath = "C:\Allods2\Server$i\server.cfg"
        
        if (Test-Path $filePath) {
            # Read the content of the file
            $content = Get-Content -Path $filePath

            # Find the start and end of the [Maps] section
            $startIndex = $content.IndexOf("[Maps]")
            if ($startIndex -eq -1) { continue } # Skip if no [Maps] section

            $startIndex++
            $endIndex = $content.IndexOf("[", $startIndex)
            if ($endIndex -eq -1) {
                $endIndex = $content.Length
            } else {
                $endIndex -= 1
            }

            # Extract the [Maps] section
            $mapsSection = $content[$startIndex..$endIndex] -match "^maps.*"

            # Shuffle the [Maps] section
            $shuffledMapsSection = $mapsSection | Sort-Object {Get-Random}

            # Rebuild the file content
            $newContent = @()
            $newContent += $content[0..($startIndex - 1)]
            $newContent += $shuffledMapsSection
            $newContent += $content[$endIndex..($content.Length - 1)]

            # Write the new content back to the file
            $newContent | Set-Content -Path $filePath
        }
    }

    # Delay to prevent double restart
    Start-Sleep -Seconds 60

    # Step 2: Start allods.cmd
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c allods.cmd"

    # Step 3: Wait for the set time
    $current_hour = (Get-Date).Hour
    $current_minute = (Get-Date).Minute
    $current_time = $current_hour * 60 + $current_minute
    $restart_time = $restart_hour * 60 + $restart_minute
    $waiting_time = ($restart_time - $current_time + 24 * 60) % (24 * 60)
    Start-Sleep -Seconds ($waiting_time * 60)

    # Step 4: Server restart notice
    $command = {
        param($server, $message)
        Start-Process -FilePath "cmd.exe" -ArgumentList "/c", "msg.cmd", "$server", "`"$message`""
    }

    for ($j = 30; $j -ge 11; $j -= 5) {
        1..$num_servers | ForEach-Object {
            $i = $_
            Invoke-Command -ScriptBlock $command -ArgumentList $i, "Server will restart in $j minutes"
        }
        Start-Sleep -Seconds 300
    }

    for ($j = 10; $j -ge 2; $j -= 1) {
        1..$num_servers | ForEach-Object {
            $i = $_
            Invoke-Command -ScriptBlock $command -ArgumentList $i, "Server will restart in $j minutes"
        }
        Start-Sleep -Seconds 60
    }

    for ($j = 60; $j -ge 1; $j -= 10) {
        1..$num_servers | ForEach-Object {
            $i = $_
            Invoke-Command -ScriptBlock $command -ArgumentList $i, "Server will restart in $j seconds"
        }
        Start-Sleep -Seconds 10
    }
}
