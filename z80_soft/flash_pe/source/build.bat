
@echo off

if not exist rom md rom
if not exist lst md lst

path=d:\yad\svn\pentevo\tools\asl\bin;d:\yad\svn\pentevo\tools\mhmt;d:\yad\svn\pentevo\tools\csum32;d:\yad\svn\pentevo\tools\noice2equ

asl -U -L -x -i d:\yad\svn\pentevo\rom -olist lst\flash_pe.lst flash_pe.a80
p2bin flash_pe.p rom\flash_pe.rom -r $-$ -k

mhmt -mlz rom\flash_pe.rom rom\flash_pe_pack.rom

asl -U -L -x -i d:\yad\svn\pentevo\rom -olist lst\make_scl.lst make_scl.a80
p2bin make_scl.p rom\make_scl.rom -r $-$ -k

csum32 rom\make_scl.rom -a
copy /B /Y rom\make_scl.rom ..\flash_pe.scl

asl -U -L -x -i d:\yad\svn\pentevo\rom -olist lst\make_hobeta.lst make_hobeta.a80
p2bin make_hobeta.p ..\flash_pe.$C -r $-$ -k

del csum32.bin
del *.rom
del *.lst

pause