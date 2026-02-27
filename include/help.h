/**
 * @file help.h
 * @brief Sistema de ayuda por comando para EAFITos.
 *
 * Define la estructura CommandHelp y la tabla de ayuda detallada
 * que permite a cmd_ayuda mostrar información específica por comando.
 */

#ifndef HELP_H
#define HELP_H

/**
 * @brief Estructura que agrupa la información de ayuda de un comando.
 *
 * Cada comando del sistema debe tener una entrada correspondiente en
 * la tabla `tabla_ayuda[]` definida en src/utils/help.c.
 */
typedef struct {
    const char *nombre;       /**< Nombre del comando (clave de búsqueda) */
    const char *descripcion;  /**< Qué hace el comando */
    const char *uso;          /**< Sintaxis del comando */
    const char *ejemplo;      /**< Ejemplo concreto de uso */
    const char *notas;        /**< Notas adicionales o casos de borde */
} CommandHelp;

/** @brief Tabla global de ayuda detallada (definida en help.c). */
extern CommandHelp tabla_ayuda[];

/** @brief Número de entradas en la tabla (calculado en help.c). */
extern int num_ayudas;

/**
 * @brief Busca y muestra la ayuda detallada de un comando específico.
 * @param nombre Nombre del comando a buscar.
 * @return 1 si el comando fue encontrado, 0 si no existe.
 */
int mostrar_ayuda_comando(const char *nombre);

#endif /* HELP_H */
