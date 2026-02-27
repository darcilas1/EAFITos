/**
 * @file shell_loop.c
 * @brief Corazón operativo de la shell (Lógica de control).
 *
 * Contiene el bucle principal (Read-Eval-Print Loop) y la lógica para
 * despachar (dispatch) la ejecución de comandos utilizando una tabla de búsqueda.
 *
 * Novedades:
 *  - Prompt personalizable con colores ANSI.
 *  - Manejo de señales SIGINT (Ctrl+C) y SIGTSTP (Ctrl+Z) con sigaction().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>   /* sigaction, SIGINT, SIGTSTP */
#include <unistd.h>   /* write, STDOUT_FILENO */
#include "shell.h"
#include "commands.h"
#include "colors.h"

/* =============================================================================
 * Variable Global: Prompt personalizable
 * =============================================================================
 * Definida aquí (extern declarada en shell.h) para que cmd_prompt() en
 * basic_commands.c pueda modificarla directamente.
 */
char prompt_personalizado[MAX_PROMPT_LEN] = "EAFITos";

/*
 * --- Registro de Comandos ---
 * Para evitar una larga cadena de 'if-else if-else', usamos dos arreglos paralelos:
 * 1. nombres_comandos: Lista de cadenas con los nombres reconocidos.
 * 2. func_comandos: Lista de punteros a funciones correspondientes.
 *
 * Para añadir un comando: agregar su nombre en nombres_comandos[] y su
 * función en func_comandos[] en la misma posición.
 */

char *nombres_comandos[] = {
    "listar",
    "leer",
    "tiempo",
    "calc",
    "ayuda",
    "salir",
    "crear",
    "limpiar",
    "eliminar",
    "buscar",
    "prompt"   /* Feature 1: Comando para cambiar el prompt */
};

/*
 * Puntero a función: void (*func)(char **)
 * Significa: "La dirección de una función que no retorna nada (void)
 * y recibe como parámetro un arreglo de cadenas (char **)".
 */
void (*func_comandos[]) (char **) = {
    &cmd_listar,
    &cmd_leer,
    &cmd_tiempo,
    &cmd_calc,
    &cmd_ayuda,
    &cmd_salir,
    &cmd_crear_archivo,
    &cmd_limpiar,
    &cmd_eliminar_archivo,
    &cmd_buscar,
    &cmd_prompt   /* Feature 1: Puntero al nuevo comando prompt */
};

/**
 * @brief Calcula dinámicamente cuántos comandos hay registrados.
 *
 * @return int El número de comandos (tamaño del array / tamaño de un elemento ptr).
 */
int num_comandos() {
    return sizeof(nombres_comandos) / sizeof(char *);
}

/* =============================================================================
 * Feature 3: Manejadores de Señales
 * =============================================================================
 *
 * POSIX exige que las funciones de manejo de señales sean "async-signal-safe".
 * Por eso usamos write() en lugar de printf() dentro de los manejadores,
 * ya que printf() no está garantizado como seguro en este contexto.
 */

/**
 * @brief Manejador de SIGINT (Ctrl+C).
 *
 * En lugar de terminar el proceso, imprime un mensaje orientador y
 * muestra el prompt de nuevo para continuar la sesión.
 *
 * @param sig Número de la señal recibida (siempre SIGINT aquí).
 */
static void manejador_sigint(int sig) {
    (void)sig; /* Silencia advertencia de parámetro no usado */

    /* Usamos write() porque es async-signal-safe */
    const char *msg = "\n" COLOR_YELLOW "[Ctrl+C]" COLOR_RESET
                      " Usa 'salir' para terminar la shell.\n";
    write(STDOUT_FILENO, msg, strlen(msg));

    /* Reescribimos el prompt manualmente para que el usuario sepa que sigue activo */
    write(STDOUT_FILENO, COLOR_CYAN, strlen(COLOR_CYAN));
    write(STDOUT_FILENO, prompt_personalizado, strlen(prompt_personalizado));
    write(STDOUT_FILENO, COLOR_RESET "> ", strlen(COLOR_RESET "> "));
}

