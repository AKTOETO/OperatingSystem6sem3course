#include "tasks.h"

// foreground задача
task_t g_fg_task = 
{
    .m_cmd = NULL,
    .m_argc = 0,
    .m_pid_id = -1,
    .m_status = FINISHED,
    .m_type = FOREGROUND
};

// все background задачи
task_t* g_bg_task = NULL;   // массив
size_t g_bg_count = 0;      // количество задач
size_t g_bg_capacity = 0;   // вместимость массива

// ЗАДАЧИ
// MAIN
// выход из оболочки
int out(size_t argc, char **argv)
{
    INFOS("out invoked\n");
    return -1;
}

// смена директории
int cd(size_t argc, char **argv)
{
    if (argc <= 1 || argv[1] == NULL)
    {
        ERRORS("Недостаточно аргументов для команды\n");
    }
    else if (chdir(argv[1]) != 0)
    {
        ERROR("Невозможно сменить директорию на: %s\n", argv[1]);
    }
    else
    {
        INFOS("Директория сменена\n");
    }

    return 0;
}

// завершение всех процессов
int tka(size_t argc, char **argv)
{
    INFOS("tka invoked\n");
    return 0;    
}

// печать информации о background процессах
int pbg(size_t argc, char **argv)
{
    INFOS("pbg invoked\n");
    printBGInfo();
    return 0;
}

// печать информации о доступных командах
int help(size_t argc, char **argv)
{
    char* cmds[] = 
    {
        "out - выход из оболочки",
        "cd - смена директории",
        "tka - завершение всех процессов",
        "watch - что-то очень долго выполняющееся (для теста аргумента &) ",
        "pbg - печать информации о background процессах",
        "help - печать информации о доступных командах",
        NULL
    };

    int i = 0;
    while(cmds[i])
    {
        fprintf(stdout, "\t%s\n", cmds[i++]);
    }

    return 0;
}

// skoof
int skoof(size_t argc, char **argv)
{
    char* prt[26] = 
    {
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠶⠒⣣⣶⣶⣶⡶⠶⠶⣶⡶⠶⣖⠒⢶⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⢿⠀⠀⠙⠿⣿⠻⠄⠀⠈⠀⠙⢦⠈⢣⣼⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃⠀⠀⠀⠀⠀⠀⢁⡊⠀⠀⢀⡀⢠⣦⡻⢧⡀⠹⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣠⣶⣀⣴⣄⣠⣆⣀⣽⣤⣰⣯⣴⣿⣿⣷⣼⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⠛⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀",
"⠀⢠⣶⣦⡀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⠱⡄",
"⢠⡿⠟⣿⢿⡄⠀⣀⣤⡴⠞⠛⠛⠷⠾⡿⢿⡿⠿⠿⠿⣿⣿⣿⣿⣿⠿⣿⣿⣿⣿⡿⣿⣿⡿⣿⣶⣶⣶⣶⣤⣄⡴⢳⣿⣿⠀⢹",
"⢿⣧⣴⣿⣼⣿⣦⣈⠉⠉⠉⣺⠟⠂⠀⠀⠈⠋⠒⠀⠀⠋⠉⠉⠉⠀⣸⠟⡀⠀⠀⣠⠉⠈⠻⠿⣷⣌⠁⣀⡴⠋⢁⣴⣾⣿⠶⠟",
"⠈⠛⠛⠛⠻⠷⢤⣈⠛⠶⣶⢇⣤⣤⣤⣤⣤⣀⡔⠒⠒⠒⠀⠀⠀⠚⢻⣾⣿⣾⣿⣿⡻⠓⠒⠚⠛⠛⣿⣿⣷⠞⠉⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠙⢿⡃⠿⣿⣥⡼⢯⡼⢿⣿⣿⠂⠀⠀⠀⠀⠀⣿⢹⣷⠿⣥⡿⢿⣿⣿⠀⠀⠀⣠⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⡗⠀⠈⠻⣷⣯⣱⣾⣩⣿⣆⠀⠀⠀⠀⠀⠘⠿⣽⣶⣏⣷⣏⣹⣿⡀⠀⠐⠋⠹⣧⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⢰⠁⠀⠀⠀⠈⠉⠛⠛⠋⢉⠁⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣯⡉⠉⠉⠀⠀⠀⠀⠀⠸⡆⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⡞⠀⠀⠀⠀⠀⠀⠀⢀⣴⠋⢀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣷⣼⣿⣄⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⢀⣤⣾⠿⠋⠛⠿⠿⠶⢦⣤⣤⣶⣾⣿⣿⣿⣿⣿⣿⠟⠻⢶⣄⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⢠⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⠉⠁⢀⣰⣶⡄⢻⡀⢀⠀⣿⡇⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠃⠀⠀⠠⠏⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣠⣤⣶⣿⣿⣿⣷⠸⡇⠈⠹⣿⡇⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⣿⡾⠿⣿⠿⣿⡿⠿⣿⣿⡿⢿⡟⢻⣿⣿⣿⡿⣹⣿⣄⣿⠀⢀⣿⡇⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠐⡄⠀⠀⠀⠀⠀⠀⠀⠘⠳⢤⣷⣤⡾⠄⠠⠼⠇⠀⠘⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠒⠛⠉⠱⣶⣶⣶⣦⣄⡀",
"⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠒⠠⠤⣤⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⡇",
"⠀⠀⠀⠀⠀⠀⠀⠀⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⣿⣷⣶⣶⣦⣤⣤⣤⣀⣀⣀⣀⣼⣿⣿⣿⡿⠁",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⢿⣿⡿⠇⢘⣩⣿⡿⠃⠀⠀⠀⠀⠉⠉⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠃⠙⠃⠐⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣷⣾⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣾⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠒⠤⣤⣤⣤⣴⣶⣶⣶⣿⣿⡿⠿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
    };

    for(int i = 0; i < 26; i++)
        fprintf(stdout, "%s\n", prt[i]);
    return 0;
}

