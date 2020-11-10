@echo off
cls
gcc -fno-rtti -fno-exceptions -s -O3 -Os -c -o ufo2.o ufo2.cpp -Wl,-e,_DllMain@12 -mwindows
gcc -fno-rtti -fno-exceptions -s -O3 -Os -mdll -o junk.tmp -Wl,--base-file,base.tmp ufo2.o -Wl,-e,_DllMain@12 -mwindows
del junk.tmp
dlltool --dllname %1.dll --base-file base.tmp --output-exp temp.exp --def ufo2.def
del base.tmp
windres -i ufo2.rc -o ufo2res.o
gcc -fno-rtti -fno-exceptions -s -O3 -Os -mdll -o ufo2.dll ufo2.o ufo2res.o -Wl,temp.exp -mwindows -Wl,-e,_DllMain@12 
del temp.exp
