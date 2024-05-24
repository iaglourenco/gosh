// error_handling.c
#include <stdio.h>
#include "error_handling.h"
#include <string.h>

void handle_error(int error_number)
{
    char *error_message = strerror(error_number);
    printf(" : %s\n", error_message);
}
