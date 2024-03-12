#include "taskmanager.h"

int createTask(size_t argc, char **argv,
               int (*parent)(pid_t, size_t argc, char **argv),
               int (*child)(size_t argc, char **argv))
{
    INFOS("Создание задачи\n");   

    // добавление fg процесса

    // получаю pid нового процесса
    pid_t pid = fork();

    // проверяю три возможные ситуации
    // 1) поток не был создан
    if(pid < 0)
    {
        ERRORS("Поток не был создан\n");
    }
    // 2) сейчас выполняется дочерний процесс
    if(pid == 0)
    {
        // вызов обработки создание дочернего процесса
        child(argc, argv);
            
        // выход из дочернего процесса
        exit(0);
    }
    // 3) сейчас выполняется родительский процесс
    else
    {
        // вызов обработки нового процесса в родительском объекте
        parent(pid, argc, argv);
    }
            
    return 0;
}

int fgProcChild(size_t argc, char **argv)
{
    // индекс задачи из подготовленного списка возможных задач
    int ind = 0;

    // попытка запустить нужную программу
    // проверка: задача для bg из массива или нет
    if((ind = isItFromFgTaskList(argv[0])) != -1)
    {   
        INFO("Запускаем задачу g_fg_task_func[%d]\n", ind);

        // запускаем соответствующую функцию
        g_fg_task_func[ind](argc, argv);
    }

    // если задача не из массива - запускаем execvp
    // проверяем, запустился ли код 
    else if(execvp(argv[0], argv) == -1)
    {
        // код не был запущен
        ERROR("Execvp не смогла запустить: [%s]\n", argv[0]);
    }
    // если же код не -1, тогда процесс успешно запущен
    else
    {
        INFO("Процесс [%s] успешно запущен\n", argv[0]);
    }
    return 0;
}

int fgProcParent(pid_t pid, size_t argc, char **argv)
{
    // сохранение fg процесса в массив
    addForegroundTask(pid, argc, argv);

    // ожидание завершения fg процесса
    waitFGTask();
    return 0;
}

int bgProcChild(size_t argc, char **argv)
{
    // индекс задачи из подготовленного списка возможных задач
    int ind = 0;

    // попытка запустить нужную программу
    // проверка: задача для bg из массива или нет
    if((ind = isItFromBgTaskList(argv[0])) != -1)
    {   
        INFO("Запускаем задачу g_bg_task_func[%d]\n", ind);

        // запускаем соответствующую функцию
        g_bg_task_func[ind](argc, argv);
    }

    // если задача не из массива - запускаем execvp
    // проверяем, запустился ли код 
    else if(execvp(argv[0], argv) == -1)
    {
        // код не был запущен
        ERROR("Execvp не смогла запустить: [%s]\n", argv[0]);
    }
    // если же код не -1, тогда процесс успешно запущен
    else
    {
        INFO("Процесс [%s] успешно запущен\n", argv[0]);
    }
    return 0;
}

int bgProcParent(pid_t pid, size_t argc, char **argv)
{
    printTokens(argv);

    // сохранение bg процесса в массив
    addBackgroundTask(pid, argc, argv);
    return 0;
}

int createForegroundTask(size_t argc, char **argv)
{
    INFOS("Создание fg задачи\n");
    createTask(argc, argv, fgProcParent, fgProcChild); 
    return 0;
}

int createBackgroundTask(size_t argc, char **argv)
{
    INFOS("Создание bg задачи\n");
    createTask(argc, argv, bgProcParent, bgProcChild);
    return 0;
}


// массив указателей на функции, которые могут быть исполнены в bg 
int(*g_bg_task_func[BG_LIST_SIZE])(size_t, char **) = 
{
    watch
};


// КОНЕЦ МАССИВОВ СООТВЕТСТВИЙ КОМАНД




int addForegroundTask(pid_t pid, size_t argc, char **argv)
{
    // сохраняем информацию о процессе
    g_fg_task.m_argc = argc;
    g_fg_task.m_cmd = argv;
    g_fg_task.m_pid_id = pid;
    g_fg_task.m_type = FOREGROUND;
    g_fg_task.m_status = RUNNING;

    INFO("Foreground задача с pid: %d добавлена\n", pid);

    return 0;
}

int addBackgroundTask(pid_t pid, size_t argc, char **argv)
{
    // проверяем, есть ли место для добавления процесса
    if(g_bg_count >= g_bg_capacity)
    {
        // новый размер 
        g_bg_capacity += TASK_ARR_SIZE;

        // перевыделяем память
        g_bg_task = (task_t*)realloc(g_bg_task, g_bg_capacity * sizeof(task_t));
        INFO("Перевыделена память под новый массив Background задач размером: %ld\n", g_bg_capacity);
    }

    // добавляем новую задачу
    g_bg_task[g_bg_count].m_argc = argc;
    // дублирование токенов
    g_bg_task[g_bg_count].m_cmd = dublicateToken(argc, argv);

    g_bg_task[g_bg_count].m_pid_id = pid;
    g_bg_task[g_bg_count].m_type = BACKGROUND;
    g_bg_task[g_bg_count].m_status = RUNNING;

    // переход к следующему элементу
    g_bg_count++;

    INFO("Добавлена Background задача на место %ld в массиве задaч\n", g_bg_count - 1);
    return 0;
}

