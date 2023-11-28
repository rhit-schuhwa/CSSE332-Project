#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 *
 * find_next_field()
 *
 * Given an input string (represented by a char pointer), the string can be
 * divided into multiple fields by using a certain separator. In this problem,
 * the separator is defined as as DOUBLE SPACE, i.e., "  ".
 *
 * Given the input pointer as the starting point, the function will find the
 * position of the beginning of the next field. Namely, you need to find the
 * position of the next double space and then return the pointer that points to
 * the character right after the double space.
 *
 * If no separator is found, the function returns the original pointer.
 *
 * For example:
 * 1)
 *  char* message = "0123  45."
 *  char* ret = find_next_field(message)
 *
 *  "0123  45."
 *         |
 *         |
 *         v
 *        ret
 *  //ret == message + 6
 *  //now ret points to the position shown above
 *
 * 2)
 *  char* message = "find next  field."
 *  char* ret = find_next_field(message)
 *
 *  "find next  field."
 *              |
 *              |
 *              v
 *             ret
 *  //ret == message + 11
 *  //now ret points to the position shown above
 *
 * YOU MAY NOT CALL OTHER FUNCTIONS FROM string.h EXCEPT FOR strlen() and
 * strncmp().
 *
 * @param input - the input string to search
 * @returns a pointer to the beginning of the next field.
 */
char *find_next_field(char *input) {
    char *ret = input;
    for (int i = 0; i < strlen(input) - 1; i++) {
	if (input[i] == ' ' && input[i+1] == ' ') {
	    ret = input + i + 2;
	    break;
	}
    }
    return ret;
}

//
// add a main function to test the following inputs:
//
// Test case 1:
//  Input: "a  b"
//  Expected output: "b"
//
// Test case 2:
//  Input: "A  big long test  string with* things    in it."
//  Expected output: "big long test  string with* things    in it."
//
// Test case 3:
//  Input: "big long test  string with* things    in it."
//  Expected output: "string with* things    in it."
//
//  Note: Do not create a new test string for test 3, use the output of test 2.
//

// Your code starts here.
int main() {
    char* test_str = "a  b";
    char* res = find_next_field(test_str);
    printf("Test Case 1:\nInput: \"%s\"\nExpected Output: \"b\"\nActual Output: \"%s\"\n", test_str, res);
    if (strcmp("b", res) == 0) {
	printf("Test 1 PASSED\n\n");
    } else {
	printf("Test 1 FAILED\n\n");
    }

    test_str = "A  big long test  string with* things    in it.";
    res = find_next_field(test_str);
    printf("Test Case 2:\nInput: \"%s\"\nExpected Output: \"big long test  string with* things    in it.\"\nActual Output: \"%s\"\n", test_str, res);
    if (strcmp("big long test  string with* things    in it.", res) == 0) {
	printf("Test 2 PASSED\n\n");
    } else {
	printf("Test 2 FAILED\n\n");
    }

    test_str = find_next_field(res);
    printf("Test Case 3:\nInput: \"%s\"\nExpected Output: \"string with* things    in it.\"\nActual Output: \"%s\"\n", res, test_str);
    if (strcmp("string with* things    in it.", test_str) == 0) {
	printf("Test 3 PASSED\n\n");
    } else {
	printf("Test 3 FAILED\n\n");
    }
}
