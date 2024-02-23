# Операционные системы
Лабораторные работы по курсу операционные системы.

**3 курс. 6 сем.**

## Первая лабораторная работа *Архиватор*
1. Написать программу архиватор, на вход которой передается папка с файлами, результатом работы программы является файл, объединяющий все файлы из входной директории, а также содержащий заголовок с информацией об именах файлов и их размерах.
2. Написать программу разархиватор, разделяющий архивированный файл обратно на файлы.
Читать: 
>Глава 3. Работа с файлами.

## Решение

**Структура заархивированного файла**
```
[number of files]
[file name]
[file size]
...
[file name]
[file size]
[file data]...[file data]
```
Где:
1. `number of files` - количество заархивированных файлов;
2. `file name` - имя файла;
3. `file size` - размер файла в байтах;
4. `file data` - информация в файле.

## Алгоритм поиска файлов в каталоге
1. Считаю количество файлов в каталоге;
2. Выделяю память под все файлы;
3. Заново прохожусь по файлам и считываю пути;
