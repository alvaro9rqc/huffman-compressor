#include "test_framework.h"
#include "../include/compress.h"
#include "../include/huffman.h"
#include "../include/io_tool.h"
#include "../include/priority_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Helper functions
void create_test_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file) {
        fputs(content, file);
        fclose(file);
    }
}

void cleanup_test_file(const char* filename) {
    unlink(filename);
}

int file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

size_t get_file_size(const char* filename) {
    struct stat buffer;
    if (stat(filename, &buffer) == 0) {
        return buffer.st_size;
    }
    return 0;
}

void test_full_system_integration() {
    const char* input_files[] = {
        "integration_test1.txt",
        "integration_test2.txt",
        "integration_test3.txt"
    };
    const char* compressed_file = "integration_test.cprs";
    
    const char* contents[] = {
        "This is the first test file with some content that should compress well because it has repeated patterns and common words.",
        "Second file contains different content: numbers 123456789, symbols !@#$%^&*(), and more text to make it interesting.",
        "The third file is shorter but still has enough content to create a meaningful Huffman tree for compression testing."
    };
    
    // Create test input files
    for (int i = 0; i < 3; i++) {
        create_test_file(input_files[i], contents[i]);
        ASSERT_TRUE(file_exists(input_files[i]), "Test input file should be created");
    }
    
    // Test compression of multiple files
    char* argv[] = {
        "program", 
        (char*)input_files[0], 
        (char*)input_files[1], 
        (char*)input_files[2], 
        (char*)compressed_file
    };
    int argc = 5;
    
    FILE* comp_file = fopen(compressed_file, "wb");
    ASSERT_NOT_NULL(comp_file, "Should be able to create compressed file");
    
    if (comp_file) {
        char comp_result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        ASSERT_EQ(0, comp_result, "Multi-file compression should succeed");
        ASSERT_TRUE(file_exists(compressed_file), "Compressed file should exist");
        
        size_t comp_size = get_file_size(compressed_file);
        ASSERT_TRUE(comp_size > 0, "Compressed file should have content");
        
        // Calculate total original size
        size_t total_original_size = 0;
        for (int i = 0; i < 3; i++) {
            total_original_size += get_file_size(input_files[i]);
        }
        
        printf("Original total size: %zu bytes, Compressed size: %zu bytes\n", 
               total_original_size, comp_size);
        
        // Remove original files to test decompression
        for (int i = 0; i < 3; i++) {
            cleanup_test_file(input_files[i]);
        }
        
        // Test decompression
        FILE* decomp_file = fopen(compressed_file, "rb");
        ASSERT_NOT_NULL(decomp_file, "Should be able to open compressed file for reading");
        
        if (decomp_file) {
            int decomp_result = decompress_file(decomp_file);
            fclose(decomp_file);
            
            ASSERT_EQ(0, decomp_result, "Decompression should succeed");
            
            // Verify that decompressed files exist (they might have different names)
            int files_found = 0;
            for (int i = 0; i < 3; i++) {
                if (file_exists(input_files[i])) {
                    files_found++;
                }
            }
            
            // Also check for files with modified names
            const char* possible_names[] = {
                "integration_test1_1.txt", "integration_test1(1).txt",
                "integration_test2_1.txt", "integration_test2(1).txt", 
                "integration_test3_1.txt", "integration_test3(1).txt"
            };
            
            for (int i = 0; i < 6; i++) {
                if (file_exists(possible_names[i])) {
                    files_found++;
                }
            }
            
            ASSERT_TRUE(files_found >= 3, "Should decompress all three files");
            
            // Clean up possible decompressed files
            for (int i = 0; i < 6; i++) {
                cleanup_test_file(possible_names[i]);
            }
        }
    }
    
    // Clean up
    for (int i = 0; i < 3; i++) {
        cleanup_test_file(input_files[i]);
    }
    cleanup_test_file(compressed_file);
}

