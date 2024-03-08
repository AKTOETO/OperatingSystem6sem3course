#include "taskmanager.h"

// выполнение процесса
int launch(size_t argc, char **argv)
{
    int code = 0;
    pid_t pid;
    pid = fork();

    if(pid < 0)
    {
        ERRORS("Процесс не был создан\n");
    }
    // ребенок
    else if(pid == 0)
    {
        // если есть символ &, убираем его
        if(strcmp(argv[argc - 1], "&") == 0)
        {
            INFOS("Символ & удален из команды\n");
            argv[argc - 1] = NULL;
        }
        
        // после завершения работы дочернего процесса, нужно 
        // завершить процесс (не выйти из функции, а завершить процесс!!)
        code = childProcTask(argc, argv);

        exit(code);
    }
    // родитель
    else
    {
        // добавляем задачу
        addTask(pid, argc, argv);
    }
    return code;
}

// обработка команд
int argvProcessing(char **argv, size_t argc)
{
    // если нет токенов - выходим
    if(!argv[0]) return NULL_TOKENS;

    // пытаюсь запустить задачу в главном процессе
    // если не получилось и код ошибки равен TASK_UNKNOWN
    // то запускаем в дочернем процессе
    int code = mainProcTask(argc, argv);
    if(code == TASK_EXIT)
        return TASK_EXIT;

    if(code == TASK_UNKNOWN)
    {
        code = launch(argc, argv);
        if(code == TASK_UNKNOWN)
        {
            // TODO
            // это никогда не сработает, потому что переменную с кодом возврата меняет дочерний поток
            // родительский никогда не узнает об этом (вся инфа при форке дублируется)
            // надо через waitpid получать ошибку, либо просто забить :)
            ERROR("Неизвестная команда: [%s]\n", argv[0]);
            return TASK_UNKNOWN;
        }
    }

    return code;
}

int mainProcTask(size_t argc, char **argv)
{
    // задачи, которые должны выполняться в основном процессе
    if(strcmp(argv[0], "out") == 0)
    {
        return out(argc, argv);
    }
    else if(strcmp(argv[0], "cd") == 0)
    {
        return cd(argc, argv);
    }
    else if(strcmp(argv[0], "tka") == 0)
    {
        return tka(argc, argv);
    }
    else if(strcmp(argv[0], "skoof") == 0)
    {
        return skoof(argc, argv);
    }    // вывод информации обо всех задачах background
    else if(strcmp(argv[0], "pbg") == 0)
    {
        return pbg(argc, argv);
    }

    return TASK_UNKNOWN;
}

int childProcTask(size_t argc, char **argv)
{
    // запуск долгой задачи
    if(strcmp(argv[0], "watch") == 0)
    {
        return watch(argc, argv);
    }
    // запуск задачи из переменноф PATH
    else if(execvp(argv[0], argv) == -1)
    {
        return TASK_UNKNOWN;
    }

    return TASK_UNKNOWN;
}




int out(size_t argc, char **argv)
{
    killAndDeleteAllBGTask();
    return TASK_EXIT;
}

int cd(size_t argc, char **argv)
{
    if (argv[1] == NULL)
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

    return TASK_COMPLETE;
}

int tka(size_t argc, char **argv)
{
    quit();
    return TASK_COMPLETE;
}

int watch(size_t argc, char **argv)
{
    fprintf(stdout,"\n%s\n","Процесс watch начат");
    for(int i = 0; i < 10; i++)
    {
        fprintf(stdout, "\nwatch i:%d\n", i);
        sleep(1);
    }
    fprintf(stdout,"\n%s\n","Процесс watch закончен");
    return TASK_COMPLETE;
}

int pbg(size_t argc, char **argv)
{
    printBGInfo();
    return TASK_COMPLETE;
}

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
    return TASK_COMPLETE;
}
