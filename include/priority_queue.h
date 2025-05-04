#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "huffman.h"

typedef struct PriorityQueue {
  Node** arr;
  int size;
  int capacity;
} PriorityQueue;

char pq_is_full(PriorityQueue* pq);

char pq_is_empty(PriorityQueue* pq);

void pq_new(PriorityQueue* pq, Node* arr, int size);

void pq_erase(PriorityQueue* pq);

void pq_heapifyUp(PriorityQueue* pq,int idx);

void pq_heapifyDown(PriorityQueue* pq,int idx);

void pq_push(PriorityQueue* pq, Node* n);

Node* pq_top(PriorityQueue* pq);

void pq_pop(PriorityQueue* pq);


#endif
