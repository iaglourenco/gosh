// error_handling.c
#include <stdio.h>
#include "error_handling.h"

void print_error(ErrorCode error_code)
{
    switch (error_code)
    {
    case NO_ERROR:
        printf("No error.\n");
        break;
    case INVALID_COMMAND:
        printf("Error: Invalid command.\n");
        break;
    case READ_ERROR:
        printf("Error: Read error or EOF.\n");
        break;
    case CD_FAILED:
        printf("Error: Error in 'CD' command.\n");
        break;
    case EXEC_FAILED:
        printf("Error: Error in fork 'LS'.\n");
        break;
    case PATH_FAILED:
        printf("Error: Error in 'PATH' command.\n");
        break;
    case MALLOC_FAILED:
        printf("Error: Memory allocation error.\n");
        break;
    case FORK_FAILED:
        printf("Error: Fork error.\n");
        break;
    case WAIT_FAILED:
        printf("Error: Wait error.\n");
        break;
    case MAX_PATHS_REACHED:
        printf("Error: Maximum number of paths reached.\n");
        break;

    default:
        printf("Unknown error.\n");
        break;
    }
}
