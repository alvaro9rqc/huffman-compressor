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
    Node **root;
    printf("Comprimiendo: %s\n", argv[i]);
    unsigned char **huff_code = hc_endoce_file(argv[i], root);
    if (huff_code == NULL) {
      status = 1;
      break;
    }

    status = io_save_code(file, argv[i], huff_code, *root);
    // handle error
    if (status < 0) {
      fprintf(stderr, "Error saving code for file: %s\n", argv[i]);
      break;
    }

    // free huffman tree
    hc_free_tree(*root);
  }
  return status;
  //
}

/* Read file name
 * Read tree
 * Read the final bytes of the file
 * Read code
 */
int decompress_file(FILE *file) {
  // Read file name
  char filename[256];
  int n = io_read_filename(file, filename);
  if (n < 0) {
    fprintf(stderr, "Error reading filename.\n");
    return -1;
  }
  filename[n] = '\0'; // Null-terminate the string
  printf("Decompressing file: %s\n", filename);
  // Read huffman tree
  Node *root = io_read_huffman_tree(file);
  if (root == NULL) {
    fprintf(stderr, "Error reading huffman tree.\n");
    return -1;
  }
  // Read file size
  off_t file_size = io_read_file_size(file);
  if (file_size < 0) {
    fprintf(stderr, "Error reading file size.\n");
    hc_free_tree(root);
    return -1;
  }
  // Write decompressed file
  FILE *out_file = io_open_unique_file(filename, "wb");
  if (out_file == NULL) {
    fprintf(stderr, "Error opening output file: %s\n", filename);
    hc_free_tree(root);
    return -1;
  }
  int status = io_write_decompress_file(out_file, file, root, file_size);
  if (status < 0) {
    fprintf(stderr, "Error writing decompressed file: %s\n", filename);
    fclose(out_file);
    hc_free_tree(root);
    return -1;
  }
  fclose(out_file);
  return 0;
}
