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

    // считывание сообщения и параллельная отправка его  
    do
    {
        // читаем буфер
        read_byte = msgReadBufer(f, &msg);
        
        // тип
        msg.m_num = getpid();

        // отправляем сообщение
        if(msgsnd(msgqid, (void *)&msg, MSG_LEN, 0) <= -1)
        {
           ERRORS("Сообщение не было корректно отправлено\n");
           return -1;
        }
    } 
    // читаем пока есть сообщения
    while(read_byte != 0);

    // закрыть дескриптор
    closeFile(f);

    // удалить файл
    deleteFile(f);

    return 0;
}