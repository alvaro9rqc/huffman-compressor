#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct Node {
  unsigned char byte;
  double frequency;
  char is_leaf;
  struct Node *left, *right;
} Node;

int** hc_endoce_file(char* file_name);

#endif
