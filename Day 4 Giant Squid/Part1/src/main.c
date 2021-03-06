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

#pragma warning(disable:4706)

// debug build
//#define DEBUG
#define ROW_COUNT 5
#define ROW_WIDTH 5

struct field
{
	int value;
	bool marked;
};

struct board
{
	int board_number;
	bool bingo;
	int winning_number;
	struct field grid[ROW_COUNT][ROW_WIDTH];
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

	arrput(input, '\n');

	for(int i = 0; i < len; i++) {
		if(input[i] != ',' && input[i+1] != ',' && input[i+1] != '\n') { // two digits 
			int decimal_system_base = 10;
			int n = (input[i] * decimal_system_base) + input[i+1];
			arrput(final, n);
			i++;
		} else if(input[i] != ',' && input[i+1] == ',') { // single number
			arrput(final, input[i]);
		} else if(input[i] != ',' && input[i+1] == '\n') { // final case for last single number
			arrput(final, input[i]);
		}
	}
	
	return final;
}

bool is_number(char c)
{
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

	for(int y = 0; y < ROW_COUNT; y++) {
		for(int x = 0; x < 15; x++) { // TODO: WHY 15??? don't change or break, but change to name 
			int c = fgetc(f);
			int c2 = fgetc(f);
			fgetc(f); // skip the empty space between inputs

			if(is_number((char)c) && is_number((char)c2)) {
				int decimal_system_base = 10;
				arrput(input, (c - '0') * decimal_system_base + (c2 - '0'));
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

bool check_for_bingo(struct board *boards)
{
	bool found_bingo = false;
	int horizontal_marker_count = 0;
	int vertical_marker_count = 0;
	
	// check for horizontal bingo	
	for(int i = 0; i < arrlen(boards); i++) {
		for(int y = 0; y < ROW_COUNT; y++) {
			for(int x = 0; x < ROW_WIDTH; x++) {
				if(boards[i].grid[y][x].marked)
					horizontal_marker_count++;
			}
			if(horizontal_marker_count == 5) {
				boards[i].bingo = true;
				found_bingo = true;
			}
			horizontal_marker_count = 0;
		}
	}

	// check for vertical bingo
	for(int i = 0; i < arrlen(boards); i++) {
		for(int x = 0; x < ROW_WIDTH; x++) {
			for(int y = 0; y < ROW_COUNT; y++) {
				if(boards[i].grid[y][x].marked)
					vertical_marker_count++;
			}
			if(vertical_marker_count == 5) {
				boards[i].bingo = true;
				found_bingo = true;
			}
			vertical_marker_count = 0;
		}
	}

	return found_bingo;
}

void test_number(struct board *b, int num)
{
	for(int i = 0; i < arrlen(b); i++) {
		for(int y = 0; y < ROW_COUNT; y++) {
			for(int x = 0; x < ROW_WIDTH; x++) {
				if(b[i].grid[y][x].value == num) {
					b[i].grid[y][x].marked = true;
					if(check_for_bingo(b)) {
						b[i].winning_number = num;
					}
				}
			}
		}
	}
}

int calculate_score(struct board b)
{
	int unmarked_sum = 0;

	for(int y = 0; y < ROW_COUNT; y++) {
		for(int x = 0; x < ROW_COUNT; x++) {
			if(!b.grid[y][x].marked) {
				unmarked_sum += b.grid[y][x].value;
			}
		}	
	}

	return unmarked_sum * b.winning_number;
}

int main(void)
{
	FILE *file;
	
	#ifdef DEBUG
	fopen_s(&file, "debug.txt", "r");
	#else
	fopen_s(&file, "input.txt", "r");
	#endif
	
	int *numbers = read_bingo_numbers_from_file(file);
	int **raw_board_data = NULL;

	int len_raw_data_array = 0;
	while(arrput(raw_board_data, read_bingo_board(file))) {
		len_raw_data_array++;
	}
	
	struct board *boards = NULL;
	arrsetlen(boards, len_raw_data_array);

	for(int i = 0; i < arrlen(boards); i++) {
		for(int y = 0; y < ROW_COUNT; y++) {
			for(int x = 0; x < ROW_WIDTH; x++) {
				boards[i].grid[y][x].value = raw_board_data[i][(y*5)+x];
				boards[i].grid[y][x].marked = false;
				boards[i].board_number = i;
				boards[i].bingo = false;
			}
		}
	}

	// print input numbers
	for(int i = 0; i < arrlen(numbers); i ++) {
		printf("%d ", numbers[i]);
	}
	printf("\n\n");
	
	// print boards
	for(int i = 0; i < arrlen(boards); i++) {
		for(int y = 0; y < ROW_COUNT; y++) {
			for(int x = 0; x < ROW_WIDTH; x++) {
				printf("%d ", boards[i].grid[y][x].value);
			}
			printf("\n");
		}
		printf("\n");
	}

	for(int i = 0; i < arrlen(numbers); i++) {
		test_number(boards, numbers[i]);
		for(int j = 0; j < arrlen(boards); j++) {
			if(boards[j].bingo) {
				printf("Bingo on board %d\n", boards[j].board_number);
				int score = calculate_score(boards[j]);
				printf("%d\n", score);
				return 0;
			} 
		}							
	}
	return 0;
}
