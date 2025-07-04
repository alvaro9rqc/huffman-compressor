#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>

char compress_encode_files(FILE *file, int argc, char *argv[]);

[[nodiscard("Handling error")]]
int decompress_file(FILE *file);

#endif
