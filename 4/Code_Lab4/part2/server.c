#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#define BACKLOG 100    
#define MAXDATALEN 256 
#define PORT 8078

typedef struct
{
    int port;
    char username[15];
} User;

char username[15];
User users[50] = {0};
int userLast = 0;
User groups[50][50] = {0};
int groupLast[50] = {0};
char buffer[MAXDATALEN];

int search(int port, User *list, int last)
{
    for (int i = 0; i < last; i++)
    {
        if (list[i].port == port)
            return i;
    }
    return -1;
}


void delete_list(int port, User *list, int *last)
{
    int ptr = search(port, list, *last);
    if (ptr == -1)
    {
        return;
    }

    for (int i = ptr; i < *last - 1; i++)
    {
        list[i] = list[i + 1];
    }
    (*last)--;
}

void display_list(const User *list, int last)
{
    printf("Current online users:\n");
    if (last== 0)
    {
        printf("No one is online\n");
        return;
    }

    for (int i = 0; i < last; i++)
    {
        printf("%s\t",list[i].username);
    }
    printf("\n\n");
}


void *server(void *arguments)
{
    User *args = arguments;

    char buffer[MAXDATALEN], uname[10];
    char *msg = (char *)malloc(MAXDATALEN);
    int my_port, x, y;
    int msglen;

    my_port = args->port; 
    strcpy(uname, args->username);

    while (true)
    {
        bzero(buffer, 256);
        y = recv(my_port, buffer, MAXDATALEN, 0);

        /* Client quits */
        if (!y || strncmp(buffer, "/quit", 5) == 0)
        {
            printf("!!! %s left the chat !!!\n\n",uname);

            delete_list(my_port, users, &userLast);
            for (int i = 0; i < 50; i++)
            {
                delete_list(my_port, groups[i], &groupLast[i]);
            }

            display_list(users, userLast);

            close(my_port);
            free(msg);

            break;
        }
        else if (strncmp(buffer, "join", 4) == 0)
        {
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strcpy(group_id_str, buffer + 6);
            int group_id = atoi(group_id_str);
            printf("!!! %d: %s joined group number %d !!!\n\n", my_port, uname, group_id);
            User *temp;
            temp = malloc(sizeof(User));
            temp->port = my_port;
            strcpy(temp->username, uname);
            groups[group_id][groupLast[group_id]++] = *temp;

        }

        else if (strncmp(buffer, "send", 4) == 0)
        {
            int nextSpace;

            for(int k = 5; k < strlen(buffer); k++)
                if(buffer[k] == ' ')
                {
                    nextSpace = k;
                    break;
                }
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strncpy(group_id_str, buffer + 6, nextSpace);
            int group_id = atoi(group_id_str);

            if (search(my_port, groups[group_id], groupLast[group_id]) == -1)
            {
                continue;
            }

            printf("%s %s\n", uname, buffer);

            strcpy(msg, uname);
            strcat(msg, " ");
            strcat(msg, buffer + nextSpace);
            msglen = strlen(msg);

            for (int i = 0; i < groupLast[group_id]; i++)
            {
                if (groups[group_id][i].port == my_port)
                    continue;
                send(groups[group_id][i].port, msg, msglen, 0);
            }

            bzero(msg, MAXDATALEN);
        }

        else if (strncmp(buffer, "leave", 5) == 0)
        {
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strcpy(group_id_str, buffer + 7);
            printf("%s\n", group_id_str);
            int group_id = atoi(group_id_str);
            printf("!!! %s left group number %d !!!\n\n", uname, group_id);

            delete_list(my_port, groups[group_id], &groupLast[group_id]);
        }
        display_list(users, userLast);
    }
    return 0;
}


int main(int argc, char *argv[])
{

    int sockfd, new_fd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int z;
    pthread_t thr;
    int yes = 1;

    printf("Starting Server\n");
   
    int temp = PORT;

    
    server_addr.sin_family = AF_INET;               
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(temp);
    int addlen = sizeof(struct sockaddr_in);    
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("error");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        printf("error");
        exit(1);
    }

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("failed\n"); 
        exit(1);
    }

    listen(sockfd, BACKLOG);
    printf("waiting for clients to join...\n");

            
    while (true)
    {
        new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addlen);

        bzero(username, 10);
        recv(new_fd, username, sizeof(username), 0);

        printf("%s JOINED CHAT \n\n",username);
        User *temp;
        temp = malloc(sizeof(User));
        temp->port = new_fd;
        strcpy(temp->username, username);
        users[userLast++] = *temp;

        User args;
        args.port = new_fd;
        strcpy(args.username, username);

        pthread_create(&thr, NULL, server, (void *)&args); 
        pthread_detach(thr);
    }

    close(sockfd);
}
