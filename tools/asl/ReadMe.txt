
Исходники взяты отсюда (latest beta version) http://john.ccac.rwth-aachen.de:8000/ftp/as/source/c_version/

Изменения:
- перед сборкой для Win32 копируется файл Makefile.def в папку исходников.
- (для сборки 262) файл codez80.c в функции CodeEnd() строка "else Type = UInt16;" изменена на "else Type = UInt32;"
  для возможности собирать бинарники с размером больше 64к.
- файл codez80.c в функции SwitchTo_Z80() добавлена проверка 
  else if (MomCPU == CPUZ80U) SegLimits[SegCode] = 0xfffffffful;
  для возможности собирать бинарники с размером больше 64к.

