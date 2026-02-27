# EAFITos 🎓

**EAFITos** es un sistema operativo educativo diseñado para aprender los fundamentos de la programación de sistemas y la operación interna de una shell utilizando el lenguaje C.

## 🚀 Objetivos

- Entender la **gestión de procesos** y memoria.
- Aprender sobre **hilos (threads)** y concurrencia.
- Explorar el **sistema de archivos** y llamadas al sistema (syscalls).
- Implementar **estructuras de datos básicas** usadas en kernels.

---

## 🐚 La Shell de EAFITos

El sistema inicia con una interfaz de línea de comandos (Shell) interactiva con soporte de **colores ANSI**, **prompt personalizable** y **manejo robusto de señales**.

### ¿Qué son los Argumentos (`args`)?

En una shell, cuando escribes un comando necesitas enviarle información adicional. Internamente en C, esto se maneja mediante un arreglo de cadenas (`char **args`):

- **`args[0]`**: Es siempre el nombre del comando (ej. `calc`).
- **`args[1]`, `args[2]`, etc**: Son los parámetros que le pasas al comando.

**Ejemplo con `calc 10 + 5`:**

| Posición | Valor |
|:---|:---|
| `args[0]` | `"calc"` |
| `args[1]` | `"10"` |
| `args[2]` | `"+"` |
| `args[3]` | `"5"` |

---

## 📋 Comandos Disponibles

### 📁 Archivos y Directorios

| Comando | Argumentos | Descripción | Ejemplo |
| :--- | :--- | :--- | :--- |
| `listar` | Ninguno | Lista los archivos y carpetas del directorio actual con iconos y colores. | `listar` |
| `leer` | `<archivo>` | Muestra el contenido completo de un archivo de texto. | `leer README.md` |
| `crear` | `<archivo>` | Crea un archivo vacío. Pide confirmación si ya existe. | `crear notas.txt` |
| `eliminar` | `<archivo>` | Elimina un archivo con confirmación previa. | `eliminar viejo.txt` |
| `buscar` | `<texto> <archivo>` | Busca una cadena de texto dentro de un archivo, mostrando número de línea. | `buscar hola notas.txt` |

### ⚙️ Sistema

| Comando | Argumentos | Descripción | Ejemplo |
| :--- | :--- | :--- | :--- |
| `tiempo` | Ninguno | Muestra la fecha y hora actual del sistema. | `tiempo` |
| `calc` | `<n1> <op> <n2>` | Realiza operaciones aritméticas (`+`, `-`, `*`, `/`). La `x` también funciona como `*`. | `calc 10 * 2.5` |
| `limpiar` | Ninguno | Limpia la pantalla de la terminal. | `limpiar` |

### 🖥️ Shell

| Comando | Argumentos | Descripción | Ejemplo |
| :--- | :--- | :--- | :--- |
| `prompt` | `<texto>` | Cambia el indicador de la shell en tiempo de ejecución. | `prompt MiShell` |
| `ayuda` | `[comando]` | Sin argumentos: lista todos los comandos. Con argumento: muestra ayuda detallada de ese comando. | `ayuda` / `ayuda calc` |
| `salir` | Ninguno | Termina la sesión de EAFITos. | `salir` |

---

## ✨ Features Avanzadas

### 1. 🎨 Interfaz Mejorada — Colores y Prompt Personalizable

EAFITos utiliza **secuencias de escape ANSI** (definidas en `include/colors.h`) para colorizar la salida:

- **Prompt**: aparece en **cyan** con la forma `EAFITos> ` por defecto.
- **Errores**: mensajes en **rojo** con prefijo `[ERROR]`.
- **Éxito**: mensajes en **verde** (ej. archivo creado/eliminado).
- **Advertencias**: en **amarillo** (ej. confirmaciones, consejos de uso).
- **`listar`**: directorios en **azul** con ícono 📁, archivos con 📄.

Para cambiar el prompt durante la sesión:

