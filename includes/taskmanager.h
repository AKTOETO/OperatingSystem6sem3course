#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

/*
* Запуск, распределение, удаление, управление задачами
*/

#include "includes.h"
#include "args.h"
#include "utils.h"
#include "tasks.h"

// выполнение процесса
int launch(size_t argc, char **argv);

// обработка команд
int argvProcessing(char **argv, size_t argc);

// ЗАДАЧИ
// выход из оболочки
int out(size_t argc, char **argv);

// смена директории
int cd(size_t argc, char **argv);

// завершение всех процессов
int tka(size_t argc, char **argv);

// что-то очень долго выполняющееся
int watch(size_t argc, char **argv);

// skoof
int skoof(size_t argc, char **argv);

#endif