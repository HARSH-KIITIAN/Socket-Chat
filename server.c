#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int server_socket, client_socket1, client_socket2;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_socket);
        exit(1);
    }

    // Listen
    if (listen(server_socket, 2) == -1) {
        perror("Listening failed");
        close(server_socket);
        exit(1);
    }

    printf("Server is waiting for a connection...\n");

    // Accept connection
    client_socket1 = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket1 == -1) {
        perror("Accepting connection failed");
        close(server_socket);
        exit(1);
    }

    printf("Connection established with client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Accept connection
    client_socket2 = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket2 == -1) {
        perror("Accepting connection failed");
        close(server_socket);
        exit(1);
    }

    printf("Connection established with client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Exchange messages
    while (strncmp(buffer, "No", 2) != 0) {
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket1, buffer, sizeof(buffer), 0);
        send(client_socket2, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket2, buffer, sizeof(buffer), 0);
        send(client_socket1, buffer, strlen(buffer), 0);
    }

    // Close connection
    printf("Connection closed by the client.\n");
    close(client_socket1);
    close(client_socket2);
    close(server_socket);

    return 0;
}