C:\Allods2\exescript\LOI.exe dir C:\Allods2\chr > C:\Games\Allods\temp\loi%DATE%.log
sleep 1
"D:\Program Files\7-Zip\7z.exe" a -t7z D:\Allods\Archives\List_of_items\loi_%DATE%.7z  C:\Allods2\temp\loi%DATE%.log -mx=9 -y
del C:\Allods2\temp\loi%DATE%.log
