#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "admin-ops.h"
#include "../constants/menu.h"
#include "../model/student.h"
#include "../dao/student-dao.h"

void createStudent(int socket_fd);

void handle_admin_operations(int socket_fd) {
    printf("in handle_admin_operations \n");
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

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
    switch(choice) {
        case 1 : 
            printf("Add a student \n");
            createStudent(socket_fd);
            // return yes or no to continue
            printf("after adding a student \n");
            break;
        case 2 :
            printf("View student details \n");
            break;
        default :
            bzero(write_buffer, sizeof(write_buffer));
            strcpy(write_buffer, "Invalid option was selected !!!");
            write(socket_fd, write_buffer, strlen(write_buffer));
            break;
    }
}

void createStudent(int socket_fd) {
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

    insertStudent(name, age, email);
}