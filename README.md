# Huffman-compressor

Hi there, since my university (which is public) is currently on strike and my Systems Programming course isn't very good, I decided to create a file compressor in C to practice my programming skills and learn the things the university doesn't teach me

# Design Phase

- I need to encode one or more files
    - Create a new file
    - For each file
        - Create a huffman code
        - Write in the compress file:
            - name
            - bytes
            - encoded bytes
            - Huffman code
- I need to decode all the files
    - Read compress file until EOF
    - Get line: Name
    - Get long long: Size
    - Get short: encoded bytes
    - Read Huffman Code
    - For size of file
        - Create file
            - Read encoded bytes and write decoded information
        - Close file

- solution to save file: just save the amount of bytes in the file

# Process of implementation

1. hc_endoce_file
    1. create priority_queue 
    2. erase priority_queue
    3. huffman tree
        1. pop priority_queue
        2. build code matrix
    



# Promises:

## About compress file

Each huffman code in the file begins at a different byte than were the previous one ends


## About huffman tree

- The left child is 0 and the most likely of appearing

## Testing

This project includes a comprehensive test suite that validates all functionalities:

### Test Suite Features

- **Custom Testing Framework**: Lightweight C testing framework with colored output
- **Comprehensive Coverage**: Unit tests, integration tests, and system tests
- **Multiple Test Levels**:
  - Unit tests for individual modules (priority queue, Huffman algorithm, I/O tools)
  - Integration tests for module interactions
  - End-to-end system tests with real file compression/decompression
- **Error Handling**: Tests for edge cases, invalid inputs, and error conditions
- **Memory Management**: Memory leak detection and resource cleanup validation

### Running Tests

```bash
# Build and run all tests
make test

# Run individual test modules
make test-pq          # Priority queue tests
make test-huffman     # Huffman algorithm tests
make test-io          # I/O tools tests
make test-compress    # Compression/decompression tests
make test-integration # Integration tests

# Quick development cycle
make quick-test       # Clean, build, and test

# Memory leak testing (requires valgrind)
make test-memory

# Run test demonstration
./demo_tests.sh
```

### Test Structure

```
tests/
├── test_framework.h        # Custom testing framework
├── test_priority_queue.c   # Priority queue module tests
├── test_huffman.c          # Huffman algorithm tests
├── test_io_tool.c          # I/O tools tests
├── test_compress.c         # Compression/decompression tests
├── test_integration.c      # End-to-end integration tests
├── test_runner.c           # Test runner and summary
└── README.md               # Detailed testing documentation
```

The test suite includes:
- **90+ individual test cases** covering all major functionality
- **Positive, negative, and edge case testing**
- **Memory management validation**
- **Cross-platform compatibility testing**
- **Automated CI/CD integration ready**

For detailed testing documentation, see [tests/README.md](tests/README.md).
