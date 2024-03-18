#ifndef MSG_H
#define MSG_H

#include "file.h"

#define QKEY 789456
#define MSG_ALL_BUF_SIZE 1024
#define MSG_LEN_SIZE 8 
#define MSG_BUFFER_SIZE MSG_ALL_BUF_SIZE - MSG_LEN_SIZE - 1

// структура сообщеиня
typedef struct
{
    long int m_num;
    char m_buffer[MSG_ALL_BUF_SIZE];
} msg_t;

// m_buffer:
// |size| buffer |\0|

// чтение буфера из файла
ssize_t msgReadBufer(File *f, msg_t *msg);

// чтение из сообщения
ssize_t msgReadSizeBuffer(msg_t *msg);

// отправка сообщения
int msqSend(File *f);

// получение сообщения
int msgRecv(File **f);

// закрыть очередь
int msgCloseQ(int msgqid);


#endif // !MSG_H