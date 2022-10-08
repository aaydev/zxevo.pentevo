
@ECHO OFF

..\..\..\tools\asl\bin\asl -U -L -x rbios.a80
..\..\..\tools\asl\bin\p2bin rbios.p ..\rbios.rom -r $-$ -k

..\..\..\tools\mhmt\mhmt cpm.img cpm_pack.img
