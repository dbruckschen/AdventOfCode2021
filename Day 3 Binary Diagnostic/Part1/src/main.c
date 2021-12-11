#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#define DEBUG 

#ifndef DEBUG
#define INPUT_NUMBER_LINES 1000
#define INPUT_NUMBER_OF_BITS 12
#else
#define INPUT_NUMBER_LINES 12
#define INPUT_NUMBER_OF_BITS 5
#endif

#define REST_BITS (32-INPUT_NUMBER_OF_BITS)

int calc_most_common_bit(char input[][INPUT_NUMBER_OF_BITS+2], int position)
{
    int cnt_0 = 0;
    int cnt_1 = 0;
	
    for(int i = 0; i < INPUT_NUMBER_LINES; i++) {
	if(input[i][position] == '0') {
	    cnt_0++;
	} else if(input[i][position] == '1') {
	    cnt_1++;
	}
    }
    return (cnt_0 > cnt_1) ?  0 : 1;
}

int main(void)
{
    uint32_t gamma_rate = 0;
    uint32_t epsilon_rate = 0;
    uint32_t power_consumption = 0;
    
    char input_lines[INPUT_NUMBER_LINES][INPUT_NUMBER_OF_BITS+2];

    #ifndef DEBUG
    FILE *file = fopen("input.txt", "r");
    #else
    FILE *file = fopen("debug.txt", "r");
    #endif
    
    if(file) {
	for(int i = 0; i < INPUT_NUMBER_LINES; i++) {
	    fgets(input_lines[i], INPUT_NUMBER_OF_BITS+2, file);
	}
    }
    
    for(int i = 0; i < INPUT_NUMBER_OF_BITS; i++) {
	int bit = calc_most_common_bit(input_lines, i);
	gamma_rate ^= bit << (INPUT_NUMBER_OF_BITS-1 - i);
    }

    epsilon_rate = ((~gamma_rate) << REST_BITS) >> REST_BITS;
    power_consumption = gamma_rate * epsilon_rate;

    printf("gamma rate: %u, epsilon rate: %u, power_consumption: %u\n", gamma_rate, epsilon_rate, power_consumption);


    fclose(file);
    return 0;
}
