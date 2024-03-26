#include "utils.h"

char *g_in_file;
char *g_out_file;
int g_thread_numbers;

bool strEndIn(const char *str, const char *end)
{
    size_t str_len = strlen(str);
    size_t end_len = strlen(end);

    char *pos = strstr(str, end);
    return (pos != NULL) && (pos + end_len == str + str_len);
}

bool strEndArrIn(const char *str, const char *end[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        if(strEndIn(str, end[i]) == true)
            return true;
    }
    return false;
}

void processARGV(int argc, char **argv)
{
    g_in_file = DEF_IN_FILE;
    g_out_file = DEF_OUT_FILE;
    g_thread_numbers = 1;

    int opt;
    while ((opt = getopt(argc, argv, ":iv:o:t:")) != -1)
    {
        switch(opt)
        {
            // отключение информационных сообщений
            case 'i':
                g_use_info = 0;
                break;
            
            // установка входного файла
            case 'v':
                g_in_file = optarg;
                break;
            
            // установка выходного файла
            case 'o':
                g_out_file = optarg;
                break;

            // установка количества потоков
            case 't':
                g_thread_numbers = atoi(optarg);
                // если количество потоков меньше нуля - ошибка
                if(g_thread_numbers < 1)
                {                    
                    ERRORS("Невозможно создать меньше одного потока. Задано значение 1.\n");
                    g_thread_numbers = 1;
                }
                break;

            // нехватает опции 
            case ':':
                ERRORS("нужна опция\n");
                break;

            // неизвестный ключ
            case '?':
                ERROR("Неизвестный ключ: <%c>\n", optopt);
//                 INFOS("Ключи:\n -f <filename> - установка имени файла с ключом\n\
//  -s <size> - длина ключа\n -c <chars> - символы, используемые в ключе\n");
                break;
        }
    }    
}