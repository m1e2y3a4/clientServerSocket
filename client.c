#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char const *argv[]) {
    // Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
        perror("Failed to initialize Winsock");
        exit(EXIT_FAILURE);
    }

    // Create socket
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        perror("Failed to create socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Get user input
    int num;
    printf("Enter an integer between 1 and 100: ");
    scanf("%d", &num);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);  // Replace with actual server port number
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with actual server address

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("Failed to connect to server");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Send message to server
    char name[] = "Client of John Q. Smith";  // Replace with your name
    char message[100];
    sprintf(message, "%s;%d", name, num);
    if (send(client_socket, message, strlen(message), 0) == SOCKET_ERROR) {
        perror("Failed to send message to server");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Receive message from server
    char server_message[100];
    if (recv(client_socket, server_message, 100, 0) == SOCKET_ERROR) {
        perror("Failed to receive message from server");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Parse server message
    char server_name[50];
    int server_num;
    sscanf(server_message, "%[^;];%d", server_name, &server_num);

    // Compute sum and display results
    printf("Server name: %s\n", server_name);
    printf("Server number: %d\n", server_num);
    printf("Client number: %d\n", num);
    printf("Sum: %d\n", server_num + num);

    // Clean up
    closesocket(client_socket);
    WSACleanup();
    return 0;
}
