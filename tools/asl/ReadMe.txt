
Исходники взяты отсюда (latest beta version) http://john.ccac.rwth-aachen.de:8000/ftp/as/source/c_version/

Изменения:
- перед сборкой для Win32 копируется файл Makefile.def в папку исходников.
- в файл codez80.c в функции SwitchTo_Z80() добавлены строки перед default: 
    case e_core_z80u:
      SegLimits[SegCode] = 0xffffffu;
      break;
  для возможности собирать бинарники с размером больше 64к.
