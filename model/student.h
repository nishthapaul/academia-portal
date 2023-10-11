#ifndef STUDENT_MODEL
#define STUDENT_MODEL

struct student {
    char std_id[5]; //MTxxx
    char name[50];
    int age;
    char email[50];
    char password[50];
    int no_of_courses_enrolled;
};

#endif