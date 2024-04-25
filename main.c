#include <stdio.h>
#include "error_handling.h"
//#include "shell_functions.h"

int main() {
    ErrorCode error_code = INVALID_COMMAND;
    print_error(error_code);
    return 0;
}
