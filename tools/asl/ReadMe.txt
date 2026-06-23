
Исходники взяты отсюда (latest beta version) http://john.ccac.rwth-aachen.de:8000/ftp/as/source/c_version/

Весрия asl-current-142-bld301.tar.gz

Изменения:
- перед сборкой для Win32 копируется файл Makefile.def в папку исходников.
- в файл codez80.c в функции SwitchTo_Z80() добавлены строки перед default: 
    case e_core_z80u:
      code_segment_int_type = UInt24;
      break;
  для возможности собирать бинарники с размером больше 64к.
