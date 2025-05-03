cmake_minimum_required(VERSION 3.10)
project(Compresor LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Busca todos los .c automáticamente
file(GLOB LIB_SOURCES "${SRC_DIR}/*.c")

# Crear la biblioteca con todos los .c encontrados
add_library(core STATIC ${LIB_SOURCES})

target_include_directories(core PUBLIC ${INCLUDE_DIR})

# Detecta main.c automáticamente y exclúyelo de la biblioteca
list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.c$")
add_executable(compresor "${SRC_DIR}/main.c")
target_link_libraries(compresor PRIVATE core)

