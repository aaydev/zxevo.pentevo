
echo off

if not exist tmp md tmp

if "%BUILDFULL%"=="" call ../../setpath.bat

asl -U -L -x -olist tmp\main.lst -i %includes% main.a80
p2bin main.p tmp\main.rom -r $-$ -k

asl -U -L -x -olist tmp\main_fe.lst -i %includes% -D DOS_FE main.a80
p2bin main.p tmp\main_fe.rom -r $-$ -k

asl -U -L -x -olist tmp\cmosset.lst -i %includes% cmosset.a80
p2bin cmosset.p tmp\cmosset.rom -r $-$ -k

mhmt -mlz tmp\main.rom ..\main_pack.rom
mhmt -mlz tmp\main_fe.rom ..\main_fe_pack.rom
mhmt -mlz tmp\cmosset.rom ..\cmosset_pack.rom
mhmt -mlz chars_eng.bin ..\chars_pack.rom

rem del *.rom

if NOT "%BUILDFULL%"=="" exit /b

set PATH=%oldpath%
