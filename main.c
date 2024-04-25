#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "error_handling.h"
#include "shell_functions.h"

#define MAX_COMMAND_LENGTH 1024

int main(int argc, char *argv[]) {
    char command[MAX_COMMAND_LENGTH];
    char cwd[MAX_COMMAND_LENGTH];
    FILE *batch_file = NULL;
    
    if (argc > 1) { // Se foi fornecido um arquivo batch
        batch_file = fopen(argv[1], "r");
        if (!batch_file) {
            perror("Erro ao abrir arquivo batch");
            exit(EXIT_FAILURE);
        }
    }
    
    while (1) {
        if (batch_file) { // Se estiver lendo de um arquivo batch
            if (fgets(command, MAX_COMMAND_LENGTH, batch_file) == NULL) {
                break; 
            }
        } else { // Se estiver lendo interativamente do terminal
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Projeto-Shell>%s: ", cwd); // Adiciona o caminho atual ao prompt
            } else {
                perror("getcwd");
                exit(EXIT_FAILURE);
            }
            if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
                print_error(READ_ERROR);
                break;
            }
        }
        
        // Remove o caractere de nova linha, se presente
        command[strcspn(command, "\n")] = '\0';
        

        execute_command(command);
    }
    
    if (batch_file) {
        fclose(batch_file);
    }
    
    return 0;
}
