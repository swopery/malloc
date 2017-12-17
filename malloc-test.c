/*
 * malloc-test.c
 * Ryan Swope and Anna Parker
 * CS315 - Systems Programming
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int test_basic();
int test_multiple();
int test_overflow();
int test_extra();
int test_calloc();
int test_realloc();

int main(int argc, char *argv[]) {
	test_basic();
	test_multiple();
	test_overflow();
	test_extra();
	test_calloc();
	test_realloc();
	return 0;
}

int test_basic(){
	// TEST BASIC MALLOC/FREE FUNCTIONALITY
	// (1) calls malloc of size < MACRO_SPACE
	// (2) stores character + newline in memory allocated
	// (3) write char to stdout
	// (4) free allocated memory
	void *ptr = malloc(100);
	*(char *)ptr = 'a';
	char *next = (char *)ptr + 1;
	*next = '\n';
	write(1, ptr, sizeof(*(char *)ptr) + 1);
	free(ptr);
	return 0;
}

int test_multiple(){
	// TEST MULTIPLE  MALLOC/FREE FUNCTIONALITY
	// (1) calls malloc of size < MACRO_SPACE
	// (2) calls malloc of size + prev_size < MACRO_SPACE
	// (3) stores character at first memory allocation
	// (4) stores character and newline in second memory allocation
	// (5) write all chars to stdout
	// (6) free both memory allocations
	void *ptr = malloc(100);
	void *ptr2 = malloc(200);
	*(char *)ptr = 'a';
	*(char *)ptr2 = 'b';
	char *next = (char *)ptr2 + 1;
	*next = '\n';
	write(1, ptr, sizeof(*(char *)ptr));
	write(1, ptr2, sizeof(*(char *)ptr2) + 1);
	free(ptr);
	free(ptr2);
	return 0;
}

int test_overflow(){
	// TEST MALLOC OVERFLOW/FREE FUNCTIONALITY
	// (1) calls malloc of size > MACRO_SPACE
	// (2) stores character + newline in memory allocated
	// (3) write char to stdout
	// (4) free allocated memory
	void *ptr = malloc(2500);
	*(char *)ptr = 'o';
	char *next = (char *)ptr + 1;
	*next = '\n';
	write(1, ptr, sizeof(*(char *)ptr) + 1);
	free(ptr);
	return 0;
}

int test_extra(){
	// TEST EXTRA  MALLOC/FREE FUNCTIONALITY
	// (1) calls malloc of size < MACRO_SPACE multiple times
	// (2) free all memory allocations
	void *ptr = malloc(100);
	void *ptr2 = malloc(200);
	void *ptr3 = malloc(300);
	void *ptr4 = malloc(400);
	void *ptr5 = malloc(500);
	void *ptr6 = malloc(600);
	void *ptr7 = malloc(700);
	void *ptr8 = malloc(800);
	void *ptr9 = malloc(900);
	void *ptr10 = malloc(1000);
	free(ptr);
	free(ptr2);
	free(ptr3);
	free(ptr4);
	free(ptr5);
	free(ptr6);
	free(ptr7);
	free(ptr8);
	free(ptr9);
	free(ptr10);
	return 0;
}

int test_calloc(){
	// TEST CALLOC FUNCTIONALITY
	// (1) calls calloc for two items of size 1
	// (2) assign chars to memory
	// (3) write chars to stdout
	void *ptr = calloc(2, 1);
	*(char *)ptr = 'c';
	char *next = (char *)ptr + 1;
	*next = '\n';
	write(1, ptr, 2);
	return 0;
}

int test_realloc(){
	// TEST REALLOC/FREE FUNCTIONALITY
	// (1) calls malloc for size 1
	// (2) assign char to memory from malloc
	// (3) realloc memory to size of 2
	// (4) assign char to new memory from realloc
	// (5) write bytes to stdout
	// (6) free memory
	void *ptr = malloc(1);
	*(char *)ptr = 'r';
	void *new_ptr = realloc(ptr, 2);
	char *next = (char *)new_ptr + 1;
	*next = '\n';
	write(1, new_ptr, 2);
	free(new_ptr);
	return 0;
}
