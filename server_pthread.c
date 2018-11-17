#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<pthread.h>

#define SRV_PORT     23333
#define MAX_SRV_NUM  10
#define MAX_DATA_LENGTH 100

char rx_buffer[MAX_DATA_LENGTH];
char tx_buffer[MAX_DATA_LENGTH];

void* socket_recv_handler(void* sock_handler);

int main(void)
{
    int sock_handler, socked_handler;
    struct sockaddr_in addr_local, addr_remote;
    int ret, rx_size;
    pthread_t thread_id;

    sock_handler = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_handler == -1)
    {
        printf("[SYSTEM] Error when init socket: %d\n", errno);
        exit(-1);
    }

    addr_local.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_local.sin_port = htons(SRV_PORT);
    addr_local.sin_family = AF_INET;
    bzero(&(addr_local.sin_zero), 8);

    ret = bind(sock_handler, (struct sockaddr*) &addr_local, sizeof(struct sockaddr));
    if(ret == -1)
    {
        printf("[SYSTEM] Bind Error: %d\n", errno);
        exit(-1);
    }

    ret = listen(sock_handler, MAX_SRV_NUM);
    if(ret == -1)
    {
        printf("[SYSTEM] Error listening: %d\n", errno);
        exit(-1);
    }
    else
    {
        printf("[SYSTEM] Listening on port: %d\n", SRV_PORT);
    }

    while(1)
    {
        rx_size = sizeof(struct sockaddr_in);
        socked_handler = accept(sock_handler, (struct sockaddr*) &addr_remote, &rx_size);
        if(socked_handler == -1)
        {
            printf("[SYSTEM] Connection Error: %d\n", errno);
            continue;
        }
        else
        {
            printf("[SYSTEM] Connected: %s:%d\n", inet_ntoa(addr_remote.sin_addr), (int)addr_remote.sin_port);
            send(socked_handler, "Hello, You've Connected !\n", MAX_DATA_LENGTH, 0);
            pthread_create(&thread_id, NULL, socket_recv_handler, (void*) &socked_handler);
        }
    }
    return 0;
}


void* socket_recv_handler(void* sock_handler)
{
    int* socket_handler = (int* ) sock_handler;
    int ret;
    while(1)
    {
        ret = recv(*socket_handler, rx_buffer, MAX_DATA_LENGTH, 0);
        if(ret > 0)
        {
            printf("[Client] %s\n", rx_buffer);
            send(*socket_handler, "OK", MAX_DATA_LENGTH, 0);
        }
    }
}

