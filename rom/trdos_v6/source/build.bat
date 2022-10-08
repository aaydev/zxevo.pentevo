
@echo off

..\..\..\tools\asl\bin\asl -U -L trdos_v6.a80
..\..\..\tools\asl\bin\p2bin trdos_v6.p ..\dosatm3.rom -r $-$ -k
