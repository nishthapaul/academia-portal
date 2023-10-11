/*
======================================================================================
Name : server.c
Author : Nishtha Paul
======================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CONNECTIONS 5

void *handle_client(void *client_socket_fd);

int main() {
    int server_socket_fd, client_socket_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;

    // Create socket
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket_fd, MAX_CONNECTIONS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a client connection
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket_fd == -1) {
            perror("Accept failed");
            continue;
        }
        printf("Accepted connection from %s: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket_fd) != 0) {
            perror("Thread creation failed");
            close(client_socket_fd);
        }
    }

    close(server_socket_fd);
    return 0;
}

void *handle_client(void *arg) {
    int client_socket_fd = *((int *)arg);
    char read_buffer[1000];
    int bytes_received;

    bzero(read_buffer, sizeof(read_buffer));

    // Receive data from the client
    bytes_received = read(client_socket_fd, read_buffer, sizeof(read_buffer));
    printf("bytes recvd %d \n", bytes_received);
    if (bytes_received <= 0) {
        printf("Error while reading from client");
    } else {
        // Send the received data back to the client.
        write(client_socket_fd, read_buffer, bytes_received);
    }

    close(client_socket_fd);
    pthread_exit(NULL);
}
