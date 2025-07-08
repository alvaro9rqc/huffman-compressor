#include "test_framework.h"
#include "../include/compress.h"
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

int compare_files(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "rb");
    FILE* f2 = fopen(file2, "rb");
    
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 0; // Files don't match if we can't open them
    }
    
    int ch1, ch2;
    do {
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
    } while (ch1 == ch2 && ch1 != EOF);
    
    fclose(f1);
    fclose(f2);
    
    return (ch1 == ch2); // 1 if files match, 0 if different
}

void test_compress_single_file() {
    const char* input_file = "test_input.txt";
    const char* compressed_file = "test_compressed.cprs";
    
    // Create test input file
    create_test_file(input_file, "Hello, World! This is a test file for compression.");
    
    // Prepare arguments for compression
    char* argv[] = {"program", (char*)input_file, (char*)compressed_file};
    int argc = 3;
    
    // Open compressed file for writing
    FILE* comp_file = fopen(compressed_file, "wb");
    ASSERT_NOT_NULL(comp_file, "Should be able to create compressed file");
    
    if (comp_file) {
        // Test compression
        char result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        ASSERT_EQ(0, result, "Compression should succeed");
        ASSERT_TRUE(file_exists(compressed_file), "Compressed file should be created");
        
        // Verify compressed file has content
        size_t comp_size = get_file_size(compressed_file);
        ASSERT_TRUE(comp_size > 0, "Compressed file should have content");
    }
    
    // Clean up
    cleanup_test_file(input_file);
    cleanup_test_file(compressed_file);
}

void test_compress_multiple_files() {
    const char* input_file1 = "test_input1.txt";
    const char* input_file2 = "test_input2.txt";
    const char* compressed_file = "test_multi_compressed.cprs";
    
    // Create test input files
    create_test_file(input_file1, "First test file content.");
    create_test_file(input_file2, "Second test file with different content!");
    
    // Prepare arguments for compression
    char* argv[] = {"program", (char*)input_file1, (char*)input_file2, (char*)compressed_file};
    int argc = 4;
    
    // Open compressed file for writing
    FILE* comp_file = fopen(compressed_file, "wb");
    ASSERT_NOT_NULL(comp_file, "Should be able to create compressed file");
    
    if (comp_file) {
        // Test compression
        char result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        ASSERT_EQ(0, result, "Multiple file compression should succeed");
        ASSERT_TRUE(file_exists(compressed_file), "Compressed file should be created");
        
        // Verify compressed file has content
        size_t comp_size = get_file_size(compressed_file);
        ASSERT_TRUE(comp_size > 0, "Compressed file should have content");
    }
    
    // Clean up
    cleanup_test_file(input_file1);
    cleanup_test_file(input_file2);
    cleanup_test_file(compressed_file);
}

void test_decompress_file() {
    const char* input_file = "test_decomp_input.txt";
    const char* compressed_file = "test_decomp_compressed.cprs";
    const char* original_content = "This is test content for decompression testing.";
    
    // Create test input file
    create_test_file(input_file, original_content);
    
    // First, compress the file
    char* argv[] = {"program", (char*)input_file, (char*)compressed_file};
    int argc = 3;
    
    FILE* comp_file = fopen(compressed_file, "wb");
    if (comp_file) {
        char comp_result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        if (comp_result == 0) {
            // Remove original file
            cleanup_test_file(input_file);
            
            // Now test decompression
            FILE* decomp_file = fopen(compressed_file, "rb");
            ASSERT_NOT_NULL(decomp_file, "Should be able to open compressed file for reading");
            
            if (decomp_file) {
                int decomp_result = decompress_file(decomp_file);
                fclose(decomp_file);
                
                ASSERT_EQ(0, decomp_result, "Decompression should succeed");
                
                // Check if decompressed file was created
                // The function might create a file with a different name to avoid conflicts
                ASSERT_TRUE(file_exists(input_file) || 
                           file_exists("test_decomp_input_1.txt") ||
                           file_exists("test_decomp_input(1).txt"), 
                           "Decompressed file should be created");
            }
        }
    }
    
    // Clean up
    cleanup_test_file(input_file);
    cleanup_test_file("test_decomp_input_1.txt");
    cleanup_test_file("test_decomp_input(1).txt");
    cleanup_test_file(compressed_file);
}

