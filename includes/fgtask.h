#ifndef FGTASK_H
#define FGTASK_H

#include "tasks.h"

// FOREGROUND
#define FG_LIST_SIZE 0

// массив возможных fg задач (не из PATH)
extern char *g_fg_task_list[FG_LIST_SIZE];

// проверка: переданная задача из main task list
int isItFromFgTaskList(char *task_name);

// массив указателей на функции, которые могут быть исполнены в main 
extern int(*g_fg_task_func[FG_LIST_SIZE])(size_t, char **);



#endif // !FGTASK_H
