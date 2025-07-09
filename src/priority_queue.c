#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

char pq_is_full(PriorityQueue *pq) { return pq->size == pq->capacity; }

char pq_is_empty(PriorityQueue *pq) { return pq->size == 0; }

void pq_new(PriorityQueue *pq, Node *arr, int size) {
  pq->arr = (Node **)calloc(size, sizeof(Node *));
  pq->size = 0;
  pq->capacity = size;
  if (arr != NULL) {
    for (int i = 0; i < size; ++i) {
      // Copy
      Node *copy = malloc(sizeof(Node));
      if (copy == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
      }
      *copy = arr[i];
      pq_push(pq, copy);
    }
  }
}

void pq_erase(PriorityQueue *pq) { free(pq->arr); }

void pq_heapifyUp(PriorityQueue *pq, int idx) {
  int p;
  while (idx > 0 &&
         pq->arr[idx]->frequency < pq->arr[p = (idx - 1) / 2]->frequency) {
    Node *t = pq->arr[idx];
    pq->arr[idx] = pq->arr[p];
    pq->arr[p] = t;
    idx = p;
  }
}

void pq_heapifyDown(PriorityQueue *pq, int idx) {
  while (2 * idx + 1 < pq->size) {
    int l = 2 * idx + 1;
    int r = l + 1;
    int lowest = idx;
    if (l < pq->size && pq->arr[l]->frequency < pq->arr[lowest]->frequency)
      lowest = l;
    if (r < pq->size && pq->arr[r]->frequency < pq->arr[lowest]->frequency)
      lowest = r;
    if (lowest == idx)
      break;
    Node *t = pq->arr[lowest];
    pq->arr[lowest] = pq->arr[idx];
    pq->arr[idx] = t;
    idx = lowest;
  }
}

void pq_push(PriorityQueue *pq, Node *n) {
  if (pq_is_full(pq)) {
    fprintf(stderr, "Error: PriorityQueue is full\n");
    exit(EXIT_FAILURE);
  }
  pq->arr[pq->size] = n;
  pq_heapifyUp(pq, pq->size);
  pq->size++;
}

Node *pq_top(PriorityQueue *pq) {
  if (pq_is_empty(pq)) {
    fprintf(stderr,
            "Error, se intentó extraer elemento de cola de prioridad vacía\n");
    exit(EXIT_FAILURE);
  }
  return pq->arr[0];
}

void pq_pop(PriorityQueue *pq) {
  if (pq_is_empty(pq)) {
    fprintf(stderr,
            "Error, se intentó extraer elemento de cola de prioridad vacía\n");
    exit(EXIT_FAILURE);
  }
  pq->size--;
  if (pq->size > 0) {
    pq->arr[0] = pq->arr[pq->size];
    pq_heapifyDown(pq, 0);
  }
}
