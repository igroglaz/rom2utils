@echo off
cls
gcc -fno-rtti -fno-exceptions -s -O3 -Os -c -o Clash.o Clash.cpp -Wl,-e,_DllMain@12 -mwindows
gcc -fno-rtti -fno-exceptions -s -O3 -Os -mdll -o junk.tmp -Wl,--base-file,base.tmp Clash.o -Wl,-e,_DllMain@12 -mwindows
del junk.tmp
dlltool --dllname %1.dll --base-file base.tmp --output-exp temp.exp --def Clash.def
del base.tmp
windres -i Clash.rc -o Clashres.o
gcc -fno-rtti -fno-exceptions -s -O3 -Os -mdll -o Clash.dll Clash.o Clashres.o -Wl,temp.exp -mwindows -Wl,-e,_DllMain@12 
del temp.exp
