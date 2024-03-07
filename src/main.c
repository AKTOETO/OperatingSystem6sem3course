#include "errors.h"

int main(int argc, char** argv)
{
    pid_t pid = fork();
    
    if(pid == 0)
    {
        INFOS("imma child\n");
        execlp("ls", "ls", "-la", NULL);
        exit(1);
    }
    else
    {
        INFOS("waiting child\n");
        waitpid(pid, NULL, 0);
    }

    return 0;
}
