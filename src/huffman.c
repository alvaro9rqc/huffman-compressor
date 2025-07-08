#include "huffman.h"
#include "io_tool.h"
#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

#define ALPHABET_SIZE 0x100
#define C_LENGHT 0

// Select just nodes with frequency greater than 0
static int select_nodes(Node *pq, double total) {
  int l = 0, r = ALPHABET_SIZE - 1;
  
  // Separate nodes with frequency > 0 from nodes with frequency = 0
  while (l < r) {
    // Find the next zero frequency from the right
    while (r >= l && pq[r].frequency == 0)
      r--;
    // Find the next non-zero frequency from the left  
    while (l <= r && pq[l].frequency > 0)
      l++;
    
    // Swap if needed
    if (l < r) {
      Node t = pq[r];
      pq[r] = pq[l];
      pq[l] = t;
    }
  }
  
  // Count how many nodes have non-zero frequency
  int count = 0;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (pq[i].frequency > 0) {
      count++;
    }
  }
  
  // Normalize frequencies
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (pq[i].frequency > 0) {
      pq[i].frequency /= total;
    }
  }
  
  return count; // return number of nodes with non-zero frequency
}

// Build Huffman tree
static Node *hc_build_tree(PriorityQueue *pq) {
  int nodes = pq->size;
  
  // Handle empty queue
  if (nodes == 0) {
    return NULL;
  }
  
  // Handle single node (single character file)
  if (nodes == 1) {
    return pq_top(pq);
  }
  
  // Build tree for multiple nodes
  for (int i = 0; i < nodes - 1; ++i) {
    Node *n = calloc(1, sizeof(Node));
    n->is_leaf = 0;
    n->left = pq_top(pq);
    pq_pop(pq);
    n->right = pq_top(pq);
    pq_pop(pq);
    n->frequency = n->left->frequency + n->right->frequency;
    pq_push(pq, n);
  }
  return pq_top(pq);
}

static void hc_inorden(Node *node, unsigned char **code, int depth,
                       unsigned char *prefix) {
  if (node->is_leaf) {
    size_t s = depth / 8 + 1; // this extra one is to save the depth
    s += (depth % 8) ? 1 : 0;
    code[node->byte] = (unsigned char *)calloc(s, sizeof(unsigned char));
    code[node->byte][C_LENGHT] = (unsigned char)depth;
    for (int i = 0; i < depth; ++i) {
      int idx = 1 + i / 8; // this extra one is to save the depth
      code[node->byte][idx] |= (prefix[idx - 1] & (1 << (7 - i % 8)));
    }

  } else {
    // visit left children (0)
    hc_inorden(node->left, code, depth + 1, prefix);
    // visit right children (1)
    int idx = depth / 8;
    prefix[idx] |= (1 << (7 - depth % 8)); // turn on bit
    hc_inorden(node->right, code, depth + 1, prefix);
    // turn off bit
    prefix[idx] = ~prefix[idx];
    prefix[idx] |= (1 << (7 - depth % 8));
    prefix[idx] = ~prefix[idx];
  }
}

static unsigned char **hc_build_code(Node *root) {
  if (root == NULL) {
    return NULL;
  }
  
  unsigned char **code =
      (unsigned char **)calloc(ALPHABET_SIZE, sizeof(char *));
  // remember prefix code
  unsigned char *p =
      (unsigned char *)calloc(ALPHABET_SIZE, sizeof(unsigned char));
  hc_inorden(root, code, 0, p);
  free(p);
  return code;
}

// similar a adjacent matrix
// dynamic array of unsigned char arrays
// each element contains size code and code
unsigned char **hc_endoce_file(char *file_name, Node **root) {
  // Create nodes
  Node *arr = (Node *)malloc(ALPHABET_SIZE * sizeof(Node));
  // Initialization of each node
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    arr[i].byte = i;
    arr[i].frequency = 0;
    arr[i].is_leaf = 1;
    arr[i].left = arr[i].right = NULL;
  }
  double tbytes = io_read_bytes(arr, file_name);
  // erase not used bytes
  double size = select_nodes(arr, tbytes);

  // priority queue
  PriorityQueue pq;
  pq_new(&pq, arr, size);
  // TODO: error here
  free(arr);
  // huffman tree
  *root = hc_build_tree(&pq);
  pq_erase(&pq);
  // huffman code
  unsigned char **code = hc_build_code(*root);
  return code;
}

int hc_free_tree(Node *root) {
  if (root == NULL)
    return 0;
  hc_free_tree(root->left);
  hc_free_tree(root->right);
  free(root);
  return 0;
}

int hc_free_code(unsigned char **code) {
  if (code == NULL)
    return 0;
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    if (code[i] != NULL) {
      free(code[i]);
    }
  }
  free(code);
  return 0;
}
