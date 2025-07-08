#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global test counters
extern int tests_run;
extern int tests_passed;
extern int tests_failed;

// Color codes for output
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_RESET   "\033[0m"

// Test macros
#define ASSERT_TRUE(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            printf(COLOR_GREEN "✓ PASS: %s" COLOR_RESET "\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "✗ FAIL: %s" COLOR_RESET "\n", message); \
            printf("   Expected: true, Got: false\n"); \
        } \
    } while(0)

#define ASSERT_FALSE(condition, message) \
    do { \
        tests_run++; \
        if (!(condition)) { \
            tests_passed++; \
            printf(COLOR_GREEN "✓ PASS: %s" COLOR_RESET "\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "✗ FAIL: %s" COLOR_RESET "\n", message); \
            printf("   Expected: false, Got: true\n"); \
        } \
    } while(0)

#define ASSERT_EQ(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            tests_passed++; \
            printf(COLOR_GREEN "✓ PASS: %s" COLOR_RESET "\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "✗ FAIL: %s" COLOR_RESET "\n", message); \
            printf("   Expected: %d, Got: %d\n", (int)(expected), (int)(actual)); \
        } \
    } while(0)

#define ASSERT_NEQ(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) != (actual)) { \
            tests_passed++; \
            printf(COLOR_GREEN "✓ PASS: %s" COLOR_RESET "\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "✗ FAIL: %s" COLOR_RESET "\n", message); \
            printf("   Expected: not %d, Got: %d\n", (int)(expected), (int)(actual)); \
        } \
    } while(0)

#define ASSERT_STR_EQ(expected, actual, message) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) == 0) { \
            tests_passed++; \
            printf(COLOR_GREEN "✓ PASS: %s" COLOR_RESET "\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "✗ FAIL: %s" COLOR_RESET "\n", message); \
            printf("   Expected: \"%s\", Got: \"%s\"\n", (expected), (actual)); \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr, message) \
    do { \
        tests_run++; \
        if ((ptr) != NULL) { \
            tests_passed++; \
            printf(COLOR_GREEN "✓ PASS: %s" COLOR_RESET "\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "✗ FAIL: %s" COLOR_RESET "\n", message); \
            printf("   Expected: non-NULL pointer, Got: NULL\n"); \
        } \
    } while(0)

#define ASSERT_NULL(ptr, message) \
    do { \
        tests_run++; \
        if ((ptr) == NULL) { \
            tests_passed++; \
            printf(COLOR_GREEN "✓ PASS: %s" COLOR_RESET "\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "✗ FAIL: %s" COLOR_RESET "\n", message); \
            printf("   Expected: NULL pointer, Got: non-NULL\n"); \
        } \
    } while(0)

// Test suite macros
#define RUN_TEST(test_func) \
    do { \
        printf(COLOR_BLUE "\n--- Running " #test_func " ---" COLOR_RESET "\n"); \
        test_func(); \
    } while(0)

#define TEST_SUMMARY() \
    do { \
        printf(COLOR_BLUE "\n=== Test Summary ===" COLOR_RESET "\n"); \
        printf("Tests run: %d\n", tests_run); \
        printf(COLOR_GREEN "Passed: %d" COLOR_RESET "\n", tests_passed); \
        if (tests_failed > 0) { \
            printf(COLOR_RED "Failed: %d" COLOR_RESET "\n", tests_failed); \
        } else { \
            printf("Failed: 0\n"); \
        } \
        printf("Success rate: %.1f%%\n", \
               tests_run > 0 ? (100.0 * tests_passed / tests_run) : 0.0); \
        return (tests_failed == 0) ? 0 : 1; \
    } while(0)

// Initialize test counters
void init_tests(void);

#endif // TEST_FRAMEWORK_H