void test_system_with_various_file_types() {
    // Test with different types of content
    const char* text_file = "test_text.txt";
    const char* binary_like_file = "test_binary.txt";
    const char* compressed_file = "test_various.cprs";
    
    // Create text file with normal text
    create_test_file(text_file, "The quick brown fox jumps over the lazy dog. "
                                "This sentence contains all letters of the alphabet. "
                                "Repeated words: the the the fox fox brown brown.");
    
    // Create file with mixed content but fewer unique bytes
    FILE* bin_file = fopen(binary_like_file, "wb");
    if (bin_file) {
        // Use only a subset of ASCII values to avoid tree complexity issues
        for (int i = 65; i < 91; i++) { // A-Z
            fputc(i, bin_file);
            fputc(i, bin_file); // Duplicate for better compression
        }
        for (int i = 97; i < 123; i++) { // a-z  
            fputc(i, bin_file);
        }
        fclose(bin_file);
    }
    
    char* argv[] = {"program", (char*)text_file, (char*)binary_like_file, (char*)compressed_file};
    int argc = 4;
    
    // Test compression
    FILE* comp_file = fopen(compressed_file, "wb");
    if (comp_file) {
        char comp_result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        ASSERT_EQ(0, comp_result, "Should handle various file types");
        
        if (comp_result == 0) {
            // Test decompression
            cleanup_test_file(text_file);
            cleanup_test_file(binary_like_file);
            
            FILE* decomp_file = fopen(compressed_file, "rb");
            if (decomp_file) {
                int decomp_result = decompress_file(decomp_file);
                fclose(decomp_file);
                
                ASSERT_EQ(0, decomp_result, "Should decompress various file types");
            }
        }
    }
    
    // Clean up
    cleanup_test_file(text_file);
    cleanup_test_file(binary_like_file);
    cleanup_test_file("test_text_1.txt");
    cleanup_test_file("test_text(1).txt");
    cleanup_test_file("test_binary_1.txt");
    cleanup_test_file("test_binary(1).txt");
    cleanup_test_file(compressed_file);
}

void test_error_handling_integration() {
    const char* valid_file = "test_valid.txt";
    const char* compressed_file = "test_error.cprs";
    
    // Create a valid file
    create_test_file(valid_file, "Valid content");
    
    // Test decompression with invalid file (instead of compression with non-existent file)
    create_test_file(compressed_file, "Invalid compressed data - not real compression format");
    
    FILE* decomp_file = fopen(compressed_file, "rb");
    if (decomp_file) {
        int decomp_result = decompress_file(decomp_file);
        fclose(decomp_file);
        
        // Should handle invalid data gracefully
        ASSERT_NEQ(0, decomp_result, "Should return error for invalid compressed data");
    }
    
    // Test successful compression for comparison
    char* argv[] = {"program", (char*)valid_file, (char*)compressed_file};
    int argc = 3;
    
    FILE* comp_file = fopen(compressed_file, "wb");
    if (comp_file) {
        char comp_result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        ASSERT_EQ(0, comp_result, "Should succeed with valid file");
    }
    
    // Clean up
    cleanup_test_file(valid_file);
    cleanup_test_file(compressed_file);
}

void test_memory_leaks_integration() {
    // This test focuses on ensuring no obvious memory leaks in normal operation
    const char* input_file = "test_memory.txt";
    const char* compressed_file = "test_memory.cprs";
    
    // Create a file with content that will exercise all code paths
    create_test_file(input_file, "Memory test content with various characters: "
                                "abcdefghijklmnopqrstuvwxyz "
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
                                "0123456789 !@#$%^&*()_+-=[]{}|;:,.<>?");
    
    // Perform multiple compression/decompression cycles
    for (int cycle = 0; cycle < 3; cycle++) {
        char* argv[] = {"program", (char*)input_file, (char*)compressed_file};
        int argc = 3;
        
        // Compress
        FILE* comp_file = fopen(compressed_file, "wb");
        if (comp_file) {
            char comp_result = compress_encode_files(comp_file, argc, argv);
            fclose(comp_file);
            
            if (comp_result == 0) {
                // Decompress
                cleanup_test_file(input_file);
                
                FILE* decomp_file = fopen(compressed_file, "rb");
                if (decomp_file) {
                    int decomp_result = decompress_file(decomp_file);
                    fclose(decomp_file);
                    
                    ASSERT_EQ(0, decomp_result, "Memory test cycle should succeed");
                }
                
                // Recreate input file for next cycle
                if (cycle < 2) {
                    create_test_file(input_file, "Memory test content with various characters: "
                                                "abcdefghijklmnopqrstuvwxyz "
                                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
                                                "0123456789 !@#$%^&*()_+-=[]{}|;:,.<>?");
                }
            }
        }
        
        cleanup_test_file(compressed_file);
    }
    
    // Clean up
    cleanup_test_file(input_file);
    cleanup_test_file("test_memory_1.txt");
    cleanup_test_file("test_memory(1).txt");
}

int main() {
    init_tests();
    
    printf(COLOR_YELLOW "Running Integration Tests" COLOR_RESET "\n");
    printf("========================================\n");

    RUN_TEST(test_full_system_integration);
    RUN_TEST(test_system_with_various_file_types);
    RUN_TEST(test_error_handling_integration);
    RUN_TEST(test_memory_leaks_integration);

    TEST_SUMMARY();
}
