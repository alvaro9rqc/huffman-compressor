#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct Node {
  unsigned char byte;
  double frequency;
  char is_leaf;
  struct Node *left, *right;
} Node;

unsigned char **hc_endoce_file(char *file_name, Node *root);

int hc_free_tree(Node *root);

int hc_free_code(unsigned char **code);

#endif
