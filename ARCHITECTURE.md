# Arquitectura del Compresor Huffman

Este documento describe la arquitectura del software para el compresor Huffman, un programa en C diseñado para comprimir y descomprimir archivos utilizando el algoritmo de Huffman.

## Descripción General

El proyecto sigue una arquitectura modular, donde la funcionalidad se divide en varios componentes, cada uno con una responsabilidad específica. Esta separación de preocupaciones facilita el mantenimiento, la prueba y la reutilización del código. Los módulos principales son:

- **`main`**: El punto de entrada de la aplicación.
- **`compress`**: Orquesta el proceso de compresión y descompresión.
- **`huffman`**: Implementa la lógica central del algoritmo de Huffman.
- **`priority_queue`**: Proporciona una estructura de datos de cola de prioridad.
- **`io_tool`**: Maneja todas las operaciones de entrada/salida de archivos.

A continuación, se detalla cada módulo.

### `main`

El módulo `main` es responsable de:

- Analizar los argumentos de la línea de comandos para determinar si se debe comprimir o descomprimir.
- Abrir y cerrar los archivos de entrada y salida.
- Invocar al módulo `compress` para realizar la operación solicitada.

### `compress`

Este módulo actúa como un intermediario entre la interfaz de usuario (`main`) y la lógica de compresión/descompresión (`huffman` y `io_tool`). Sus responsabilidades son:

- Para la **compresión**:
    - Iterar sobre los archivos de entrada.
    - Invocar a `huffman` para generar el árbol y los códigos de Huffman para cada archivo.
    - Invocar a `io_tool` para escribir la información de compresión (nombre del archivo, árbol de Huffman, datos comprimidos) en el archivo de salida.
- Para la **descompresión**:
    - Leer los datos comprimidos del archivo de entrada.
    - Invocar a `io_tool` para leer el nombre del archivo, el árbol de Huffman y los datos.
    - Invocar a `io_tool` para escribir el archivo descomprimido.

### `huffman`

Este es el núcleo del compresor. Contiene la implementación del algoritmo de Huffman:

- **Análisis de frecuencia**: Lee un archivo de entrada para contar la frecuencia de cada byte.
- **Construcción del árbol**: Utiliza una cola de prioridad (`priority_queue`) para construir el árbol de Huffman a partir de los nodos hoja que representan cada byte.
- **Generación de códigos**: Recorre el árbol de Huffman para generar los códigos de longitud variable para cada byte.

### `priority_queue`

Este módulo implementa una cola de prioridad mínima utilizando un heap binario. Es una estructura de datos genérica utilizada por el módulo `huffman` para construir el árbol de manera eficiente.

### `io_tool`

Este módulo encapsula todas las operaciones de entrada y salida, aislando al resto de la aplicación de los detalles del manejo de archivos. Sus funciones incluyen:

- Leer bytes de los archivos de entrada.
- Escribir el árbol de Huffman y los datos comprimidos en un formato binario específico.
- Leer la estructura del árbol de Huffman y los datos del archivo comprimido.
- Manejar la creación de archivos y directorios.

## Comparación con Arquitecturas Conocidas

La arquitectura de este proyecto se puede comparar con varios patrones arquitectónicos establecidos.

### Arquitectura Modular

La característica más destacada es su **modularidad**. El código está organizado en módulos cohesivos (`.c` y `.h` pares) con responsabilidades bien definidas. Esto es similar a los principios de diseño de software que promueven el bajo acoplamiento y la alta cohesión.

## Diagrama de Dependencias

```
  main
   |
   v
compress
   |
   +-----> huffman
   |         |
   |         +-----> priority_queue
   |         |
   |         v
   +-----> io_tool

test_runner
    ↓
test_framework ← test_priority_queue
    ↓           ← test_huffman  
    ↓           ← test_io_tool
    ↓           ← test_compress
    ↓           ← test_integration
    ↓
   core ← todos los módulos de prueba
    ↓
[módulos principales del proyecto]
```

Como se puede ver, `main` depende de `compress`. `compress` depende de `huffman` y `io_tool`. `huffman` a su vez depende de `priority_queue` y `io_tool`. Esta estructura es mayormente jerárquica, con `io_tool` actuando como un servicio de bajo nivel utilizado por múltiples componentes.

## Arquitectura de Pruebas

El proyecto incluye una arquitectura de pruebas completa que valida todas las funcionalidades del sistema. La arquitectura de pruebas sigue los siguientes principios:

### Estructura de Pruebas

