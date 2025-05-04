#include <stdio.h>

#include "compress.h"
#include "huffman.h"

char compress_encode_files(FILE *file, int argc, char **argv) {
  // por cada archivo
  //  crear código de huffman
  //  escribir nombre
  //  escribir posible tamaño
  //  guardar código
  //  escribir tamaño anterior
  char error = 0;
  for (int i = 1; i < argc-1; ++i) {
    printf("Comprimiendo: %s\n", argv[i]);
    unsigned char** huff_code = hc_endoce_file(argv[i]);
    if (huff_code == NULL) {
      error = 1; 
      break;
    }

    //io_save_code(file, fi, huff_code);
  }
  return error;
  // 

}
