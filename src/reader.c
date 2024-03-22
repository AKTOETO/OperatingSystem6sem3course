#include "errors.h"
#include "msg.h"

int main(int argc, char **argv)
{

    // если нет пути к файлу - выдаем ошибку
    if(argc <= 1)
    {
        ERRORS("Нужно передать путь к файлу \n");
        return -1;
    }

    INFO("Запуск чтения файла: %s\n", argv[1]);

    // сообщение
    msg_t msg;

    // создание очереди сообщений
    int msgqid = msgget(QKEY, 0666 | IPC_CREAT);

    // если создание прошло некорректно 
    if(msgqid <= -1)
    {
        ERRORS("Очередь сообщений не была создана\n");
        return -1;
    }

    INFO("Очередь создана msgid: %d\n", msgqid);

    // открыть файл
    File *f = createFile();

    // установить путь до файла
    setFilepath(f, argv[1]);

    // открыть файл для чтения
    openInputFile(f);

    // узнать размер
    readFileSize(f);

    // количество считанных байт
    ssize_t read_byte = 0;

    //INFO("Полученный буфер: [%s]\n", msg.m_buffer);

    // считывание сообщения и параллельная отправка его  
    do
    {
        // // читаем буфер
        // read_byte = read(f->m_id, msg.m_buffer, MSG_ALL_BUF_SIZE - 1);
        // INFO("Количество считанных байт: %ld\n", read_byte);
        
        // // ошибка при чтении
        // if(read_byte == -1)
        // {
        //     ERRORS("Ошибка при чтении\n");
        //     return -1;
        // }
        // msg.m_buffer[read_byte] = '\0';

        // INFO("Последний символ: [%c]\n", msg.m_buffer[read_byte]);
        // INFO("Буфер: [%s]\n", msg.m_buffer);

        // читаем буфер
        read_byte = msgReadBufer(f, &msg);
        //INFO("red: %ld\n", read_byte);
        // тип
        msg.m_num = getpid();

        // отправляем сообщение
        if(msgsnd(msgqid, (void *)&msg, MSG_LEN, 0) <= -1)
        {
           ERRORS("Сообщение не было корректно отправлено\n");
           return -1;
        }
    } while(read_byte != 0);

    // закрыть дескриптор
    closeFile(f);

    // удалить файл
    deleteFile(f);

    //msgCloseQ(msgqid);

    return 0;
}