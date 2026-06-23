# Компьютер-конструктор ZX Evolution     

##### Компьютер-конструктор ZX Evolution!

#### [english](http://nedopc.com/zxevo/zxevo_eng.php)

**ВНИМАНИЕ!** **Изделие поставляется независимым производителем. Обращаться по поводу приобретения к Виталию [tetroid@inbox.ru](mailto:tetroid@inbox.ru).**

- - -

[![zxevo rev.C](https://raw.githubusercontent.com/aaydev/zxevo.pentevo/main/photos/zxevo_top_c.jpg)](http://nedopc.com/zxevo/zxevo_top_c.jpg)

[![zxevo rev.C](https://raw.githubusercontent.com/aaydev/zxevo.pentevo/main/photos/zxevo_bot_c.jpg)](http://nedopc.com/zxevo/zxevo_bot_c.jpg)

[![zxevo rev.C](https://raw.githubusercontent.com/aaydev/zxevo.pentevo/main/photos/zxevo_mnt_c.jpg)](http://nedopc.com/zxevo/zxevo_mnt_c.jpg)

[![zxevo rev.C](https://raw.githubusercontent.com/aaydev/zxevo.pentevo/main/photos/zxevo_front_c.jpg)](http://nedopc.com/zxevo/zxevo_front_c.jpg)

Компьютер конструктор **ZX Evolution** является развитием Spectrum-совместимых компьютеров, но собран на современной элементной базе и под современную периферию.

**Разработка:**  
hardware: Вадим Акимов (LVD), Чунин Роман (CHRV), Дмитриев Дмитрий (DDP);  
software: Вячеслав Савенков (Savelij).

### Характеристики:

*   Z80 3.5 МГц (классический)/ 7 МГц (турбо режим без WAIT)/ 14МГц (мега турбо режим с WAIT);
*   4 МБ ОЗУ, 512КБ ПЗУ;
*   Форм-фактор miniITX (microATX для rev.A и B), 2 слота ZXBUS, питание ATX или +5,+12В;
*   Гибкая архитектура, основанная на fpga (EP1K50);
*   Периферийный контроллер ATMEGA128;
*   PS/2 интерфейс клавиатуры и мыши;
*   Интерфейсы Floppy (1818ВГ93), IDE (один канал, два устройства master/slave), SD(HC) карта, RS232;
*   Видеовыходы RGB, VGA (аппаратный скандаблер);
*   Звуковые интерфейсы AY, Beeper, Covox (аппаратный ШИМ);
*   Поддержка механической клавиатуры и джойстика;
*   Поддержка Tape интерфейса (магнитофонный вход/выход);
*   Энергонезависимые часы/календарь.

Список мониторов протестированных на совместимость с ZX Evolution в VGA режиме [здесь](http://svn.nedopc.com/dl.php?repname=pentevo&path=%2Fdocs%2Fsupported_monitors.pdf).

Тестовая программа для PC для тестирования монитора на совместимость [здесь](http://nedopc.com/zxevo/tools/zxevo_vga_test_20100221.rar).

### Документация:

**_Форум поддержки пользователей_** [здесь](http://forum.nedopc.com/).

Принципиальная схема (revision C) [здесь](http://nedopc.com/zxevo/zxevo_sch_revc.pdf).

Монтажная схема (revision C) [здесь](http://nedopc.com/zxevo/zxevo_mon_revc.pdf).

Cписок деталей (revision C) [здесь](http://nedopc.com/zxevo/zxevo_bom_revc.pdf).

Руководство пользователя (revision C) [здесь](http://nedopc.com/zxevo/zxevo_user_manual_revc.pdf).

Руководство по сборке платы (revision C) [здесь](http://nedopc.com/zxevo/zxevo_solder_revc.pdf).

Документация для производства (revision C) [здесь](http://nedopc.com/zxevo/zxevo_revc.zip) \[PCAD2002\] (использование в коммерческих целях только по договоренности с представителями [NedoPC](http://nedopc.com/aboutus.php)).

**ЦЕНЫ:** [здесь](http://nedopc.com/catalog.php).

**Внимание!**  
Описание стартового меню (EVO RESET SERVICE), документация программиста (для конфигурации BASECONF) и документация по процедуре обновления прошивок (BOOTLOADER) находится в таблице **"Прошивки и Документация"**

### Прошивки и Документация:

|     |     |     |     |     |     |
| --- | --- | --- | --- | --- | --- |
| **Название** | **Версия** | **Назначение** | **Документация** | **Файлы** | **SVN** |
| **_EVO RESET SERVICE FE,  <br>EVO DOS FE,  <br>EVO PROF_** | latest | Улучшен эмулятор дисководов, теперь он работает с неизменной пзу TR-DOS.  <br>Для работы требуется соответствующая прошивка FPGA  <br>Стартовое меню, сервисные расширения и ZX ПЗУ.  <br>Предназначена для прошивки в ROM.  <br>_Прошивку можно произвести из предыдущей версии или с помощью **TEST&SERVICE**._  <br>Поддержка: Savelij. | [Описание сервис-прошивки ''EVO Reset Service''.pdf](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Fdocs%2F%D0%9E%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5+%D1%81%D0%B5%D1%80%D0%B2%D0%B8%D1%81-%D0%BF%D1%80%D0%BE%D1%88%D0%B8%D0%B2%D0%BA%D0%B8+%27%27EVO+Reset+Service%27%27.pdf) |     | [zxevo\_fe.rom](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2From%2Fzxevo_fe.rom) |
| **_BASECONF FE_** | latest | Основная рабочая конфигурация (от NedoPC).  <br><br>*   Поддержка эмуляции портов дисковода/ВГ93, для использования этой фичи нужен ROM zxevo\_fe.rom.<br>*   Добавлена палитра DDP без ШИМа (alone).<br>*   Ускорена работа процессора на 14 МГц (psk7).<br><br>Предназначена для прошивки в ATMEGA128.  <br>_Прошивка производится с помощью **BOOTLOADER**._  <br>Поддержка: CHRV (ATMEGA128), LVD (EP1K50QC208). | [zxevo\_base\_configuration.odt](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Fdocs%2Fzxevo_base_configuration.odt)<br><br>[zxevo\_base\_configuration.pdf](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Fdocs%2Fzxevo_base_configuration.pdf) |     | [zxevo\_fw.bin](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Fcfgs%2Fstandalone_base_trdemu%2Ftrunk%2Fzxevo_fw.bin) |
| **_BOOTLOADER_** | 2012.01.24 | Загружает и обновляет рабочие конфигурации.  <br>Предназначена для прошивки в ATMEGA128.  <br>_Прошивка производится с помощью программатора._  <br>Поддержка: DDp. | [zxevo\_firmware\_update.pdf](http://nedopc.com/zxevo/rom/zxevo_firmware_update.pdf) | [bootloader20120124.zip](http://nedopc.com/zxevo/rom/bootloader20120124.zip) | [zxevo\_bl.hex](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Favrboot%2Ftrunk%2Favr%2Fzxevo_bl.hex)<br><br>[zxevo\_bl.e2p](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Favrboot%2Ftrunk%2Favr%2Fzxevo_bl.e2p)<br><br>[read\_me.txt](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Favrboot%2Ftrunk%2Fread_me.txt) |
| **_TEST&SERVICE_** | 2016.01.04 | Cервисная конфигурация для тестирования и настройки ZXEvo. Полезная конфигурация для тех кто сам собирает ZXEvo.  <br>Предназначена для прошивки в ATMEGA128.  <br>_Прошивка производится с помощью **BOOTLOADER**._  <br>Поддержка: DDp. | [zxevo\_testservice.pdf](http://nedopc.com/zxevo/rom/zxevo_testservice.pdf)  <br>[zxevo\_testservice\_flash.pdf](http://nedopc.com/zxevo/rom/zxevo_testservice_flash.pdf) | [testservice20160104.zip](http://nedopc.com/zxevo/rom/testservice20160104.zip) | [zxevo\_fw.bin](http://svn.nedopc.com/filedetails.php?repname=pentevo&path=%2Ftest_n_service%2Ftrunk%2Frus%2Fzxevo_fw.bin) |

Зеркало исходников (SVN) доступно на сайте [**ZXEVO.RU**.](http://svn.zxevo.ru/listing.php?repname=pentevo)

### Прошивки сторонних разработчиков:

**ВНИМАНИЕ:** Вопросы по поддержке прошивок задавайте непосредственно авторам прошивок. NedoPC не несет ответственности за ошибки или недоработки в прошивках сторонних разработчиков.

|     |     |     |     |     |
| --- | --- | --- | --- | --- |
| **Название** | **Версия** | **Назначение** | **Документация** | **Файлы** |
| [**_ZXEVO TS_**](http://forum.tslabs.info/viewtopic.php?f=12&t=20) |     | ZXEVO TS конфигурация с поддержкой спрайтов и оригинальных расширенных видеорежимов. Полноценная рабочая конфигурации отличается не только схемой, но и ROM. Также под нее созданы специализированное утилиты, игры и прочее ПО. [Подробности здесь.](http://forum.tslabs.info/viewtopic.php?f=12&t=20) | [FAQ](http://forum.tslabs.info/viewtopic.php?f=12&t=157) | [Инструкция по установке конфигурации.](http://forum.nedopc.com/viewtopic.php?f=30&t=622&start=652) |
| **_ScorpEvo_** | 6.1 (05.11.2011) | Scorpion Evolution (ScorpEvo) задуман как полноценная альтернатива (в дальнейшем – замена) самого знаменитого и в то же время противоречивого петербургского компьютера «Scorpion ZS 256 turbo+» Сергея Зонова. | [scorpevo.pdf](http://nedopc.com/zxevo/scorpion/scorpevo.pdf) | [scorpevo061.zip](http://nedopc.com/zxevo/scorpion/scorpevo061.zip)  <br>[svn scorpevo](http://svn.nedopc.com/listing.php?repname=pentevo&path=%2Fscorpevo%2F) |

- - -

### История версий:

**Revision C:**  
Изменения от предыдущей версии:

*   Форм фактор miniITX (172x170mm);
*   Убран PAL кодер;
*   Добавлен принтерный интерфейс по стандарту AY-принтер;
*   Добавлено управление множителем частоты;
*   Добавлены аудиовходы (всего 3 аудиовхода);
*   Используется процессор Z80 в QFP корпусе;
*   Убран отдельный RGB разъем (все выведено на VGA);
*   Добавлен RS232-USB мост;
*   Убран AT разъем питания;
*   Добавлен разъем для подключения PAL кодера.

  

**Revision B:**  
Изменения от предыдущей версии:

*   Исправлен шаблон PS/2 клавиатуры-мыши;
*   Исправлен ОЕ на ICS501;
*   Увеличена площадь радиатора 78M05;
*   Добавлен джампер регулирующий присутствие 12V на слотах;
*   Добавлены направляющие для разъема ATX питания;
*   Увеличены контактные площадки дросселей;
*   Уменьшен размер платы;
*   Убран джампер разрешающий програмирование контроллера;
*   Сделано для диодов 4148 универсальная площадка (на смд и выводные);
*   Для ZXBUS сделан указатель 1-го вывода;
*   Исправлена ошибка сброса ВГ93 (завел VG\_RES на D19);
*   Сделан ключ разъема FDD как на ПЦ платах (5-ый контакт);
*   Добавлены на плату надписи "HDD Led", "PWR Led", "ZX Keyboard/ZX Joystick", "Slot1", "Slot2";
*   Перемещен разъем VGA, чтобы был паралельно RGB-out.

Принципиальная схема (revision B) [здесь](http://nedopc.com/zxevo/zxevo_sch_revb.pdf).

Монтажная схема (revision B) [здесь](http://nedopc.com/zxevo/zxevo_mon_revb.pdf).

Cписок деталей (revision B) [здесь](http://nedopc.com/zxevo/zxevo_bom_revb.pdf).

Доработка для работы с принтером [AY-printer](http://nedopc.com/zxevo/zxevo_ay_printer.pdf).

Руководство пользователя (revision B) [здесь](http://nedopc.com/zxevo/zxevo_user_manual_revb.pdf).

Руководство по сборке платы (revision B) [здесь](http://nedopc.com/zxevo/zxevo_solderind_and_start_revb.pdf).

Документация для производства (revision B) [здесь](http://nedopc.com/zxevo/zxevo_revb.zip) \[PCAD2002\] (использование в коммерческих целях только по договоренности с представителями [NedoPC](http://nedopc.com/aboutus.php)).

  

**Revision A:**  
Первая версия - экспериментальная, только у разработчиков.

Принципиальная схема (revision A) [здесь](http://nedopc.com/zxevo/zxevo_sch_reva.pdf).

Монтажная схема (revision A) [здесь](http://nedopc.com/zxevo/zxevo_mon_reva.pdf).

Cписок деталей (revision A) [здесь](http://nedopc.com/zxevo/zxevo_detali_reva.pdf).

Документация для производства (revision A) [здесь](http://nedopc.com/zxevo/zxevo_reva.zip) \[PCAD2002\] (использование в коммерческих целях только по договоренности с представителями [NedoPC](http://nedopc.com/aboutus.php)).

- - -

Email: [nedopc@mail.ru](mailto:nedopc@mail.ru), [nedopc@yandex.ru](mailto:nedopc@yandex.ru).
