/**
 * @file colors.h
 * @brief Macros de colores ANSI para la interfaz de la shell EAFITos.
 *
 * Proporciona secuencias de escape ANSI para colorear la salida de la terminal.
 * Uso: printf(COLOR_GREEN "Texto verde" COLOR_RESET "\n");
 *
 * Referencia de secuencias ANSI:
 *   \033[  -> ESC (inicio de secuencia)
 *   1;     -> Negrita (bold)
 *   3X     -> Color de texto (30-37)
 *   0m     -> Reset (vuelve al color por defecto)
 */

#ifndef COLORS_H
#define COLORS_H

/* --- Colores de texto normales --- */
#define COLOR_RESET     "\033[0m"
#define COLOR_BOLD      "\033[1m"
#define COLOR_DIM       "\033[2m"

/* --- Colores de texto brillantes (bold) --- */
#define COLOR_RED       "\033[1;31m"
#define COLOR_GREEN     "\033[1;32m"
#define COLOR_YELLOW    "\033[1;33m"
#define COLOR_BLUE      "\033[1;34m"
#define COLOR_MAGENTA   "\033[1;35m"
#define COLOR_CYAN      "\033[1;36m"
#define COLOR_WHITE     "\033[1;37m"

/* --- Colores de texto normales (sin bold) --- */
#define COLOR_RED_N     "\033[0;31m"
#define COLOR_GREEN_N   "\033[0;32m"
#define COLOR_YELLOW_N  "\033[0;33m"
#define COLOR_CYAN_N    "\033[0;36m"

/* --- Macros de conveniencia para tipos de mensajes --- */
#define MSG_ERROR(msg)   COLOR_RED    "[ERROR] " msg COLOR_RESET
#define MSG_OK(msg)      COLOR_GREEN  "[OK] "    msg COLOR_RESET
#define MSG_WARN(msg)    COLOR_YELLOW "[AVISO] " msg COLOR_RESET
#define MSG_INFO(msg)    COLOR_CYAN   "[INFO] "  msg COLOR_RESET

#endif /* COLORS_H */
