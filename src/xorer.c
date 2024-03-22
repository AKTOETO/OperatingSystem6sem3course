#include "msg.h"

// ксорим каждый бит данных с каждым битом ключа
void xor(char *data, ssize_t data_size,
         char *key, ssize_t key_size)
{
    INFOS("======XOR======\n");
    //INFO("Исходный буфер: %p\n", *out_buf);
    //INFO("Размер буфера данных: %ld\n", data_sz);
    //INFO("Размер буфера ключа: %ld\n", key_sz);
    //INFO("Последний символ в буфере data: [%c]\n", data[data_sz]);
    //INFO("Буфер: [%s]\n", data);

    // проходимся по конечному буферу и выполняем xor
    for(ssize_t i = 0; i < key_size; i++)
    {
        char new_ch = data[i] ^ key[i % key_size];
        //INFO("Cимвол на позиции: %ld: %c\n", i + *ob_sz, (*out_buf)[i + *ob_sz]);
        //if(new_ch == '\0')
        //    INFO("найден \\0 на позиции: [%ld] ключ[%ld]: [%c] со значением: [%c]\n", i, i % key_size, key[i % key_size], data[i]);
        data[i] = new_ch;
    }

    // печать буфера
    INFOS("Печать буфера\n[");
    for(ssize_t i = 0; i < data_size; i++)
    {
        putchar(data[i]);
    }
    printf("]\n");
}



// дописываем в буфер инфу
void add(char **data, ssize_t *data_size, char *add, ssize_t add_size)
{
    // добавляем память
    *data = realloc(*data, *data_size + add_size);

    // копируем информацию в основной буфер
    //strncpy(*data + *data_size, add, add_size);
    memcpy(*data + *data_size, add, add_size);

    // изменяем размер буфера
    (*data_size) += add_size;
    INFO("Новый размер буфера: %ld\n", *data_size);
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

    // ждем добавления всех сообщений в очередь
    waitpid(pid[0], NULL, 0);
    waitpid(pid[1], NULL, 0);

    // создание очереди сообщений (Если очередь создана, мы просто получим ее id)
    int msgqid = msgget(QKEY, 0666 | IPC_CREAT);

    // если создание прошло некорректно 
    if(msgqid <= -1)
    {
        ERRORS("Очередь сообщений не была создана\n");
        return -1;
    }

    // структура сообщения
    msg_t recv;

    // количество считанных байт
    ssize_t bytes;    

    // буфер данных
    char *out1 = NULL;
    ssize_t out1_size = 0;
    bool is_red1 = 0;

    // буфер ключа
    char *out2 = NULL;
    ssize_t out2_size = 0;
    bool is_red2 = 0;

    // читаем, пока количество считанных байт не меньше размера буфера
    // собираем буферы с двух процессов
    do
    {
        // сброс буфера чтения
        //memset(recv.m_buffer,'\0', MSG_ALL_BUF_SIZE);
        
        // пытаемся прочитать сообщение
        if((bytes = msgrcv(msgqid, (void *)&recv, MSG_LEN, 0, 0)) == -1)
        {
            ERRORS("Сообщение не было считано корректно\n");
            return -1;
        }

        INFO("pid: %ld\n", recv.m_num);
        INFO("Считано байт: [%ld]\n", recv.m_buffer_size);
        //INFO("Считано сообщение: [%s]\n", recv.m_buffer);

        // получение размера буфера
        //bytes = msgReadSizeBuffer(&recv);

        // если размер сообщения 0
        if(recv.m_buffer_size == 0)
        {
            if(recv.m_num == pid[0])
                is_red1 = 1;
            else if(recv.m_num == pid[1])
                is_red2 = 1;
        }

        // если пид первого сообщения равен pid[0], дополняем первый буфер
        if(!is_red1 && recv.m_num == pid[0])
        {
            INFOS("Добавление в буфер данных\n");
            add(&out1, &out1_size, recv.m_buffer, recv.m_buffer_size);
            INFO("размер буфера после перевыделения памяти: %ld\n", out1_size);
        }
        // иначе просто меняем буферы местами
        else if (!is_red2 && recv.m_num == pid[1])
        {
            add(&out2, &out2_size, recv.m_buffer, recv.m_buffer_size);     
            INFOS("Ключ добавлен\n");     
        }
        
    } while(!is_red1 || !is_red2);

    INFO("Размер выходного буфера 1: %ld\n", out1_size);
    INFO("Размер выходного буфера 2: %ld\n", out2_size);
    out1[out1_size] = '\0';
    out2[out2_size] = '\0';
    INFO("СЧИТАННЫЕ БУФЕРЫ:\n\n<%s>\n\n", out1);
    INFO("СЧИТАННЫЕ БУФЕРЫ:\n\n<%s>\n\n", out2);



    // буфер xor
    //char *out2 = NULL;
    //ssize_t out2_size = 0;


    // // выполнение операции xor
    // xor(&out1, &out1_size, recv[0].m_buffer, bytes[0], recv[1].m_buffer, bytes[1]);
    xor(out1, out1_size, out2, out2_size);

    // удаление очереди сообщений
    msgCloseQ(msgqid);

    INFO("Размер выходного буфера: %ld\n", out1_size);
    // выходной файл
    File *f = createFile();
    setFilepath(f, "xor.txt");
    openOutputFile(f);
    setFileBufferSize(f, out1, out1_size);
    writeFile(f);
    closeFile(f);
    deleteFile(f);

    // удаление буфера
    free(out1);
    free(out2);

    return 0;
}