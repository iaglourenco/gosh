#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell_functions.h"

void execute_command(char *command) {
    char *args[64]; 
    int arg_count = 0;
    pid_t pid;
    
    char *token;
    const char *delimiters = " \t\n";
    
    token = strtok(command, delimiters);
    while (token != NULL && arg_count < 64) {
        args[arg_count++] = token;
        token = strtok(NULL, delimiters);
    }
    args[arg_count] = NULL;
    
    // Comandos internos
    if (strcmp(args[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    } else if (strcmp(args[0], "cd") == 0) {
        if (arg_count > 1) {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        } else {
            printf("Uso: cd <caminho>\n");
        }
        return; 
    } else if (strcmp(args[0], "path") == 0) {
        // Implemente o tratamento do comando "path" aqui
        printf("Comando 'path' ainda não implementado.\n");
        return; 
    }
    
    // Comando externo
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Processo filho
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else { // Processo pai
        int status;
        waitpid(pid, &status, 0);
    }
}
