#ifndef MSG_H
#define MSG_H

#include "file.h"

#define QKEY 789456
#define MSGBUF 1024

// структура сообщеиня
typedef struct
{
    long int m_num;
    char m_buffer[MSGBUF];
} msg_t;

// отправка сообщения
int msqSend(File *f);

// получение сообщения
int msgRecv(File **f);


#endif // !MSG_H