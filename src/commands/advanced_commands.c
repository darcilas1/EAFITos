/**
 * @file advanced_commands.c
 * @brief Implementacion de comandos avanzados del sistema.
 *
 * Contiene comandos que requieren interaccion adicional con el usuario,
 * como la creacion de archivos con confirmacion de sobrescritura.
 */

#include <stdio.h>
#include <string.h>
#include "commands.h"

/**
 * @brief Comando CREAR_ARCHIVO (crear un archivo vacio)
 *
 * Si el archivo ya existe, pregunta al usuario si desea sobrescribirlo.
 * Si el usuario responde que no, se retorna al menu principal.
 *
 * @param args args[1] debe contener el nombre del archivo a crear.
 */
void cmd_crear_archivo(char **args) {
    if (args[1] == NULL) {
        printf("Error: Debes darle un nombre al archivo.\nUso: crear <nombre_archivo>\n");
        return;
    }

    const char *nombre = args[1];

    FILE *fp = fopen(nombre, "r");
    if (fp != NULL) {
        fclose(fp);
        printf("El archivo '%s' ya existe. Desea sobrescribirlo? (s/n): ", nombre);

        char respuesta[8] = {0};
        if (fgets(respuesta, sizeof(respuesta), stdin) == NULL) {
            printf("No se pudo leer la respuesta. Operacion cancelada.\n");
            return;
        }

        if (respuesta[0] != 's' && respuesta[0] != 'S') {
            printf("Operacion cancelada. Volviendo al menu principal.\n");
            return;
        }
    }

    fp = fopen(nombre, "w");
    if (fp == NULL) {
        printf("Error: No se pudo crear o sobrescribir el archivo '%s'.\n", nombre);
        return;
    }

    fclose(fp);
    printf("Archivo '%s' creado correctamente.\n", nombre);
}
