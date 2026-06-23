
echo off

if not exist tmp md tmp

if "%BUILDFULL%"=="" call ../../setpath.bat

asl -U -L -x -olist tmp\dos_fe.lst -i %includes% dos_fe.a80
p2bin dos_fe.p tmp\dos_fe.rom -r $-$ -k

mhmt -mlz tmp\dos_fe.rom ..\dos_fe_pack.rom

if NOT "%BUILDFULL%"=="" exit /b

set PATH=%oldpath%
