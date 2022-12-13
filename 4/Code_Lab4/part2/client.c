#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define PORT 8078           
#define MAXDATALEN 256

int sockfd;
int n;                     
struct sockaddr_in serv_addr;
char buffer[MAXDATALEN];
char buf[10];


void *chat_read(int sockfd)
{
    while (1)
    {
        n = recv(sockfd, buffer, MAXDATALEN - 1, 0);
        if (n == 0)
        {
            printf("\nSERVER IS OFF :(\n");
            exit(0);
        }

        if (n > 0)
        {
            printf("-> %s", buffer);
            bzero(buffer, MAXDATALEN);
        }
    }
}

void *chat_write(int sockfd)
{
    while (1)
    {
        fgets(buffer, MAXDATALEN - 1, stdin);

        n = send(sockfd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "/quit", 5) == 0)
            exit(0);

        bzero(buffer, MAXDATALEN);
    }
}


int main(int argc, char *argv[])
{

    pthread_t thr1, thr2;

    if (argc != 4)
    {
        printf("Enter server ip, port and your username in order :(.\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        printf("client socket error\n");
    else
        printf("socket created\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);


    strcpy(buf, argv[3]);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("client connect error\n");
        exit(0);
    }
    else
        printf("\rYOU JOINED AS %s\n", buf);

    send(sockfd, buf, strlen(buf), 0);

    pthread_create(&thr2, NULL, (void *)chat_write, (void *) (intptr_t) sockfd);
    pthread_create(&thr1, NULL, (void *)chat_read, (void *) (intptr_t) sockfd);

    pthread_join(thr2, NULL);
    pthread_join(thr1, NULL);
}

