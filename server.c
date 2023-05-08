#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char const *argv[]) {
    // Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }

    // Create socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Failed to create socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);  // Replace with desired server port number
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind to address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("Failed to bind to address");
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 1) == SOCKET_ERROR) {
        perror("Failed to listen for connections");
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Server name and integer value
    char server_name[] = "Server of John Q. Smith";  // Replace with your name
    srand(time(NULL));
    int server_num = rand() % 100 + 1;

    while (1) {
        // Accept incoming connection
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            perror("Failed to accept connection");
            closesocket(server_socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // Receive message from client
        char client_message[100];
        if (recv(client_socket, client_message, 100, 0) == SOCKET_ERROR) {
            perror("Failed to receive message from client");
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // Parse client message
        char client_name[50];
        int client_num;
        sscanf(client_message, "%[^;];%d", client_name, &client_num);

        // Check if client number is in range
        if (client_num < 1 || client_num > 100) {
            // If out of range, terminate
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return 0;
        }

        // Display client and server information
        printf("Client name: %s\n", client_name);
        printf("Server name: %s\n", server_name);
        printf("Client number: %d\n", client_num);
        printf("Server number: %d\n", server_num);
        printf("Sum: %d\n", client_num + server_num);

        // Send message back to client
        char message[100];
        sprintf(message, "%s;%d", server_name, server_num);
        if (send(client_socket, message, strlen(message), 0) == SOCKET_ERROR) {
            perror("Failed to send message to client");
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // Clean up
        closesocket(client_socket);
    }

    // Clean up
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
