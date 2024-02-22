#ifndef ERRORS_H
#define ERRORS_H

// возможные ошибки при работе программы
// ФАЙЛЫ
#define OK                          (0)
#define FILE_DOESNT_EXIST           (1 << 1)
#define FILE_HAS_NO_DESCRIPTOR      (1 << 2)
#define FILE_HAS_NO_PATH            (1 << 3)
#define FILE_HAS_NO_SIZE            (1 << 4)
#define FILE_HAS_NO_BUFFER          (1 << 5)
#define FILE_INCORRECT_INPUT_SIZE   (1 << 6)

// расшифорвка сообщений об ошибках
#define STR(str) #str
#define IF_ERR(src, er_code) if(src & er_code)
#define CR_ERR_MSG(str, src, er_code) \
if(src & er_code)str = #er_code;

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

#endif // !ERRORS_H