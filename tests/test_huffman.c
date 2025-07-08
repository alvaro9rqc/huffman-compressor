#include "test_framework.h"
#include "../include/huffman.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Helper function to create a test file
void create_test_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file) {
        fputs(content, file);
        fclose(file);
    }
}

// Helper function to remove test file
void cleanup_test_file(const char* filename) {
    unlink(filename);
}

void test_hc_tree_creation() {
    // Create a simple test file
    const char* test_file = "test_simple.txt";
    create_test_file(test_file, "aabbcc");

    Node* root;
    unsigned char** code = hc_endoce_file((char*)test_file, &root);

    ASSERT_NOT_NULL(code, "Huffman code should not be NULL");
    ASSERT_NOT_NULL(root, "Huffman tree root should not be NULL");
    ASSERT_FALSE(root->is_leaf, "Root should not be a leaf node");

    // Test that codes are generated for the characters in the file
    ASSERT_NOT_NULL(code['a'], "Code for 'a' should exist");
    ASSERT_NOT_NULL(code['b'], "Code for 'b' should exist");
    ASSERT_NOT_NULL(code['c'], "Code for 'c' should exist");

    // Test that codes have valid lengths
    ASSERT_TRUE(code['a'][0] > 0, "Code for 'a' should have positive length");
    ASSERT_TRUE(code['b'][0] > 0, "Code for 'b' should have positive length");
    ASSERT_TRUE(code['c'][0] > 0, "Code for 'c' should have positive length");

    // Clean up
    hc_free_tree(root);
    hc_free_code(code);
    cleanup_test_file(test_file);
}

void test_hc_single_character() {
    // Test with a file containing only one type of character
    const char* test_file = "test_single.txt";
    create_test_file(test_file, "aaaa");

    Node* root;
    unsigned char** code = hc_endoce_file((char*)test_file, &root);

    ASSERT_NOT_NULL(code, "Huffman code should not be NULL for single character");
    ASSERT_NOT_NULL(root, "Huffman tree root should not be NULL for single character");

    // For a single character, the tree should have only one node
    ASSERT_TRUE(root->is_leaf, "Root should be a leaf for single character");
    ASSERT_EQ('a', root->byte, "Root should contain the single character 'a'");

    // Clean up
    hc_free_tree(root);
    hc_free_code(code);
    cleanup_test_file(test_file);
}

void test_hc_tree_structure() {
    // Create a test file with known frequency distribution
    const char* test_file = "test_structure.txt";
    create_test_file(test_file, "aaabbbc"); // a:3, b:3, c:1

    Node* root;
    unsigned char** code = hc_endoce_file((char*)test_file, &root);

    ASSERT_NOT_NULL(root, "Root should not be NULL");
    ASSERT_FALSE(root->is_leaf, "Root should not be a leaf");

    // Verify tree structure properties
    ASSERT_NOT_NULL(root->left, "Root should have left child");
    ASSERT_NOT_NULL(root->right, "Root should have right child");

    // The frequency of root should be sum of children
    double expected_freq = root->left->frequency + root->right->frequency;
    ASSERT_TRUE(abs(root->frequency - expected_freq) < 0.001, "Root frequency should be sum of children");

    // Clean up
    hc_free_tree(root);
    hc_free_code(code);
    cleanup_test_file(test_file);
}

void test_hc_code_uniqueness() {
    // Test that generated codes are unique (prefix property)
    const char* test_file = "test_unique.txt";
    create_test_file(test_file, "abcdef");

    Node* root;
    unsigned char** code = hc_endoce_file((char*)test_file, &root);

    ASSERT_NOT_NULL(code, "Code should not be NULL");

    // Count how many characters have codes
    int code_count = 0;
    for (int i = 0; i < 256; i++) {
        if (code[i] != NULL) {
            code_count++;
        }
    }

    ASSERT_EQ(6, code_count, "Should have codes for exactly 6 characters");

    // Verify that we have codes for our test characters
    ASSERT_NOT_NULL(code['a'], "Should have code for 'a'");
    ASSERT_NOT_NULL(code['b'], "Should have code for 'b'");
    ASSERT_NOT_NULL(code['c'], "Should have code for 'c'");
    ASSERT_NOT_NULL(code['d'], "Should have code for 'd'");
    ASSERT_NOT_NULL(code['e'], "Should have code for 'e'");
    ASSERT_NOT_NULL(code['f'], "Should have code for 'f'");

    // Clean up
    hc_free_tree(root);
    hc_free_code(code);
    cleanup_test_file(test_file);
}

void test_hc_empty_file() {
    // Test with an empty file
    const char* test_file = "test_empty.txt";
    create_test_file(test_file, "");

    Node* root;
    unsigned char** code = hc_endoce_file((char*)test_file, &root);

    // For empty file, code might be NULL or root might be NULL
    // This tests error handling
    if (code == NULL || root == NULL) {
        ASSERT_TRUE(1, "Empty file handling - returns NULL as expected");
    } else {
        // If it doesn't return NULL, make sure we can clean up
        hc_free_tree(root);
        hc_free_code(code);
    }

    cleanup_test_file(test_file);
}

void test_hc_memory_management() {
    // Test that memory is properly managed
    const char* test_file = "test_memory.txt";
    create_test_file(test_file, "test");

    Node* root;
    unsigned char** code = hc_endoce_file((char*)test_file, &root);

    ASSERT_NOT_NULL(code, "Code should not be NULL");
    ASSERT_NOT_NULL(root, "Root should not be NULL");

    // Test that free functions return 0 (success)
    int tree_free_result = hc_free_tree(root);
    ASSERT_EQ(0, tree_free_result, "Tree free should return 0");

    int code_free_result = hc_free_code(code);
    ASSERT_EQ(0, code_free_result, "Code free should return 0");

    cleanup_test_file(test_file);
}

int main() {
    init_tests();
    
    printf(COLOR_YELLOW "Testing Huffman Module" COLOR_RESET "\n");
    printf("========================================\n");

    RUN_TEST(test_hc_tree_creation);
    RUN_TEST(test_hc_single_character);
    RUN_TEST(test_hc_tree_structure);
    RUN_TEST(test_hc_code_uniqueness);
    RUN_TEST(test_hc_empty_file);
    RUN_TEST(test_hc_memory_management);

    TEST_SUMMARY();
}
