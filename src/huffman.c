#include "huffman.h"
#include "io_tool.h"
#include "priority_queue.h"
#include <stdlib.h>

#define ALPHABET_SIZE 0x100 

// Select just nodes with frequency greater than 0
static int select_nodes(Node* pq) {
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
  return l; //return new size
}

// Build Huffman tree
Node* hc_build_tree(PriorityQueue* pq) {

}

// nx2 matrix with the char, code and length
// n is amount of chars
// matrix[ni][0] is the generated code
// matrix[mi][1] is the length of the code
int** hc_endoce_file(char* file_name) {
  // Create nodes
  Node arr[ALPHABET_SIZE];
  // Initialization of each node
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    arr[i].byte=i;
    arr[i].frequency=0;
    arr[i].is_leaf=1;
    arr[i].left = arr[i].right = NULL;
  }
  double tbytes = io_read_bytes(arr, file_name);
  // erase not used bytes
  double size = select_nodes(arr);

  // priority queue
  PriorityQueue pq;
  pq_new(&pq, arr, size);
  // huffman tree
  Node* root = hc_build_tree(&pq);
  // huffman code
  pq_erase(&pq);
}
