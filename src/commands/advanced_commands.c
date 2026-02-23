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

/**
 * @brief comando ELIMINAR_ARCHIVO
 * 
 * Elimina un archivo del sistema 
 * si el archivo no existe, muestra un mensaje de error.
 * 
 * @param args args [1] debe contener el nombre del arcivo a eliminar
 */

 void cmd_eliminar_archivo(char **args) {
    if (args[1] == NULL) {
        printf("Error: Debes indicar el nombre del archivo.\nUso: eliminar <nombre_archivo>\n");
        return;
    }

    const char *nombre = args[1];

    FILE *fp = fopen(nombre, "r");
    if (fp == NULL) {
        printf("Error: El archivo '%s' no existe.\n", nombre);
        return;
    }
    fclose(fp);

    printf("Estás seguro de eliminar '%s'? (s/n): ", nombre);
    char respuesta[8] = {0};
    if (fgets(respuesta, sizeof(respuesta), stdin) == NULL) {
        printf("No se pudo leer la respuesta.\n");
        return;
    }

    if (respuesta[0] != 's' && respuesta[0] != 'S') {
        printf("Operación cancelada\n");
        return;
    }

    if (remove(nombre) == 0) {
        printf("Archivo '%s' eliminado \n", nombre);
    } else {
        perror("Error al eliminar el archivo");
    }
 }

 /**
 * @brief Comando BUSCAR
 * 
 * Busca una cadena de texto dentro de un archivo linea por linea.
 * Si el texto es encontrado, muestra el numero de linea y el contenido.
 * 
 * @param args args[1] debe contener el texto a buscar
 *             args[2] debe contener el nombre del archivo
 */

void cmd_buscar(char **args) {

    if (args[1] == NULL || args[2] == NULL) {
        printf("Error: Uso incorrecto del comando.\n");
        printf("Uso: buscar <texto> <nombre_archivo>\n");
        return;
    }

    const char *texto = args[1];
    const char *nombre = args[2];

    FILE *fp = fopen(nombre, "r");
    if (fp == NULL) {
        printf("Error: El archivo '%s' no existe o no se puede abrir.\n", nombre);
        return;
    }

    char linea[1024];
    int numeroLinea = 1;
    int encontrados = 0;

    while (fgets(linea, sizeof(linea), fp) != NULL) {

        if (strstr(linea, texto) != NULL) {
            printf("%d: %s", numeroLinea, linea);
            encontrados++;
        }

        numeroLinea++;
    }

    if (encontrados == 0) {
        printf("No se encontro el texto '%s' en el archivo '%s'.\n", texto, nombre);
    } else {
        printf("\nTotal de coincidencias encontradas: %d\n", encontrados);
    }

    fclose(fp);
}

