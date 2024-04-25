#include <stdio.h>
#include "error_handling.h"

void print_error(ErrorCode error_code) {
    switch (error_code) {
        case NO_ERROR:
            printf("No error.\n");
            break;
        case INVALID_COMMAND:
            printf("Error: Invalid command.\n");
            break;

        default:
            printf("Unknown error.\n");
            break;
    }
}
