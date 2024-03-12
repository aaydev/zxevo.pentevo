
@echo off

if not exist bin_262 md bin_262

cd asl-current_262

copy /Y /B ..\makefile.def

make

copy /Y /B *.exe ..\bin_262
copy /Y /B *.ms? ..\bin_262

make clean

cd ..
