// error_handling.h
#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

typedef enum
{
    NO_ERROR,
    INVALID_COMMAND,             // Comando inválido
    READ_ERROR,                  // Erro de leitura ou EOF
    CD_FAILED,                   // Erro no comando "CD"
    EXEC_FAILED,                 // Erro no fork "LS"
    PATH_FAILED,                 // Erro no comando "PATH"
    MALLOC_FAILED,               // Erro ao alocar memória
    FORK_FAILED,                 // Erro no fork
    WAIT_FAILED,                 // Erro no wait
    MAX_PATHS_REACHED            // Número máximo de caminhos atingido

} ErrorCode;

void print_error(ErrorCode error_code);


#endif
