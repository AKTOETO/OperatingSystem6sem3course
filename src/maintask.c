#include "maintask.h"

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

// печать информации о доступных командах
int help(size_t argc, char **argv)
{
    char* cmds[] = 
    {
        "pbg - печать всех background процессов",
        "out - выход из оболочки",
        "cd - смена директории",
        "watch - что-то очень долго выполняющееся (для теста аргумента &) ",
        "help - печать информации о доступных командах",
        "exit [PID] - удаление дочернего процесса",
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
int exitt(size_t argc, char **argv)
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
        if(f(src, g_bg_task + i) && g_bg_task[i].m_status != FINISHED)
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
        ERRORS("Не был найден такой дочерний процесс \n");
    }
    return 0;
}


// MAIN
// массив возможных main задач (не из PATH)
char *g_main_task_list[MAIN_LIST_SIZE] = 
{
    "pbg",
    "out",
    "cd",
    "skoof",
    "help",
    "exit"
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
    pbg,
    out,
    cd,
    skoof,
    help,
    exitt
};