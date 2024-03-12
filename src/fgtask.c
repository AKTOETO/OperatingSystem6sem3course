#include "fgtask.h"


// FOREGROUND
// массив возможных fg задач (не из PATH)
char *g_fg_task_list[FG_LIST_SIZE] = 
{
};

// проверка: переданная задача из main task list
int isItFromFgTaskList(char *task_name)
{
    // индекс задачи
    int i = 0;

    // проходимся по задачам и сравниваем
    for(; i < FG_LIST_SIZE; i++)
    {
        // если есть совпадение - возвращаем индекс
        if(strcmp(g_fg_task_list[i], task_name) == 0)
        {
            return i;
        }
    }

    // иначе возвращаем -1
    return -1;
}

// массив указателей на функции, которые могут быть исполнены в main 
int(*g_fg_task_func[FG_LIST_SIZE])(size_t, char **) =
{
};