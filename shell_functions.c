//shell_functions.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell_functions.h"
#include "error_handling.h"

#define MAX_PATHS 10

char *search_paths[MAX_PATHS];
int num_paths = 0;

void initialize_paths() {
    // Inicializa a lista de caminhos com o caminho padrão
    search_paths[0] = "/bin"; // Adicione mais caminhos padrão, se desejar
    num_paths = 1;
}

void add_path(char *path) {
    // Adiciona um novo caminho à lista
    if (num_paths < MAX_PATHS) {
        search_paths[num_paths++] = path;
    } else {
        printf("Limite máximo de caminhos atingido.\n");
    }
}

char *search_executable(char *command) {
    // Procura o executável nas listas de caminhos
    for (int i = 0; i < num_paths; i++) {
        char *full_path = malloc(strlen(search_paths[i]) + strlen(command) + 2); // +2 para / e \0
        if (!full_path) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        sprintf(full_path, "%s/%s", search_paths[i], command);
        if (access(full_path, X_OK) == 0) {
            return full_path; // Retorna o caminho completo se encontrado
        }
        free(full_path);
    }
    return NULL; // Retorna NULL se não encontrado
}

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
                print_error(CD_FAILED);
            }
        } else {
            printf("Uso: cd <caminho>\n");
        }
        return; 
    } else if (strcmp(args[0], "path") == 0) {
        // Define os caminhos especificados
        for (int i = 1; i < arg_count; i++) {
            add_path(args[i]);
        }
        return; 
    }

    // Comando externo
    char *executable_path = search_executable(args[0]);
    if (executable_path != NULL) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            return;
        } else if (pid == 0) { 
            if (execv(executable_path, args) == -1) {
                print_error(EXEC_FAILED);
                exit(EXIT_FAILURE);
            }
        } else { 
            int status;
            waitpid(pid, &status, 0);
        }
    free(executable_path);
    } else {
        //print_error(INVALID_COMMAND);
    }
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Processo filho
        if (execvp(args[0], args) == -1) {
            print_error(EXEC_FAILED);
            exit(EXIT_FAILURE);
        }
    } else { // Processo pai
        int status;
        waitpid(pid, &status, 0);
    }
}
