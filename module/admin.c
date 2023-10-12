#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "admin-ops.h"
#include "../constants/menu.h"
#include "../constants/view-details.h"
#include "../model/student.h"
#include "../dao/student-dao.h"

int createStudent(int socket_fd);
struct Student updateStudent(int socket_fd);

void handle_admin_operations(int socket_fd) {
    printf("in handle_admin_operations \n");
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    do {
        bzero(write_buffer, sizeof(write_buffer));

        strcpy(write_buffer, ADMIN_MENU);

        if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1)
            perror("Error while displaying admin menu to the user");
        printf("admin menu written \n");
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client");
        }
        int choice = atoi(read_buffer);
        printf("Choice: %d \n", choice);

        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        switch(choice) {
            case 1 : 
                printf("Add a student \n");
                int rollno = createStudent(socket_fd);
                char student_id[6];
                sprintf(student_id, "MT%03d", rollno);
                strcat(write_buffer, "============= Student added successfully =============\n");
                strcat(write_buffer, "========== Login Id of the student is : ");
                strcat(write_buffer, student_id);
                strcat(write_buffer, " ========\n");
                break;
            case 2 :
                printf("View student details \n");
                strcat(write_buffer, "Enter the roll number of the student whose details \nyou want want to view: ");
                if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
                    perror("Error while asking the client to enter roll no to view student details");
                }
                if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
                    perror("Error while reading roll no of student from client");
                }
                bzero(write_buffer, sizeof(write_buffer));
                struct Student student = getStudentDetails(read_buffer);
                sprintf(write_buffer, STUDENT_DETAILS, student.std_id, student.name, student.age, student.email, student.password, student.no_of_courses_enrolled);
                strcat(write_buffer, "\n");
                break;
            case 3 :
                printf("Modify student details \n");
                struct Student updatedStudent = updateStudent(socket_fd);
                strcat(write_buffer, "Student details are updated successfully \n");
                sprintf(write_buffer, STUDENT_DETAILS, updatedStudent.std_id, updatedStudent.name, updatedStudent.age, updatedStudent.email, updatedStudent.password, updatedStudent.no_of_courses_enrolled);
                strcat(write_buffer, "\n");
                break;
            default :
                bzero(write_buffer, sizeof(write_buffer));
                strcpy(write_buffer, "Invalid option was selected. Please try again. \n");
                write(socket_fd, write_buffer, strlen(write_buffer));
                break;
        }
        strcat(write_buffer, "Do you want to continue (yes/no)? : ");
        if(write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
            perror("Error while asking the client: to continue admin flow");
        }
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client: to continue admin flow");
        }
        printf("%s \n", read_buffer);
    } while (strcmp(read_buffer, "yes") == 0);
}

int createStudent(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcpy(write_buffer, "Enter the name of the student: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char name[50];
    strcpy(name, read_buffer);

    strcpy(write_buffer, "Enter the age of the student: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    int age = atoi(read_buffer);

    strcpy(write_buffer, "Enter the email id of the student: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char email[50];
    strcpy(email, read_buffer);
    return insertStudent(name, age, email);
}

struct Student updateStudent(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcat(write_buffer, "Enter the roll number of the student whose details \nyou want want to update: ");
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter roll no to update student details");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading roll no of student from client");
    }
    int rollno;
    sscanf(read_buffer, "MT%03d", &rollno);


    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcat(write_buffer, UPDATE_STUDENT_MENU);
    strcat(write_buffer, "\n");
    strcat(write_buffer, "Choose the option to update student details: ");
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter which student details has to be updated");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading which student details has to be updated from client");
    }
    int choice = atoi(read_buffer);


    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcat(write_buffer, "Enter the value to be updated: ");
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter the value to be updated");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading the value to be updated from client");
    }

    struct Student updatedStudent;
    switch(choice) {
        case 1:
            printf("updating name \n");
            updatedStudent = updateStudentName(rollno, read_buffer);
            break;
        case 2:
            printf("updating age \n");
            updatedStudent = updateStudentAge(rollno, atoi(read_buffer));
            break;
        case 3:
            printf("updating email \n");
            updatedStudent = updateStudentEmail(rollno, read_buffer);
            break;
        default:
            printf("wrong choice\n");
            break;
    }
    return updatedStudent;
}