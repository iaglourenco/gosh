// shell_functions.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell_functions.h"
#include <errno.h>
#include "error_handling.h"

#define MAX_PATHS 10

char *search_paths[MAX_PATHS];
int num_paths = 0;

void help_message()
{
    printf("gosh - Great, Another Shell\n");
    printf("\tUsage: gosh script-file\n\n");
    printf("\tCommands:\n");
    printf("\t\tcd <path> - Change the current directory\n");
    printf("\t\texit - Exit the shell\n");
    printf("\t\tpath [path1 path2 ...] - Show or set the search paths\n");
    printf("\t\thelp - Show this help message\n");
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
        perror("Erro ao obter o diretório atual");
        handle_error(errno);
    }
}

void add_path(char *path)
{
    // Adiciona um novo caminho à lista
    if (num_paths >= MAX_PATHS)
    {
        errno = E2BIG;
        perror("Número máximo de caminhos atingido");
        return;
    }
    char *path_copy = strdup(path);
    if (path_copy == NULL)
    {
        perror("Erro ao alocar memória");
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
            perror("Erro ao alocar memória");
            return NULL;
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
                perror("Erro ao mudar de diretório");
            }
        }
        else
        {
            errno = EINVAL;
            perror("Uso: cd <caminho>");
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
            perror("Erro ao criar processo");
            return;
        }
        else if (pid == 0)
        {
            if (execv(executable_path, args) == -1)
            {
                perror("Erro ao executar comando");
                exit(EXIT_FAILURE); // Sai do processo filho se houver erro
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
        printf("Comando não encontrado: %s", args[0]);
        errno = ENOENT;
        handle_error(errno);
    }
}