```
tests/
├── test_framework.h        # Marco de pruebas personalizado
├── test_framework.c        # Implementación del marco de pruebas
├── test_priority_queue.c   # Pruebas del módulo de cola de prioridad
├── test_huffman.c          # Pruebas del algoritmo de Huffman
├── test_io_tool.c          # Pruebas de herramientas de E/S
├── test_compress.c         # Pruebas de compresión/descompresión
├── test_integration.c      # Pruebas de integración end-to-end
├── test_runner.c           # Ejecutor principal de pruebas
└── README.md               # Documentación de pruebas
```

### Niveles de Pruebas

La arquitectura de pruebas implementa múltiples niveles de validación:

#### 1. Pruebas Unitarias
- **Objetivo**: Validar funciones individuales y componentes aislados
- **Cobertura**: Cada función pública tiene al menos una prueba
- **Módulos**: `test_priority_queue.c`, `test_huffman.c`, `test_io_tool.c`

#### 2. Pruebas de Integración  
- **Objetivo**: Validar la interacción entre módulos
- **Cobertura**: Flujos de trabajo completos (compresión → descompresión)
- **Módulos**: `test_compress.c`, `test_integration.c`

#### 3. Pruebas de Sistema
- **Objetivo**: Validar el comportamiento end-to-end
- **Cobertura**: Escenarios reales de uso con múltiples archivos
- **Módulos**: `test_integration.c`

### Marco de Pruebas Personalizado

El proyecto utiliza un marco de pruebas ligero y personalizado que proporciona:

- **Aserciones**: `ASSERT_TRUE`, `ASSERT_EQ`, `ASSERT_NOT_NULL`, etc.
- **Reportes**: Salida colorizada con estadísticas detalladas
- **Gestión**: Contadores automáticos de pruebas exitosas/fallidas
- **Portabilidad**: Solo depende de la biblioteca estándar de C

### Estrategias de Prueba

#### Casos de Prueba Positivos
- Operaciones exitosas con datos válidos
- Múltiples archivos y tipos de contenido
- Roundtrip completo (compresión → descompresión → verificación)

#### Casos de Prueba Negativos
- Archivos inexistentes o corruptos
- Datos de entrada inválidos
- Condiciones de error del sistema

#### Casos Extremos
- Archivos vacíos
- Archivos con un solo carácter
- Archivos con todos los caracteres ASCII
- Patrones de datos repetitivos

#### Validación de Memoria
- Detección de memory leaks
- Liberación correcta de recursos
- Múltiples ciclos de operación

### Integración con el Sistema de Construcción

Las pruebas están completamente integradas con CMake:

```cmake
# Habilitar pruebas
enable_testing()

# Biblioteca del marco de pruebas
add_library(test_framework STATIC ${TEST_DIR}/test_framework.c)

# Ejecutables de prueba individuales
add_executable(test_huffman ${TEST_DIR}/test_huffman.c)
target_link_libraries(test_huffman PRIVATE core test_framework)

# Integración con CTest
add_test(NAME HuffmanTests COMMAND test_huffman)

# Target personalizado para ejecutar todas las pruebas
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_priority_queue test_huffman test_io_tool test_compress test_integration
)
```

### Dependencias de Pruebas

```
test_runner
    ↓
test_framework ← test_priority_queue
    ↓           ← test_huffman  
    ↓           ← test_io_tool
    ↓           ← test_compress
    ↓           ← test_integration
    ↓
   core ← todos los módulos de prueba
    ↓
[módulos principales del proyecto]
```

### Patrones de Diseño en Pruebas

#### Patrón Arrange-Act-Assert
Cada prueba sigue la estructura:
1. **Arrange**: Configurar datos de prueba
2. **Act**: Ejecutar la función bajo prueba  
3. **Assert**: Verificar el resultado esperado
4. **Cleanup**: Limpiar recursos

#### Patrón Test Fixture
- Funciones auxiliares para crear/limpiar datos de prueba
- Archivos temporales con nombres predecibles
- Gestión automática de recursos

#### Patrón Builder para Datos de Prueba
- Funciones helper para crear estructuras de datos complejas
- Datos de prueba reutilizables entre diferentes casos

### Métricas y Reportes

El sistema de pruebas proporciona:

- **Estadísticas por módulo**: Pruebas ejecutadas/exitosas/fallidas
- **Salida colorizada**: Verde para éxito, rojo para fallos
- **Detalles de fallos**: Valores esperados vs. obtenidos
- **Resumen ejecutivo**: Tasa de éxito general

### Consideraciones de Rendimiento

- **Ejecución paralela**: Pruebas independientes pueden ejecutarse en paralelo
- **Cleanup automático**: Archivos temporales se eliminan automáticamente
- **Optimización de E/S**: Minimización de operaciones de archivo en pruebas

Esta arquitectura de pruebas asegura la calidad y confiabilidad del compresor Huffman, proporcionando confianza en todos los niveles del sistema.
