//main.c
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
    FILE *shellFile = NULL;

    if (argc > 1) { // Se foi fornecido um arquivo shell
        shellFile = fopen(argv[1], "r");
        if (!shellFile) {
            perror("Failed to open the file");
            exit(EXIT_FAILURE);
        }
    }

    // Inicializa a lista de caminhos com os programas padrão
    initialize_paths();

    while (1) {
        if (shellFile) { // Se estiver lendo de um arquivo shell
            if (fgets(command, MAX_COMMAND_LENGTH, shellFile) == NULL) {
                break;
            }
        } else { // Se estiver lendo interativamente do terminal
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                // Substitui /home/usuario por ~
                char *home = getenv("HOME");
                if (home != NULL) {
                    char *home_pos = strstr(cwd, home);
                    if (home_pos != NULL) {
                        char *new_cwd = malloc(MAX_COMMAND_LENGTH);
                        strcpy(new_cwd, "~");
                        strcat(new_cwd, home_pos + strlen(home));
                        strcpy(cwd, new_cwd);
                        free(new_cwd);
                    }
                }

                printf("gosh:%s$: ", cwd); // Adiciona o caminho atual ao prompt
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

        // Ignora comandos vazios
        if(command[0] == '\0' || command[0] == '\n') {
            continue;
        }

        execute_command(command);
    }

    if (shellFile) {
        fclose(shellFile);
    }

    return 0;
}
