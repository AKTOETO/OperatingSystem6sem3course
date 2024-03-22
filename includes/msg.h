#ifndef MSG_H
#define MSG_H

#include "file.h"

#define QKEY 789456
#define MSG_BUFFER_SIZE 1024

// структура сообщеиня
typedef struct
{
    long int m_num;                 // pid процесса, сформировавшего сообщение
    char m_buffer[MSG_BUFFER_SIZE]; // информация в сообщении
    long m_buffer_size;             // размер буфера сообщения
} msg_t;

// длина сообщения
#define MSG_LEN (sizeof(msg_t) - sizeof(long))

// чтение буфера из файла
ssize_t msgReadBufer(File *f, msg_t *msg);

// закрыть очередь
int msgCloseQ(int msgqid);


#endif // !MSG_H