#ifndef TASKS_H
#define TASKS_H

#include "includes.h"
#include "errors.h"
#include "args.h"

// типы процессов
typedef enum
{
    MAIN = 0,
    FOREGROUND,
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
    size_t m_argc;          // количество токенов
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

#endif // !TASKS_H