int killFGTask()
{
    // НЕ РАСКОММЕНТИРОВАТЬ - ПАДАЕТ ГРАФИЧЕСКИЙ ИНТЕРФЕЙС
    // if(kill(g_fg_task.m_pid_id, SIGTERM) != 0)
    // {
    //     ERROR("Процесс с pid: %d не был корректно завершен\n", g_fg_task.m_pid_id);
    //     g_fg_task.m_status = FINISHED;
    // }
    // else
    // {
    //     INFO("Процесс PID:%d завершен\n", g_fg_task.m_pid_id);
    // }
    // INFO("Процесс PID:%d завершен\n", g_fg_task.m_pid_id);
    return 0;
}

int killAllBGTask()
{
    // проходимся по всем дочерним задачам и завершаем их
    for(size_t i = 0; i < g_bg_count; i++)
    {
        if(kill(g_bg_task[i].m_pid_id, SIGKILL) != 0)
        {
            ERROR("Процесс с pid: %d не был корректно завершен\n", g_bg_task[i].m_pid_id);
        }
        else
        {
            INFO("Процесс PID:%d завершен\n", g_bg_task[i].m_pid_id);
        }
        g_bg_task[i].m_status = FINISHED;
    }
    INFO("%s\n", "Все Background задачи убиты");
    return 0;
}

int killAndDeleteAllBGTask()
{
    killAllBGTask();
    for(int i = 0; i < g_bg_count; i++)     
    {
        int j = 0;
        while(g_bg_task[i].m_cmd[j])
        {
            free(g_bg_task[i].m_cmd[j++]);
        }
        free(g_bg_task[i].m_cmd);
    }

    g_bg_capacity = 0;
    g_bg_count = 0;

    INFOS("Все Background задачи удалены\n");
    return 0;
}

int waitFGTask()
{
    // если задача еще не завершена
    if(g_fg_task.m_pid_id != -1 && g_fg_task.m_status != FINISHED)
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

int waitBGTask()
{
    // если задача еще не завершена
    if(g_bg_task[g_bg_count - 1].m_pid_id != -1 && g_bg_task[g_bg_count - 1].m_status != FINISHED)
    {
        // статус завершения задачи
        int status = 0;

        // ждем его завершения
        if(waitpid(g_bg_task[g_bg_count - 1].m_pid_id, &status, WNOHANG) == -1)
        {
            ERROR("Что-то пошло не так с закрытием дочернего процесса pid: %d\n", g_fg_task.m_pid_id);
        }
        g_fg_task.m_status = FINISHED;

        INFOS("Background процесс завершен\n");
        status = WEXITSTATUS(status);
        INFO("Статус завершения процесса: %d\n", status);
        // получение статуса завершения программы
        return status;
    }
    else
    {
        INFOS("Задача уже завершена\n");
    }
    return 0;
}

int quit()
{
    INFOS("Выполняется завершение работы\n");
    waitFGTask();
    killAndDeleteAllBGTask();
    exit(0);
    return 0;
}

int printBGInfo()
{
    fprintf(stdout, "Количество background задач: %ld\n", g_bg_count);
    fprintf(stdout, "Вместимость массива задач: %ld\n", g_bg_capacity);
    fprintf(stdout, "Информация по каждой задаче:\n\n");

    /*
    {
        pid_t m_pid_id;         // номер процесса
        char** m_cmd;           // команда для его вызова (токенизированная строка)
        process_type m_type;    // тип процесса
        process_status m_status;
    } task_t;
    */

    for(int i = 0; i < g_bg_count; i++)
    {
        fprintf(stdout, "Порядковый номер процесса: %d\n", i);
        fprintf(stdout, "\t PID: %d\n", g_bg_task[i].m_pid_id);
        fprintf(stdout, "\t argc: %ld\n", g_bg_task[i].m_argc);
        fprintf(stdout, "Name: %p=%s\n",g_bg_task[i].m_cmd, *g_bg_task[i].m_cmd);
        printTokens(g_bg_task[i].m_cmd);
        fprintf(stdout, "\t Тип процесса: %d\n", g_bg_task[i].m_type);
        fprintf(stdout, "\t Статус процесса: %d\n\n", g_bg_task[i].m_status);
    }

    return 0;
}
