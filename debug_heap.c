#include "include/priority_queue.h"
#include "include/huffman.h"
#include <stdio.h>
#include <stdlib.h>

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
    printf("Heap contents (size=%d, capacity=%d):\n", pq->size, pq->capacity);
    for (int i = 0; i < pq->size; i++) {
        printf("  [%d]: '%c' freq=%.1f\n", i, pq->arr[i]->byte, pq->arr[i]->frequency);
    }
    printf("\n");
}

void debug_push(PriorityQueue* pq, Node* node) {
    printf("Pushing '%c' freq=%.1f\n", node->byte, node->frequency);
    pq_push(pq, node);
    print_heap(pq);
}

int main() {
    // Create test nodes
    Node nodes[5];
    nodes[0] = *create_test_node('a', 5.0);
    nodes[1] = *create_test_node('b', 2.0);
    nodes[2] = *create_test_node('c', 8.0);
    nodes[3] = *create_test_node('d', 1.0);
    nodes[4] = *create_test_node('e', 3.0);

    PriorityQueue pq;
    pq.arr = (Node **)calloc(5, sizeof(Node *));
    pq.size = 0;
    pq.capacity = 5;

    printf("Building heap step by step:\n");
    for (int i = 0; i < 5; ++i) {
        Node *copy = malloc(sizeof(Node));
        *copy = nodes[i];
        debug_push(&pq, copy);
    }

    printf("Final heap:\n");
    print_heap(&pq);

    printf("Top element: '%c' freq=%.1f\n", pq_top(&pq)->byte, pq_top(&pq)->frequency);

    pq_pop(&pq);
    printf("After popping first element:\n");
    print_heap(&pq);

    printf("New top element: '%c' freq=%.1f\n", pq_top(&pq)->byte, pq_top(&pq)->frequency);

    pq_erase(&pq);
    return 0;
}
