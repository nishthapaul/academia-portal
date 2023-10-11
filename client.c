/*
======================================================================================
Name : client.c
Author : Nishtha Paul
======================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Replace with the server's IP address
#define SERVER_PORT 8080

void connection_handler(int socket_fd);

int main() {
    int client_socket_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    } else {
        printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);
        int client_port = getsockname(client_socket_fd, (struct sockaddr*)&client_addr, &addr_len);
        printf("Client Port = %d\n", ntohs(client_addr.sin_port));

        connection_handler(client_socket_fd);
    }
    // Close the client socket
    close(client_socket_fd);

    return 0;
}

void connection_handler(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];

    bzero(read_buffer, sizeof(read_buffer));
    bzero(write_buffer, sizeof(write_buffer));
    
    // Send data to the server
    printf("Enter a message to send to the server: ");
    fgets(read_buffer, sizeof(read_buffer), stdin);

    write(socket_fd, read_buffer, strlen(read_buffer));

    // Receive and display the response from the server
    read(socket_fd, write_buffer, sizeof(write_buffer));
    printf("Server Response: %s", write_buffer);
}
