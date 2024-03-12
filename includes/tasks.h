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


// ЗАДАЧИ
// MAIN
// выход из оболочки
int out(size_t argc, char **argv);

// смена директории
int cd(size_t argc, char **argv);

// завершение всех процессов
int tka(size_t argc, char **argv);

// печать информации о background процессах
int pbg(size_t argc, char **argv);

// печать информации о доступных командах
int help(size_t argc, char **argv);

// skoof
int skoof(size_t argc, char **argv);

// завершить процесс по пид
int pidkill(size_t argc, char **argv);

// FOREGROUND

// BACKGROUND
// что-то очень долго выполняющееся
int watch(size_t argc, char **argv);

// КОНЕЦ ЗАДАЧ



// МАССИВЫ СООТВЕТСТВИЙ КОМАНД
// MAIN
#define MAIN_LIST_SIZE 7

// массив возможных bg задач (не из PATH)
extern char *g_main_task_list[MAIN_LIST_SIZE];

// проверка: переданная задача из bg task list
int isItFromMainTaskList(char *task_name);

// массив указателей на функции, которые могут быть исполнены в bg 
extern int(*g_main_task_func[MAIN_LIST_SIZE])(size_t, char **);



// FOREGROUND
#define FG_LIST_SIZE 0

// массив возможных fg задач (не из PATH)
extern char *g_fg_task_list[FG_LIST_SIZE];

// проверка: переданная задача из main task list
int isItFromFgTaskList(char *task_name);

// массив указателей на функции, которые могут быть исполнены в main 
extern int(*g_fg_task_func[FG_LIST_SIZE])(size_t, char **);



// BACKGROUND
// размер массива задач bg
#define BG_LIST_SIZE 1

// массив возможных bg задач (не из PATH)
extern char *g_bg_task_list[BG_LIST_SIZE];

// проверка: переданная задача из bg task list
int isItFromBgTaskList(char *task_name);

// массив указателей на функции, которые могут быть исполнены в bg 
extern int(*g_bg_task_func[BG_LIST_SIZE])(size_t, char **);

// КОНЕЦ МАССИВОВ СООТВЕТСТВИЙ КОМАНД

// создание fg процесса
int createForegroundTask(size_t argc, char **argv);

// создание bg процесса
int createBackgroundTask(size_t argc, char **argv);


// добавляем foreground задачу
int addForegroundTask(pid_t pid, size_t argc, char **argv);

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

// ждем выполнения background задачи без блокировки главного процесса
int waitBGTask(); 

// завершение всех задач
int quit();

// печать информации о всех background задачах
int printBGInfo();

#endif // !TASKS_H