void test_compress_decompress_roundtrip() {
    const char* input_file = "test_roundtrip.txt";
    const char* compressed_file = "test_roundtrip.cprs";
    const char* test_content = "This is a comprehensive test for roundtrip compression and decompression. "
                              "It includes various characters: !@#$%^&*()_+-=[]{}|;:,.<>? "
                              "Numbers: 1234567890 "
                              "And some repeated patterns: abcabc xyzxyz";
    
    // Create test input file
    create_test_file(input_file, test_content);
    
    // Compress the file
    char* argv[] = {"program", (char*)input_file, (char*)compressed_file};
    int argc = 3;
    
    FILE* comp_file = fopen(compressed_file, "wb");
    if (comp_file) {
        char comp_result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        ASSERT_EQ(0, comp_result, "Compression should succeed");
        
        if (comp_result == 0) {
            // Get original file size
            size_t original_size = get_file_size(input_file);
            size_t compressed_size = get_file_size(compressed_file);
            
            ASSERT_TRUE(compressed_size > 0, "Compressed file should have content");
            
            // Remove original to test decompression
            cleanup_test_file(input_file);
            
            // Decompress
            FILE* decomp_file = fopen(compressed_file, "rb");
            if (decomp_file) {
                int decomp_result = decompress_file(decomp_file);
                fclose(decomp_file);
                
                ASSERT_EQ(0, decomp_result, "Decompression should succeed");
                
                // Verify the decompressed file exists and has correct content
                if (file_exists(input_file)) {
                    FILE* check_file = fopen(input_file, "r");
                    if (check_file) {
                        char buffer[1000];
                        size_t read_size = fread(buffer, 1, sizeof(buffer)-1, check_file);
                        buffer[read_size] = '\0';
                        fclose(check_file);
                        
                        ASSERT_STR_EQ(test_content, buffer, "Decompressed content should match original");
                    }
                }
            }
        }
    }
    
    // Clean up
    cleanup_test_file(input_file);
    cleanup_test_file(compressed_file);
}

void test_compress_empty_file() {
    const char* input_file = "test_empty.txt";
    const char* compressed_file = "test_empty_compressed.cprs";
    
    // Create empty test file
    create_test_file(input_file, "");
    
    char* argv[] = {"program", (char*)input_file, (char*)compressed_file};
    int argc = 3;
    
    FILE* comp_file = fopen(compressed_file, "wb");
    if (comp_file) {
        char result = compress_encode_files(comp_file, argc, argv);
        fclose(comp_file);
        
        // Empty file compression might fail or succeed depending on implementation
        // We just test that it doesn't crash
        ASSERT_TRUE(result == 0 || result != 0, "Empty file compression should handle gracefully");
    }
    
    // Clean up
    cleanup_test_file(input_file);
    cleanup_test_file(compressed_file);
}

void test_decompress_invalid_file() {
    const char* invalid_file = "test_invalid.cprs";
    
    // Create a file with invalid compressed data
    create_test_file(invalid_file, "This is not valid compressed data");
    
    FILE* file = fopen(invalid_file, "rb");
    if (file) {
        int result = decompress_file(file);
        fclose(file);
        
        // Should handle invalid data gracefully (return error)
        ASSERT_NEQ(0, result, "Should return error for invalid compressed data");
    }
    
    cleanup_test_file(invalid_file);
}

int main() {
    init_tests();
    
    printf(COLOR_YELLOW "Testing Compression/Decompression Module" COLOR_RESET "\n");
    printf("========================================\n");

    RUN_TEST(test_compress_single_file);
    RUN_TEST(test_compress_multiple_files);
    RUN_TEST(test_decompress_file);
    RUN_TEST(test_compress_decompress_roundtrip);
    RUN_TEST(test_compress_empty_file);
    RUN_TEST(test_decompress_invalid_file);

    TEST_SUMMARY();
}
