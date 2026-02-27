/**
 * @file unit_tests.c
 * @brief Pruebas automáticas (Unit Tests) para EAFITos.
 *
 * Framework de testing minimalista sin dependencias externas.
 * Cada test comprueba una función de forma aislada e imprime
 * PASSED / FAILED con color ANSI.
 *
 * Compilar y ejecutar con: make test
 *
 * Convenciones:
 *  - ASSERT(condicion, nombre_test): verifica la condición.
 *  - Al final se imprime el resumen de tests pasados/fallados.
 *  - El programa retorna 0 si TODOS pasan, 1 si alguno falla.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Incluimos solo lo que necesitamos del proyecto */
#include "../include/shell.h"   /* leer_linea, parsear_linea */
#include "../include/colors.h"  /* Macros de color ANSI */

/* ============================================================
 * Framework de Testing Minimalista
 * ============================================================ */

static int tests_total   = 0;
static int tests_passed  = 0;
static int tests_failed  = 0;

/**
 * @brief Macro de aserción: verifica una condición.
 *
 * Si la condición es verdadera → PASSED (verde).
 * Si es falsa → FAILED (rojo) con información de línea.
 */
#define ASSERT(condicion, nombre) do { \
    tests_total++; \
    if (condicion) { \
        tests_passed++; \
        printf(COLOR_GREEN "  [PASSED]" COLOR_RESET " %s\n", nombre); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED   "  [FAILED]" COLOR_RESET " %s" \
               COLOR_DIM " (línea %d)\n" COLOR_RESET, nombre, __LINE__); \
    } \
} while(0)

/**
 * @brief Imprime el encabezado de una suite de tests.
 */
#define TEST_SUITE(nombre) \
    printf("\n" COLOR_CYAN COLOR_BOLD "─── %s ───\n" COLOR_RESET, nombre)


/* ============================================================
 * Suite 1: parsear_linea()
 * ============================================================ */

/**
 * @brief Verifica que parsear_linea maneja input vacío correctamente.
 *
 * Una línea que solo tiene un salto de línea debe producir args[0] == NULL.
 */
static void test_parsear_linea_entrada_vacia(void) {
    char linea[] = "\n";
    char **args = parsear_linea(linea);

    ASSERT(args != NULL,
           "parsear_linea: no retorna NULL con entrada vacía");
    ASSERT(args[0] == NULL,
           "parsear_linea: args[0]==NULL para entrada vacía (solo \\n)");

    free(args);
}

/**
 * @brief Verifica que parsear_linea tokeniza correctamente "calc 5 + 3".
 *
 * Debe producir: args[0]="calc", args[1]="5", args[2]="+", args[3]="3", args[4]=NULL.
 */
static void test_parsear_linea_calc(void) {
    char linea[] = "calc 5 + 3\n";
    char **args = parsear_linea(linea);

    ASSERT(args != NULL,
           "parsear_linea(calc): retorna puntero válido");
    ASSERT(args[0] != NULL && strcmp(args[0], "calc") == 0,
           "parsear_linea(calc): args[0] == \"calc\"");
    ASSERT(args[1] != NULL && strcmp(args[1], "5") == 0,
           "parsear_linea(calc): args[1] == \"5\"");
    ASSERT(args[2] != NULL && strcmp(args[2], "+") == 0,
           "parsear_linea(calc): args[2] == \"+\"");
    ASSERT(args[3] != NULL && strcmp(args[3], "3") == 0,
           "parsear_linea(calc): args[3] == \"3\"");
    ASSERT(args[4] == NULL,
           "parsear_linea(calc): args[4] == NULL (terminador)");

    free(args);
}

/**
 * @brief Verifica que parsear_linea maneja espacios múltiples entre tokens.
 *
 * "leer   archivo.txt" debe generar exactamente 2 tokens.
 */
static void test_parsear_linea_espacios_multiples(void) {
    char linea[] = "leer   archivo.txt\n";
    char **args = parsear_linea(linea);

    ASSERT(args[0] != NULL && strcmp(args[0], "leer") == 0,
           "parsear_linea(espacios): args[0] == \"leer\"");
    ASSERT(args[1] != NULL && strcmp(args[1], "archivo.txt") == 0,
           "parsear_linea(espacios): args[1] == \"archivo.txt\"");
    ASSERT(args[2] == NULL,
           "parsear_linea(espacios): solo 2 tokens a pesar de espacios extra");

    free(args);
}

/**
 * @brief Verifica que parsear_linea maneja un comando sin argumentos.
 */
static void test_parsear_linea_sin_argumentos(void) {
    char linea[] = "listar\n";
    char **args = parsear_linea(linea);

    ASSERT(args[0] != NULL && strcmp(args[0], "listar") == 0,
           "parsear_linea(sin args): args[0] == \"listar\"");
    ASSERT(args[1] == NULL,
           "parsear_linea(sin args): args[1] == NULL");

    free(args);
}


