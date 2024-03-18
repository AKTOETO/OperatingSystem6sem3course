#include "msg.h"

ssize_t msgReadBufer(File *f, msg_t *msg)
{
    // временный буфер чтения
    char temp[MSG_BUFFER_SIZE + 1];

    // читаем во временный буфер и запоминаем сколько байт считалось
    ssize_t read_byte = read(f->m_id, temp, MSG_BUFFER_SIZE);
    INFO("считанной количество байт: %ld из максимума: %d\n", read_byte, MSG_BUFFER_SIZE);
    temp[read_byte] = '\0';

    // записываем конечный буфер
    snprintf(msg->m_buffer, MSG_ALL_BUF_SIZE, "%ld%s", read_byte, temp);
    INFO("Считанный буфер: [%s]\n", msg->m_buffer);

    return read_byte;
}

ssize_t msgReadSizeBuffer(msg_t *msg)
{
    return atol(msg->m_buffer);
}

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

int msgCloseQ(int msgqid)
{
    // удаление очереди сообщений
    if(msgctl(msgqid, IPC_RMID, 0) == -1)
    {
        ERRORS("Удаление очереди прошло неуспешно\n");
        return -1;
    }
    return 0;
}
