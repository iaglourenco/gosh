// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "error_handling.h"
#include "shell_functions.h"

#define MAX_COMMAND_LENGTH 1024
#define MAX_COMMANDS 10

int main(int argc, char *argv[])
{
    char command[MAX_COMMAND_LENGTH];
    char cwd[MAX_COMMAND_LENGTH];
    FILE *shellFile = NULL;

    // Help message
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        help_message();
        return 0;
    }

    if (argc > 1)
    { // Se foi fornecido um arquivo shell
        shellFile = fopen(argv[1], "r");
        if (!shellFile)
        {
            perror("Failed to open the file");
            exit(EXIT_FAILURE);
        }
    }

    // Inicializa a lista de caminhos com os programas padrão
    initialize_paths();

    while (1)
    {
        if (shellFile)
        { // Se estiver lendo de um arquivo shell
            if (fgets(command, MAX_COMMAND_LENGTH, shellFile) == NULL)
            {
                break;
            }
        }
        else
        { // Se estiver lendo interativamente do terminal
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                // Substitui /home/usuario por ~
                char *home = getenv("HOME");
                if (home != NULL)
                {
                    char *home_pos = strstr(cwd, home);
                    if (home_pos != NULL)
                    {
                        char *new_cwd = malloc(MAX_COMMAND_LENGTH);
                        strcpy(new_cwd, "~");
                        strcat(new_cwd, home_pos + strlen(home));
                        strcpy(cwd, new_cwd);
                        free(new_cwd);
                    }
                }

                printf("gosh:%s$: ", cwd); // Adiciona o caminho atual ao prompt
            }
            else
            {
                perror("getcwd");
                exit(EXIT_FAILURE);
            }
            if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
            {
                print_error(READ_ERROR);
                break;
            }
        }

        // Remove o caractere de nova linha, se presente
        command[strcspn(command, "\n")] = '\0';

        // Ignora comandos vazios
        if (command[0] == '\0' || command[0] == '\n')
        {
            continue;
        }

        // Divide os comandos pelo operador '&'
        char *commands[MAX_COMMANDS];
        int command_count = 0;
        char *token = strtok(command, "&");
        while (token != NULL)
        {
            commands[command_count++] = token;
            token = strtok(NULL, "&");
        }
        commands[command_count] = NULL;

        for (int i = 0; i < command_count; i++)
        {
            char *command = commands[i];

            // Remove espaços em branco no início do comando
            while (*command == ' ')
                command++;

            // Verifica se há redirecionamento de saída
            char *redirection = strstr(command, ">");
            int saved_stdout = -1;
            int fd = -1;

            if (redirection)
            {
                *redirection = '\0'; // Remove o > do comando
                redirection++;       // Pula o > e pega o nome do arquivo

                // Pula os espaços em branco
                while (*redirection == ' ')
                    redirection++;

                // Remove os espaços em branco no final
                char *end = redirection + strlen(redirection) - 1;
                while (*end == ' ')
                {
                    *end = '\0';
                    end--;
                }

                // Abre o arquivo para escrita
                fd = open(redirection, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("Failed to open the file");
                    exit(EXIT_FAILURE);
                }

                // Salva o descritor de arquivo padrão
                saved_stdout = dup(STDOUT_FILENO);
                if (saved_stdout < 0)
                {
                    perror("Failed to duplicate the file descriptor");
                    close(fd);
                    continue;
                }

                // Redireciona a saída padrão para o arquivo
                if (dup2(fd, STDOUT_FILENO) < 0)
                {
                    perror("Failed to duplicate the file descriptor");
                    close(fd);
                    continue;
                }

                // Fecha o descritor de arquivo
                close(fd);
            }

            // Executa o ultimo comando no pai
            if (i == command_count - 1)
            {
                execute_command(commands[i]);
            }
            else
            {

                pid_t pid = fork();
                if (pid == 0)
                {

                    execute_command(command);
                }
                else if (pid < 0)
                {
                    print_error(FORK_FAILED);
                }
                else
                {
                    wait(NULL);
                }
            }
            // Restaura a saída padrão, se necessário
            if (saved_stdout >= 0)
            {
                if (dup2(saved_stdout, STDOUT_FILENO) < 0)
                {
                    perror("Failed to duplicate the file descriptor");
                    close(saved_stdout);
                    continue;
                }
            }
        }
    }

    if (shellFile)
    {
        fclose(shellFile);
    }

    return 0;
}
