#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

//#define DEBUG

#ifndef DEBUG
#define NUMBER_INPUT_LINES 1000
#define NUMBER_DIGITS_PER_LINE 12
#else
#define NUMBER_INPUT_LINES 12
#define NUMBER_DIGITS_PER_LINE 5
#endif

#define OXYGEN_GENERATOR_RATING 1
#define CO2_SCRUBBER_RATING 0

int find_most_common_bit(char **input, int position, int priority)
{
    if(priority != 0 && priority != 1)
		exit(EXIT_FAILURE);
    
    int cnt_0 = 0;
    int cnt_1 = 0;
	
    for(int i = 0; i < arrlen(input); i++) {
		if(input[i][position] == '0') {
			cnt_0++;
		} else if(input[i][position] == '1') {
			cnt_1++;
		}
    }
    
    if(priority)
		return (cnt_0 > cnt_1) ?  0 : 1;
    else
		return (cnt_0 <= cnt_1) ?  0 : 1;
}

void discard_entries_with_bit(char **report, int bit, int position)
{
    for(int i = 0; i < arrlen(report); i++) {
		int foo = (int)report[i][position] - '0';
		int bar = bit;
		if(foo == bit) {
			printf("deleted: %s", report[i]);
			arrdel(report, i);
			i--;
		}
    }
}

int main(void)
{
#ifndef DEBUG
    FILE *file = fopen("input.txt", "r");
#else
    FILE *file = fopen("debug.txt", "r");
#endif
    
    char **diagnostic_report = NULL;
    int oxygen_gen_rating = 0;
    int co2_scrubber_rating = 0;
    int life_support_rating = 0;

    arrsetlen(diagnostic_report, NUMBER_INPUT_LINES);
	for(int i = 0; i < NUMBER_INPUT_LINES; i++) {
		diagnostic_report[i] = NULL;
		arrsetlen(diagnostic_report[i], NUMBER_DIGITS_PER_LINE+2);
		// +2 because fgets adds \n and \0 to end of lines.
		fgets(diagnostic_report[i], NUMBER_DIGITS_PER_LINE+2, file);
	}

    // find oxygen generator rating
    for(int i = 0; i < NUMBER_DIGITS_PER_LINE; i++) {
		int bit = find_most_common_bit(diagnostic_report, i, OXYGEN_GENERATOR_RATING);
		discard_entries_with_bit(diagnostic_report, !bit, i);
		//bit_criteria ^= bit << (NUMBER_DIGITS_PER_LINE-1 - i);
    }
	oxygen_gen_rating = strtol(diagnostic_report[0], NULL, 2);

	// Delete the report and read it again.
	// Pretty slow solution but the first thing I thougt about.
	for(int i = 0; i < arrlen(diagnostic_report); i++) {
		arrfree(diagnostic_report[i]);
		diagnostic_report[i] = NULL;
	}
	arrfree(diagnostic_report);
	diagnostic_report = NULL;
	rewind(file);

	arrsetlen(diagnostic_report, NUMBER_INPUT_LINES);
	for(int i = 0; i < NUMBER_INPUT_LINES; i++) {
		diagnostic_report[i] = NULL;
		arrsetlen(diagnostic_report[i], NUMBER_DIGITS_PER_LINE+2);
		// +2 because fgets adds \n and \0 to end of lines.
		fgets(diagnostic_report[i], NUMBER_DIGITS_PER_LINE+2, file);
    }

	// find the co2 scrubber rating
	for(int i = 0; i < NUMBER_DIGITS_PER_LINE; i++) {
		int bit = find_most_common_bit(diagnostic_report, i, CO2_SCRUBBER_RATING);
		if(arrlen(diagnostic_report) == 1) {
			break;
		} else {
			discard_entries_with_bit(diagnostic_report, !bit, i);
		}
    }
	co2_scrubber_rating = strtol(diagnostic_report[0], NULL, 2);
	
	life_support_rating = oxygen_gen_rating * co2_scrubber_rating;
	
	printf("Oxygen Generator Rating: %d\n", oxygen_gen_rating);
	printf("CO2 Scrubber Rating: %d\n", co2_scrubber_rating);
	printf("Life Support Rating: %d\n", life_support_rating);

	fclose(file);

    return 0;
}
