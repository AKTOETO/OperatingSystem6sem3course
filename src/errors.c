#include "errors.h"

// функция печати ошибок
void errorPrint(int er)
{
    if(er != OK)
    {
        const char* er_msg = "\0";

        // проверка на каждый код ошибки
        CR_ERR_MSG(er_msg, er, FILE_DOESNT_EXIST);
        CR_ERR_MSG(er_msg, er, FILE_HAS_NO_DESCRIPTOR);
        CR_ERR_MSG(er_msg, er, FILE_HAS_NO_PATH);
        CR_ERR_MSG(er_msg, er, FILE_HAS_NO_SIZE);
        CR_ERR_MSG(er_msg, er, FILE_INCORRECT_INPUT_SIZE);
        CR_ERR_MSG(er_msg, er, FILE_INCORRECT_OUTPUT_SIZE);

        CR_ERR_MSG(er_msg, er, FARR_DOESNT_EXIST);
        
        // печать системных ошибок
        perror("system error");

        // печать ошибки приложения
        fprintf(stderr, "error msg: <%s>\n\n", er_msg);
    }
}