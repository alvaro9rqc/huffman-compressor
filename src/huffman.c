#include "huffman.h"
#include "io_tool.h"
#include <stdlib.h>

#define ALPHABET_SIZE 0x100 

// nx2 matrix with the char, code and length
// n is amount of chars
// matrix[ni][0] is the generated code
// matrix[mi][1] is the length of the code
int** hc_endoce_file(char* file_name) {
  // Create nodes
  Node pq[ALPHABET_SIZE];
  // Initialization of each node
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    pq[i].byte=i;
    pq[i].frequency=0;
    pq[i].is_leaf=1;
    pq[i].left = pq[i].right = NULL;
  }
  double tbytes = io_read_bytes(pq, file_name);
  // erase not used bytes
  // priority queue
  // huffman tree
  // huffman code
}

int select_nodes(Node* pq) {
  int l = 0, r = ALPHABET_SIZE-1;
  while(l < r) {
    if (pq[r].frequency==0 && r >= 0) --r;
    if (pq[l].frequency && l < ALPHABET_SIZE) ++l;
    if (l < r) {
      Node t = pq[r];
      pq[r] = pq[l];
      pq[l]=t;
    }
  }
  while(pq[l].frequency)++l;
  return l;
}
