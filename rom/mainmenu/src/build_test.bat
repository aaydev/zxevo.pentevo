
echo off

if not exist tmp md tmp
set path=d:\yad\svn\pentevo\tools\asl\bin\;d:\yad\svn\pentevo\tools\mhmt\
set includes=d:\yad\svn\pentevo\rom

asl -U -L -C -x -olist tmp\main.lst -i %includes% main.a80 || goto error
p2bin main.p main.rom -r $-$ -k

asl -U -L -C -x -olist tmp\main_fe.lst -i %includes% -D DOS_FE main.a80 || goto error
p2bin main.p main_fe.rom -r $-$ -k

asl -U -L -C -x -olist tmp\cmosset.lst -i %includes% cmosset.a80 || goto error
p2bin cmosset.p cmosset.rom -r $-$ -k

mhmt -mlz main.rom ..\main_pack.rom
mhmt -mlz main_fe.rom ..\main_fe_pack.rom
mhmt -mlz cmosset.rom ..\cmosset_pack.rom
mhmt -mlz chars_eng.bin ..\chars_pack.rom

echo ########################
echo # -= End Compile Ok =- #
echo ########################
goto exit

:error
echo #######################
echo # -= Error Compile =- #
echo #######################

:exit