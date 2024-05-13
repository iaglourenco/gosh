// shell_functions.c
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

void help_message()
{
    printf("gosh - Great, Another Shell\n");
    printf("\tUsage: gosh script-file\n\n");
    printf("\tInternal commands:\n");
    printf("\t\tcd <path> - Change the current directory\n");
    printf("\t\texit - Exit the shell\n");
    printf("\t\tpath [path1 path2 ...] - Show or set the search paths\n");
    printf("\t\thelp - Show this help message\n");
    printf("\tExternal commands:\n");
    printf("\t\tls [-l] [-a] [path] - List files and directories\n");
    printf("\t\tcat <file> - Show the content of a file\n");
}

void initialize_paths()
{
    // Inicializa a lista de caminhos com o caminho padrão
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        char *local_cat = malloc(strlen(cwd) + strlen("/cat") + 1);
        strcpy(local_cat, cwd);
        strcat(local_cat, "/cat");
        char *local_ls = malloc(strlen(cwd) + strlen("/ls") + 1);
        strcpy(local_ls, cwd);
        strcat(local_ls, "/ls");

        search_paths[0] = local_cat;
        search_paths[1] = local_ls;
        num_paths = 2;
    }
    else
    {
        perror("getcwd");
    }
}

void add_path(char *path)
{
    // Adiciona um novo caminho à lista
    if (num_paths >= MAX_PATHS)
    {
        print_error(MAX_PATHS_REACHED);
        return;
    }
    char *path_copy = strdup(path);
    if (path_copy == NULL)
    {
        print_error(MALLOC_FAILED);
        return;
    }
    search_paths[num_paths++] = path_copy;
}

char *search_executable(char *command)
{
    // Procura o executável nas listas de caminhos
    for (int i = 0; i < num_paths; i++)
    {
        char *full_path = malloc(strlen(search_paths[i]) + strlen(command) + 2); // +2 para / e \0
        if (!full_path)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        sprintf(full_path, "%s/%s", search_paths[i], command);
        if (access(full_path, X_OK) == 0)
        {
            return full_path; // Retorna o caminho completo se encontrado
        }
        free(full_path);
    }
    return NULL; // Retorna NULL se não encontrado
}

void execute_command(char *command)
{
    char *args[64];
    int arg_count = 0;
    pid_t pid;

    char *token;
    const char *delimiters = " \t\n";

    token = strtok(command, delimiters);
    while (token != NULL && arg_count < 64)
    {
        args[arg_count++] = token;
        token = strtok(NULL, delimiters);
    }
    args[arg_count] = NULL;

    // Comandos internos
    if (strcmp(args[0], "help") == 0)
    {
        help_message();
        return;
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (arg_count > 1)
        {
            if (chdir(args[1]) != 0)
            {
                print_error(CD_FAILED);
            }
        }
        else
        {
            printf("Uso: cd <caminho>\n");
        }
        return;
    }
    else if (strcmp(args[0], "path") == 0)
    {
        // Mostra os caminhos definidos
        if (arg_count == 1)
        {
            for (int i = 0; i < num_paths; i++)
            {
                printf("%s\n", search_paths[i]);
            }
            return;
        }

        // Define os caminhos especificados
        for (int i = 1; i < arg_count; i++)
        {
            add_path(args[i]);
        }
        return;
    }

    // Comando externo
    char *executable_path = search_executable(args[0]);
    if (executable_path != NULL)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return;
        }
        else if (pid == 0)
        {
            if (execv(executable_path, args) == -1)
            {
                print_error(EXEC_FAILED);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
        free(executable_path);
    }
    else
    {
        print_error(INVALID_COMMAND);
    }

    // pid = fork();
    // if (pid < 0) {
    //     perror("fork");
    //     exit(EXIT_FAILURE);
    // } else if (pid == 0) { // Processo filho
    //     if (execvp(args[0], args) == -1) {
    //         print_error(EXEC_FAILED);
    //         exit(EXIT_FAILURE);
    //     }
    // } else { // Processo pai
    //     int status;
    //     waitpid(pid, &status, 0);
    // }
}
