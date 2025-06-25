#include "compress.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
  //
  if (argc < 3) {
    fprintf(stderr,
            "to comprees files: compress file1 file2 ... compresFile.cprs\n");
    fprintf(stderr, "to decompress file: compress -d file1.cprs\n");
    return 0;
  }
  if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-decode") == 0) {
    printf("Descomprimir\n");
  } else {
    // second argument is compressed file name
    printf("Code: \n");
    FILE *file = fopen(argv[argc - 1], "w");
    compress_encode_files(file, argc, argv);
    fclose(file);
  }
}