/**
 * @brief Manejador de SIGTSTP (Ctrl+Z).
 *
 * Informa al usuario que la suspensión de procesos no está implementada
 * en EAFITos. Continúa la sesión normalmente.
 *
 * @param sig Número de la señal recibida (siempre SIGTSTP aquí).
 */
static void manejador_sigtstp(int sig) {
    (void)sig;

    const char *msg = "\n" COLOR_YELLOW "[Ctrl+Z]" COLOR_RESET
                      " Suspensión de procesos no disponible en EAFITos.\n";
    write(STDOUT_FILENO, msg, strlen(msg));

    write(STDOUT_FILENO, COLOR_CYAN, strlen(COLOR_CYAN));
    write(STDOUT_FILENO, prompt_personalizado, strlen(prompt_personalizado));
    write(STDOUT_FILENO, COLOR_RESET "> ", strlen(COLOR_RESET "> "));
}

/**
 * @brief Registra los manejadores de señales usando sigaction().
 *
 * Se prefiere sigaction() sobre signal() porque:
 *  1. Es el estándar POSIX recomendado.
 *  2. Permite más control (ej: SA_RESTART para auto-reiniciar syscalls).
 *  3. El comportamiento de signal() varía entre sistemas.
 */
static void registrar_manejadores_senales(void) {
    struct sigaction sa_int, sa_tstp;

    /* --- Configurar SIGINT (Ctrl+C) --- */
    sa_int.sa_handler = manejador_sigint;
    sigemptyset(&sa_int.sa_mask);   /* No bloquear señales adicionales */
    sa_int.sa_flags = SA_RESTART;   /* Reiniciar syscalls interrumpidas (como getline) */
    sigaction(SIGINT, &sa_int, NULL);

    /* --- Configurar SIGTSTP (Ctrl+Z) --- */
    sa_tstp.sa_handler = manejador_sigtstp;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_tstp, NULL);
}

/**
 * @brief Busca y ejecuta el comando solicitado por el usuario.
 *
 * @param args Lista de argumentos parseados. args[0] es el nombre del comando.
 */
void ejecutar(char **args) {
    if (args[0] == NULL) {
        /* El usuario presionó Enter sin escribir nada. */
        return;
    }

    /* Recorremos el registro de comandos buscando una coincidencia. */
    for (int i = 0; i < num_comandos(); i++) {
        /* strcmp: Retorna 0 si dos cadenas son idénticas. */
        if (strcmp(args[0], nombres_comandos[i]) == 0) {
            /* ¡Coincidencia encontrada! Llamamos a la función a través del puntero. */
            (*func_comandos[i])(args);
            return;
        }
    }

    /* Si salimos del ciclo, el comando no existe. */
    printf(COLOR_RED "Comando desconocido: " COLOR_BOLD "%s\n" COLOR_RESET,
           args[0]);
    printf("Escribe " COLOR_CYAN "'ayuda'" COLOR_RESET
           " para ver los comandos disponibles.\n");
}

/**
 * @brief Bucle principal Read-Eval-Print Loop (REPL).
 *
 * Ciclo de vida:
 *  1. Read:    Muestra el prompt colorizado y lee la entrada.
 *  2. Parse:   Divide la entrada en argumentos.
 *  3. Execute: Busca y ejecuta el comando.
 *  4. Loop:    Libera memoria y repite.
 */
void loop_shell() {
    char *linea;      /* Almacenará la línea cruda */
    char **args;      /* Almacenará los tokens */
    int status = 1;   /* Variable de control del bucle */

    /* Feature 3: Registrar manejadores de señales ANTES del loop */
    registrar_manejadores_senales();

    do {
        /* Feature 1: Prompt colorizado usando la variable global */
        printf(COLOR_CYAN COLOR_BOLD "%s" COLOR_RESET "> ", prompt_personalizado);
        fflush(stdout); /* Asegurar que el prompt aparezca antes de leer */

        /* 1. Lectura */
        linea = leer_linea();

        /* 2. Parseo */
        args = parsear_linea(linea);

        /* 3. Ejecución */
        ejecutar(args);

        /* 4. Limpieza de memoria (Gestión manual requerida en C) */
        free(linea); /* Libera el buffer de getline */
        free(args);  /* Libera el arreglo de punteros */

    } while (status); /* El loop corre hasta que cmd_salir hace exit(0) */
}
