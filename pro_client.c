#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024
#define MAX_NAME_SIZE 32


int main(int argc, char *argv[]) {
    int sockfd, port;
    struct sockaddr_in serv_addr;
    char msg[MAX_MSG_SIZE];
    char name[MAX_NAME_SIZE];
    int bytes_received;

    // Check for server address and port provided as command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [server_address] [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *server_address = argv[1];
    port = atoi(argv[2]);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_address);
    serv_addr.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }
    // Ask user for their name
    printf("Enter your name: ");
    fgets(name, MAX_NAME_SIZE, stdin);
    if (name[strlen(name) - 1] == '\n') {
        name[strlen(name) - 1] = '\0';
    }

    // Send name to server
    if (send(sockfd, name, strlen(name), 0) < 0) {
        perror("Error sending name to server");
        exit(EXIT_FAILURE);
    }

    // Send and receive messages
    while (1) {
        printf("Enter message: ");
        fgets(msg, MAX_MSG_SIZE, stdin);

        // Remove trailing newline character
        if (msg[strlen(msg) - 1] == '\n') {
            msg[strlen(msg) - 1] = '\0';
        }

        // Send message to server
        if (send(sockfd, msg, strlen(msg), 0) < 0) {
            perror("Error sending message");
            break;
        }

        // Receive message from server
        bytes_received = recv(sockfd, msg, MAX_MSG_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Error receiving message");
            break;
        } else if (bytes_received == 0) {
            printf("Server disconnected.\n");
            break;
        }

        msg[bytes_received] = '\0';
        //printf("Server: %s\n", msg);
    }

    close(sockfd);
    return 0;
}
