del output.xml
"C:\wamp\bin\php\php5.6.40\php.exe" "C:\Users\nick\Documents\GitHub\rom2maps\edit_data\convert.php"
pause
del world\data\data.bin
databin2xml --compress "C:\Users\nick\Documents\GitHub\rom2maps\edit_data\original.xml" "C:\Users\nick\Documents\GitHub\rom2maps\edit_data\world\data\data.bin"
del world.res
res.exe ar "C:\Users\nick\Documents\GitHub\rom2maps\edit_data\world.res" world\data "C:\Users\nick\Documents\GitHub\rom2maps\edit_data\world\data"
pause