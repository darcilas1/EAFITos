/**
 * @file help.c
 * @brief Implementación del sistema de ayuda por comando de EAFITos.
 *
 * Define la tabla de ayuda detallada para todos los comandos y
 * la función que busca y muestra la información de un comando específico.
 */

#include <stdio.h>
#include <string.h>
#include "help.h"
#include "colors.h"

/**
 * @brief Tabla de ayuda detallada por comando.
 *
 * Cada entrada contiene nombre, descripción, sintaxis, ejemplo y notas.
 * Para añadir un nuevo comando, agrega una entrada aquí.
 */
CommandHelp tabla_ayuda[] = {
    {
        "listar",
        "Lista todos los archivos y directorios del directorio actual.",
        "listar",
        "listar",
        "Equivalente a 'ls' en Unix. No acepta argumentos adicionales por ahora."
    },
    {
        "leer",
        "Muestra el contenido completo de un archivo de texto en pantalla.",
        "leer <nombre_archivo>",
        "leer README.md",
        "El archivo debe existir y ser legible. Similar al comando 'cat' de Unix."
    },
    {
        "tiempo",
        "Muestra la fecha y hora actual del sistema.",
        "tiempo",
        "tiempo",
        "Obtiene la hora local del sistema operativo. No acepta argumentos."
    },
    {
        "calc",
        "Realiza operaciones aritméticas básicas entre dos números.",
        "calc <num1> <operador> <num2>",
        "calc 10 + 5\ncalc 3.14 * 2\ncalc 100 / 4",
        "Operadores soportados: + (suma), - (resta), * o x (multiplicación), / (división).\nLa división por cero está protegida."
    },
    {
        "crear",
        "Crea un archivo vacío. Si ya existe, pide confirmación antes de sobreescribirlo.",
        "crear <nombre_archivo>",
        "crear nuevo.txt",
        "Si el archivo existe, verás una pregunta de confirmación (s/n)."
    },
    {
        "eliminar",
        "Elimina un archivo del sistema con confirmación previa.",
        "eliminar <nombre_archivo>",
        "eliminar viejo.txt",
        "Siempre pide confirmación antes de borrar. La operación es irreversible."
    },
    {
        "buscar",
        "Busca una cadena de texto dentro de un archivo línea por línea.",
        "buscar <texto> <nombre_archivo>",
        "buscar hola notas.txt",
        "Muestra el número de línea y el contenido donde se encontró el texto.\nLa búsqueda es sensible a mayúsculas/minúsculas."
    },
    {
        "limpiar",
        "Limpia la pantalla de la terminal.",
        "limpiar",
        "limpiar",
        "Usa secuencias de escape ANSI. Equivalente a 'clear' en Unix."
    },
    {
        "prompt",
        "Cambia el texto que aparece como indicador (prompt) de la shell.",
        "prompt <nuevo_texto>",
        "prompt MiShell\nprompt [SO]",
        "El prompt nuevo se mantiene durante toda la sesión. Máximo 63 caracteres."
    },
    {
        "ayuda",
        "Muestra la lista de comandos disponibles o la ayuda detallada de uno específico.",
        "ayuda [comando]",
        "ayuda\nayuda calc\nayuda buscar",
        "Sin argumentos: lista todos los comandos.\nCon argumento: muestra descripción detallada del comando indicado."
    },
    {
        "salir",
        "Termina la sesión de EAFITos y cierra el programa.",
        "salir",
        "salir",
        "Devuelve el código de salida 0 al sistema operativo (EXIT_SUCCESS)."
    }
};

/** @brief Número de comandos en la tabla (calculado automáticamente). */
int num_ayudas = sizeof(tabla_ayuda) / sizeof(CommandHelp);

/**
 * @brief Busca y muestra la ayuda detallada de un comando específico.
 *
 * Recorre la tabla_ayuda buscando el comando por nombre y, si lo
 * encuentra, muestra su información con formato colorizado.
 *
 * @param nombre Nombre del comando a buscar.
 * @return 1 si fue encontrado, 0 si no existe.
 */
int mostrar_ayuda_comando(const char *nombre) {
    for (int i = 0; i < num_ayudas; i++) {
        if (strcmp(nombre, tabla_ayuda[i].nombre) == 0) {
            printf("\n");
            printf(COLOR_CYAN COLOR_BOLD "═══════════════════════════════════════\n" COLOR_RESET);
            printf(COLOR_BOLD " Comando: " COLOR_CYAN "%s\n" COLOR_RESET, tabla_ayuda[i].nombre);
            printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);

            printf(COLOR_YELLOW " Descripción:\n" COLOR_RESET);
            printf("   %s\n\n", tabla_ayuda[i].descripcion);

            printf(COLOR_YELLOW " Uso:\n" COLOR_RESET);
            printf("   " COLOR_GREEN "%s\n" COLOR_RESET "\n", tabla_ayuda[i].uso);

            printf(COLOR_YELLOW " Ejemplo(s):\n" COLOR_RESET);
            printf("   " COLOR_GREEN_N "%s\n" COLOR_RESET "\n", tabla_ayuda[i].ejemplo);

            printf(COLOR_YELLOW " Notas:\n" COLOR_RESET);
            printf("   " COLOR_DIM "%s\n" COLOR_RESET, tabla_ayuda[i].notas);
            printf("\n");
            return 1;
        }
    }
    return 0;
}
