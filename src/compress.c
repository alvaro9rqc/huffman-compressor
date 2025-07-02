#include <stdio.h>

#include "compress.h"
#include "huffman.h"
#include "io_tool.h"

char compress_encode_files(FILE *file, int argc, char **argv) {
  // por cada archivo
  //  crear código de huffman
  //  escribir nombre
  //  escribir posible tamaño
  //  guardar código
  //  escribir tamaño anterior
  int status = 0;
  for (int i = 1; i < argc - 1; ++i) {
    Node *root;
    printf("Comprimiendo: %s\n", argv[i]);
    unsigned char **huff_code = hc_endoce_file(argv[i], root);
    if (huff_code == NULL) {
      status = 1;
      break;
    }

    status = io_save_code(file, argv[i], huff_code, root);
    // handle error
    if (status < 0) {
      fprintf(stderr, "Error saving code for file: %s\n", argv[i]);
      break;
    }

    // free huffman tree
    hc_free_tree(root);
  }
  return status;
  //
}
