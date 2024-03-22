#include "time.h"
#include "file.h"
#include "getopt.h"

// стандартные настроки работы программы
#define DEFAULT_CHARS "1234567890qwertyuiopasdfghjklzxcvbnm"
#define DEFAULT_LEN 50
#define DEFAULT_FILENAME "key.txt"

void generateKey(char **g_key, ssize_t *g_key_size, char *g_chars, ssize_t g_chars_size)
{
    // перевыделение памяти под новый ключ
    (*g_key) = calloc(DEFAULT_LEN, sizeof(char));

    // проходимся по всему массиву ключа и вставляем случайные элементы
    for(ssize_t i = 0; i < *g_key_size; i++)
    {
        int ind = rand() % (g_chars_size);
        //INFO("g_chars[%d] = <%c>\n", ind, g_chars[ind]);
        (*g_key)[i] = g_chars[ind];
    }
}

ssize_t getStrSize(const char *arr)
{
    ssize_t size = 0;
    while(arr[size ++] != '\0');
    return size - 1;
}

// путь до файла
char *g_filename;

// ключ
char *g_key;
ssize_t g_key_size;

// символы для генерации ключа
char *g_chars;
ssize_t g_chars_size;

// установка значений по умолчанию
void setDefault()
{
    // путь до файла
    g_filename = DEFAULT_FILENAME;

    // ключ
    g_key = NULL;
    g_key_size = DEFAULT_LEN;

    // символы для генерации ключа
    g_chars = DEFAULT_CHARS;
    g_chars_size = getStrSize(DEFAULT_CHARS);
}

void processARGV(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, ":f:c:s:")) != -1)
    {
        switch(opt)
        {
            // установка пути до файла
            case 'f':
                g_filename = optarg;
                break;
            
            // установка символов ключа
            case 'c':
                g_chars = optarg;
                g_chars_size = getStrSize(g_chars);
                break;
            
            // установка длины ключа
            case 's':
                ssize_t temp = atol(optarg);
                // если tempp больше 57, то выходим
                // столько памяти нельзя выделить
                if(temp > 57)
                {                    
                    ERRORS("Нельзя выделить память под больше, чем 57 символов\n");
                }
                else 
                    g_key_size = temp;
                break;

            // нехватает опции 
            case ':':
                ERRORS("нужна опция\n");
                break;

            // неизвестный ключ
            case '?':
                ERROR("неизвестный ключ: <%c>\n", optopt);
                INFOS("Ключи:\n -f <filename> - установка имени файла с ключом\n\
 -s <size> - длина ключа\n -c <chars> - символы, используемые в ключе\n");
                break;
        }
    }    
}

int main(int argc, char **argv)
{
    // изначальное значение 
    setDefault();

    // отключение вывода info сообщений
    //g_use_info = 0;

    srand(time(NULL));

    // обработка параметров
    if(argc > 1)
        processARGV(argc, argv);

    // создание файла
    File *f = createFile();
    setFilepath(f, g_filename);
    openOutputFile(f);

    // генерация ключа
    generateKey(&g_key, &g_key_size, g_chars, g_chars_size);

    // печать параметров
    INFO("     g_chars: <%s>\n", g_chars);
    INFO("g_chars_size: <%ld>\n", g_chars_size);
    INFO("  g_key size: <%ld>\n", g_key_size);
    INFO("  g_filename: <%s>\n", g_filename);
    INFO("       g_key: <%s>\n", g_key);
    INFO("  g_key_size: <%ld>\n", g_key_size);

    // установка буфера для файла
    setFileBufferSize(f, g_key, g_key_size);
    writeFile(f);


    // удаление файла
    closeFile(f);
    deleteFile(f);
    free(g_key);
    return 0;
}