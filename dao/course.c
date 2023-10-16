#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#include "./course-dao.h"
#include "../model/course.h"
#include "../commons/common.h"

void generateCourseId(char* new_id);
void incrementCourseId(const char* input_courseid, char* new_courseid, int size);

int insertCourse(char faculty_id[], char name[], int total_seats, int credits) {
    char new_id[6];
    generateCourseId(new_id);
    struct Course course;
    bzero(&course, sizeof(course));
    strcpy(course.course_id, new_id);
    strcpy(course.name, name);
    course.total_seats = total_seats;
    course.filled_seats = 0;
    strcpy(course.faculty_id, faculty_id);
    course.isActivated = true;
    course.credits = credits;
    printf("id: %s \n", course.course_id);
    printf("name: %s \n", course.name);
    // Apply locking on whole file - i think this is not needed
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/course.txt", O_WRONLY | O_APPEND);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    printf("id: %s \n", course.course_id);
    if(write(fd, &course, sizeof(struct Course)) == -1) {
        perror("Course details could not be written to the file");
    }
    close(fd);
    char path[100];
    sprintf(path, "/Users/nishthapaul/iiitb/academia-portal/courses/%s.txt", course.course_id);
    fd = creat(path, 0744);
    close(fd);
    return getSuffix(course.course_id);
}

void generateCourseId(char* new_id) {
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course temp;
    int offset = lseek(fd, -1 * sizeof(struct Course), SEEK_END);
    printf("offset: %d \n", offset);
    // if file is empty, then it is the first student
    if (offset == -1) {
        sprintf(new_id, "CO001");
        printf("Inside gen: %s \n", new_id);
        return;
    }
    read(fd, &temp, sizeof(struct Course));
    close(fd);
    char last_id[6];
    strcpy(last_id, temp.course_id);
    incrementCourseId(last_id, new_id, sizeof(new_id));
}

void incrementCourseId(const char* input_courseid, char* new_courseid, int size) {
    char prefix[3];
    int numeric_part;
    sscanf(input_courseid, "%2s%d", prefix, &numeric_part);
    numeric_part++;
    snprintf(new_courseid, size, "%s%03d", prefix, numeric_part);
    printf("New course id: %s\n", new_courseid);
}

/* This method checks the validity of the format of course id */
bool isCourseIDValid(char course_id[]) {
    return (course_id[0] == 'C' && course_id[1] == 'O');
}

/* This method checks if the course is present in our data or not */
struct Course doesCourseExist(char course_id[]) {
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;
    while (read(fd, &course, sizeof(struct Course)) > 0) {
        printf("course id %s ", course.course_id);
        printf("faculty id %s \n", course.faculty_id);
        if (strcmp(course.course_id, course_id) == 0) {
            return course;
        }
    }
    memset(&course, 0, sizeof(struct Course));
    return course;
}

/* This method checks if the specified course is offered by the specified faculty */
struct Course isCourseOfferedByFaculty(char course_id[], char faculty_id[]) {
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;
    while (read(fd, &course, sizeof(struct Course)) > 0) {
        printf("course id %s ", course.course_id);
        printf("faculty id %s \n", course.faculty_id);
        if (strcmp(course.course_id, course_id) == 0 && strcmp(course.faculty_id, faculty_id) == 0) {
            return course;
        }
    }
    memset(&course, 0, sizeof(struct Course));
    return course;
}

struct Course* findCoursesByFacultyId(char* course_id, const char* faculty_id, int* num_matches) {
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}

    struct Course* course_array = NULL;
    *num_matches = 0;
    struct Course course;

    while (read(fd, &course, sizeof(struct Course)) > 0) {
        if (strcmp(course.faculty_id, faculty_id) == 0) {
            course_array = realloc(course_array, (*num_matches + 1) * sizeof(struct Course));
            if (course_array == NULL) {
                perror("Memory allocation error");
                close(fd);
                free(course_array);
                return NULL;
            }
            course_array[*num_matches] = course;
            (*num_matches)++;
        }
    }
    
    close(fd);
    return course_array;
}

struct Course updateCourseActivationStatus(char course_id[], bool isActive) {
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/course.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;

    int courseno = getSuffix(course_id);
    lseek(fd, (courseno - 1) * sizeof(struct Course), SEEK_SET);
    read(fd, &course, sizeof(struct Course));
    course.isActivated = isActive;
    lseek(fd, -1 * sizeof(struct Course), SEEK_CUR);
    write(fd, &course, sizeof(struct Course));

    close(fd);
    return course;
}

struct Course updateCourseTotalSeats(char course_id[], int seats) {
    int fd = open("/Users/nishthapaul/iiitb/academia-portal/data/course.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;

    int courseno = getSuffix(course_id);
    lseek(fd, (courseno - 1) * sizeof(struct Course), SEEK_SET);
    read(fd, &course, sizeof(struct Course));
    course.total_seats = seats;
    lseek(fd, -1 * sizeof(struct Course), SEEK_CUR);
    write(fd, &course, sizeof(struct Course));

    close(fd);
    return course;
}