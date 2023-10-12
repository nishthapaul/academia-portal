#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "student-dao.h"
#include "../model/student.h"

void generateStudentId(char* new_id);
void generateStudentPassword(char* password);
void incrementRollNumber(const char* input_rollno, char* new_rollno, int size);

int insertStudent(char name[], int age, char email[]) {
    char new_id[6], password[50];
    generateStudentId(new_id);
    generateStudentPassword(password);
    printf("%s \n", new_id);
    printf("%s \n", password);
    struct Student student;
    bzero(&student, sizeof(student));
    strcpy(student.std_id, new_id);
    strcpy(student.name, name);
    student.age = age;
    strcpy(student.email, email);
    strcpy(student.password, password);
    student.no_of_courses_enrolled = 0;
    printf("id: %s \n", student.std_id);
    printf("name: %s \n", student.name);
    printf("age: %d \n", student.age);
    printf("email: %s \n", student.email);
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/student.txt", O_WRONLY | O_APPEND);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    if(write(fd, &student, sizeof(struct Student)) == -1) {
        perror("Student details could not be written to the file");
    }
    close(fd);
    int rollno;
    sscanf(student.std_id, "MT%d", &rollno);
    return rollno;
}

void generateStudentId(char* new_id) {
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/student.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    struct Student temp;
    int offset = lseek(fd, -1 * sizeof(struct Student), SEEK_END);
    printf("offset: %d \n", offset);
    // if file is empty, then it is the first student
    if (offset == -1) {
        sprintf(new_id, "MT001");
        printf("Inside gen: %s \n", new_id);
        return;
    }
    read(fd, &temp, sizeof(struct Student));
    close(fd);
    char last_id[6];
    strcpy(last_id, temp.std_id);
    incrementRollNumber(last_id, new_id, sizeof(new_id));
}

void generateStudentPassword(char* password) {
    sprintf(password, "password");
}

void incrementRollNumber(const char* input_rollno, char* new_rollno, int size) {
    char prefix[3];
    int numeric_part;
    sscanf(input_rollno, "%2s%d", prefix, &numeric_part);
    numeric_part++;
    snprintf(new_rollno, size, "%s%03d", prefix, numeric_part);
    printf("New roll: %s\n", new_rollno);
}