#include "msg.h"

int main()
{
    msgCloseQ(msgget(QKEY, 0666 | IPC_CREAT));
    return 0;
}