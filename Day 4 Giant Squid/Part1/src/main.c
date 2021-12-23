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

int *read_bingo_numbers_from_file(FILE *f)
{
	int len = 0;
	int *input = NULL;
	int *final = NULL;
	
	int c;
	while((c = fgetc(f)) != '\n') {
		if(c != ',') {
			arrput(input, c - '0');
		} else {
			arrput(input, c);
		}
		
		len++;
	}

	for(int i = 0; i < len; i++) {
		if(input[i] != ',' && input[i+1] != ',') {
			int n = (input[i] * 10) + input[i+1];
			arrput(final, n);
			i++;
		} else if(input[i] != ',' && input[i+1] == ',') {
			arrput(final, input[i]);
		}
	}
	
	return final;
}

bool is_number(char c) {
	return (c >= '0' && c <= '9') ? true : false;
}

int *read_bingo_board(FILE *f)
{
	int *input = NULL;
	// find start of bingo input
	int err;
	while((err = fgetc(f)) != '\n') {
		if(err == EOF) {
			return NULL;
		}
	}

	for(int y = 0; y < 5; y++) {
		for(int x = 0; x < 15; x++) {
			int c = fgetc(f);
			int c2 = fgetc(f);
			fgetc(f); // skip the whitespace between inputs

			if(is_number((char)c) && is_number((char)c2)) {
				arrput(input, (c - '0') * 10 + (c2 - '0'));
			} else if(is_number((char)c) && c2 == ' ') {
				//NOTE: This line will never be reached with the current input for board 1
				arrput(input, c - '0');
			} else if(c == ' ' && is_number((char)c2)) {
				arrput(input, c2 - '0');
			}
			x+=2;
		}
	}
	return input;
}

int main(void)
{
	FILE *file = fopen("input.txt", "r");
	int *input = read_bingo_numbers_from_file(file);
	int **boards = NULL;
	
	for(int i = 0; i < arrlen(input); i++) {
		printf("%d ", input[i]);
	}

	for(int i = 0; i < 10; i++) {
		arrput(boards, read_bingo_board(file));
	}
	//int *boards = read_bingo_board(file);
	//int *board2 = read_bingo_board(file);
	
	printf("\n-----------------------------\n");
	
	for(int i = 0; i < arrlen(boards); i++) {
		for(int k = 0; k < 25; k++) {
			if(!(k % 5)) {
				printf("\n");
			}
			printf("%d ", boards[i][k]);
		}
		printf("\n");
	}
	
	return 0;
}
