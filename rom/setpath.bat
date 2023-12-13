
if NOT "%rootdir%"=="" exit /b

call :ExpandRootDir %~dp0
if "%oldpath%"=="" set oldpath=%PATH%
if "%includes%"=="" set includes=%rootdir%

set path=%rootdir%\..\tools\asl\bin;%rootdir%\..\tools\mhmt;%path%

exit /b


:ExpandRootDir
set rootdir=%~f1
exit /b