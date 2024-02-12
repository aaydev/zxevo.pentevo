
@echo off

copy /Y /B makefile.def asl-current

cd asl-current

make

copy /Y /B *.exe ..\bin
copy /Y /B *.msh ..\bin

make clean

cd ..
