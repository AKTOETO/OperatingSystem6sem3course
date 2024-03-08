#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

/*
* Запуск, распределение, удаление, управление задачами
*/

#include "includes.h"
#include "args.h"
#include "utils.h"
#include "tasks.h"

#define TASK_COMPLETE 2
#define TASK_UNKNOWN 1
#define TASK_EXIT -1

// выполнение процесса
int launch(size_t argc, char **argv);

// обработка команд
int argvProcessing(char **argv, size_t argc);

// задачи, выполняемые в главном процессе
int mainProcTask(size_t argc, char **argv);

// задачи, выполняемые в дочернем процессе
int childProcTask(size_t argc, char **argv);

// ЗАДАЧИ
// выход из оболочки
int out(size_t argc, char **argv);

// смена директории
int cd(size_t argc, char **argv);

// завершение всех процессов
int tka(size_t argc, char **argv);

// что-то очень долго выполняющееся
int watch(size_t argc, char **argv);

// печать информации о background процессах
int pbg(size_t argc, char **argv);

// печать информации о доступных командах
int help(size_t argc, char **argv);

// skoof
int skoof(size_t argc, char **argv);

#endif