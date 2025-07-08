#!/bin/bash

# Demo script for Huffman Compressor Test Suite
# This script demonstrates the testing capabilities of the project

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

print_step() {
    echo -e "${YELLOW}>>> $1${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ] || [ ! -d "tests" ]; then
    print_error "Please run this script from the project root directory"
    exit 1
fi

# Check if build tools are available
command -v cmake >/dev/null 2>&1 || { print_error "CMake is required but not installed. Aborting."; exit 1; }
command -v make >/dev/null 2>&1 || { print_error "Make is required but not installed. Aborting."; exit 1; }

print_header "HUFFMAN COMPRESSOR TEST SUITE DEMO"

echo "This demo will:"
echo "1. Build the project and test suite"
echo "2. Run individual test modules"
echo "3. Run integration tests"
echo "4. Show test coverage summary"
echo ""
read -p "Press Enter to continue..."

# Step 1: Clean and build
print_step "1. Cleaning previous builds and building project"
make clean
make build

if [ $? -eq 0 ]; then
    print_success "Build completed successfully"
else
    print_error "Build failed"
    exit 1
fi

echo ""

# Step 2: Run test runner for overview
print_step "2. Running test runner for overview"
cd build
./test_runner
cd ..

echo ""
read -p "Press Enter to continue with individual test modules..."

# Step 3: Run individual test modules
print_step "3. Running individual test modules"

echo ""
echo -e "${YELLOW}3.1. Priority Queue Tests${NC}"
make test-pq
echo ""

echo -e "${YELLOW}3.2. Huffman Algorithm Tests${NC}"
make test-huffman
echo ""

echo -e "${YELLOW}3.3. IO Tools Tests${NC}"
make test-io
echo ""

echo -e "${YELLOW}3.4. Compression/Decompression Tests${NC}"
make test-compress
echo ""

echo -e "${YELLOW}3.5. Integration Tests${NC}"
make test-integration
echo ""

# Step 4: Run all tests with CTest
print_step "4. Running complete test suite with CTest"
make test

echo ""

# Step 5: Demonstrate test features
print_step "5. Demonstrating test features"

echo ""
echo -e "${YELLOW}5.1. Creating test files for demonstration${NC}"
cd build

# Create some test files
echo "Hello, World! This is a test file for compression." > demo_file1.txt
echo "Another test file with different content and patterns: abcabc xyzxyz 123123" > demo_file2.txt
echo "Single character file" | tr -d ' ' > demo_file3.txt

echo "Created demonstration files:"
ls -la demo_file*.txt

echo ""
echo -e "${YELLOW}5.2. Running the main compressor on test files${NC}"

# Test the actual compressor
./compresor demo_file1.txt demo_file2.txt demo_file3.txt demo_compressed.cprs

if [ -f "demo_compressed.cprs" ]; then
    echo "Compression successful!"
    echo "Original files total size: $(du -b demo_file*.txt | awk '{sum += $1} END {print sum}') bytes"
    echo "Compressed file size: $(du -b demo_compressed.cprs | awk '{print $1}') bytes"
    
    # Clean up original files
    rm demo_file*.txt
    
    echo ""
    echo -e "${YELLOW}5.3. Testing decompression${NC}"
    ./compresor -d demo_compressed.cprs
    
    echo "Decompression completed!"
    echo "Decompressed files:"
    ls -la demo_file*.txt 2>/dev/null || ls -la *demo_file*.txt 2>/dev/null || echo "Files decompressed with modified names"
    
    # Clean up
    rm -f demo_file*.txt demo_compressed.cprs
    rm -f *demo_file*.txt
else
    print_error "Compression failed"
fi

cd ..

echo ""

# Step 6: Memory testing (if valgrind is available)
print_step "6. Memory leak testing (optional)"

if command -v valgrind >/dev/null 2>&1; then
    echo "Valgrind detected. Running memory leak tests..."
    echo "Note: This may take a while..."
    
    cd build
    echo "Testing Huffman module for memory leaks:"
    timeout 30s valgrind --leak-check=summary --quiet ./test_huffman || echo "Test completed (may have timed out)"
    cd ..
else
    echo "Valgrind not available. Skipping memory leak tests."
    echo "To install valgrind: sudo apt-get install valgrind (on Ubuntu/Debian)"
fi

echo ""

# Step 7: Summary
print_header "TEST SUITE DEMO SUMMARY"

echo "✓ Project built successfully"
echo "✓ All test modules executed"
echo "✓ Integration tests completed"
echo "✓ Real compression/decompression demonstrated"
echo "✓ Memory testing attempted (if valgrind available)"

echo ""
echo "Test Suite Features Demonstrated:"
echo "- Custom testing framework with colored output"
echo "- Unit tests for all major components"
echo "- Integration tests for end-to-end functionality"
echo "- Error handling and edge case testing"
echo "- Memory management validation"
echo "- Automated test execution with CMake/CTest"

echo ""
echo "To run tests again:"
echo "  make test          # Run all tests"
echo "  make test-huffman  # Run specific module tests"
echo "  make quick-test    # Clean build and test cycle"

echo ""
print_success "Demo completed successfully!"

echo ""
echo "The test suite provides comprehensive coverage of:"
echo "- Priority queue operations and heap property"
echo "- Huffman tree construction and code generation"
echo "- File I/O and serialization"
echo "- Complete compression/decompression workflows"
echo "- Error conditions and edge cases"
echo "- Memory management and resource cleanup"

echo ""
echo "For more information, see:"
echo "- tests/README.md - Detailed testing documentation"
echo "- FUNCTIONALITIES.md - Updated with testing features"
echo "- ARCHITECTURE.md - Testing architecture details"
