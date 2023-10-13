#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int getSuffix(char input_id[]) {
    int numeric_part;
    if(sscanf(input_id + 2, "%d", &numeric_part) == 1) {
        return numeric_part;
    } else {
        return -1;
    }
}