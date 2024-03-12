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

// доп функция для убийства процесса по pid
bool pidEqual(task_t *f, task_t *s)
{
    return f->m_pid_id == s->m_pid_id;
}

// убийство процесса по pid
int pidkill(size_t argc, char **argv)
{
    // если аргументов меньше 2, выходим
    if(argc < 2)
    {
        ERRORS("Недостаточно аргументов\n");
        return 0;
    }

    // создаем задачу с нужным pid
    task_t t;
    t.m_pid_id = atoi(argv[1]);

    // запускаем удаление процесса по условию
    killBGTask(&t,pidEqual);

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
    "help",
    "pidkill"
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
    help,
    pidkill
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

int createForegroundTask(size_t argc, char **argv)
{
    INFOS("Добавление fg задачи\n");

    // индекс задачи из подготовленного списка возможных задач
    int ind = 0;

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
        // можно вынести в указательна функцию и передават ее при вызове
        // это нужно для создания одной функции для вызова создания bg и fg процессов
        {
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
                ERRORS("Execvp не смогла запустить код\n");
            }
            // если же код не -1, тогда процесс успешно запущен
            else
            {
                INFO("Процесс [%s] успешно запущен\n", argv[0]);
            }
        }
            
        // выход из дочернего процесса
        exit(0);
    }
    // 3) сейчас выполняется родительский процесс
    else
    {
        // сохранение fg процесса в массив
        addForegroundTask(pid, argc, argv);

        // ожидание завершения fg процесса
        waitFGTask();
    }
            
    return 0;
}

int createBackgroundTask(size_t argc, char **argv)
{
    INFOS("Добавление bg задачи\n");
    // индекс задачи из подготовленного списка возможных задач
    int ind = 0;
    
    // добавление bg процесса

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
        // можно вынести в указательна функцию и передават ее при вызове
        // это нужно для создания одной функции для вызова создания bg и fg процессов
        {
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
                ERRORS("Execvp не смогла запустить код\n");
            }
            // если же код не -1, тогда процесс успешно запущен
            else
            {
                INFO("Процесс [%s] успешно запущен\n", argv[0]);
            }

        }
            
        // выход из дочернего процесса
        exit(0);
    }
    // 3) сейчас выполняется родительский процесс
    else
    {
        printTokens(argv);

        // сохранение bg процесса в массив
        addBackgroundTask(pid, argc, argv);
    }

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

int killBGTask(task_t* src, bool (*f)(task_t*, task_t*))
{
    // указатель процесса
    size_t i = 0;

    // проходимся по всем задачам и ищем подходящую
    for(; i < g_bg_count; i++)
    {
        INFO("Сравниваем %d с %d\n", src->m_pid_id, g_bg_task[i].m_pid_id);
        // если переданная функция верна
        // (то есть такой процесс есть в списке дочерних процессов)
        // удаляем процесс
        if(f(src, g_bg_task + i))
        {
            if(kill(g_bg_task[i].m_pid_id, SIGTERM) != 0)
            {
                ERROR("Процесс с pid: %d не был корректно завершен\n", g_bg_task[i].m_pid_id);
            }
            else
            {
                INFO("Задача с pid: %d завершена успешно\n", g_bg_task[i].m_pid_id);
            }
            g_bg_task[i].m_status = FINISHED;
            return 0;
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
