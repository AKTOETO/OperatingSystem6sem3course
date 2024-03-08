#ifndef TASKS_H
#define TASKS_H

#include "includes.h"
#include "errors.h"
#include "args.h"

// типы процессов
typedef enum
{
    FOREGROUND = 0,
    BACKGROUND
} process_type;

// состояния процессов
typedef enum
{
    RUNNING = 0,
    FINISHED
} process_status;

// инфа по задаче
typedef struct 
{
    pid_t m_pid_id;         // номер процесса
    char** m_cmd;           // команда для его вызова (токенизированная строка)
    process_type m_type;    // тип процесса
    process_status m_status;
} task_t;

// хранение текущей foreground задачи и 
// всех background задач

// foreground задача
extern task_t g_fg_task;

// все background задачи
extern task_t* g_bg_task;      // массив
extern size_t g_bg_count;      // количество задач
extern size_t g_bg_capacity;   // вместимость массива

// добавление задачи
int addTask(pid_t pid, size_t argc, char **argv);

// проверка на запуск команды в background режиме
process_type getTaskType(char **argv);

// добавляем foreground задачу
int addForegroundTask(pid_t pid, char **argv);

// добавляем background задачу
int addBackgroundTask(pid_t pid, size_t argc, char **argv);

// убить foreground задачу
int killFGTask();

// убить background задачу
int killBGTask(task_t* src, bool(*f)(task_t* src, task_t* tt));

// убить все background задачи
int killAllBGTask();

// убить и удалить все задачи
int killAndDeleteAllBGTask();

// ждем выполнения foreground задачи
int waitFGTask();

// завершение всех задач
int quit();

// печать информации о всех background задачах
int printBGInfo();

#endif // !TASKS_H