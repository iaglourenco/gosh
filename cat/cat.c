#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char ch;

    // Checa se o nome do arquivo foi passado como argumento
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }


    // Abre o arquivo
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    // Lê e imprime o conteúdo do arquivo
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    // Fecha o arquivo
    fclose(file);

    return 0;
}