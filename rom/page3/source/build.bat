
echo off

..\..\..\tools\asl\bin\asl -U -L basic48.a80
..\..\..\tools\asl\bin\p2bin basic48.p ..\basic48_128.rom -r $-$ -k
 