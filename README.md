# Операционные системы
Лабораторные работы по курсу операционные системы.

**3 курс. 6 сем.**

# Вариант 3, 4

## Лабораторная работа №2. Управление процессом

1. Написать программу «терминал», которая анализирует входную строку и при 
обнаружении ключевых слов «ls», «cat», «nice» и «killall» запускает 
соответствующие процессы.
2. В программе реализовать возможность запуска процессов других программ, 
например, браузера.
3. Написать обработчики сигналов, например, при получении сигнала CTRL+C
завершить запущенный программой процесс.
4. Заменить системный bash на собственный терминал.

Литература: Основы программирования в Linux. Автор: Мэтью Нейл, Стоунс 
Ричард. Глава 11. Процессы и сигналы.

Варианты: 
1. Программа хранит все открытые процессы. Запустить 3 программы через 
терминал, сигнал CTRL+C должен закрывать программы в 
последовательности FIFO
2. Программа хранит все открытые процессы. Запустить 3 программы через 
терминал, сигнал CTRL+C должен закрывать программы в 
последовательности LIFO
3. ***Программа хранит все открытые процессы. Обеспечить возможность 
завершения процесса командой exit с указанием номера процесса.***
4. ***Программа хранит все открытые процессы. Завершаются сразу все 
процессы.***
5. Реализовать пакетный, одновременный запуск программ.
6. Реализовать возможность приостановки и продолжения выполнения 
процессов через терминал.
7. Добавить функционал для управления приоритетами процессов.
8. Разработать механизм автоматического перезапуска процесса в случае 
его аварийного завершения.
9. Реализовать логирование действий терминала и выполненных команд.
10. Создать механизм для уведомления пользователя о завершении 
выполнения процесса.

### Алгоритм работы
TODO

### Типы процессов
Всего существует 3 типа процессов:
- main
- foreground
- background

**Main** процесс позволяет запустить функцию в главном процессе, без вызова функций `fork()` и `execvp()`;

**Foreground** процесс запускает функцию в новом процессе и ждет ее выполнения. Здесь используются функции `fork()` и `execvp()`;

**Background** тоже самое, что и *Foreground*, только все добавленные процессы хранятся в массиве задач. При добавление такого типа задачи, ожидание ее завершения не происходит.