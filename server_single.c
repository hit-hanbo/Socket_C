#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>

#define SRV_PORT     23333
#define MAX_SRV_NUM  10
#define MAX_DATA_LENGTH 100

char rx_buffer[MAX_DATA_LENGTH];
char tx_buffer[MAX_DATA_LENGTH];

int main(void)
{
    int sock_handler, socked_handler;
    struct sockaddr_in addr_local, addr_remote;
    int ret, rx_size;

    sock_handler = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_handler == -1)
    {
        printf("Error when init socket: %d\n", errno);
        exit(-1);
    }

    addr_local.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_local.sin_port = htons(SRV_PORT);
    addr_local.sin_family = AF_INET;
    bzero(&(addr_local.sin_zero), 8);

    ret = bind(sock_handler, (struct sockaddr*) &addr_local, sizeof(struct sockaddr));
    if(ret == -1)
    {
        printf("Bind Error: %d\n", errno);
        exit(-1);
    }

    ret = listen(sock_handler, MAX_SRV_NUM);
    if(ret == -1)
    {
        printf("Error listening: %d\n", errno);
        exit(-1);
    }
    else
    {
        printf("Listening on port: %d\n", SRV_PORT);
    }
    while(1)
    {
        rx_size = sizeof(struct sockaddr_in);
        socked_handler = accept(sock_handler, (struct sockaddr*) &addr_remote, &rx_size);
        if(socked_handler == -1)
        {
            printf("Connection Error: %d\n", errno);
            continue;
        }
        else
        {
            printf("Connected: %s : %d\n", inet_ntoa(addr_remote.sin_addr), (int)addr_remote.sin_port);
            send(socked_handler, "Hello, You've Connected !\n", MAX_DATA_LENGTH, 0);
        }
        while(1)
        {
            recv(socked_handler, rx_buffer, MAX_DATA_LENGTH, 0);
            printf("[Client] %s\n", rx_buffer);
            strcpy(tx_buffer, "OjbK, I Recv !\n");
            send(socked_handler, tx_buffer, MAX_DATA_LENGTH, 0);
        }
    }
    return 0;
}
