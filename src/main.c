#include "utils.h"
#include "tasks.h"

void killAllBGTaskSignal(int sig)
{
    killFGTask();
    killAllBGTask();
}

// является тип задачи bg
bool isItBackground(size_t* argc, char **argv)
{
    // проверяем на наличие токенов
    if(argc <= 0 || argv == NULL)
        ERRORS("Недостаточно токенов");

    // берем последний токен
    // если он &, удаляем его (NULL а его место), возвращаем true
    if(argv[(*argc) - 1][0] == '&')
    {
        argv[--(*argc)] = NULL;
        return true;
    }

    // иначе возвращаем false
    return false;
}

int main(int argc, char** args)
{
    char *str = NULL;
    char **argv = NULL;
    size_t argnum = 0;

    // // установка сигнала уничтожения всех bg задач на ctrl c
    signal(SIGINT, killAllBGTaskSignal);

    // // если есть флаг -i, то выключаем информационные сообщения
    // if(argc > 1 && strcmp(args[1], "-i") == 0)
    // {
    //     INFOS("Информационные сообщения выключены\n");
    //     g_use_info = 0;
    // }
    
    while(true)
    {
        // печать приглашения
        printHi();

        // считывание строки
        str = readLine();
        if(strlen(str) == 0)
        {
            ERRORS("Строка не была создана\n");
            continue;
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

        // индекс задачи из подготовленного списка возможных задач
        int ind = 0;

        // проверяем на наличие символа &, если есть, то процесс bg
        if(isItBackground(&argnum, argv))
        {
            // добавление bg процесса
            createBackgroundTask(argnum, argv);
        }

        // если нет & проверяем команду на вхождение в список команд, 
        // исполняемых в main процессе, если есть совпадение - выполняем в мейне
        else if((ind = isItFromMainTaskList(argv[0])) != -1)
        {
            // выполняем команду под индексом ind из массива команд g_main_task_func
            // если вернулось -1 => была вызвана функция out => надо завершить программу
            if(g_main_task_func[ind](argnum, argv) == -1)
            {
                break;
            }
        }

        // если все предыдущее ложно - процесс fg.
        else
        {
            // добавление fg процесса
            createForegroundTask(argnum, argv);
        }

        // освобождение памяти
        free(str); 
        free(argv);
    }
    quit();
    if(str) free(str);
    if(argv) free(argv);
    return 0;
}
