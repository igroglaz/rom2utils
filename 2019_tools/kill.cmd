@echo
cls
taskkill /F /T /FI "IMAGENAME eq cmd.exe" /FI "WINDOWTITLE eq a2_"
taskkill /IM  hat2.exe /T /F
taskkill /F /T /FI "IMAGENAME eq a2serv*"
exit

