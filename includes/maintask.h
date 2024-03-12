#ifndef MAINTASK_H
#define MAINTASK_H

#include "bgtask.h"
#include "fgtask.h"

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
int exitt(size_t argc, char **argv);

// убить background задачу
int killBGTask(task_t* src, bool(*f)(task_t* src, task_t* tt));

// МАССИВЫ СООТВЕТСТВИЙ КОМАНД
// MAIN
#define MAIN_LIST_SIZE 7

// массив возможных bg задач (не из PATH)
extern char *g_main_task_list[MAIN_LIST_SIZE];

// проверка: переданная задача из bg task list
int isItFromMainTaskList(char *task_name);

// массив указателей на функции, которые могут быть исполнены в bg 
extern int(*g_main_task_func[MAIN_LIST_SIZE])(size_t, char **);


#endif // !MAINTASK_H