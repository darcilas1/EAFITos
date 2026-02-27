/**
 * @file basic_commands.c
 * @brief Implementación de comandos básicos del sistema.
 *
 * Novedades:
 *  - cmd_ayuda: soporta 'ayuda <comando>' para ayuda detallada por comando.
 *  - cmd_prompt: nuevo comando para personalizar el indicador de la shell.
 *  - Salida colorizada con macros de colors.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commands.h"
#include "shell.h"    /* Para prompt_personalizado y MAX_PROMPT_LEN */
#include "colors.h"   /* Para macros de color ANSI */
#include "help.h"     /* Para mostrar_ayuda_comando() */

/**
 * @brief Comando AYUDA
 *
 * Sin argumentos → muestra la lista general de comandos.
 * Con argumento  → delega en mostrar_ayuda_comando() para ayuda detallada.
 *
 * @param args args[0]="ayuda", args[1]=nombre_comando (opcional).
 */
void cmd_ayuda(char **args) {
    /* Feature 2: Si hay argumento, mostrar ayuda específica del comando */
    if (args[1] != NULL) {
        if (!mostrar_ayuda_comando(args[1])) {
            printf(COLOR_RED "No existe ayuda para el comando: " COLOR_BOLD "'%s'\n"
                   COLOR_RESET, args[1]);
            printf("Escribe " COLOR_CYAN "'ayuda'" COLOR_RESET
                   " sin argumentos para ver todos los comandos.\n");
        }
        return;
    }

    /* --- Ayuda general: lista todos los comandos --- */
    printf("\n");
    printf(COLOR_CYAN COLOR_BOLD
           "╔══════════════════════════════════════════╗\n"
           "║         EAFITos — Comandos Disponibles  ║\n"
           "╚══════════════════════════════════════════╝\n"
           COLOR_RESET);

    printf(COLOR_YELLOW "\n  Archivos y Directorios:\n" COLOR_RESET);
    printf(COLOR_GREEN "    listar" COLOR_RESET
           "                    Lista archivos del directorio actual.\n");
    printf(COLOR_GREEN "    leer" COLOR_RESET
           "   <archivo>        Muestra el contenido de un archivo.\n");
    printf(COLOR_GREEN "    crear" COLOR_RESET
           "  <archivo>        Crea un archivo nuevo.\n");
    printf(COLOR_GREEN "    eliminar" COLOR_RESET
           " <archivo>        Elimina un archivo con confirmación.\n");
    printf(COLOR_GREEN "    buscar" COLOR_RESET
           "  <texto> <arch>  Busca texto dentro de un archivo.\n");

    printf(COLOR_YELLOW "\n  Sistema:\n" COLOR_RESET);
    printf(COLOR_GREEN "    tiempo" COLOR_RESET
           "                   Muestra la fecha y hora actual.\n");
    printf(COLOR_GREEN "    calc" COLOR_RESET
           "   <n1> <op> <n2>  Realiza cálculos (+, -, *, /).\n");
    printf(COLOR_GREEN "    limpiar" COLOR_RESET
           "                  Limpia la pantalla.\n");

    printf(COLOR_YELLOW "\n  Shell:\n" COLOR_RESET);
    printf(COLOR_GREEN "    prompt" COLOR_RESET
           "  <texto>          Cambia el indicador de la shell.\n");
    printf(COLOR_GREEN "    ayuda" COLOR_RESET
           "   [comando]       Muestra esta ayuda o la de un comando.\n");
    printf(COLOR_GREEN "    salir" COLOR_RESET
           "                   Termina la sesión.\n");

    printf(COLOR_DIM "\n  Tip: escribe " COLOR_RESET
           COLOR_CYAN "'ayuda <comando>'" COLOR_RESET
           COLOR_DIM " para ver detalles, uso y ejemplos.\n\n" COLOR_RESET);
}

/**
 * @brief Comando SALIR
 *
 * Finaliza la ejecución del programa de forma controlada.
 *
 * @param args Argumentos del comando (ignorados).
 */
void cmd_salir(char **args) {
    printf(COLOR_CYAN "Saliendo de EAFITos. ¡Hasta pronto!\n" COLOR_RESET);
    exit(0);
    (void)args;
}

/**
 * @brief Comando TIEMPO
 *
 * Obtiene y formatea la fecha y hora del sistema.
 *
 * @param args Argumentos del comando (ignorados).
 */
void cmd_tiempo(char **args) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf(COLOR_CYAN "  Fecha y Hora del Sistema: " COLOR_RESET
           COLOR_BOLD "%02d-%02d-%04d %02d:%02d:%02d\n" COLOR_RESET,
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
           tm.tm_hour, tm.tm_min, tm.tm_sec);

    (void)args;
}

/**
 * @brief Comando PROMPT — Feature 1
 *
 * Cambia el texto del indicador (prompt) de la shell en tiempo de ejecución.
 * El cambio persiste durante toda la sesión.
 *
 * @param args args[1] debe contener el nuevo texto del prompt.
 */
void cmd_prompt(char **args) {
    if (args[1] == NULL) {
        printf(COLOR_YELLOW "Uso: " COLOR_RESET "prompt <nuevo_texto>\n");
        printf(COLOR_DIM "Prompt actual: '%s'\n" COLOR_RESET,
               prompt_personalizado);
        return;
    }

    /* strncpy garantiza que no desbordamos MAX_PROMPT_LEN */
    strncpy(prompt_personalizado, args[1], MAX_PROMPT_LEN - 1);
    prompt_personalizado[MAX_PROMPT_LEN - 1] = '\0'; /* Asegurar terminador */

    printf(COLOR_GREEN "Prompt actualizado a: " COLOR_BOLD "'%s'\n" COLOR_RESET,
           prompt_personalizado);
}
