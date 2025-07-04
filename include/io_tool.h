#ifndef IO_TOOL_H
#define IO_TOOL_H

#include "huffman.h"
#include "stdio.h"

double io_read_bytes(Node *pq, char *file);

[[nodiscard("Handling error")]]
int io_save_code(FILE *file, char *filename, unsigned char **huff_code,
                 Node *root);

int io_read_filename(FILE *file, char *filename);

#endif
