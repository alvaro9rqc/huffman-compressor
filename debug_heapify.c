#include "include/priority_queue.h"
#include "include/huffman.h"
#include <stdio.h>
#include <stdlib.h>

char pq_is_full(PriorityQueue *pq) { return pq->size == pq->capacity; }
char pq_is_empty(PriorityQueue *pq) { return pq->size == 0; }

void pq_heapifyUp_debug(PriorityQueue *pq, int idx) {
  printf("HeapifyUp called with idx=%d\n", idx);
  int p;
  while (idx > 0 &&
         pq->arr[idx]->frequency < pq->arr[p = (idx - 1) >> 2]->frequency) {
    printf("  Swapping idx=%d ('%c' freq=%.1f) with parent=%d ('%c' freq=%.1f)\n", 
           idx, pq->arr[idx]->byte, pq->arr[idx]->frequency,
           p, pq->arr[p]->byte, pq->arr[p]->frequency);
    Node *t = pq->arr[idx];
    pq->arr[idx] = pq->arr[p];
    pq->arr[p] = t;
    idx = p;
  }
  printf("HeapifyUp finished at idx=%d\n", idx);
}

void pq_push_debug(PriorityQueue *pq, Node *n) {
  if (pq_is_full(pq)) {
    fprintf(stderr, "Error: PriorityQueue is full\n");
    exit(EXIT_FAILURE);
  }
  printf("Pushing '%c' freq=%.1f at position %d\n", n->byte, n->frequency, pq->size);
  pq->arr[pq->size] = n;
  pq_heapifyUp_debug(pq, pq->size);
  pq->size++;
}

Node* create_test_node(unsigned char byte, double frequency) {
    Node* node = malloc(sizeof(Node));
    node->byte = byte;
    node->frequency = frequency;
    node->is_leaf = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void print_heap(PriorityQueue* pq) {
    printf("Heap contents (size=%d):\n", pq->size);
    for (int i = 0; i < pq->size; i++) {
        printf("  [%d]: '%c' freq=%.1f\n", i, pq->arr[i]->byte, pq->arr[i]->frequency);
    }
    printf("\n");
}

int main() {
    PriorityQueue pq;
    pq.arr = (Node **)calloc(5, sizeof(Node *));
    pq.size = 0;
    pq.capacity = 5;

    printf("=== Testing problematic sequence ===\n");
    
    // Test the specific sequence that shows the problem
    Node* a = create_test_node('a', 5.0);
    Node* b = create_test_node('b', 2.0);
    Node* c = create_test_node('c', 8.0);
    Node* d = create_test_node('d', 1.0);
    
    pq_push_debug(&pq, a);
    print_heap(&pq);
    
    pq_push_debug(&pq, b);
    print_heap(&pq);
    
    pq_push_debug(&pq, c);
    print_heap(&pq);
    
    pq_push_debug(&pq, d);
    print_heap(&pq);

    return 0;
}
