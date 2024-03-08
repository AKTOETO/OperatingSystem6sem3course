#include "tasks.h"

// foreground задача
task_t g_fg_task;

// все background задачи
task_t* g_bg_task = NULL;      // массив
size_t g_bg_count = 0;      // количество задач
size_t g_bg_capacity = 0;   // вместимость массива

int addTask(pid_t pid, char **argv)
{
    // получаем тип процесса
    process_type type = getTaskType(argv);

    // сохраняем его 
    if(type == FOREGROUND)
    {
        addForegroundTask(pid, argv);
        waitFGTask();
    }
    else
    {
        addBackgroundTask(pid, argv);
    }

    return 0;
}

process_type getTaskType(char **argv)
{
    // индекс последнего аргумента
    size_t last_arg = 0;
    while(argv[last_arg] != NULL)
    {
        last_arg++;
    }

    // если есть символ &, тогда это background задача
    if(strcmp(argv[--last_arg], "&") == 0)
    {
        // удаляем символ &
        argv[last_arg] = NULL;
        INFO("Тип процесса: %s\n", "BACKGROUND");
        return BACKGROUND;
    }
    INFO("Тип процесса: %s\n", "FOREGROUND");
    return FOREGROUND;
}

int addForegroundTask(pid_t pid, char **argv)
{
    // сохраняем информацию о процессе
    g_fg_task.m_cmd = argv;
    g_fg_task.m_pid_id = pid;
    g_fg_task.m_type = FOREGROUND;
    g_fg_task.m_status = RUNNING;

    INFO("Foreground задача с pid: %d добавлена\n", pid);

    return 0;
}

int addBackgroundTask(pid_t pid, char **argv)
{
    // проверяем, есть ли мест для добавления процесса
    if(g_bg_count >= g_bg_capacity)
    {
        // новый размер 
        g_bg_capacity += TASK_ARR_SIZE;

        // перевыделяем память
        g_bg_task = (task_t*)realloc(g_bg_task, g_bg_capacity * sizeof(task_t));
        INFO("Перевыделена память под новый массив Background задач размером: %ld\n", g_bg_capacity);
    }

    // добавляем новую задачу
    g_bg_task[g_bg_count].m_cmd = argv;
    g_bg_task[g_bg_count].m_pid_id = pid;
    g_bg_task[g_bg_count].m_type = BACKGROUND;
    g_bg_task[g_bg_count++].m_status = RUNNING;

    INFO("Добавлена Background задача на место %ld в массиве задaч\n", g_bg_count - 1);
    return 0;
}

int killBGTask(task_t* src, bool (*f)(task_t*, task_t*))
{
    // указатель процесса
    size_t i = 0;

    // проходимся по всем задачам и ищем подходящую
    for(; i < g_bg_count; i++)
    {
        // если переданная функция верна
        // (то есть такой процесс есть в списке дочерних процессов)
        // удаляем процесс
        if(f(src, &g_bg_task[i]))
        {
            if(kill(g_bg_task[i].m_pid_id, SIGTERM) != 0)
            {
                ERROR("Процесс с pid: %d не был корректно завершен\n", g_bg_task[i].m_pid_id);
                g_bg_task[i].m_status = FINISHED;
                return 0;
            }
            else
            {
                INFO("Задача с pid: %d завершена успешно\n", g_bg_task[i].m_pid_id);
            }
        }
    }

    // если процесс не был найден
    if(i == g_bg_count)
    {
        ERRORS("Не был найден такой процесс\n");
    }
    return 0;
}

int killAllBGTask()
{
    // проходимся по всем дочерним задачам и завершаем их
    for(size_t i = 0; i < g_bg_count; i++)
    {
        if(kill(g_bg_task[i].m_pid_id, SIGTERM) != 0)
        {
            ERROR("Процесс с pid: %d не был корректно завершен\n", g_bg_task[i].m_pid_id);
            g_bg_task[i].m_status = FINISHED;
        }
    }
    INFOS("Все Background задачи убиты\n");
    return 0;
}

int killAndDeleteAllBGTask()
{
    killAllBGTask();
    free(g_bg_task);
    g_bg_capacity = 0;
    g_bg_count = 0;

    INFOS("Все Background задачи удалены\n");
    return 0;
}

int waitFGTask()
{
    // если задача еще не завершена
    if(g_fg_task.m_status != FINISHED)
    {
        // ждем его завершения
        if(waitpid(g_fg_task.m_pid_id, NULL, 0) == -1)
        {
            ERROR("Что-то пошло не так с закрытием дочернего процесса pid: %d\n", g_fg_task.m_pid_id);
        }
        g_fg_task.m_status = FINISHED;

        INFOS("Foreground процесс завершен\n");
    }
    else
    {
        INFOS("Задача уже завершена\n");
    }
    return 0;
}
