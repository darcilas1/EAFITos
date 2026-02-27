/**
 * @file advanced_commands.c
 * @brief Implementación de comandos avanzados del sistema.
 *
 * Contiene comandos que requieren interacción adicional con el usuario,
 * como la creación de archivos con confirmación de sobrescritura.
 * Salida colorizada con colors.h.
 */

#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "colors.h"

/**
 * @brief Comando CREAR_ARCHIVO
 *
 * Si el archivo ya existe, pregunta al usuario si desea sobrescribirlo.
 *
 * @param args args[1] debe contener el nombre del archivo a crear.
 */
void cmd_crear_archivo(char **args) {
    if (args[1] == NULL) {
        printf(COLOR_YELLOW "Uso: " COLOR_RESET "crear <nombre_archivo>\n");
        return;
    }

    const char *nombre = args[1];

    FILE *fp = fopen(nombre, "r");
    if (fp != NULL) {
        fclose(fp);
        printf(COLOR_YELLOW "El archivo '%s' ya existe. ¿Desea sobreescribirlo? (s/n): "
               COLOR_RESET, nombre);

        char respuesta[8] = {0};
        if (fgets(respuesta, sizeof(respuesta), stdin) == NULL) {
            printf(COLOR_RED "[ERROR]" COLOR_RESET
                   " No se pudo leer la respuesta. Operación cancelada.\n");
            return;
        }

        if (respuesta[0] != 's' && respuesta[0] != 'S') {
            printf(COLOR_DIM "Operación cancelada.\n" COLOR_RESET);
            return;
        }
    }

    fp = fopen(nombre, "w");
    if (fp == NULL) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET
               " No se pudo crear el archivo '%s'.\n", nombre);
        return;
    }

    fclose(fp);
    printf(COLOR_GREEN "  Archivo '%s' creado correctamente.\n" COLOR_RESET, nombre);
}

/**
 * @brief Comando ELIMINAR_ARCHIVO
 *
 * Elimina un archivo del sistema con confirmación previa.
 *
 * @param args args[1] debe contener el nombre del archivo a eliminar.
 */
void cmd_eliminar_archivo(char **args) {
    if (args[1] == NULL) {
        printf(COLOR_YELLOW "Uso: " COLOR_RESET "eliminar <nombre_archivo>\n");
        return;
    }

    const char *nombre = args[1];

    FILE *fp = fopen(nombre, "r");
    if (fp == NULL) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET
               " El archivo '%s' no existe.\n", nombre);
        return;
    }
    fclose(fp);

    printf(COLOR_YELLOW "¿Estás seguro de eliminar '%s'? (s/n): " COLOR_RESET,
           nombre);
    char respuesta[8] = {0};
    if (fgets(respuesta, sizeof(respuesta), stdin) == NULL) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " No se pudo leer la respuesta.\n");
        return;
    }

    if (respuesta[0] != 's' && respuesta[0] != 'S') {
        printf(COLOR_DIM "Operación cancelada.\n" COLOR_RESET);
        return;
    }

    if (remove(nombre) == 0) {
        printf(COLOR_GREEN "  Archivo '%s' eliminado correctamente.\n" COLOR_RESET,
               nombre);
    } else {
        printf(COLOR_RED "[ERROR]" COLOR_RESET " No se pudo eliminar '%s'.\n",
               nombre);
        perror("remove");
    }
}

/**
 * @brief Comando BUSCAR
 *
 * Busca una cadena de texto dentro de un archivo línea por línea.
 * Si el texto es encontrado, muestra el número de línea y el contenido.
 *
 * @param args args[1] texto a buscar, args[2] nombre del archivo.
 */
void cmd_buscar(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf(COLOR_YELLOW "Uso: " COLOR_RESET
               "buscar <texto> <nombre_archivo>\n");
        return;
    }

    const char *texto  = args[1];
    const char *nombre = args[2];

    FILE *fp = fopen(nombre, "r");
    if (fp == NULL) {
        printf(COLOR_RED "[ERROR]" COLOR_RESET
               " El archivo '%s' no existe o no se puede abrir.\n", nombre);
        return;
    }

    printf(COLOR_CYAN "\n Buscando '" COLOR_BOLD "%s" COLOR_RESET
           COLOR_CYAN "' en '%s':\n" COLOR_RESET, texto, nombre);
    printf(COLOR_DIM "─────────────────────────────────\n" COLOR_RESET);

    char linea[1024];
    int numeroLinea = 1;
    int encontrados = 0;

    while (fgets(linea, sizeof(linea), fp) != NULL) {
        if (strstr(linea, texto) != NULL) {
            printf(COLOR_YELLOW " %3d:" COLOR_RESET " %s",
                   numeroLinea, linea);
            encontrados++;
        }
        numeroLinea++;
    }

    printf(COLOR_DIM "─────────────────────────────────\n" COLOR_RESET);
    if (encontrados == 0) {
        printf(COLOR_YELLOW "  No se encontró '%s' en '%s'.\n" COLOR_RESET,
               texto, nombre);
    } else {
        printf(COLOR_GREEN "  Total de coincidencias: %d\n" COLOR_RESET,
               encontrados);
    }
    printf("\n");

    fclose(fp);
}
