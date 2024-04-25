//error_handling.h
#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

typedef enum {
    NO_ERROR,
    INVALID_COMMAND,
    READ_ERROR, //erro de leitura ou EOF

} ErrorCode;

void print_error(ErrorCode error_code);

#endif
