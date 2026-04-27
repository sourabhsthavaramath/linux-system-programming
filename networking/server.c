/************************************************
 * PROGRAM       : server.c
 *
 * FUNCTIONALITY :
 * TCP server that communicates with a client
 * using sockets and pthreads.
 *
 * DESCRIPTION   :
 * - Creates socket
 * - Binds to localhost and port
 * - Listens and accepts client
 * - Uses threads for send/receive
 *
 * SYSTEM CALLS  :
 * socket(), bind(), listen(), accept(),
 * read(), write(), close()
 *
 * AUTHOR        : Sourabh B Sthavaramath
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9734
#define BUF_SIZE 100

int client_sockfd;

void *sender(void *arg)
{
    char buffer[BUF_SIZE];

    while (1)
    {
        printf("Enter message to client: ");
        fgets(buffer, BUF_SIZE, stdin);

        write(client_sockfd, buffer, strlen(buffer));
    }
}

void *receiver(void *arg)
{
    char buffer[BUF_SIZE];

    while (1)
    {
        int ret = read(client_sockfd, buffer, BUF_SIZE);
        if (ret <= 0)
        {
            printf("Client disconnected\n");
            break;
        }

        buffer[ret] = '\0';
        printf("Client: %s", buffer);
    }
}

int main()
{
    int server_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    pthread_t t1, t2;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(server_sockfd, 5);
    printf("Server waiting on port %d...\n", PORT);

    client_len = sizeof(client_addr);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);

    if (client_sockfd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    pthread_create(&t1, NULL, sender, NULL);
    pthread_create(&t2, NULL, receiver, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    close(client_sockfd);
    close(server_sockfd);

    return 0;
}