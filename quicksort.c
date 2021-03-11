/*******************************************************************************
 * Name        : quicksort.c
 * Author      : Breanna Shinn & Jose de la Cruz
 * Date        : 2/26/2021
 * Description : Quicksort implementation.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "quicksort.h"
#include <stdlib.h>

/* Static (private to this file) function prototypes. */
static void swap(void *a, void *b, size_t size);
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*comp) (const void*, const void*));
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*comp) (const void*, const void*));

/**
 * Compares two integers passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to int pointers.
 * Returns:
 * -- 0 if the integers are equal
 * -- a positive number if the first integer is greater
 * -- a negative if the second integer is greater
 */
int int_cmp(const void *a, const void *b) {
    int a1 = *(const int*)a;
    int b1 = *(const int*)b;

    if (a1 == b1) {
        return 0;
    }
    else if (a1 > b1) {
        return 1;
    }
    else {
        return -1;
    }
}

/**
 * Compares two doubles passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to double pointers.
 * Returns:
 * -- 0 if the doubles are equal
 * -- 1 if the first double is greater
 * -- -1 if the second double is greater
 */
int dbl_cmp(const void *a, const void *b) {
    double a1 = *(const double*)a;
    double b1 = *(const double*)b;

    if (a1 == b1) {
        return 0;
    }
    else if (a1 > b1) {
        return 1;
    }
    else {
        return -1;
    }
}

/**
 * Compares two char arrays passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to char* pointers (i.e. char**).
 * Returns the result of calling strcmp on them.
 */
int str_cmp(const void *a, const void *b) { 
    char* a1 = *(char**)a;
    char* b1 = *(char**)b;
    int i = strcmp(a1, b1);
    //free(a1);
    //free(b1);
    return i;
}

/**
 * Swaps the values in two pointers.
 *
 * Casts the void pointers to character types and works with them as char
 * pointers for the remainder of the function.
 * Swaps one byte at a time, until all 'size' bytes have been swapped.
 * For example, if ints are passed in, size will be 4. Therefore, this function
 * swaps 4 bytes in a and b character pointers.
 */
static void swap(void *a, void *b, size_t size) {
    char* a1 = (char*)a;
    char* b1 = (char*)b;
    while(size != 0) {
        char temp = *a1;
        *a1 = *b1;
        *b1 = temp;
        a1++;
        b1++;
        size--;
    }
}

/**
 * Partitions array around a pivot, utilizing the swap function.
 * Each time the function runs, the pivot is placed into the correct index of
 * the array in sorted order. All elements less than the pivot should
 * be to its left, and all elements greater than or equal to the pivot should be
 * to its right.
 * The function pointer is dereferenced when it is used.
 * Indexing into void *array does not work. All work must be performed with
 * pointer arithmetic.
 */
static int lomuto(void *array, int left, int right, size_t elem_sz, int (*comp) (const void*, const void*)) {
    char* cur = (char*)array;    
    char *pivot = (cur + right*elem_sz);
    int i = (left - 1); 
        for (int j = left; j <= (right - 1); j++) { 
            if (comp((cur + j*elem_sz), pivot) <= 0) { 
            i++;
            swap((cur+i*elem_sz), (cur+j*elem_sz), elem_sz); 
            } 
        }
    swap((cur+(i+1)*elem_sz), (cur+right*elem_sz), elem_sz);
    //free(cur);
    return (i + 1);
}

/**
 * Sorts with lomuto partitioning, with recursive calls on each side of the
 * pivot.
 * This is the function that does the work, since it takes in both left and
 * right index values.
 */
static void quicksort_helper(void *array, int left, int right, size_t elem_sz, int (*comp) (const void*, const void*)) {
    // TODO
    if (left < right) {
        int part = lomuto(array, left, right, elem_sz, comp);
        quicksort_helper(array, left, (part-1), elem_sz, comp);
        quicksort_helper(array, (part+1), right, elem_sz, comp);
    }
}

/**
 * Quicksort function exposed to the user.
 * Calls quicksort_helper with left = 0 and right = len - 1.
 */
void quicksort(void *array, size_t len, size_t elem_sz,
               int (*comp) (const void*, const void*)) {
    // TODO
    quicksort_helper(array, 0, (len-1), elem_sz, comp);
}
