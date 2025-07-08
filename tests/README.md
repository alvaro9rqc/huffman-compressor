# Test Suite for Huffman Compressor

This directory contains comprehensive tests for all modules of the Huffman Compressor project.

## Test Structure

The test suite is organized into the following modules:

### 1. Test Framework (`test_framework.h/c`)
- Simple, lightweight testing framework
- Provides assertion macros (ASSERT_TRUE, ASSERT_EQ, etc.)
- Color-coded output for better readability
- Test statistics and reporting

### 2. Priority Queue Tests (`test_priority_queue.c`)
Tests for the min-heap priority queue implementation:
- Basic operations (push, pop, top)
- Heap property maintenance
- Empty and full queue handling
- Memory management

### 3. Huffman Algorithm Tests (`test_huffman.c`)
Tests for the core Huffman compression algorithm:
- Huffman tree construction
- Code generation and uniqueness
- Single character handling
- Tree structure validation
- Memory management
- Edge cases (empty files)

### 4. IO Tools Tests (`test_io_tool.c`)
Tests for file input/output operations:
- File reading and byte counting
- Tree serialization/deserialization
- Unique file creation
- EOF detection
- Error handling

### 5. Compression/Decompression Tests (`test_compress.c`)
Tests for the main compression and decompression functionality:
- Single file compression
- Multiple file compression
- Decompression functionality
- Roundtrip compression/decompression
- Error handling for invalid data

### 6. Integration Tests (`test_integration.c`)
End-to-end system tests:
- Full system integration
- Various file types
- Error handling integration
- Memory leak testing

## Building and Running Tests

### Prerequisites
- CMake 3.10 or higher
- C compiler supporting C11 standard
- The main project must be built first

### Building Tests

```bash
# From the project root directory
cd build
cmake ..
make

# Build individual test executables
make test_priority_queue
make test_huffman
make test_io_tool
make test_compress
make test_integration
```

### Running Tests

#### Run Individual Test Modules

```bash
# Run specific test modules
./test_priority_queue
./test_huffman
./test_io_tool
./test_compress
./test_integration
```

#### Run All Tests with CTest

```bash
# Run all tests using CTest
ctest --verbose

# Or use the custom target
make run_all_tests
```

#### Run Test Runner

```bash
# Run the test runner for an overview
./test_runner

# Run specific modules through test runner
./test_runner pq huffman
./test_runner integration
./test_runner all
```

## Test Output

### Successful Test Output
```
✓ PASS: Test description
```

### Failed Test Output
```
✗ FAIL: Test description
   Expected: value, Got: different_value
```

### Test Summary
```
=== Test Summary ===
Tests run: 25
Passed: 24
Failed: 1
Success rate: 96.0%
```

## Test Coverage

The test suite covers:

1. **Functionality Testing**
   - All public functions are tested
   - Edge cases and boundary conditions
   - Error conditions and invalid inputs

2. **Integration Testing**
   - Module interactions
   - End-to-end workflows
   - File format compatibility

3. **Memory Management**
   - Proper allocation and deallocation
   - Memory leak detection
   - Resource cleanup

4. **Error Handling**
   - Invalid file inputs
   - Corrupted data handling
   - System resource limitations

## Adding New Tests

To add new tests:

1. **For existing modules**: Add test functions to the appropriate test file
2. **For new modules**: Create a new test file following the naming convention `test_<module>.c`
3. **Update CMakeLists.txt**: Add the new test executable and link to CTest

### Test Function Template

```c
void test_new_functionality() {
    // Setup
    setup_test_data();
    
    // Execute
    int result = function_under_test();
    
    // Assert
    ASSERT_EQ(expected_value, result, "Test description");
    
    // Cleanup
    cleanup_test_data();
}
```

## Troubleshooting

### Common Issues

1. **Build Errors**
   - Ensure the main project builds successfully first
   - Check that all header files are accessible
   - Verify CMake configuration

2. **Test Failures**
   - Check file permissions for test file creation/deletion
   - Ensure sufficient disk space for temporary files
   - Verify that the working directory is correct

3. **Memory Issues**
   - Run tests with valgrind to detect memory leaks
   - Ensure proper cleanup in test functions

### Running with Debugging

```bash
# Build with debug symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Run with valgrind (if available)
valgrind --leak-check=full ./test_huffman

# Run with GDB
gdb ./test_huffman
```

## Test Data

Tests create temporary files during execution:
- All test files are prefixed with "test_"
- Files are automatically cleaned up after each test
- Test files are created in the current working directory

## Continuous Integration

This test suite is designed to be easily integrated into CI/CD pipelines:

```bash
# Example CI script
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

## Contributing

When contributing to the test suite:

1. Follow the existing test naming conventions
2. Include both positive and negative test cases
3. Test edge cases and boundary conditions
4. Ensure proper cleanup of resources
5. Add descriptive test messages
6. Update this README if adding new test modules

## License

This test suite is part of the Huffman Compressor project and follows the same license terms.
