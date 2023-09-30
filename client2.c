#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(1);
    }

    printf("Connected to server %s:%s\n", argv[1], argv[2]);

    // Exchange messages
    while (strncmp(buffer, "No", 2) != 0) {
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Client1: %s", buffer);
        memset(buffer, 0, sizeof(buffer));
        printf("Enter your message: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    // Close connection
    printf("Closing the connection.\n");
    close(client_socket);

    return 0;
}