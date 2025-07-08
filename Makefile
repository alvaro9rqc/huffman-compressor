# Makefile for Huffman Compressor with Tests
# This is a convenience wrapper around CMake

BUILD_DIR := build
SOURCE_DIR := .

.PHONY: all build test clean help run-tests build-tests

# Default target
all: build

# Create build directory and configure with CMake
$(BUILD_DIR)/Makefile: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(SOURCE_DIR)

# Build the main project and all tests
build: $(BUILD_DIR)/Makefile
	@echo "Building project and tests..."
	@cd $(BUILD_DIR) && $(MAKE)

# Build only the tests
build-tests: $(BUILD_DIR)/Makefile
	@echo "Building test executables..."
	@cd $(BUILD_DIR) && $(MAKE) test_priority_queue test_huffman test_io_tool test_compress test_integration test_runner

# Run all tests using CTest
test: build
	@echo "Running all tests with CTest..."
	@cd $(BUILD_DIR) && ctest --verbose

# Run all tests using custom target
run-tests: build
	@echo "Running all tests..."
	@cd $(BUILD_DIR) && $(MAKE) run_all_tests

# Run individual test modules
test-pq: build
	@echo "Running Priority Queue tests..."
	@cd $(BUILD_DIR) && ./test_priority_queue

test-huffman: build
	@echo "Running Huffman algorithm tests..."
	@cd $(BUILD_DIR) && ./test_huffman

test-io: build
	@echo "Running IO Tools tests..."
	@cd $(BUILD_DIR) && ./test_io_tool

test-compress: build
	@echo "Running Compression/Decompression tests..."
	@cd $(BUILD_DIR) && ./test_compress

test-integration: build
	@echo "Running Integration tests..."
	@cd $(BUILD_DIR) && ./test_integration

# Run the test runner
test-runner: build
	@echo "Running test runner..."
	@cd $(BUILD_DIR) && ./test_runner

# Build main executable only
main: $(BUILD_DIR)/Makefile
	@echo "Building main executable..."
	@cd $(BUILD_DIR) && $(MAKE) compresor

# Clean build artifacts
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

# Run tests with memory checking (requires valgrind)
test-memory: build
	@echo "Running tests with memory checking..."
	@cd $(BUILD_DIR) && valgrind --leak-check=full --show-leak-kinds=all ./test_huffman
	@cd $(BUILD_DIR) && valgrind --leak-check=full --show-leak-kinds=all ./test_integration

# Quick test cycle (build and run tests)
quick-test: clean build test

# Development cycle: build and run specific test
dev-test-%: build
	@echo "Running development test for $*..."
	@cd $(BUILD_DIR) && ./test_$*

# Install (if you want to add installation rules later)
install: build
	@cd $(BUILD_DIR) && $(MAKE) install

# Package (if you want to add packaging later)
package: build
	@cd $(BUILD_DIR) && $(MAKE) package

# Show help
help:
	@echo "Huffman Compressor - Available targets:"
	@echo ""
	@echo "Building:"
	@echo "  build           - Build everything (main program + tests)"
	@echo "  build-tests     - Build only test executables"
	@echo "  main            - Build only the main executable"
	@echo ""
	@echo "Testing:"
	@echo "  test            - Run all tests with CTest"
	@echo "  run-tests       - Run all tests with custom runner"
	@echo "  test-runner     - Run the test runner interface"
	@echo ""
	@echo "Individual Tests:"
	@echo "  test-pq         - Run priority queue tests"
	@echo "  test-huffman    - Run Huffman algorithm tests"
	@echo "  test-io         - Run IO tools tests"
	@echo "  test-compress   - Run compression/decompression tests"
	@echo "  test-integration - Run integration tests"
	@echo ""
	@echo "Development:"
	@echo "  dev-test-<name> - Run specific test (e.g., dev-test-huffman)"
	@echo "  quick-test      - Clean, build, and test cycle"
	@echo "  test-memory     - Run tests with valgrind memory checking"
	@echo ""
	@echo "Maintenance:"
	@echo "  clean           - Remove build directory"
	@echo "  help            - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build everything"
	@echo "  make test         # Run all tests"
	@echo "  make test-huffman # Run only Huffman tests"
	@echo "  make quick-test   # Full clean build and test cycle"
