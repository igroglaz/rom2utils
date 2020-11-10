@echo off
cls
gcc -fno-rtti -fno-exceptions -s -O3 -Os -c -o FO2.o FO2.cpp -Wl,-e,_DllMain@12 -mwindows
gcc -fno-rtti -fno-exceptions -s -O3 -Os -mdll -o junk.tmp -Wl,--base-file,base.tmp FO2.o -Wl,-e,_DllMain@12 -mwindows
del junk.tmp
dlltool --dllname %1.dll --base-file base.tmp --output-exp temp.exp --def FO2.def
del base.tmp
windres -i FO2.rc -o FO2res.o
gcc -fno-rtti -fno-exceptions -s -O3 -Os -mdll -o FO2.dll FO2.o FO2res.o -Wl,temp.exp -mwindows -Wl,-e,_DllMain@12 
del temp.exp