```
EAFITos> prompt MiShell
EAFITos> prompt [SO-Lab]
```

### 2. 📖 Sistema de Ayuda por Comando

El comando `ayuda` ahora soporta **ayuda detallada por comando**:

```
EAFITos> ayuda calc
```

Muestra:
- **Descripción** completa del comando.
- **Sintaxis** de uso.
- **Ejemplos** concretos.
- **Notas** sobre casos de borde.

La información de ayuda se almacena en `src/utils/help.c` mediante la estructura `CommandHelp`, facilitando su mantenimiento y extensión.

### 3. 🛑 Manejo de Señales — Ctrl+C y Ctrl+Z

EAFITos maneja correctamente las señales del sistema usando `sigaction()` (estándar POSIX):

| Señal | Atajo | Comportamiento |
|:---|:---|:---|
| `SIGINT` | Ctrl+C | Muestra un mensaje orientador y continúa la sesión. No termina el programa. |
| `SIGTSTP` | Ctrl+Z | Informa que la suspensión no está disponible y continúa la sesión. |

### 4. 🧪 Pruebas Automáticas — Unit Tests

El proyecto incluye una **suite de unit tests** en `tests/unit_tests.c` con un framework minimalista sin dependencias externas.

#### Ejecutar los tests:

```bash
make test
```

#### Suites de prueba incluidas:

| Suite | Tests | Descripción |
|:---|:---:|:---|
| `parsear_linea()` | 4 | Entrada vacía, tokenización, espacios múltiples, sin argumentos. |
| `cmd_calc` — Aritmética | 5 | Suma, resta, multiplicación, división, división por cero. |
| Validación de Strings | 4 | `strlen`, `strcmp`, `strncpy` con límites. |
| **Total** | **13** | |

El programa retorna **código 0** si todos pasan, **código 1** si algún test falla.

---

## 🛠️ Estructura del Proyecto

```
SistemaOperativo/
├── include/
│   ├── shell.h        # Definiciones del núcleo y variable global de prompt
│   ├── commands.h     # Prototipos de todos los comandos
│   ├── colors.h       # Macros de colores ANSI (NUEVO)
│   └── help.h         # Estructura CommandHelp para el sistema de ayuda (NUEVO)
├── src/
│   ├── core/
│   │   ├── main.c         # Punto de entrada
│   │   ├── shell_loop.c   # REPL, despacho de comandos, señales, prompt
│   │   └── parser.c       # Lectura y tokenización de entrada
│   ├── commands/
│   │   ├── basic_commands.c    # ayuda (por cmd), salir, tiempo, prompt
│   │   ├── file_commands.c     # listar, leer
│   │   ├── advanced_commands.c # crear, eliminar, buscar
│   │   └── system_commands.c   # limpiar, calc
│   └── utils/
│       ├── help.c         # Tabla de ayuda detallada por comando (NUEVO)
│       ├── helpers.c
│       ├── error_handler.c
│       └── memory_manager.c
├── tests/
│   ├── unit_tests.c       # Suite de unit tests (NUEVO)
│   ├── integration_tests.c
│   └── test_runner.sh
├── build/                 # Archivos compilados (generado por make)
├── docs/
├── Makefile
└── README.md
```

---

## ⚡ Cómo compilar y ejecutar

### Compilar

```bash
make
```

### Ejecutar

```bash
make run
# O directamente:
./build/sistema_os
```

### Limpiar y recompilar desde cero

```bash
make clean
make
```

### Ejecutar los tests automáticos

```bash
make test
```

---

## 📚 Documentación con Doxygen

El proyecto incluye un sistema de autodocumentación basado en **Doxygen**. Permite generar un sitio web técnico a partir de los comentarios del código fuente.

El archivo `Doxyfile` contiene la configuración para analizar el código C y generar salida HTML.

### Generar la documentación

```bash
doxygen Doxyfile
```

Esto crea la carpeta `docs/html`. Abre `docs/html/index.html` en tu navegador para navegar por la documentación interactiva.
