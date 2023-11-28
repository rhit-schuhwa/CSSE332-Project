#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct num_seq {
  int* array;
  int length;
};

/**
 * alloc_num_seq()
 *
 * Given a struct num_seq define above, this function will create a struct on the
 * heap and create spaces on the heap and also populate the data for the
 * corresponding fields (string and array).
 *
 * The function takes two strings as input. The first is a number (passed as a
 * string) defining the starting range N. The second is the end of the range M.
 *
 * Given the input, you need to
 *  1) Create a new struct num_seq on the HEAP.
 *
 *  2) Given N and M, you need to create an int array on the HEAP that includes
 *    numbers ranging from N to M (inclusively). You can assume N is less than M.
 *    For example, if the inputs are "1" and "9", the int array should be
 *    {1,2,3,4,5,6,7,8,9}.
 *
 *  After creating and populating this array, you need to
 *  make the field "array" of the struct point to this new array.
 *
 *  3) You need to assign the length of the array to the field "length".
 *    Following the previous example, the length should be 9.
 *
 *  4) As the return value of this function, you need to return the address of
 *    the struct you created on the HEAP.
 *
 * For example:
 *
 *  struct num_seq* test = alloc_num_seq("3", "5");
 *
 * Then:
 *  test->array will point to an int array like {3, 4, 5} on the heap;
 *  test->length will be 3;
 *
 *
 * NOTE: You need to allocate exact amount of space required for the int array
 * that the field "array" points to.
 *
 * Don't call free in this function.
 *
 * YOU MAY NOT CALL OTHER FUNCTIONS EXCEPT FOR strlen(), sizeof(), memcpy()
 *
 */
struct num_seq* alloc_num_seq(const char *start, const char *end) {
    struct num_seq *s = malloc(sizeof(struct num_seq));
    int start_int = atoi(start);
    int end_int = atoi(end);
    int arr_length = end_int - start_int + 1;
    
    s->length = arr_length;
    s->array = malloc(arr_length * sizeof(int));

    int index = start_int;
    for (int i = 0; index <= end_int; i++) {
	s->array[i] = index;
	index++;
    }
    return s;
}

/**
 * free_num_seq()
 *
 * This function takes a allocated struct num_seq pointer and free ALL the
 * memory allocated for that struct.
 *
 * You may assume that the structure has been properly allocated.
 *
 * You need to make sure that no memory leaks are possible after calling on
 * this function.
 */
void free_num_seq(struct num_seq *s) {
  free(s->array);
  free(s);
}

int main(int argc, char **argv) {
  int i;
  struct num_seq *s = alloc_num_seq("1", "9");

  i = 0;
  printf("Allocated s at %p, s->array at %p, printing the array:\n", s,
      s->array);
  for(; i < s->length; i++) {
    printf("%d ", s->array[i]);
  }
  printf("\n");

  free_num_seq(s);
}
