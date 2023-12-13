
echo off

if not exist rom md rom
if not exist lst md lst

if "%BUILDFULL%"=="" call ../../setpath.bat

cd %rootdir%\fat_boot\source

call build.bat

cd %rootdir%\page5\source

mhmt -mlz 8x8_ar.fnt 8x8_ar_pack.bin
mhmt -mlz 866_code.fnt 866_code_pack.bin
mhmt -mlz atm_code.fnt atm_code_pack.bin
mhmt -mlz perfpack.bin perfpack_pack.bin

asl -U -L -x -olist lst\rst8service.lst -i %includes% rst8service.a80
p2bin rst8service.p ..\rst8service.rom -r $-$ -k

asl -U -L -x -olist lst\rst8service_fe.lst -i %includes% -D DOS_FE rst8service.a80
p2bin rst8service.p ..\rst8service_fe.rom -r $-$ -k


if NOT "%BUILDFULL%"=="" exit /b

set PATH=%oldpath%