
echo off

if not exist rom md rom
if not exist lst md lst
set path=d:\yad\svn\pentevo\tools\asl\bin\;d:\yad\svn\pentevo\tools\mhmt\
set includes=d:\yad\svn\pentevo\rom

asl -U -L -x -olist lst\main.lst -i %includes% main.a80
p2bin main.p rom\main.rom -r $-$ -k

asl -U -L -x -olist lst\main_fe.lst -i %includes% -D DOS_FE main.a80
p2bin main.p rom\main_fe.rom -r $-$ -k

asl -U -L -x -olist lst\cmosset.lst -i %includes% cmosset.a80
p2bin cmosset.p rom\cmosset.rom -r $-$ -k

mhmt -mlz rom\main.rom ..\main_pack.rom
mhmt -mlz rom\main_fe.rom ..\main_fe_pack.rom
mhmt -mlz rom\cmosset.rom ..\cmosset_pack.rom
mhmt -mlz rom\chars_eng.bin ..\chars_pack.rom

rem del *.rom
