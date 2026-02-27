/**
 * @file system_commands.c
 * @brief Comandos de utilería del sistema.
 * 
 * Actualmente contiene la calculadora, pero podría expandirse para incluir
 * gestión de procesos o informes de memoria.
 */

#include <stdio.h>
#include <stdlib.h>  /* Para atof (ASCII to Float conversion) */
#include "commands.h"
#include "colors.h"

/**
 * @brief Comando LIMPIAR
 * 
 * Limpia la pantalla de la terminal utilizando secuencias de escape ANSI.
 * Es equivalente al comando 'clear' en Unix o 'cls' en Windows.
 * 
 * @param args Argumentos del comando (ignorados).
 */
void cmd_limpiar(char **args) {
    /*
     * Secuencia de limpieza completa (equivalente al comando 'clear'):
     *   \033[2J  → Borra la pantalla visible completa
     *   \033[3J  → Borra el scrollback buffer (historial de la terminal)
     *   \033[H   → Mueve el cursor a la esquina superior izquierda (Home)
     *
     * La diferencia con \033[H\033[J (versión anterior):
     *   La versión anterior solo ocultaba el contenido moviéndolo fuera
     *   de la vista, pero al deslizar hacia arriba seguía siendo visible.
     *   Con \033[3J se elimina completamente del buffer de la terminal.
     */
    printf("\033[2J\033[3J\033[H");
    fflush(stdout);
    (void)args;
}

/**
 * @brief Comando CALC (Calculadora)
 * 
 * Realiza operaciones aritméticas básicas entre dos números.
 * Sintaxis esperada: calc <num1> <operador> <num2>
 * 
 * @param args Lista de argumentos donde:
 *             args[1]: Primer operando (cadena)
 *             args[2]: Operador (+, -, *, /)
 *             args[3]: Segundo operando (cadena)
 */
void cmd_calc(char **args) {
    // 1. Validación de argumentos. Necesitamos exáctamente 3 partes después del comando.
    if (args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        printf(COLOR_YELLOW "Uso: " COLOR_RESET
               "calc <num1> <operador> <num2>\n"
               COLOR_DIM "Ejemplo: calc 5 + 3\n" COLOR_RESET);
        return;
    }

    // 2. Conversión de tipos (Parsing)
    // atof(): Convierte una cadena numérica a un float (double precision).
    float n1 = atof(args[1]);
    char op = args[2][0]; // Tomamos el primer caracter del argumento operador
    float n2 = atof(args[3]);
    float res = 0;

    // 3. Lógica de selección (Switch)
    switch(op) {
        case '+': 
            res = n1 + n2; 
            break;
        case '-': 
            res = n1 - n2; 
            break;
        case '*': 
        case 'x': // Permitimos 'x' como alias de multiplicación
            res = n1 * n2; 
            break;
        case '/':
            if (n2 == 0) {
                printf(COLOR_RED "[ERROR]" COLOR_RESET
                       " División por cero no permitida.\n");
                return;
            }
            res = n1 / n2;
            break;
        default:
            printf(COLOR_RED "[ERROR]" COLOR_RESET
                   " Operador '%c' no reconocido. Use +, -, * o /.\n", op);
            return;
    }

    /* Resultado en verde */
    printf(COLOR_GREEN "  Resultado: " COLOR_BOLD "%.2f\n" COLOR_RESET, res);
}
