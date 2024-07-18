
echo off

set path=d:\yad\svn\pentevo\tools\asl\bin;d:\yad\svn\pentevo\tools\mhmt;d:\yad\svn\pentevo\tools\csum32;d:\yad\svn\pentevo\tools\noice2equ\
set includes=d:\yad\svn\pentevo\rom\
if not exist tmp md tmp

cls

asl -U -L -x -olist tmp\micro_boot_fat.lst -g noice -i %includes% -D DRV_VAR=0x4000 micro_boot_fat.a80 || goto error
p2bin micro_boot_fat.p ..\micro_boot_fat_ers.rom -r $-$ -k

asl -U -L -x -olist tmp\micro_boot_fat.lst -g noice -i %includes% -D DRV_VAR=0x4000,ALL_DRVS micro_boot_fat.a80 || goto error
p2bin micro_boot_fat.p ..\micro_boot_fat.rom -r $-$ -k

mhmt -mlz ..\micro_boot_fat_ers.rom ..\micro_boot_fat_ers.pack
mhmt -mlz ..\micro_boot_fat.rom ..\micro_boot_fat.pack

echo ########################
echo # -= End Compile Ok =- #
echo ########################
goto exit

:error
echo #######################
echo # -= Error Compile =- #
echo #######################

:exit
