#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "common.h"

int getSuffix(char input_id[]) {
    int numeric_part;
    if(sscanf(input_id + 2, "%d", &numeric_part) == 1) {
        return numeric_part;
    } else {
        return -1;
    }
}

int isFacultyEmpty(struct Faculty faculty) {
    return (memcmp(&faculty, &(struct Faculty){0}, sizeof(struct Faculty)) == 0);
}

int isStudentEmpty(struct Student student) {
    return (memcmp(&student, &(struct Student){0}, sizeof(struct Student)) == 0);
}

int isCourseEmpty(struct Course course) {
    return (memcmp(&course, &(struct Course){0}, sizeof(struct Course)) == 0);
}