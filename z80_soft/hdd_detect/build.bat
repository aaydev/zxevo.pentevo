
@echo off

if not exist tmp md tmp
set path=d:\yad\svn\pentevo\tools\asl\bin\;d:\yad\svn\pentevo\tools\csum32\;d:\yad\svn\pentevo\tools\mhmt\
set includes=d:\yad\svn\pentevo\rom

asl -U -L -C -x -olist tmp\hdddetect.lst -i %includes% -D ADRSTART=0x8000 hdddetect.a80
p2bin hdddetect.p tmp\main.rom -r $-$ -k

mhmt -mlz tmp\main.rom tmp\main.pack

asl -U -L -C -x -olist tmp\make_scl.lst -i %includes% -D ADRSTART=0x8000 make_scl.a80
p2bin make_scl.p hddetect.scl -r $-$ -k

csum32 hddetect.scl -a
