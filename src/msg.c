#include "msg.h"

ssize_t msgReadBufer(File *f, msg_t *msg)
{
    // читаем во временный буфер и запоминаем сколько байт считалось
    msg->m_buffer_size = read(f->m_id, msg->m_buffer, MSG_BUFFER_SIZE);
    INFO("Считанно количество байт: %ld из максимума: %d\n", msg->m_buffer_size, MSG_BUFFER_SIZE);
    return msg->m_buffer_size;
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
