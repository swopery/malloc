/*
* my-malloc.c
* Ryan Swope and Anna Parker
* CS315 - Systems Programming
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define MACRO_SPACE 1024

// Linked-list struct
typedef struct {
	void *next;
	void *prev;
	bool free;
	int size;
} mem_struct;

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *address);

// Last struct in our program
static mem_struct *brk_struct = NULL;
// Struct to begin searching from
static mem_struct *start_struct = NULL;

void *malloc(size_t size) {

	// If no memory has been allocated, allocate space according
	// to macro and put struct at beginning
	if (start_struct == NULL){

		// initialize new data struct
		mem_struct initial_struct = {NULL, NULL, true,  MACRO_SPACE-sizeof(initial_struct)};
		void *initial_pointer = sbrk(MACRO_SPACE);
		*(mem_struct *)initial_pointer = initial_struct;

		// update pointers to reflect changes
		start_struct = initial_pointer;
		brk_struct = initial_pointer;
	}

	// Iterate through existing linked list to check if there is
	// enough space anywhere in our existing allocated space
	mem_struct *current_struct = start_struct;
	do {

		// if we find free struct of the same size, use it
		if (current_struct->free && current_struct->size == size){
			write(1, "same size\n", 10);
			current_struct->free = false;
			return current_struct + 1;
		}

		// otherwise if we find a free struct with enough space, use it
		else if (current_struct->free && current_struct->size >= size + sizeof(*current_struct)){
			write(1, "space found\n", 12);
			mem_struct next_struct = {
				current_struct->next,
				current_struct,
				true,
				current_struct->size - size - sizeof(next_struct)
			};
			mem_struct *next_pointer = (mem_struct *)((char *)current_struct + sizeof(*current_struct) + size);
			*next_pointer = next_struct;
			current_struct->next = next_pointer;
			current_struct->size = size;
			current_struct->free = false;
			brk_struct = (void *) next_pointer;
			return current_struct + 1;
		}
		current_struct = (mem_struct *)current_struct->next;
	} while (current_struct != NULL);

	// otherwise: allocate more space
	// add it to the linked list (new end of the linked list)
	// re-iterate through linked list to store user info
	current_struct = brk_struct;
	int needed_space = size - current_struct->size + sizeof(*current_struct);

	void *new_pointer;
	for (int i=0; i<=needed_space/MACRO_SPACE; i++ ){
		new_pointer = sbrk(MACRO_SPACE);
	}
	int remain_space = needed_space%MACRO_SPACE;
	new_pointer = (char *)new_pointer + remain_space - sizeof(*current_struct);
	mem_struct new_struct = {current_struct->next, current_struct, true, MACRO_SPACE - remain_space};
	*(mem_struct *)new_pointer= new_struct;
	current_struct->next = new_pointer;
	current_struct->size = size;
	current_struct->free = false;
	brk_struct = new_pointer;
	return current_struct + 1;
}

void *calloc(size_t nmemb, size_t size){
	if (nmemb == 0 || size == 0){
		return NULL;
	}
	int bytes = nmemb * size;
	void *ptr = malloc(bytes);
	memset(ptr, 0, bytes);
	return ptr;
}

void *realloc(void *ptr, size_t size){
	void *return_pointer;
	if (ptr == NULL){
		malloc(size);
	} else if (size == 0){
		free(ptr);
	} else {
		mem_struct *alloc = (mem_struct *)ptr;
		if (alloc->size > size){
			// get new memory
			alloc->size = size;
			mem_struct *new_alloc = malloc(size);
			memcpy(new_alloc, alloc, alloc->size);
			free(alloc);
			return_pointer = (void *)new_alloc;
		} else {
			// memory up to size will not be changed, rest is freed
			alloc->size = size;
			free(alloc+alloc->size);
			return_pointer = (void *)alloc;
		}
	}
	return return_pointer;
}

void free(void *address) {
	if (address == NULL){
		return;
	}
	mem_struct *free_struct = ((mem_struct *)address - 1);
	// Notify struct it is free so it can be written over
	free_struct->free = true;
}
