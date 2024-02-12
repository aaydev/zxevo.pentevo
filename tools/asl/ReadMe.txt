
Исходники взяты отсюда (latest beta version) http://john.ccac.rwth-aachen.de:8000/ftp/as/source/c_version/

Изменения:
- перед сборкой для Win32 копируется файл Makefile.def в папку исходников.
- файл codez80.c в функции CodeEnd() строка "else Type = UInt16;" изменена на "else Type = UInt32;"
  для возможности собирать бинарники с размером больше 64к.
