#include "msg.h"

// работа с сообщениями
int msqSend(File *f)
{
    // // создание очереди сообщений
    // int msgqid = msgget(QKEY, 0666 | IPC_CREAT);

    // // если создание прошло некорректно 
    // if(msgqid == -1)
    // {
    //     ERRORS("Очередь сообщений не была создана\n");
    //     return -1;
    // }

    // // создание структуры сообщения
    // msg_t send = 
    // {
    //     .f = f,
    //     .m_num = 1
    // };

    // // отправляем сообщение
    // if(msgsnd(msgqid, (void *)&send, sizeof(File *), 0) == -1)
    // {
    //     ERRORS("Сообщение не было корректно отправлено\n");
    //     return -1;
    // }

    return 0;
}

int msgRecv(File **f)
{
    // создание очереди сообщений (Если очередь создана, мы просто получим ее id)
    int msgqid = msgget(QKEY, 0666 | IPC_CREAT);

    // если создание прошло некорректно 
    if(msgqid == -1)
    {
        ERRORS("Очередь сообщений не была создана\n");
        return -1;
    }

    // структура сообщения
    msg_t recv;

    // пытаемся прочитать сообщение
    if(msgrcv(msgqid, (void *)&recv, sizeof(File*), 0, 0) == -1)
    {
        ERRORS("Сообщение не было считано корректно\n");
        return -1;
    }

    // удаление очереди сообщений
    if(msgctl(msgqid, IPC_RMID, 0) == -1)
    {
        ERRORS("Удаление очереди прошло неуспешно\n");
        return -1;
    }

    return 0;
}
