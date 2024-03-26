#include "msg.h"

// ксорим каждый бит данных с каждым битом ключа
void xor(char *data, ssize_t data_size,
         char *key, ssize_t key_size)
{
    INFOS("======XOR======\n");

    // проходимся по конечному буферу и выполняем xor
    for(ssize_t i = 0; i < data_size; i++)
    {
        data[i] ^= key[i % key_size];
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
    memcpy(*data + *data_size, add, add_size);

    // изменяем размер буфера
    (*data_size) += add_size;
    INFO("Новый размер буфера: %ld\n", *data_size);
}

// создание процесса чтения
void createReader(char* filepath, pid_t* pid)
{
    // запуск первого reader процесса
    *pid = fork();

    // 1) поток не был создан
    if(*pid < 0)
    {
        ERRORS("Поток не был создан\n");
    }
    // 2) сейчас выполняется дочерний процесс
    if(*pid == 0)
    {
        if(execl("reader", "reader", filepath, NULL) == -1)
        {
            ERRORS("Не получилось запустить reader\n");
        }
        INFOS("exec не выполнен\n");
        // выход из дочернего процесса
        exit(0);
    }

    INFO("Новый пид процесса 0: %d\n", *pid);
}

// структура чтения всего буфера из очереди
typedef struct 
{
    char *m_data;       // само сообщение
    ssize_t m_data_size;// размер сообщения
    bool m_is_read;     // прочитано ли сообщение
} qdata_t;


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
    // (читает данные)
    createReader(argv[1], &pid[0]);

    // запуск второго reader процесса
    // (читает ключ)
    createReader(argv[2], &pid[1]);

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
    qdata_t data = {NULL, 0, 0};

    // буфер ключа
    qdata_t key = {NULL, 0, 0};

    // читаем, пока количество считанных байт не меньше размера буфера
    // собираем буферы с двух процессов
    do
    {
        // пытаемся прочитать сообщение
        if((bytes = msgrcv(msgqid, (void *)&recv, MSG_LEN, 0, 0)) == -1)
        {
            ERRORS("Сообщение не было считано корректно\n");
            return -1;
        }

        INFO("pid: %ld\n", recv.m_num);
        INFO("Считано байт: [%ld]\n", recv.m_buffer_size);

        // если размер сообщения 0 - читать больше нечего из текущего процесса
        if(recv.m_buffer_size == 0)
        {
            if(recv.m_num == pid[0])
                data.m_is_read = 1;
            else if(recv.m_num == pid[1])
                key.m_is_read = 1;
        }
        else 
        {
            // если пид первого сообщения равен pid[0], дополняем первый буфер
            if(recv.m_num == pid[0])
            {
                INFOS("Добавление в буфер данных\n");
                add(&data.m_data, &data.m_data_size, recv.m_buffer, recv.m_buffer_size);
            }
            // иначе просто меняем буферы местами
            else if (recv.m_num == pid[1])
            {
                add(&key.m_data, &key.m_data_size, recv.m_buffer, recv.m_buffer_size);     
                INFOS("Ключ добавлен\n");     
            }   
        }
    }
    // пока не прочитали оба файла 
    while(!data.m_is_read || !key.m_is_read);

    INFO("Размер выходного буфера 1: %ld\n", data.m_data_size);
    INFO("Размер выходного буфера 2: %ld\n", key.m_data_size);
    //data.m_data[data.m_data_size - 1] = '\0';
    key.m_data[key.m_data_size] = '\0';
    //INFO("СЧИТАННЫЕ БУФЕРЫ:\n\n<%s>\n\n", data.m_data);
    INFO("СЧИТАННЫЕ БУФЕРЫ:\n\n<%s>\n\n", key.m_data);

    // выполнение операции xor
    xor(data.m_data, data.m_data_size, key.m_data, key.m_data_size);

    // удаление очереди сообщений
    msgCloseQ(msgqid);

    // выходной файл
    File *f = createFile();
    setFilepath(f, "xor.txt");
    openOutputFile(f);
    setFileBufferSize(f, data.m_data, data.m_data_size);
    writeFile(f);
    closeFile(f);
    deleteFile(f);

    // удаление буфера
    free(data.m_data);
    free(key.m_data);

    return 0;
}