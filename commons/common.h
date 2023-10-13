#ifndef COMMONS
#define COMMONS

#include "../model/faculty.h"
#include "../model/student.h"

int getSuffix(char input_id[]);
int isStudentEmpty(struct Student student);
int isFacultyEmpty(struct Faculty faculty);

#endif