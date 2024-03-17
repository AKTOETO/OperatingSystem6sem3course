#include "msg.h"

// символ для выполнения операции xor по умолчанию
// используется, если один из буферов опустел, а другой - нет
#define DEF_XOR_CHAR '1'

void xor(char **out_buf, ssize_t *ob_sz,
         char *data, ssize_t data_sz,
         char *key, ssize_t key_sz)
{
    INFO("Исходный буфер: %p\n", *out_buf);
    INFO("Размер первого буфера: %ld\n", data_sz);
    INFO("Размер второго буфера: %ld\n", key_sz);
    INFO("последний символ в буфере data: [%c]\n", data[data_sz]);

    // перевыделение памяти под буфер
    ssize_t max_new_sz = (data_sz > key_sz ? --data_sz : --key_sz);
    INFO("Добавляем %ld байт\n", max_new_sz);
    *out_buf = realloc(*out_buf, *ob_sz + max_new_sz);
    INFO("Новый размер выходного буфера %ld\n", *ob_sz + max_new_sz);

    INFO("Новая память: %p\n", *out_buf);

    // проходимся по конечному буферу и выполняем xor
    for(ssize_t i = 0; i < *ob_sz + max_new_sz - 1; i++)
    {
        // если есть первый и второй буфер
        if(i < data_sz && i < key_sz)
            (*out_buf)[i + *ob_sz] = data[i] ^ key[i];
        // если нет второго
        if(i >= key_sz)
            (*out_buf)[i + *ob_sz] = data[i] ^ DEF_XOR_CHAR;
        // если нет первого
        if(i >= data_sz)
            (*out_buf)[i + *ob_sz] = key[i] ^ DEF_XOR_CHAR;
    }

    // печать буфера
    INFOS("Печать буфера\n");
    for(ssize_t i = 0; i < *ob_sz + max_new_sz; i++)
    {
        putchar((*out_buf)[i]);
    }
    putchar('\n');

    // изменяем размер конечного массива
    *ob_sz += max_new_sz;
}

int main(int argc, char **argv)
{    
    // если нет пути к файлу - выдаем ошибку
    if(argc <= 2)
    {
        ERRORS("Нужно передать путь к двум файлам \n");
        return -1;
    }

    // хранение pid двух reader процессов
    pid_t pid[2];

    // запуск первого reader процесса
    pid[0] = fork();

    // 1) поток не был создан
    if(pid[0] < 0)
    {
        ERRORS("Поток не был создан\n");
    }
    // 2) сейчас выполняется дочерний процесс
    if(pid[0] == 0)
    {
        if(execl("reader", "reader", argv[1], NULL) == -1)
        {
            ERRORS("Не получилось запустить reader\n");
        }
        INFOS("exec не выполнен\n");
        // выход из дочернего процесса
        exit(0);
    }

    INFO("Новый пид процесса 0: %d\n", pid[0]);

    // запуск второго reader процесса
    pid[1] = fork();

    // 1) поток не был создан
    if(pid[1] < 0)
    {
        ERRORS("Поток не был создан\n");
    }
    // 2) сейчас выполняется дочерний процесс
    if(pid[1] == 0)
    {
        if(execl("reader", "reader", argv[2], NULL) == -1)
        {
            ERRORS("Не получилось запустить reader\n");
        }       
        // выход из дочернего процесса
        exit(0);
    }

    INFO("Новый пид процесса 1: %d\n", pid[1]);

    // создание очереди сообщений (Если очередь создана, мы просто получим ее id)
    int msgqid = msgget(QKEY, 0666 | IPC_CREAT);

    // если создание прошло некорректно 
    if(msgqid <= -1)
    {
        ERRORS("Очередь сообщений не была создана\n");
        return -1;
    }

    // структура сообщения
    msg_t recv[2];

    // количество считанных байт
    ssize_t bytes[2];    

    // конечный буфер
    char *out = NULL;
    ssize_t out_size = 0;

    // читаем, пока количество считанных байт не меньше размера буфера
    do
    {
        // пытаемся прочитать сообщение
        if((bytes[0] = msgrcv(msgqid, (void *)&recv[0], MSGBUF, 0, 0)) == -1)
        {
            ERRORS("Сообщение не было считано корректно\n");
            return -1;
        }
        
        // пытаемся прочитать сообщение
        if((bytes[1] = msgrcv(msgqid, (void *)&recv[1], MSGBUF, 0, 0)) == -1)
        {
            ERRORS("Сообщение не было считано корректно\n");
            return -1;
        }

        // выполнение операции xor
        xor(&out, &out_size, recv[0].m_buffer, bytes[0], recv[1].m_buffer, bytes[1]);

    } while(bytes[0] == MSGBUF - 1 || bytes[1] == MSGBUF - 1);

    // удаление очереди сообщений
    if(msgctl(msgqid, IPC_RMID, 0) == -1)
    {
        ERRORS("Удаление очереди прошло неуспешно\n");
        return -1;
    }

    INFO("Размер выходного буфера: %ld\n", out_size);
    // выходной файл
    File *f = createFile();
    setFilepath(f, "xor.txt");
    openOutputFile(f);
    setFileBufferSize(f, out, out_size);
    writeFile(f);
    closeFile(f);
    deleteFile(f);

    // удаление буфера
    free(out);

    return 0;
}