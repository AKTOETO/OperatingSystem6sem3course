#include "bgtask.h"

// BACKGROUND
// long process
int watch(size_t argc, char **argv)
{
    fprintf(stdout,"\n%s\n","Процесс watch начат");
    for(int i = 0; i < 10; i++)
    {
        fprintf(stdout, "\nwatch i:%d\n", i);
        sleep(1);
    }
    fprintf(stdout,"\n%s\n","Процесс watch закончен");
    return 0;
}


// BACKGROUND
// массив возможных bg задач (не из PATH)
char *g_bg_task_list[BG_LIST_SIZE] = 
{
    "watch"
};

// проверка: переданная задача из bg task list
int isItFromBgTaskList(char *task_name)
{
    // индекс задачи
    int i = 0;

    // проходимся по задачам и сравниваем
    for(; i < BG_LIST_SIZE; i++)
    {
        // если есть совпадение - возвращаем индекс
        if(strcmp(g_bg_task_list[i], task_name) == 0)
        {
            return i;
        }
    }

    // иначе возвращаем -1
    return -1;
}

// проверка: есть ли символ & в конце аргументов
bool isItBackground(size_t* argc, char **argv)
{
    // проверяем на наличие токенов
    if(argc <= 0 || argv == NULL)
        ERRORS("Недостаточно токенов");

    // берем последний токен
    // если он &, удаляем его (NULL а его место), возвращаем true
    if(argv[(*argc) - 1][0] == '&')
    {
        argv[--(*argc)] = NULL;
        return true;
    }

    // иначе возвращаем false
    return false;
}