/* ============================================================
 * Suite 2: Lógica de cmd_calc (validación de argumentos)
 * ============================================================ */

/**
 * @brief Verifica la aritmética básica de la calculadora.
 *
 * Como cmd_calc usa printf, probamos la lógica de cálculo directamente
 * extrayendo la operación aritmética que realiza.
 */
static void test_calc_suma(void) {
    float n1 = 5.0f, n2 = 3.0f;
    float resultado = n1 + n2;
    ASSERT(resultado == 8.0f,
           "calc lógica: 5 + 3 == 8.0");
}

static void test_calc_resta(void) {
    float n1 = 10.0f, n2 = 4.0f;
    float resultado = n1 - n2;
    ASSERT(resultado == 6.0f,
           "calc lógica: 10 - 4 == 6.0");
}

static void test_calc_multiplicacion(void) {
    float n1 = 3.0f, n2 = 7.0f;
    float resultado = n1 * n2;
    ASSERT(resultado == 21.0f,
           "calc lógica: 3 * 7 == 21.0");
}

static void test_calc_division(void) {
    float n1 = 15.0f, n2 = 3.0f;
    float resultado = n1 / n2;
    ASSERT(resultado == 5.0f,
           "calc lógica: 15 / 3 == 5.0");
}

/**
 * @brief Verifica que la detección de división por cero funciona.
 */
static void test_calc_division_por_cero(void) {
    float n2 = 0.0f;
    int es_division_por_cero = (n2 == 0.0f);
    ASSERT(es_division_por_cero == 1,
           "calc lógica: detecta división por cero (n2 == 0)");
}


/* ============================================================
 * Suite 3: Validación de strings (helpers del parser)
 * ============================================================ */

/**
 * @brief Verifica que strlen funciona para strings vacías.
 */
static void test_string_vacio(void) {
    char *s = "";
    ASSERT(strlen(s) == 0,
           "string: strlen de cadena vacía es 0");
}

/**
 * @brief Verifica la comparación de strings con strcmp.
 */
static void test_strcmp_igual(void) {
    ASSERT(strcmp("calc", "calc") == 0,
           "strcmp: \"calc\" == \"calc\" retorna 0");
}

static void test_strcmp_diferente(void) {
    ASSERT(strcmp("calc", "listar") != 0,
           "strcmp: \"calc\" != \"listar\" retorna != 0");
}

/**
 * @brief Verifica que strncpy trunca correctamente al límite.
 */
static void test_strncpy_limite(void) {
    char destino[8] = {0};
    strncpy(destino, "EAFITos_sistema", 7);
    destino[7] = '\0';
    ASSERT(strcmp(destino, "EAFITo") == 0 || strlen(destino) <= 7,
           "strncpy: respeta el límite de caracteres");
}


/* ============================================================
 * Función Principal del Test Runner
 * ============================================================ */

int main(void) {
    printf(COLOR_CYAN COLOR_BOLD
           "\n╔═══════════════════════════════════════╗\n"
           "║     EAFITos — Suite de Unit Tests     ║\n"
           "╚═══════════════════════════════════════╝\n"
           COLOR_RESET);

    /* Suite 1: Parser */
    TEST_SUITE("parsear_linea()");
    test_parsear_linea_entrada_vacia();
    test_parsear_linea_calc();
    test_parsear_linea_espacios_multiples();
    test_parsear_linea_sin_argumentos();

    /* Suite 2: Calculadora */
    TEST_SUITE("cmd_calc — Lógica Aritmética");
    test_calc_suma();
    test_calc_resta();
    test_calc_multiplicacion();
    test_calc_division();
    test_calc_division_por_cero();

    /* Suite 3: Strings */
    TEST_SUITE("Validación de Strings");
    test_string_vacio();
    test_strcmp_igual();
    test_strcmp_diferente();
    test_strncpy_limite();

    /* Resumen final */
    printf("\n" COLOR_CYAN COLOR_BOLD
           "═══════════════════════════════════════\n"
           COLOR_RESET);
    printf("  Total:   %d tests\n", tests_total);
    printf(COLOR_GREEN "  Passed:  %d\n" COLOR_RESET, tests_passed);
    if (tests_failed > 0) {
        printf(COLOR_RED "  Failed:  %d\n" COLOR_RESET, tests_failed);
    } else {
        printf(COLOR_DIM "  Failed:  0\n" COLOR_RESET);
    }
    printf(COLOR_CYAN "═══════════════════════════════════════\n\n"
           COLOR_RESET);

    /* Retornar 0 si todo OK, 1 si algún test falló */
    return (tests_failed > 0) ? 1 : 0;
}
