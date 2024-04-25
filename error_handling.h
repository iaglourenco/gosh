#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

typedef enum {
    NO_ERROR,
    INVALID_COMMAND,
} ErrorCode;

void print_error(ErrorCode error_code);

#endif
