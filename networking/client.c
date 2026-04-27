/************************************************
 * PROGRAM       : client.c
 *
 * FUNCTIONALITY :
 * TCP client that communicates with server
 * using sockets and pthreads.
 *
 * DESCRIPTION   :
 * - Connects to server
 * - Sends and receives messages
 * - Uses threads for parallel communication
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

int sockfd;

void *sender(void *arg)
{
    char buffer[BUF_SIZE];

    while (1)
    {
        printf("Enter message to server: ");
        fgets(buffer, BUF_SIZE, stdin);

        write(sockfd, buffer, strlen(buffer));
    }
}

void *receiver(void *arg)
{
    char buffer[BUF_SIZE];

    while (1)
    {
        int ret = read(sockfd, buffer, BUF_SIZE);
        if (ret <= 0)
        {
            printf("Server disconnected\n");
            break;
        }

        buffer[ret] = '\0';
        printf("Server: %s", buffer);
    }
}

int main()
{
    struct sockaddr_in server_addr;
    pthread_t t1, t2;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    pthread_create(&t1, NULL, sender, NULL);
    pthread_create(&t2, NULL, receiver, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    close(sockfd);
    return 0;
}