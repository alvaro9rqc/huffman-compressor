cmake_minimum_required(VERSION 3.10)
project(Compresor LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

# Busca todos los .c automáticamente
file(GLOB LIB_SOURCES "${SRC_DIR}/*.c")

# Crear la biblioteca con todos los .c encontrados
add_library(core STATIC ${LIB_SOURCES})

target_include_directories(core PUBLIC ${INCLUDE_DIR})

# Detecta main.c automáticamente y exclúyelo de la biblioteca
list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.c$")
add_executable(compresor "${SRC_DIR}/main.c")
target_link_libraries(compresor PRIVATE core)

# Enable testing
enable_testing()

# Test framework library
add_library(test_framework STATIC ${TEST_DIR}/test_framework.c)
target_include_directories(test_framework PUBLIC ${TEST_DIR} ${INCLUDE_DIR})

# Individual test executables
add_executable(test_priority_queue ${TEST_DIR}/test_priority_queue.c)
target_link_libraries(test_priority_queue PRIVATE core test_framework)
target_include_directories(test_priority_queue PRIVATE ${INCLUDE_DIR} ${TEST_DIR})

add_executable(test_huffman ${TEST_DIR}/test_huffman.c)
target_link_libraries(test_huffman PRIVATE core test_framework)
target_include_directories(test_huffman PRIVATE ${INCLUDE_DIR} ${TEST_DIR})

add_executable(test_io_tool ${TEST_DIR}/test_io_tool.c)
target_link_libraries(test_io_tool PRIVATE core test_framework)
target_include_directories(test_io_tool PRIVATE ${INCLUDE_DIR} ${TEST_DIR})

add_executable(test_compress ${TEST_DIR}/test_compress.c)
target_link_libraries(test_compress PRIVATE core test_framework)
target_include_directories(test_compress PRIVATE ${INCLUDE_DIR} ${TEST_DIR})

add_executable(test_integration ${TEST_DIR}/test_integration.c)
target_link_libraries(test_integration PRIVATE core test_framework)
target_include_directories(test_integration PRIVATE ${INCLUDE_DIR} ${TEST_DIR})

add_executable(test_runner ${TEST_DIR}/test_runner.c)
target_link_libraries(test_runner PRIVATE test_framework)
target_include_directories(test_runner PRIVATE ${TEST_DIR})

# Add tests to CTest
add_test(NAME PriorityQueueTests COMMAND test_priority_queue)
add_test(NAME HuffmanTests COMMAND test_huffman)
add_test(NAME IOToolTests COMMAND test_io_tool)
add_test(NAME CompressTests COMMAND test_compress)
add_test(NAME IntegrationTests COMMAND test_integration)

# Custom target to run all tests
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_priority_queue test_huffman test_io_tool test_compress test_integration
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

