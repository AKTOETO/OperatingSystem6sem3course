#include "strings.h"
#define STR_SIZE 256

// печать приветствия
void printHi()
{
    char buffer[STR_SIZE ];
    if(getcwd(buffer, STR_SIZE) == NULL)
    {
        ERRORS("Не получилось выяснить рабочую диреткорию\n");
    }
    else
        printf("%s:> ", buffer);
}

// выполнение процесса
void launch(char **argv)
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
        if(execvp(argv[0], argv) == -1)
        {
            ERROR("Неизвестная команда: [%s]\n", argv[0]);
        }
    }
    // родитель
    else
    {
        // сохраняем pid ребенка
        // обрабатываем background
        // и foreground процессы отдельно
        if(waitpid(pid, NULL, 0) == -1)
        {
            ERRORS("Что-то пошло не так с закрытием дочернего процесса o_0\n");
        }
    }
}

int main()
{
    char *str = NULL;
    char **argv = NULL;

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

        if(argv[0] && strcmp(argv[0], "out") == 0)
            break;

        if(argv[0] && (strcmp(argv[0], "ls") == 0 || strcmp(argv[0], "firefox") == 0))
            launch(argv);

        free(str); 
        free(argv);
    }

    if(str) free(str);
    if(argv) free(argv);
    return 0;
}
