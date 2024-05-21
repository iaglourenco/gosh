#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <bits/getopt_core.h>

int compare(const void *a, const void *b)
{
    const char **entry1 = (const char **)a;
    const char **entry2 = (const char **)b;

    return strcasecmp(*entry1, *entry2);
}

// Tipo de arquivo
char get_file_type(mode_t mode)
{
    if (S_ISREG(mode))
        return '-';
    if (S_ISDIR(mode))
        return 'd';
    if (S_ISCHR(mode))
        return 'c';
    if (S_ISBLK(mode))
        return 'b';
    if (S_ISFIFO(mode))
        return 'p';
    if (S_ISLNK(mode))
        return 'l';
    if (S_ISSOCK(mode))
        return 's';
    return '?';
}

// Permissoes em rwx
char *get_permissions(mode_t mode)
{
    static char permissions[10];
    permissions[0] = get_file_type(mode);
    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';
    return permissions;
}

// Informaçoes do usuario
char *get_user(uid_t uid)
{
    struct passwd *pw = getpwuid(uid);
    return pw->pw_name;
}

// Informaçoes do grupo
char *get_group(gid_t gid)
{
    struct group *gr = getgrgid(gid);
    return gr->gr_name;
}

int main(int argc, char *argv[])
{
    int option;
    bool l, a;
    char *path = ".";

    // --help
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s [-l] [-a] [path]\n", argv[0]);
        printf("List information about the FILEs (the current directory by default).\n");
        printf("\n");
        printf("Options:\n");
        printf("  -l\t\tuse a long listing format\n");
        printf("  -a\t\tdo not ignore entries starting with .\n");
        printf("  -h, --help\tdisplay this help and exit\n");
        exit(0);
    }


    // Parseia os argumentos de linha de comando
    while ((option = getopt(argc, argv, "hla")) != -1)
    {
        switch (option)
        {
        case 'h':
            printf("Usage: %s [-l] [-a] [path]", argv[0]);
            printf("List information about the FILEs (the current directory by default).\n");
            printf("\n");
            printf("  -l     use a long listing format\n");
            printf("  -a     do not ignore entries starting with .\n");
            printf("  -h,--help     display this help and exit\n");
            exit(0);
        case 'l':
            l = true;
            break;
        case 'a':
            a = true;
            break;
        default:
            printf("Usage: %s [-l] [-a] [path]\n", argv[0]);
            break;
        }
    }
    if (optind < argc)
    {
        path = argv[optind];
    }

    // Abre o diretório
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        printf("No such file or directory: %s\n", argv[1]);
        return 1;
    }

    // Lê o diretório
    struct dirent *entry;
    struct stat st;
    char **entries = NULL;
    char full_path[1024];
    int n = 0;
    while ((entry = readdir(dir)) != NULL)
    {

        if (!a && entry->d_name[0] == '.') // Ignora arquivos ocultos se -a não foi passado
        {
            continue;
        }

        entries = realloc(entries, (n + 1) * sizeof(char *));
        entries[n] = strdup(entry->d_name);
        n++;
    }

    // Ordena os arquivos
    qsort(entries, n, sizeof(char *), compare);

    if (l) // Lista com detalhes
    {
        for (int i = 0; i < n; i++)
        {
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entries[i]);

            if (stat(full_path, &st) == -1)
            {
                perror("stat");
                continue;
            }

            printf("%s %2ld %s %s %5.2f kB %.12s %s\n",
                   get_permissions(st.st_mode),
                   st.st_nlink,
                   get_user(st.st_uid),
                   get_group(st.st_gid),
                   st.st_size/1024.0,
                   ctime(&st.st_mtime) + 4,
                   entries[i]);
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            printf("%s\t", entries[i]);
        }
    }
    printf("\n");

    free(entries);

    // Fecha o diretório
    closedir(dir);

    return 0;
}