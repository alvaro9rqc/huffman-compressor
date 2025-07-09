# Funcionalidades y Supuestos del Proyecto

Este documento describe las funcionalidades principales del proyecto compresor Huffman y los supuestos subyacentes realizados durante su desarrollo.

## Matriz de Funcionalidades

El proyecto es un compresor y descompresor de archivos basado en el algoritmo de codificación de Huffman. Ofrece las siguientes características:

| ID | Nombre | Descripción | Prioridad | Dependencias |
| :-- | :--- | :--- | :--- | :--- |
| F01 | **Compresión** | Codifica uno o más archivos en un único archivo comprimido. Para cada archivo, calcula las frecuencias de bytes, construye un árbol de Huffman, genera códigos y escribe los datos comprimidos en el archivo. | Alta | F03, F04 |
| F02 | **Descompresión** | Descodifica archivos de un archivo comprimido. Lee el archivo, reconstruye el árbol de Huffman y lo utiliza para decodificar los datos y recrear los archivos originales. | Alta | F03, F04 |
| F03 | **Estructuras de Datos** | Implementa una cola de prioridad (min-heap) para construir eficientemente el árbol de Huffman. Utiliza un árbol binario para representar el código de Huffman, donde cada nodo hoja es un byte y la ruta desde la raíz representa su código. | Alta | N/A |
| F04 | **E/S de Archivos** | Proporciona un conjunto de funciones para leer y escribir en archivos, incluido el manejo del formato específico del archivo comprimido. Incluye una utilidad para crear un nombre de archivo único para evitar sobrescrituras accidentales. | Alta | N/A |
| F05 | **Suite de Pruebas** | Sistema completo de pruebas automatizadas que valida todas las funcionalidades del compresor. Incluye pruebas unitarias, de integración y manejo de errores. | Media | F01, F02, F03, F04 |

## Matriz de Supuestos

El proyecto se basa en los siguientes supuestos:

| ID | Supuesto | Impacto si es Falso | Validación |
| :-- | :--- | :--- | :--- |
| A01 | **Formato de Archivo** | El archivo comprimido tiene un formato específico y personalizado. Cada archivo comprimido se almacena con sus metadatos (nombre, tamaño y árbol de Huffman) en un orden predefinido. | La descompresión fallaría o produciría datos corruptos si el formato no se respeta. | Pruebas unitarias que verifiquen la correcta lectura y escritura del formato de archivo (`test_io_tool.c`, `test_integration.c`). |
| A02 | **Árbol de Huffman** | El árbol de Huffman se serializa y almacena en el archivo comprimido de una manera que permite una reconstrucción perfecta durante la descompresión. El hijo izquierdo siempre es 0. | La reconstrucción del árbol sería incorrecta, lo que llevaría a una decodificación errónea. | Pruebas de compresión y descompresión de archivos de muestra para garantizar la integridad de los datos (`test_huffman.c`, `test_compress.c`). |
| A03 | **Manejo de Errores** | Se espera que las funciones marcadas con `[[nodiscard]]` tengan sus valores de retorno verificados por el llamador para manejar posibles errores. | Los errores podrían pasar desapercibidos, lo que podría llevar a un comportamiento inesperado o a fallos en el programa. | Revisiones de código y análisis estático para asegurar que los valores de retorno se manejan adecuadamente. Pruebas específicas de manejo de errores en todos los módulos de prueba. |
| A04 | **Dependencias** | El proyecto es autocontenido y solo depende de la biblioteca estándar de C. | El proyecto no compilará o no se ejecutará en un entorno que no tenga las dependencias necesarias. | El proyecto se compila y ejecuta correctamente utilizando solo un compilador de C estándar. Verificado en la suite de pruebas de integración. |
| A05 | **Sistema de Construcción** | El proyecto utiliza CMake para la compilación. Se proporciona un archivo `CMakeLists.txt` para gestionar el proceso de construcción. | Los desarrolladores no podrían compilar el proyecto utilizando el método previsto. | El proyecto se compila y construye correctamente utilizando los comandos de CMake. Incluye targets para todas las pruebas. |
| A06 | **Calidad del Código** | El código mantiene estándares altos de calidad, incluyendo manejo correcto de memoria, validación de entrada y robustez ante errores. | Fallos en tiempo de ejecución, memory leaks, comportamiento impredecible. | Suite completa de pruebas automatizadas que cubren funcionalidad, manejo de errores, casos extremos y gestión de memoria (`tests/` directorio completo). |

## Métricas de Calidad

| Métrica | Objetivo | Herramienta de Validación |
| :--- | :--- | :--- |
| **Cobertura de Código** | >90% de líneas cubiertas | Suite de pruebas + gcov |
| **Pruebas Exitosas** | 100% de pruebas pasan | CTest + test_runner |
| **Memory Leaks** | 0 leaks detectados | Valgrind + pruebas de integración |
| **Manejo de Errores** | Todos los valores `[[nodiscard]]` verificados | Análisis estático + pruebas de error |
| **Portabilidad** | Compila en múltiples plataformas | CI/CD + CMake |