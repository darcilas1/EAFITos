/**
 * @file file_commands.c
 * @brief Comandos para manipulación y visualización de archivos.
 *
 * Implementa funcionalidades similares a 'ls' (listar) y 'cat' (leer) de Unix,
 * interactuando con las APIs del sistema de archivos.
 * Salida colorizada con colors.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>    /* Librería POSIX para manejo de directorios */
#include <sys/stat.h>  /* Para stat() y verificar si es directorio */
#include "commands.h"
#include "colors.h"

/**
 * @brief Comando LISTAR (ls)
 *
 * Abre el directorio actual (".") e itera sobre sus entradas.
 * Coloriza: directorios en azul, archivos en blanco.
 *
 * @param args Argumentos del comando (ignorados en esta versión).
 */
void cmd_listar(char **args) {
    DIR *d;
    struct dirent *dir;
    int n_archivos = 0;

    d = opendir(".");
    if (d) {
        printf(COLOR_CYAN " Contenido del directorio actual:\n" COLOR_RESET);
        printf(COLOR_DIM " ─────────────────────────────\n" COLOR_RESET);

        while ((dir = readdir(d)) != NULL) {
            /* Filtramos las entradas especiales "." y ".." */
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                /* Usamos stat para detectar si es directorio */
                struct stat st;
                if (stat(dir->d_name, &st) == 0 && S_ISDIR(st.st_mode)) {
                    /* Directorio: color azul con indicador "/" */
                    printf(COLOR_BLUE "  📁 %s/\n" COLOR_RESET, dir->d_name);
                } else {
                    /* Archivo regular: color blanco */
                    printf("  📄 %s\n", dir->d_name);
                }
                n_archivos++;
            }
        }
        closedir(d);

        printf(COLOR_DIM " ─────────────────────────────\n" COLOR_RESET);
        printf(COLOR_DIM "  Total: %d elemento(s)\n" COLOR_RESET, n_archivos);
    } else {
        printf(COLOR_RED "[ERROR] No se pudo abrir el directorio actual.\n"
               COLOR_RESET);
        perror("opendir");
    }

    (void)args;
}

/**
 * @brief Comando LEER (cat)
 *
 * Abre un archivo en modo lectura y muestra su contenido con
 * cabecera y pie decorativos.
 *
 * @param args args[1] debe contener la ruta o nombre del archivo a leer.
 */
void cmd_leer(char **args) {
    if (args[1] == NULL) {
        printf(COLOR_YELLOW "Uso: " COLOR_RESET "leer <nombre_archivo>\n");
        return;
    }

    FILE *fp = fopen(args[1], "r");
    if (fp == NULL) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET
               " No se pudo abrir '%s'. Verifique que exista.\n", args[1]);
        return;
    }

    /* Cabecera decorativa */
    printf(COLOR_CYAN "\n ── Contenido de '%s' ──\n" COLOR_RESET, args[1]);
    printf(COLOR_DIM "─────────────────────────────────\n" COLOR_RESET);

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }

    printf(COLOR_DIM "\n─────────────────────────────────\n\n" COLOR_RESET);
    fclose(fp);
}
