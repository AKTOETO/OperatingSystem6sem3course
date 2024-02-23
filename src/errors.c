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

        CR_ERR_MSG(er_msg, er, FDIR_DOESNT_EXIST);
        CR_ERR_MSG(er_msg, er, FDIR_COULDNT_CREATE_DIR );
        CR_ERR_MSG(er_msg, er, FDIR_PATH_DOESNT_EXIST  );
        CR_ERR_MSG(er_msg, er, FDIR_COULDNT_CHANGE_DIR );
        CR_ERR_MSG(er_msg, er, FDIR_COULDNT_CLOSE_DIR  );
        CR_ERR_MSG(er_msg, er, FDIR_FILES_DOESNT_EXIST );
        CR_ERR_MSG(er_msg, er, FDIR_COULDNT_CREATE_FILES );
        CR_ERR_MSG(er_msg, er, FDIR_FILE_COUNT_IS_0 );
        
        // печать системных ошибок
        perror("system error");

        // печать ошибки приложения
        fprintf(stderr, "error msg: <%s>\n\n", er_msg);
    }
}