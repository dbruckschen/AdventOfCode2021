#include <stdio.h>
#include <stdlib.h>

typedef int bool;

enum {
    false,
    true
};

#define COUNT_FILE_ENTRIES 1000
#define MAX_FILE_ENTRY_LEN 100
#define ARRAY_LEN(a) (sizeof(a))/(sizeof(a[0]))

void get_command_and_step(char str[], int len, char cmd[], int *step) {

    for(int i = 0; i < len; i++) {
	if(str[i] != ' ' && str[i] != '\n' && str[i] != '\0')
	    cmd[i] = str[i];
	else if(str[i] == ' ') {
	    *step = str[i+1] - '0';
	    break;
	}
    }
}

bool str_cmp(char *s1, char *s2) {
    int cnt = 0;

    while(*s1 == *s2 && *s1 != '\0' && *s2 != '\0' && *s1 != '\n' && *s2 != '\n') {
        cnt++;
        s1++;
        s2++;
    }

    if(cnt == 0)
        return false;

    if(*s1 == *s2)
        return true;
    else
        return false;
}

int main(void)
{
    int x = 0;
    int depth = 0;

    FILE *file = fopen("input.txt", "r");
    if(file) {
	char str[COUNT_FILE_ENTRIES][MAX_FILE_ENTRY_LEN];
	int max_chars = 100;
	for(int i = 0; i < COUNT_FILE_ENTRIES; i++) {
	    fgets(str[i], max_chars, file);

	    char cmd[MAX_FILE_ENTRY_LEN] = {0};
	    int step = 0;
	    get_command_and_step(str[i], MAX_FILE_ENTRY_LEN, cmd, &step);

	    if(str_cmp(cmd, "forward")) {
		x += step;
	    } else if(str_cmp(cmd, "up")) {
		depth -= step;
	    } else if(str_cmp(cmd, "down")) {
		depth += step;
	    }
	}

	printf("x: %d, depth: %d --> x*depth: %d\n", x, depth, x*depth);
    }
    return 0;
}
