#include "test_framework.h"
#include "../include/priority_queue.h"
#include "../include/huffman.h"
#include <stdlib.h>

// Helper function to create a test node
Node* create_test_node(unsigned char byte, double frequency) {
    Node* node = malloc(sizeof(Node));
    node->byte = byte;
    node->frequency = frequency;
    node->is_leaf = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void test_pq_basic_operations() {
    // Create test nodes
    Node nodes[5];
    nodes[0] = *create_test_node('a', 5.0);
    nodes[1] = *create_test_node('b', 2.0);
    nodes[2] = *create_test_node('c', 8.0);
    nodes[3] = *create_test_node('d', 1.0);
    nodes[4] = *create_test_node('e', 3.0);

    PriorityQueue pq;
    pq_new(&pq, nodes, 5);

    ASSERT_FALSE(pq_is_empty(&pq), "Priority queue should not be empty after initialization");
    ASSERT_TRUE(pq_is_full(&pq), "Priority queue should be full after initialization with capacity elements");
    ASSERT_EQ(5, pq.size, "Priority queue size should be 5");

    // Test that the minimum element is at the top
    Node* top = pq_top(&pq);
    ASSERT_NOT_NULL(top, "Top element should not be NULL");
    ASSERT_EQ('d', top->byte, "Top element should be 'd' (lowest frequency)");

    // Test pop operation
    pq_pop(&pq);
    ASSERT_EQ(4, pq.size, "Size should be 4 after one pop");
    
    top = pq_top(&pq);
    ASSERT_EQ(2.0, top->frequency, "Next top element should have frequency 2.0");

    // Clean up
    pq_erase(&pq);
}

void test_pq_push_operation() {
    Node initial_nodes[2];
    initial_nodes[0] = *create_test_node('a', 5.0);
    initial_nodes[1] = *create_test_node('b', 2.0);

    PriorityQueue pq;
    // Create with capacity 3 to allow for the additional push
    pq.arr = (Node **)calloc(3, sizeof(Node *));
    pq.size = 0;
    pq.capacity = 3;
    
    // Manually push the initial nodes
    for (int i = 0; i < 2; i++) {
        Node *copy = malloc(sizeof(Node));
        *copy = initial_nodes[i];
        pq_push(&pq, copy);
    }

    // Push a new node with minimum frequency
    Node* new_node = create_test_node('c', 1.0);
    pq_push(&pq, new_node);

    ASSERT_EQ(3, pq.size, "Size should be 3 after push");
    
    Node* top = pq_top(&pq);
    ASSERT_EQ('c', top->byte, "New minimum element should be at top");

    // Clean up
    pq_erase(&pq);
}

void test_pq_empty_operations() {
    PriorityQueue pq;
    pq.arr = (Node **)calloc(5, sizeof(Node *));
    pq.size = 0;
    pq.capacity = 5;

    ASSERT_TRUE(pq_is_empty(&pq), "Empty priority queue should return true for is_empty");
    ASSERT_FALSE(pq_is_full(&pq), "Empty priority queue should not be full");

    pq_erase(&pq);
}

void test_pq_heap_property() {
    // Test that the heap property is maintained
    Node nodes[7];
    nodes[0] = *create_test_node('a', 10.0);
    nodes[1] = *create_test_node('b', 5.0);
    nodes[2] = *create_test_node('c', 3.0);
    nodes[3] = *create_test_node('d', 15.0);
    nodes[4] = *create_test_node('e', 7.0);
    nodes[5] = *create_test_node('f', 1.0);
    nodes[6] = *create_test_node('g', 12.0);

    PriorityQueue pq;
    pq_new(&pq, nodes, 7);

    // Check that elements come out in ascending order
    double prev_freq = 0.0;
    while (!pq_is_empty(&pq)) {
        Node* top = pq_top(&pq);
        ASSERT_TRUE(top->frequency >= prev_freq, "Elements should come out in ascending frequency order");
        prev_freq = top->frequency;
        pq_pop(&pq);
    }

    pq_erase(&pq);
}

int main() {
    init_tests();
    
    printf(COLOR_YELLOW "Testing Priority Queue Module" COLOR_RESET "\n");
    printf("========================================\n");

    RUN_TEST(test_pq_basic_operations);
    RUN_TEST(test_pq_push_operation);
    RUN_TEST(test_pq_empty_operations);
    RUN_TEST(test_pq_heap_property);

    TEST_SUMMARY();
}
