#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#include "faculty-ops.h"
#include "../constants/menu.h"
#include "../constants/view-details.h"
#include "../model/course.h"
#include "../dao/course-dao.h"
#include "../commons/common.h"

int createCourse(int socket_fd, char faculty_id[]);

void handle_faculty_operations(int socket_fd, char faculty_id[]) {
    printf("handle_faculty_operations");
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    do {
        bzero(write_buffer, sizeof(write_buffer));

        strcpy(write_buffer, FACULTY_MENU);

        if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1)
            perror("Error while displaying faculty menu to the user");
        printf("faculty menu written \n");
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client");
        }
        int choice = atoi(read_buffer);
        printf("Choice: %d \n", choice);

        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        struct Course course;

        switch(choice) {
            case 1 : 
                printf("Add a new course \n");
                int course_no = createCourse(socket_fd, faculty_id);
                char course_id1[6];
                sprintf(course_id1, "CO%03d", course_no);
                strcat(write_buffer, "============= Course added successfully =============\n");
                strcat(write_buffer, "========== Course Id of the course is : ");
                strcat(write_buffer, course_id1);
                strcat(write_buffer, " ========\n");
                break;
            case 2 :
                printf("View offered courses \n");
                int num_matches = 0;
                struct Course* matching_courses = findCoursesByFacultyId(course_id1, faculty_id, &num_matches);

                if (matching_courses != NULL) {
                    if (num_matches > 0) {
                        sprintf(write_buffer, "========= Matching Courses for Faculty %s: ========\n= Course ID     Course Name                          =\n", faculty_id);
                        strcat(write_buffer, "=                                                    =\n");
                        for (int i = 0; i < num_matches; i++) {
                            char buffer[100];
                            sprintf(buffer, "= %s         %s\n", matching_courses[i].course_id, matching_courses[i].name);
                            strcat(write_buffer, buffer);
                        }
                        strcat(write_buffer, "=                                                    =\n");
                        strcat(write_buffer, "======================================================\n");
                    } else {
                        sprintf(write_buffer, "No matching courses found for Faculty %s.\n", faculty_id);
                    }
                    free(matching_courses);
                } else {
                    strcpy(write_buffer, "No course has been created by you.\n");
                }
                break;
            case 3 :
                printf("De-activate an offered course \n");
                strcpy(write_buffer, "Enter the id of the course: ");
                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                char course_id3[50];
                strcpy(course_id3, read_buffer);

                if (!isCourseIDValid(course_id3)) {
                    strcpy(write_buffer, "Format of Course ID is incorrect. Please try again. \n");
                } else {
                    struct Course course = isCourseOfferedByFaculty(course_id3, faculty_id);
                    if (isCourseEmpty(course)) {
                        strcpy(write_buffer, "This course is not offered by you. \nPlease try again with your offered course. \n");
                    } else {
                        struct Course updatedCourse = updateCourseActivationStatus(course.course_id, false);
                        // TODO: update students-course files too.
                        // update free seats in a course file
                        strcpy(write_buffer, "Course de-activated successfully \n");
                        strcat(write_buffer, "Updated Course details: ");
                        sprintf(write_buffer, COURSE_DETAILS, updatedCourse.course_id, updatedCourse.name, updatedCourse.credits, updatedCourse.total_seats, updatedCourse.filled_seats, updatedCourse.faculty_id, updatedCourse.isActivated ? "activated" : "de-activated");
                        strcat(write_buffer, "\n");
                    }
                }
                break;
            case 4 :
                printf("Activate an offered course \n");
                strcpy(write_buffer, "Enter the id of the course: ");
                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                char course_id4[50];
                strcpy(course_id4, read_buffer);

                if (!isCourseIDValid(course_id4)) {
                    strcpy(write_buffer, "Format of Course ID is incorrect. Please try again. \n");
                } else {
                    struct Course course = isCourseOfferedByFaculty(course_id3, faculty_id);
                    if (isCourseEmpty(course)) {
                        strcpy(write_buffer, "This course is not offered by you. \nPlease try again with your offered course. \n");
                    } else {
                        struct Course updatedCourse = updateCourseActivationStatus(course.course_id, true);
                        strcpy(write_buffer, "Course de-activated successfully \n");
                        strcat(write_buffer, "Updated Course details: ");
                        sprintf(write_buffer, COURSE_DETAILS, updatedCourse.course_id, updatedCourse.name, updatedCourse.credits, updatedCourse.total_seats, updatedCourse.filled_seats, updatedCourse.faculty_id, updatedCourse.isActivated ? "activated" : "de-activated");
                        strcat(write_buffer, "\n");
                    }
                }
                break;
            case 5:
                printf("View course details \n");
                strcpy(write_buffer, "Enter the id of the course: ");
                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                char course_id5[50];
                strcpy(course_id5, read_buffer);
                if (!isCourseIDValid(course_id5)) {
                    strcpy(write_buffer, "Format of Course ID is incorrect. Please try again. \n");
                } else {
                    struct Course course = doesCourseExist(course_id5);
                    if (isCourseEmpty(course)) {
                        strcpy(write_buffer, "This Course ID is invalid. \nPlease try again with a valid one. \n");
                    } else {
                        struct Course course = isCourseOfferedByFaculty(course_id5, faculty_id);
                        if (isCourseEmpty(course)) {
                            strcpy(write_buffer, "This course is not offered by you. \nPlease try again with your offered course. \n");
                        } else {
                            sprintf(write_buffer, COURSE_DETAILS, course.course_id, course.name, course.credits, course.total_seats, course.filled_seats, course.faculty_id, course.isActivated ? "activated" : "de-activated");
                            strcat(write_buffer, "\n");
                        }
                    }
                }
                break;
            case 6 :
                printf("Modify total seats in a course \n");
                strcpy(write_buffer, "Enter the id of the course: ");
                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                char course_id6[50];
                strcpy(course_id6, read_buffer);

                bzero(write_buffer, sizeof(write_buffer));
                bzero(read_buffer, sizeof(read_buffer));
                strcpy(write_buffer, "Enter the total number of seats of the course: ");
                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                int total_seats = atoi(read_buffer);

                if (!isCourseIDValid(course_id6)) {
                    strcpy(write_buffer, "Format of Course ID is incorrect. Please try again. \n");
                } else {
                    struct Course course = isCourseOfferedByFaculty(course_id6, faculty_id);
                    if (isCourseEmpty(course)) {
                        strcpy(write_buffer, "This course is not offered by you. \nPlease try again with your offered course. \n");
                    } else {
                        struct Course updatedCourse = updateCourseTotalSeats(course.course_id, total_seats);
                        // TODO: de-enroll latest enrolled students
                        strcpy(write_buffer, "Total seats in a course are updated successfully \n");
                        strcat(write_buffer, "Updated Course details: ");
                        sprintf(write_buffer, COURSE_DETAILS, updatedCourse.course_id, updatedCourse.name, updatedCourse.credits, updatedCourse.total_seats, updatedCourse.filled_seats, updatedCourse.faculty_id, updatedCourse.isActivated ? "activated" : "de-activated");
                        strcat(write_buffer, "\n");
                    }
                }
                break;
            default :
                bzero(write_buffer, sizeof(write_buffer));
                strcpy(write_buffer, "Invalid option was selected. Please try again. \n");
                break;
        }
        strcat(write_buffer, "Do you want to continue (yes/no)? : ");
        if(write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
            perror("Error while asking the client: to continue faculty flow");
        }
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client: to continue faculty flow");
        }
        printf("%s \n", read_buffer);
    } while (strcmp(read_buffer, "yes") == 0);
}

int createCourse(int socket_fd, char faculty_id[]) {
    char read_buffer[1000], write_buffer[1000];

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcpy(write_buffer, "Enter the name of the course: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char name[50];
    strcpy(name, read_buffer);

    strcpy(write_buffer, "Enter the number of seats in the course: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    int total_seats = atoi(read_buffer);

    strcpy(write_buffer, "Enter the number of credits in the course: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    int credits = atoi(read_buffer);

    return insertCourse(faculty_id, name, total_seats, credits);
}