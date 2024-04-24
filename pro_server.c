#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 1024
#define MAX_NAME_SIZE 32


// Structure to store client information
typedef struct {
    int sockfd;
    struct sockaddr_in addr;
    char name[MAX_NAME_SIZE]; // Added field for storing client's name
} ClientInfo;

ClientInfo clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to broadcast a message to all clients
void broadcast(char *msg, int sender_index) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < num_clients; i++) {
        if (i != sender_index) {
            send(clients[i].sockfd, msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

// Function to handle communication with a client
void *client_handler(void *arg) {
    int client_index = *((int *)arg);
    char msg[MAX_MSG_SIZE];
    int bytes_received;

     // Ask client for their name
    send(clients[client_index].sockfd, "Enter your name: ", strlen("Enter your name: "), 0);
    bytes_received = recv(clients[client_index].sockfd, clients[client_index].name, MAX_NAME_SIZE - 1, 0);
    if (bytes_received > 0) {
        clients[client_index].name[bytes_received] = '\0';
    }

    while ((bytes_received = recv(clients[client_index].sockfd, msg, MAX_MSG_SIZE - 1, 0)) > 0) {
        msg[bytes_received] = '\0';
        printf("%s: %s\n", clients[client_index].name, msg); // Display client's name along with the message
        broadcast(msg, client_index);
    }

    // Client disconnected
    pthread_mutex_lock(&mutex);
    printf("Client %s disconnected.\n", clients[client_index].name);
    close(clients[client_index].sockfd);
    for (int i = client_index; i < num_clients - 1; i++) {
        clients[i] = clients[i + 1];
    }
    num_clients--;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, port;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    pthread_t tid;

    // Check for port number provided as command line argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    port = atoi(argv[1]);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    printf("Server listening on port %d...\n", port);

    // Accept connections and handle clients
    while (1) {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error accepting connection");
            continue;
        }

        // Add client to the list
        pthread_mutex_lock(&mutex);
        if (num_clients < MAX_CLIENTS) {
            clients[num_clients].sockfd = newsockfd;
            clients[num_clients].addr = cli_addr;
            printf("Client %s:%d connected.\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            num_clients++;
            // Create thread to handle client communication
            int *client_index_ptr = malloc(sizeof(int));
            *client_index_ptr = num_clients - 1;
            if (pthread_create(&tid, NULL, client_handler, client_index_ptr) != 0) {
                perror("Error creating thread");
            }
        } else {
            printf("Maximum number of clients reached. Connection rejected.\n");
            close(newsockfd);
        }
        pthread_mutex_unlock(&mutex);
    }

    close(sockfd);
    return 0;
}
