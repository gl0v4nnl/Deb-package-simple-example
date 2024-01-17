# Deb-package-simple-example

### Сборка
```
# Находимся в корневой папке проекта

~/poop$ sudo make all
gcc src/login.c -o libpoopl0gin.so -shared -fPIC
gcc src/input_check.c -o libpoopinputch3ck.so -shared -fPIC
gcc -c src/main.c 
gcc main.o -I/src libpoopl0gin.so libpoopinputch3ck.so -o poop.out
```

## Подготовка проекта и сборка пакета
### 0. Просмотр параметров проекта

```
# Первым делом надо посмотреть размер файлов программы, поскольку в данном случае файл один, достаточно посмотреть его размер

~/poop$ du -k ./poop.out
16      ./poop.out

# Дальше надо понять от каких пакетов будет зависеть ваша программа. Для этого воспользуйтесь командой objdump:
~/poop$ objdump -p ./poop.out | grep NEEDED
  NEEDED               libpoopl0gin.so
  NEEDED               libpoopinputch3ck.so
  NEEDED               libc.so.6

# Наши собранные библиотеки имеют собственные зависимости тоже. Поэтому их тоже нужно посмотреть:
~/poop$ objdump -p ./libpoopl0gin.so | grep NEEDED
  NEEDED               libc.so.6
~/poop$ objdump -p ./libpoopinputch3ck.so | grep NEEDED
  NEEDED               libc.so.6

# В нашем случае программе необходима только libc. Смотрим, в каком пакете она находится:
~/poop$ dpkg -S libc.so.6
libc6:amd64: /lib/x86_64-linux-gnu/libc.so.6
```

### 1. Создание манифеста

В каждом deb-пакете содержатся не только файлы самой программы, но и файл манифеста, в котором описан пакет, его зависимости и параметры. Этот файл имеет название ***control*** и должен находится в папке **DEBIAN**. Для сборки пакета будем использовать папку **package**, чтобы файлы программы не путались с исходными файлами и те не попали в пакет

```
# Создаем папку
~/poop$ mkdir -p package/DEBIAN

# Затем файл манифеста:
~/poop$ vi package/DEBIAN/control

# И наполняем следующим содержимым:
Package: poop
Version: 1.0
Section: uncrown
Priority: optional
Depends: libc6
Architecture: amd64
Essential: no
Installed-Size: 16 # значение из результата команды du -k ./poop.out
Maintainer: doob.poop <peep@doob.poop>
Description: Check random login from random user
```

Это минимальный набор параметров в файле манифеста. Вот их значение:
- **Package** - имя пакета;
- **Version** - версия программы в пакете, будет использована при обновлении пакета;
- **Section** - категория пакета, позволяет определить зачем он нужен;
- **Priority** - важность пакета, для новых пакетов, которые ни с чем не конфликтуют обычно прописывают **optional**, кроме того доступны значения **required**, **important** или **standard**;
- **Depends** - от каких пакетов зависит ваш пакет, он не может быть установлен, пока не установлены эти пакеты;
- **Recommends** - необязательные пакеты, но тем не менее они обычно устанавливаются по умолчанию в apt;
- **Conflicts** - пакет не будет установлен, пока в системе присутствуют перечисленные здесь пакеты;
- **Architecture** - архитектура системы, в которой можно установить этот пакет, доступные значения: **i386**, **amd64**, **all**, последнее означает, что архитектура не имеет значения;
- **Installed-Size** - общий размер программы после установки;
- **Maintainer** - указывает кто собрал этот пакет и кто отвечает за его поддержку;
- **Description** - краткое описание пакета.

### 2. Расположение файлов

Манифест готов. Теперь в папке пакета надо создать структуру папок, аналог того, что есть в корневой файловой системе. В данном случае:

```
# Создаем папку usr/bin и поместить туда исполняемый файл программы:
~/poop$ mkdir -p package/usr/bin
~/poop$ cp ./poop.out package/usr/bin

# Создаем папку usr/lib для наших разделяемых библиотек
~/poop$ mkdir -p package/usr/lib
~/poop$ cp ./libpoop* package/usr/lib
```

### 3. Скрипты установки
Несмотря на то, что система установки пакетов очень мощная и позволяет делать многое, некоторые вещи всё же сделать нельзя. Для решения этой проблемы была предусмотрена возможность выполнять скрипты перед установкой пакета и после. Аналогично это работает для удаления пакета - перед и после. Эти скрипты называются ***preinst***, ***postinst***, ***prerm*** и ***postrm***. Каждый файл просто содержит набор скриптов, которые надо выполнить. Например:

```
~/poop$ vi package/DEBIAN/postinst

#!/bin/bash
echo "Hello from poop installed"
```

Разработчики Debian не рекомендуют использовать эти скрипты без крайней надобности, поскольку они дают вам полный контроль над системой пользователя и вы можете случайно что-то повредить. Обычно эти скрипты используются для того чтобы задавать пользователям вопросы и на основе этого генерировать конфигурационные файлы.

### 4. Сборка и проверка пакета

Осталось собрать настроенный пакет:
```
~/poop$ dpkg-deb --build ./package
```

После завершения сборки устанавливаем его с помощью **apt**:
```
~/poop$ sudo apt install ~/package.deb
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
Note, selecting 'poop' instead of './package.deb'
The following NEW packages will be installed:
  poop
0 upgraded, 1 newly installed, 0 to remove and 8 not upgraded.
Need to get 0 B/4 454 B of archives.
After this operation, 16,4 kB of additional disk space will be used.
Get:1 /home/ivan/Projects/poop/package.deb poop amd64 1.0 [4 454 B]
Selecting previously unselected package poop.
(Reading database ... 281406 files and directories currently installed.)
Preparing to unpack .../ivan/Projects/poop/package.deb ...
Unpacking poop (1.0) ...
Setting up poop (1.0) ...
Hello from poop installed
```

Теперь проект можно запустить:
```
~/poop$ poop.out user
[check_login]: Логин не определен
```