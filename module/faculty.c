#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#include "faculty-ops.h"
#include "../constants/menu.h"
#include "../model/course.h"
#include "../dao/course-dao.h"

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
                char course_id[6];
                sprintf(course_id, "CO%03d", course_no);
                strcat(write_buffer, "============= Course added successfully =============\n");
                strcat(write_buffer, "========== Course Id of the course is : ");
                strcat(write_buffer, course_id);
                strcat(write_buffer, " ========\n");
                break;
            case 2 :
                printf("View offered courses \n");
                break;
            case 3 :
                printf("De-activate an offered course \n");
                break;
            case 4 :
                printf("Activate an offered course \n");
                break;
            case 5:
                printf("View course details \n");
                break;
            case 6 :
                printf("Modify available seats in a course \n");
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

    // enter credits

    return insertCourse(faculty_id, name, total_seats);
}s