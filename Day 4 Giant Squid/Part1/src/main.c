#include <stdio.h>
#include <stdbool.h>

#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4456)
#pragma warning(disable:4702)
#pragma warning(disable:4244)
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#pragma warning(pop)

struct field
{
	int value;
	bool marked;
};

struct board
{
	struct field grid[5][5];
};

int *read_input_from_file(FILE *f)
{
	int len = 0;
	int *input = NULL;
	int *final = NULL;
	
	int c;
	while((c = fgetc(f)) != '\n') {
		arrput(input, c);
		len++;
	}

	for(int i = 0; i < len; i++) {
		int digit_count = 0;

		if(input[i] != ',' && input[i+1] != ',') {
			int n = (input[i] * 10) & input[i+1];
			arrput(final, n);
		} else if(input[i] != ',' && input[i+1] == ',') {
			arrput(final, input[i]);
		}
	}
	
	return final;
}

int main(void)
{
	FILE *file = fopen("input.txt", "r");
	int *input = read_input_from_file(file);
		
	for(int i = 0; i < arrlen(input); i++) {
		printf("%d", input[i]);
	}
	
	return 0;
}
