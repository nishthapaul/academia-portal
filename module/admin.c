#include "admin-ops.h"
#include <stdio.h>
#include "./../constants/menu.h"

void handle_admin_operations(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    bzero(write_buffer, sizeof(write_buffer));

    strcpy(write_buffer, ADMIN_MENU);

    bytes_sent = write(socket_fd, write_buffer, strlen(write_buffer));
    if (bytes_sent == -1)
        perror("Error while displaying admin menu to the user");
    else {
        bzero(read_buffer, sizeof(read_buffer));
        bytes_rcvd = read(socket_fd, read_buffer, sizeof(read_buffer));
        if (bytes_rcvd == -1) {
            perror("Error while reading from client");
        } else {
            int choice = atoi(reaxd_buffer);
            printf("Choice: %d \n", choice);
            switch(choice) {
                case 1 : 
                    printf("Add a student");
                    break;
                case 2 :
                    printf("View student details");
                    break;
                default :
                    bzero(write_buffer, sizeof(write_buffer));
                    strcpy(write_buffer, "Invalid option was selected !!!");
                    write(socket_fd, write_buffer, strlen(write_buffer));
                    break;
            }
        }
    }
}