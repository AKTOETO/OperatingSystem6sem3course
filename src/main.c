#include "taskmanager.h"

void killAllBGTaskSignal(int sig)
{
    killAllBGTask();
}

int main(int argc, char** args)
{
    char *str = NULL;
    char **argv = NULL;
    size_t argnum = 0;

    // установка сигнала уничтожения всех bg задач на ctrl c
    signal(SIGINT, killAllBGTaskSignal);

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
        argv = tokenizeString(str, &argnum);
        if(argv == NULL)
        {
            ERRORS("Массив токенов не существует\n");
            free(str);
            return 1;
        }

        // печать токенов
        if(g_use_info)
            printTokens(argv);

        // запуск процессов
        if(argvProcessing(argv, argnum) == -1)
        {
            INFOS("Конец работы оболочки\n");
            break;
        }

        // освобождение памяти
        free(str); 
        free(argv);
    }

    if(str) free(str);
    if(argv) free(argv);
    return 0;
}
