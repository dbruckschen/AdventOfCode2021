#include <stdio.h>
#include <stdlib.h>

#define COUNT_FILE_ENTRIES 2000
int main(void)
{
    FILE *file = fopen("input.txt", "r");
    if(file) {
	int buffer[COUNT_FILE_ENTRIES];
	printf("file open...\n");

	for(int i = 0; i < COUNT_FILE_ENTRIES; i++) {
	    fscanf(file, "%d", &buffer[i]);
	}

	int increase_cnt = 0;

	for(int i = 0; i < COUNT_FILE_ENTRIES-3; i++) {
	    if((buffer[i] + buffer[i+1] + buffer[i+2]) < (buffer[i+1] + buffer[i+2] + buffer[i+3])) {
		increase_cnt++;
	    }
	}
	printf("the number of times a depth measurement increasesd: %d\n", increase_cnt);
    }

    fclose(file);
    return 0;
}
