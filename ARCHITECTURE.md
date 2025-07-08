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
```

Como se puede ver, `main` depende de `compress`. `compress` depende de `huffman` y `io_tool`. `huffman` a su vez depende de `priority_queue` y `io_tool`. Esta estructura es mayormente jerárquica, con `io_tool` actuando como un servicio de bajo nivel utilizado por múltiples componentes.
