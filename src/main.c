#include "utils.h"
#include "taskmanager.h"

void killAllBGTaskSignal(int sig)
{
    killAllBGTask();
}

void childKill(int sig)
{
    // временная переменная для хранения текущей задачи
    task_t* bt;

    // получаем pid завершенной задачи
    pid_t pid = waitpid(-1, NULL, 0);

    // если это fg процесс, завершаем его
    if (pid == g_fg_task.m_pid_id)
    {
        g_fg_task.m_status = FINISHED;
    }
    // иначе завершаем bg процесс
    else
    {
        for (size_t i = 0; i < g_bg_count; i++)
        {
            bt = &g_bg_task[i];

            if (bt->m_pid_id == pid)
            {
                INFO("Background задача с PID: %d завершена\n", pid);

                bt->m_status = FINISHED;
                break;
            }
        }
    }
}

int main(int argc, char** args)
{
    char *str = NULL;
    char **argv = NULL;
    size_t argnum = 0;

    // обработка завершения работы процесса
    signal(SIGCHLD, childKill);

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

        // если нет &, проверяем команду на вхождение в список команд, 
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
    if(str) free(str);
    if(argv) free(argv);
    quit();
    return 0;
}
