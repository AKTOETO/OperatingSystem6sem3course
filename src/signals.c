#include "signals.h"

// уничтожение background задач
void killAllBGTaskSignal(int sig)
{
    killAllBGTask();
    //clearMem();
}

// обработка zombie задач
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
        INFO("Foreground задача с PID: %d больше не zombie\n", pid);
    }
    // иначе завершаем bg процесс
    else
    {
        for (size_t i = 0; i < g_bg_count; i++)
        {
            bt = &g_bg_task[i];

            if (bt->m_pid_id == pid)
            {
                INFO("Background задача с PID: %d больше не zombie\n", pid);

                bt->m_status = FINISHED;
                break;
            }
        }
    }
}

// инициализация сигналов
void sigInit()
{
    // обработка завершения работы процесса
    signal(SIGCHLD, childKill);

    // установка сигнала уничтожения всех bg задач на ctrl c
    signal(SIGINT, killAllBGTaskSignal);
}