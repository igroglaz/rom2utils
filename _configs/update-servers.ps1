# Files to replace.
$files_to_copy = "a2serv.exe", "srvmgr.dll"

# Path to folder containing new files
$source = 'C:\Users\Admin\Desktop\'

# Path to folder containing Servers subfolders
$destination = 'C:\Allods2\'

# Fancy output
Write-Output "`r`n##############"
Write-Output "Files To Copy:"
Write-Output "##############`r`n"
foreach ($file in $files_to_copy)
{
    Write-Output $source$file
}

# Get all subfolders matching "*Server*" pattern and sort them by their numerical value
$destinationSubFolders = Get-ChildItem -Path $destination -Directory -Filter "*Server*" | 
                         Select-Object -Property FullName, @{Name='Number';Expression={[int]($_.Name -replace '\D','')}} |
                         Sort-Object -Property Number |
                         Select-Object -ExpandProperty FullName

# Fancy output
Write-Output "`r`n####################"
Write-Output "Destination Folders:"
Write-Output "####################"
Write-Output $destinationSubFolders | ft -HideTableHeaders

$counter = 0
# Copy all files
$destinationSubFolders | ForEach-Object { Get-Childitem $source -Recurse -Include $files_to_copy | Copy-Item -Destination $_}
# Rename a2serv.exe -> a2servN.exe
$destinationSubFolders | ForEach-Object { Get-Childitem $_ -Recurse -Include "a2serv.exe" | Move-Item -Destination (Join-Path $_ "\a2serv$((++$counter)).exe") -Force }

# Delete copied files
foreach ($file in $files_to_copy)
{
    Remove-Item -Path "$source\$file" -Force
}

Write-Output "`r`nAll files have been copied.`r`n"