// FOREGROUND

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

// КОНЕЦ ЗАДАЧ

// МАССИВЫ СООТВЕТСТВИЙ КОМАНД
// MAIN
// массив возможных main задач (не из PATH)
char *g_main_task_list[MAIN_LIST_SIZE] = 
{
    "out",
    "cd",
    "tka",
    "skoof",
    "pbg",
    "help"
};

// проверка: переданная задача из main task list
int isItFromMainTaskList(char *task_name)
{
    // индекс задачи
    int i = 0;

    // проходимся по задачам и сравниваем
    for(; i < MAIN_LIST_SIZE; i++)
    {
        // если есть совпадение - возвращаем индекс
        if(strcmp(g_main_task_list[i], task_name) == 0)
        {
            return i;
        }
    }

    // иначе возвращаем -1
    return -1;
}

// массив указателей на функции, которые могут быть исполнены в main 
int(*g_main_task_func[MAIN_LIST_SIZE])(size_t, char **) =
{
    out,
    cd,
    tka,
    skoof,
    pbg,
    help
};



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



// BACKGROUND
// массив возможных bg задач (не из PATH)
char *g_bg_task_list[BG_LIST_SIZE] = 
{
    "watch"
};

// проверка: пеерданная задача из bg task list
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

// массив указателей на функции, которые могут быть исполнены в bg 
int(*g_bg_task_func[BG_LIST_SIZE])(size_t, char **) = 
{
    watch
};


// КОНЕЦ МАССИВОВ СООТВЕТСТВИЙ КОМАНД





int addTask(pid_t pid, size_t argc, char **argv)
{
    // получаем тип процесса
    process_type type = getTaskType(argv);

    // сохраняем его 
    if(type == FOREGROUND)
    {
        addForegroundTask(pid, argc, argv);

        // ожидаем выполнения задачи
        waitFGTask();
    }
    else
    {
        addBackgroundTask(pid, argc, argv);
        //return waitBGTask();
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
                return 0;
            }
            else
            {
                INFO("Задача с pid: %d завершена успешно\n", g_bg_task[i].m_pid_id);
            }
            g_bg_task[i].m_status = FINISHED;
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
