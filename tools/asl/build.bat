
@echo off

path=%path%;c:\ProlinSDK2\sdk\tools\msys\bin\

mkdir bin

cd asl-current

copy /Y /B ..\makefile.def

make

copy /Y /B *.exe ..\bin
copy /Y /B *.ms? ..\bin

make clean

cd ..
