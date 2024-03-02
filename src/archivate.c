#include "filedir.h"

void writeHeader(File* out)
{
    // шапка
    // [file count]
    // [path size][path][buffer size]
    // ...
    // [path size][path][buffer size]

    // запись количества файлов
    write(out->m_id, &f_size, sizeof(f_size));

    // проходимся по всем файлам
    for(int i = 0; i < f_size; i++) 
    {
        // записываем размер пути
        write(out->m_id, &files[i]->m_path_size, sizeof(size_t));
        // записываем путь к файлу
        write(out->m_id, files[i]->m_path, files[i]->m_path_size);
        // записываем размер буфера
        write(out->m_id, &files[i]->m_size, sizeof(size_t));
    }
}

void writeBody(File* out)
{
    // тело
    // [buffer]
    // ...
    // [buffer]

    // проходимся по всем файлам
    for(int i = 0; i < f_size; i++)
    {
        // записываем буфер
        write(out->m_id, files[i]->m_buffer, files[i]->m_size);
    }
}

// считать заголовок
void readHeader(File* in)
{
    // считывание количества файлов
    read(in->m_id, &f_size, sizeof(f_size));

    // создание массива файлов
    files = createFileArr(f_size);

    // считывание информации о файлах
    for(int i = 0; i < f_size; i++)
    {
        // создание файла
        File *out = files[i] = createFile();

        // установка размера пути
        read(in->m_id, &out->m_path_size, sizeof(size_t));

        // выделение памяти под строку
        out->m_path = realloc(out->m_path, out->m_path_size);

        // установка пути
        read(in->m_id, out->m_path, out->m_path_size);

        // установка размера буфера
        read(in->m_id, &out->m_size, sizeof(size_t));
    }
}

// чтение тела файла
void readBody(File* in)
{
    // проходимся по всем файлам
    for(int i = 0; i < f_size; i++)
    {
        // выделение памяти под буфер
        files[i]->m_buffer = malloc(files[i]->m_size + 1);

        // считываем буфер
        read(in->m_id, files[i]->m_buffer, files[i]->m_size);

        // конец строки
        files[i]->m_buffer[files[i]->m_size] = '\0';
    }
}

// функция создания архива
int archivate(int argc, char** argv)
{
    // в аргументах должен быть путь к папке, которую надо заархивировать
    // и путь к файлу, в который надо выгрузить архив
    if(argc < 3)
    {
        printf("должно быть больше аргументов \n");
        return 1;
    }

    base_folder = argv[1];

    // создание массива с файлами
    fillFileArrFromDir(argv[1]);

    // печать массива с файлами
    printFileArr(files);

    // создание выходного файла
    File* f = createFile();

    // создаю дескриптор выходного файла
    setFilepath(f, argv[2]);
    openOutputFile(f);

    // печать выходнго файла
    printFile(f);

    // запись шапки в буфер выходного файла
    writeHeader(f);

    // запись тела в буфер выходного файла
    writeBody(f);

    // удаление выходного файла
    closeFile(f);
    deleteFile(f);

    // удаление массива с файлами
    deleteFileArr(files);

    return OK;
}

int dearchivate(int argc, char** argv)
{
    // в аргументах должен быть путь к архиву
    // и путь к папке, в которую надо разархивировать
    if(argc < 3)
    {
        printf("должно быть больше аргументов \n");
        return 1;
    }

    // сохранение пути, в который надо разахивировать
    base_folder = argv[2];

    // открыть архивный файл
    File *f = createFile();
    setFilepath(f, argv[1]);
    openInputFile(f);
    readFileSize(f);
    
    // считать заголовок 
    // > в нем будет создание списка файлов
    // > считывание размера каждого файла
    // > считывание размера пути файла
    // > считывание пути файла
    readHeader(f);

    // считывание тела
    readBody(f);
    printFileArr(files);

    // печать файлов в файловую систему
    fillDirFromFileArr(files);
 
    // закрыть архивный файл
    closeFile(f);
    deleteFile(f);

    // удаление массива с файлами
    deleteFileArr(files);

    return OK;
}



int main(int argc, char** argv)
{
    if(argc < 4)
    {
        ERRORS("мало аргументов, должно быть так: \n\t[folder/filepath][filepath/folder][a/d]\n")
    }

    if(argv[3][0] == 'a')
    {
        // если второй аргумент не папка, то это ошибка
        if(!S_ISDIR(getStMode(argv[1])))
            ERROR("argv[1] = <%s> не является папкой (а должна)\n", argv[1]);

        INFO("archivating folder <%s> in file <%s>\n", argv[1], argv[2])
        archivate(argc, argv);
    }

    else if(argv[3][0] == 'd')
    {
        // если второй аргумент не файл, то это ошибка
        if(!S_ISREG(getStMode(argv[1])))
            ERROR("argv[1] = <%s> не является файлом (а должен)\n", argv[1]);

        INFO("dearchivating file <%s> in folder <%s>\n", argv[1], argv[2]);
        dearchivate(argc, argv);
    }
    
    return 0;
}
