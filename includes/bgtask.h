#ifndef BGTASK_H
#define BGTASK_H

#include "tasks.h"

// BACKGROUND
// что-то очень долго выполняющееся
int watch(size_t argc, char **argv);


// BACKGROUND
// размер массива задач bg
#define BG_LIST_SIZE 1

// массив возможных bg задач (не из PATH)
extern char *g_bg_task_list[BG_LIST_SIZE];

// проверка: переданная задача из bg task list
int isItFromBgTaskList(char *task_name);

// массив указателей на функции, которые могут быть исполнены в bg 
extern int(*g_bg_task_func[BG_LIST_SIZE])(size_t, char **);

// проверка: есть ли символ & в конце аргументов
bool isItBackground(size_t* argc, char **argv);

#endif // !BGTASK_H