
echo off

if not exist rom md rom
if not exist lst md lst
set path=%path%;d:\yad\svn\pentevo\tools\asl\bin;d:\yad\svn\pentevo\tools\mhmt
set includes=d:\yad\svn\pentevo\rom

asl -U -L -x -olist lst\dos_fe.lst -i %includes% dos_fe.a80
p2bin dos_fe.p rom\dos_fe.rom -r $-$ -k

mhmt -mlz rom\dos_fe.rom ..\dos_fe_pack.rom
