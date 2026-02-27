/**
 * @file commands.h
 * @brief Interfaz para los comandos del sistema.
 *
 * Define las funciones que implementan la lógica de cada comando disponible
 * en la shell. Todas siguen la firma `void cmd_nombre(char **args)`.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

// --- Prototipos de Comandos ---

/** @brief Lista los archivos del directorio actual. */
void cmd_listar(char **args);

/** @brief Lee y muestra el contenido de un archivo. */
void cmd_leer(char **args);

/** @brief Muestra la hora del sistema. */
void cmd_tiempo(char **args);

/** @brief Realiza operaciones matemáticas simples. */
void cmd_calc(char **args);

/** @brief Muestra la lista de ayuda general o detallada por comando. */
void cmd_ayuda(char **args);

/** @brief Termina la ejecución de la shell. */
void cmd_salir(char **args);

/** @brief Crea un archivo (con confirmación si ya existe). */
void cmd_crear_archivo(char **args);

/** @brief Limpia la pantalla de la terminal. */
void cmd_limpiar(char **args);

/** @brief Elimina un archivo del sistema (con confirmación). */
void cmd_eliminar_archivo(char **args);

/** @brief Busca texto en archivos. */
void cmd_buscar(char **args);

/**
 * @brief Cambia el texto del prompt de la shell en tiempo de ejecución.
 *
 * Uso: prompt <nuevo_texto>
 * Ejemplo: prompt MiShell
 */
void cmd_prompt(char **args);

// --- Utilidades del Registro de Comandos ---

/** @brief Retorna el número total de comandos registrados. */
int num_comandos();

#endif
