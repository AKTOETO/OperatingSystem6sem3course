#include "utils.h"

// печать приветствия
void printHi()
{
    uid_t id = getuid();
    struct passwd *pw = getpwuid(id);

    char buffer[STR_SIZE ];
    if(getcwd(buffer, STR_SIZE) == NULL)
    {
        ERRORS("Не получилось выяснить рабочую диреткорию\n");
    }
    else   
    {
        printf("%s%s@%s%s:> ", HI_GREEN_WHITE, pw->pw_name, buffer, DEFAULT_WHITE);
    } 
}