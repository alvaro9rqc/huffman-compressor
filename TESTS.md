
## Detalles de la Funcionalidad de Pruebas (F05)

La suite de pruebas comprende los siguientes módulos:

### Módulos de Pruebas

| Módulo | Archivo | Descripción | Cobertura |
| :--- | :--- | :--- | :--- |
| **Framework de Pruebas** | `test_framework.h/c` | Sistema básico de aserciones y reportes con salida colorizada | Infraestructura base |
| **Pruebas de Cola de Prioridad** | `test_priority_queue.c` | Valida el min-heap: operaciones básicas, propiedad de heap, manejo de memoria | 15+ casos de prueba |
| **Pruebas de Huffman** | `test_huffman.c` | Valida construcción del árbol, generación de códigos, casos extremos | 20+ casos de prueba |
| **Pruebas de E/S** | `test_io_tool.c` | Valida lectura/escritura de archivos, serialización del árbol, manejo de errores | 18+ casos de prueba |
| **Pruebas de Compresión** | `test_compress.c` | Valida compresión/descompresión completa, múltiples archivos, roundtrip | 25+ casos de prueba |
| **Pruebas de Integración** | `test_integration.c` | Pruebas end-to-end, varios tipos de archivos, detección de memory leaks | 12+ casos de prueba |

### Casos de Prueba Incluidos

1. **Casos Positivos**
   - Compresión y descompresión exitosa
   - Múltiples archivos
   - Varios tipos de contenido
   - Archivos con un solo carácter
   - Roundtrip completo (compresión → descompresión → verificación)

2. **Casos Extremos**
   - Archivos vacíos
   - Archivos muy pequeños
   - Archivos con todos los caracteres ASCII
   - Patrones repetitivos

3. **Casos de Error**
   - Archivos inexistentes
   - Datos comprimidos corruptos
   - Falta de permisos
   - Memoria insuficiente

4. **Validación de Memoria**
   - Detección de memory leaks
   - Liberación correcta de recursos
   - Múltiples ciclos de operación

### Ejecución de Pruebas

```bash
# Compilar todas las pruebas
make

# Ejecutar pruebas individuales
./test_priority_queue
./test_huffman
./test_io_tool
./test_compress
./test_integration

# Ejecutar todas las pruebas
make run_all_tests
ctest --verbose

# Ejecutar con runner personalizado
./test_runner all
```