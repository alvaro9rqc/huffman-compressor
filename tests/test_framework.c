#include "test_framework.h"

// Global test counters
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

void init_tests(void) {
    tests_run = 0;
    tests_passed = 0;
    tests_failed = 0;
}
