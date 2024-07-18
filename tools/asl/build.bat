
@echo off

cd asl-current

copy /Y /B ..\makefile.def

make

copy /Y /B *.exe ..\bin
copy /Y /B *.ms? ..\bin

make clean

cd ..
