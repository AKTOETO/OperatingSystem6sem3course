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

    // // создаем задачу с нужным pid
    // task_t t;
    // t.m_pid_id = atoi(argv[1]);

    // // запускаем удаление процесса по условию
    // killBGTask(&t,pidEqual);

    return 0;
}

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