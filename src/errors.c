#include "errors.h"

// поиск имени файла
size_t findFileNamePos(const char* str)
{
    size_t len = strlen(str);
    while(--len > 0)
    {
        if(str[len] == '/')
        {
            return len + 1;
        }
    }
    return len + 1;
}

// печать справочной информации
void printLog(
    FILE *out_stream, const char* file,
    int line, const char* type)
{
    if(strcmp(type, "ERROR") == 0)
        fprintf(out_stream, "[%-6s][%s][line:%4d][sys er: %s] MSG: ",
                type, file + findFileNamePos(file), line, strerror(errno));

    else if(strcmp(type, "INFO") == 0)
        fprintf(out_stream, "[%-6s][%s][line:%4d] MSG: ",
                type, file + findFileNamePos(file), line);

    else
        fprintf(out_stream, "\n\tНеизвестный тип ошибки <%s>\n", type);
}