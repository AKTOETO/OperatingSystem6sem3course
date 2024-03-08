#include "args.h"
#include "utils.h"
#include "tasks.h"

int out(char **argv)
{
    killAndDeleteAllBGTask();
    return -1;
}

int cd(char **argv) {
    if (argv[1] == NULL)
    {
        ERRORS("Недостаточно аргументов для команды\n");
    }
    else if (chdir(argv[1]) != 0)
    {
        ERROR("Невозможно сменить директорию на: %s\n", argv[1]);
    }

    return 0;
}

// выполнение процесса
int launch(char **argv)
{
    pid_t pid;
    pid = fork();

    if(pid < 0)
    {
        ERRORS("Процесс не был создан\n");
    }
    // ребенок
    else if(pid == 0)
    {
        if(strcmp(argv[0], "cd") == 0)
        {
            return cd(argv);
        }
        else if(execvp(argv[0], argv) == -1)
        {
            ERROR("Неизвестная команда: [%s]\n", argv[0]);
            return -1;
        }
        return -1;
    }
    // родитель
    else
    {
        // добавляем задачу
        addTask(pid, argv);
    }
    return 0;
}

// обработка команд
int argvProcessing(char **argv)
{
    // char **allCmd = 
    // {
    //     "ls",
    //     "cd",
    //     "firefox"
    // };

    // если нет токенов - выходим
    if(!argv[0]) return 1;

    // задачи, которые должны выполняться в основном процессе
    if(strcmp(argv[0], "out") == 0)
    {
        return out(argv);
    }

    // пытаемся запустить программы
    return launch(argv);
}

int main(int argc, char** args)
{
    char *str = NULL;
    char **argv = NULL;

    // если есть флаг -i, то выключаем информационные сообщения
    if(argc > 1 && strcmp(args[1], "-i") == 0)
    {
        INFOS("Информационные сообщения выключены\n");
        g_use_info = 0;
    }

    while(true)
    {
        printHi();

        // считывание строки
        str = readLine();
        if(str == NULL)
        {
            ERRORS("Строка не была создана\n");
            return 1;
        }
        INFO("%s\n", str);

        // получение токенов
        argv = tokenizeString(str);
        if(argv == NULL)
        {
            ERRORS("Массив токенов не существует\n");
            free(str);
            return 1;
        }

        // печать токенов
        printTokens(argv);

        if(argvProcessing(argv) == -1)
            break;

        // ждем выполнения foreground задачи
        //waitFGTask();

        free(str); 
        free(argv);
    }

    if(str) free(str);
    if(argv) free(argv);
    return 0;
}
