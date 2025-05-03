#include "io_tool.h"
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 4096 //can be parameterized

double io_read_bytes(Node *pq, char *file_name){
  FILE* file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "No se pudo leer el archivo: %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  unsigned char buffer[BUFFER_SIZE];
  double total_bytes = 0;
  size_t bytes_read;
  while((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    total_bytes += bytes_read;
    for (size_t i = 0; i < bytes_read; ++i) {
      ++pq[buffer[i]].frequency;
    }
  }
  return total_bytes;
}
