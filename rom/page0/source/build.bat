
echo off

if not exist tmp md tmp
set path=d:\yad\svn\pentevo\tools\asl\bin\;d:\yad\svn\pentevo\tools\mhmt\
set includes=d:\yad\svn\pentevo\rom

cd ..\..\mainmenu\src

call build.bat

cd ..\..\page0\source

asl -U -L -C -x -olist tmp\services.lst -i %includes% services.a80 || goto error
p2bin services.p ..\services.rom -r $-$ -k

asl -U -L -C -x -olist tmp\services_fe.lst -i %includes% -D DOS_FE services.a80 || goto error
p2bin services.p ..\services_fe.rom -r $-$ -k

echo ########################
echo # -= End Compile Ok =- #
echo ########################
goto exit

:error
echo #######################
echo # -= Error Compile =- #
echo #######################

:exit