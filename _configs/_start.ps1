$restart_hour = 03
$restart_minute = 30
$num_servers = 10

while ($true) {
    # Step 0: Kill existing processes
    Set-Location "C:\Allods2\"
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c kill.cmd" -Wait

    # Delay to close existing servers
    Start-Sleep -Seconds 11

    # Step 1: Randomize maps order
    for ($i = 2; $i -le $num_servers; $i++) {
        $filePath = "C:\Allods2\Server$i\server.cfg"
        
        if (Test-Path $filePath) {
            # Read the content of the file
            $content = Get-Content -Path $filePath -Raw
            $lines = $content -split "`r?`n"

            # Find the start and end of the [Maps] section
            $startIndex = $lines.IndexOf("[Maps]")
            if ($startIndex -eq -1) { continue } # Skip if no [Maps] section

            $startIndex++
            $endIndex = $startIndex

            while ($endIndex -lt $lines.Length -and ($lines[$endIndex] -eq "" -or $lines[$endIndex] -notmatch "^\[.*\]")) {
                $endIndex++
            }

            # Extract the [Maps] section
            $mapsSection = @()
            for ($j = $startIndex; $j -lt $endIndex; $j++) {
                if ($lines[$j] -match "^maps.*") {
                    $mapsSection += $lines[$j]
                }
            }

            if ($mapsSection.Count -eq 0) { continue } # Skip if no maps found

            # Shuffle the [Maps] section
            $shuffledMapsSection = $mapsSection | Sort-Object {Get-Random}

            # Rebuild the file content
            $newContent = @()
            $newContent += $lines[0..($startIndex - 1)]
            $newContent += $shuffledMapsSection
            if ($lines[$endIndex] -ne "") {
                $newContent += ""  # Add a blank line before the next section if it's not already blank
            }
            $newContent += $lines[$endIndex..($lines.Length - 1)]

            # Remove trailing empty lines
            while ($newContent[-1] -eq "") {
                $newContent = $newContent[0..($newContent.Length - 2)]
            }

            # Write the new content back to the file
            [System.IO.File]::WriteAllText($filePath, ($newContent -join "`r`n"))
        }
    }

    # Delay to prevent double restart
    Start-Sleep -Seconds 60

    # Step 2: Start allods.cmd
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c allods.cmd" -Wait

    # Step 3: Wait for the set time
    $current_hour = (Get-Date).Hour
    $current_minute = (Get-Date).Minute
    $current_time = $current_hour * 60 + $current_minute
    $restart_time = $restart_hour * 60 + $restart_minute
    $waiting_time = ($restart_time - $current_time + 24 * 60) % (24 * 60)
    Start-Sleep -Seconds ($waiting_time * 60)

    # Step 4: Server restart notice
    $msgCmd = "C:\Allods2\msg.cmd"
    
    for ($j = 30; $j -ge 11; $j -= 5) {
        for ($i = 1; $i -le $num_servers; $i++) {
            & $msgCmd $i "Server will restart in $j minutes"
        }
        Start-Sleep -Seconds 300
    }

    for ($j = 10; $j -ge 2; $j -= 1) {
        for ($i = 1; $i -le $num_servers; $i++) {
            & $msgCmd $i "Server will restart in $j minutes"
        }
        Start-Sleep -Seconds 60
    }

    for ($j = 60; $j -ge 1; $j -= 10) {
        for ($i = 1; $i -le $num_servers; $i++) {
            & $msgCmd $i "Server will restart in $j seconds"
        }
        Start-Sleep -Seconds 10
    }
}
