# Files to replace.
$files_to_copy = "a2server.exe", "srvmgr.dll", "world.res"

# Path to folder containing new files
$source = 'D:\repos\allods\scripts\in\'

# Path to folder containing Servers subfolders
$destination = 'D:\repos\allods\scripts\out\'

# Fancy output
Write-Output "`r`n##############"
Write-Output "Files To Copy:"
Write-Output "##############`r`n"
foreach ($file in $files_to_copy)
{
    Write-Output $source$file
}

# Get all subfolders matching "*Server*" pattern
$destinationSubFolders = Get-ChildItem -Path $destination -Directory -Filter "*Server*" | Select FullName

# Fancy output
Write-Output "`r`n####################"
Write-Output "Destination Folders:"
Write-Output "####################"
Write-Output $destinationSubFolders | ft -HideTableHeaders

# Read user input
$reply = Read-Host -Prompt "Continue?[Y/N]"
if ( $reply -match "[yY]" ) { 
    # Copy files
    $destinationSubFolders | ForEach-Object { Get-Childitem $source -Recurse -Include $files_to_copy | Copy-Item -Destination $_.FullName}
    Write-Output "`r`nAll files have been copied.`r`n"
} else {
    Write-Output "`r`nCancelled.`r`n"
}

# Wait for user input
Write-Output "Press any key..."
$waitForInput = [Console]::ReadKey()
