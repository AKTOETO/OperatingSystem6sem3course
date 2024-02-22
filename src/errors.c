#include "errors.h"

// функция печати ошибок
void errorPrint(int er)
{
    if(er != OK)
    {
        const char* er_msg = "\0";

        // проверка на каждый код ошибки
        // IF_ERR(er, FILE_DOESNT_EXIST)
        //     er_msg = "FILE_DOESNT_EXIST";
        
        // IF_ERR(er, FILE_HAS_NO_DESCRIPTOR)
        //     er_msg = "FILE_HAS_NO_DESCRIPTOR";
        
        CR_ERR_MSG(er_msg, er, FILE_DOESNT_EXIST);
        CR_ERR_MSG(er_msg, er, FILE_HAS_NO_DESCRIPTOR);
        CR_ERR_MSG(er_msg, er, FILE_HAS_NO_PATH);
        CR_ERR_MSG(er_msg, er, FILE_HAS_NO_SIZE);
        CR_ERR_MSG(er_msg, er, FILE_INCORRECT_INPUT_SIZE);
        //CR_ERR_MSG(er_msg, er, FILE_DOESNT_EXIST);
        
        fprintf(stderr, "error msg: <%s>\n", er_msg);
    }
}