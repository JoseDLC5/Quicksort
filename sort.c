/*******************************************************************************
 * Name        : sort.c
 * Author      : Breanna Shinn & Jose de la Cruz
 * Date        : 2/26/2021
 * Description : Uses quicksort to sort a file of either ints, doubles, or
 *               strings.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

typedef enum {
    STRING,
    INT,
    DOUBLE
} elem_t;

/**
 * Reads data from filename into an already allocated 2D array of chars.
 * Exits the entire program if the file cannot be opened.
 */
size_t read_data(char *filename, char **data) {
	// Open the file.
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: Cannot open '%s'. %s.\n", filename,
				strerror(errno));
		free(data);
		exit(EXIT_FAILURE);
	}

	// Read in the data.
	size_t index = 0;
	char str[MAX_STRLEN + 2];
	char *eoln;
	while (fgets(str, MAX_STRLEN + 2, fp) != NULL) {
		eoln = strchr(str, '\n');
		if (eoln == NULL) {
			str[MAX_STRLEN] = '\0';
		} else {
			*eoln = '\0';
		}
		// Ignore blank lines.
		if (strlen(str) != 0) {
			data[index] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char));
			strcpy(data[index++], str);
		}
	}

	// Close the file before returning from the function.
	fclose(fp);

	return index;
}
// Usage message
char* usage = "Usage: ./sort [-i|-d] filename\n   -i: Specifies the file contains ints.\n   -d: Specifies the file contains doubles.\n   filename: The file to sort.\n   No flags defaults to sorting strings.\n";

/**
 * Basic structure of sort.c:
 *
 * Parses args with getopt.
 * Opens input file for reading.
 * Allocates space in a char** for at least MAX_ELEMENTS strings to be stored,
 * where MAX_ELEMENTS is 1024.
 * Reads in the file
 * - For each line, allocates space in each index of the char** to store the
 *   line.
 * Closes the file, after reading in all the lines.
 * Calls quicksort based on type (int, double, string) supplied on the command
 * line.
 * Frees all data.
 * Ensures there are no memory leaks with valgrind. 
 */
int main(int argc, char **argv) {

	int iFlag = 0;
	int dFlag = 0;
	int sFlag = 0;
	int c;
	opterr = 0;

	while ((c = getopt (argc,argv,":id")) != -1 )
	{
		switch (c)
		{
		case 'i':
			iFlag++;
			break;
		case 'd':
			dFlag++;
			break;
		case ':':
		// Case 1: No input arguments
			sFlag++;
			break;
		case '?':
		// Case 2: Invalid Flag & Case 7: Multiple Flags with Invalid Flag
			fprintf(stderr, "Error: Unknown option %c received.\n", optopt);
			printf("%s", usage);
			return EXIT_FAILURE;
		}
	}
	// Case 4: No filename
	if ((argc - optind) < 1) {
		fprintf(stderr, "Error:  No input file specified.\n");
		return EXIT_FAILURE;
	}
	// Case 5: Multiple filenames
	if ((argc - optind) > 1) {
		fprintf(stderr, "Error: Too many files specified.\n");
		return EXIT_FAILURE;
	}
	// Case 6: Multiple Valid Flags
	if ((iFlag + dFlag + sFlag) > 1) {
		fprintf(stderr, "Error: Too many flags specified.\n");
		return EXIT_FAILURE;
	}

	// read file into data aray and store size of array
	char **data = (char**)malloc(MAX_ELEMENTS*sizeof(char));
	size_t num_elem;
	num_elem = read_data(argv[optind], data);

	// If input is integer list
	if (iFlag == 1) {

		int *intData = (int*)malloc(num_elem*sizeof(int));
		for (size_t i = 0; i < num_elem; i++)
		{
			*(intData+i) = atoi(*(data+i));
		}
		quicksort(intData, num_elem, sizeof(int), int_cmp);
		
		for (size_t i = 0; i < num_elem; i++)
		{
			printf( "%i\n", *(intData+i) );
			free(*(data + i));
		}
		free(intData);
	}
	// If input is double list 
	else if (dFlag == 1) {
		double *dblData = (double*)malloc(num_elem*sizeof(double));
		for (size_t i = 0; i < num_elem; i++)
		{
			*(dblData+i) = atof(*(data+i));
		}
		quicksort(dblData, num_elem, sizeof(double), dbl_cmp);
		
		for (size_t i = 0; i < num_elem; i++)
		{
			printf( "%f\n", *(dblData+i) );
			free(*(data + i));
		}
		free(dblData);
	}
	// If input is string list
	else {
		quicksort(data, num_elem, sizeof(char*), str_cmp);
		//printf("%ld\n", size);
		for (size_t i = 0; i < num_elem; i++)
		{
			printf( "%s\n", *(data+i) );
			free(*(data + i));
		}
	}

	free(data);
	return 0;
}