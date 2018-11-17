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
#define SRV_IP       "127.0.0.1"
#define MAX_DATA_LENGTH 100

char rx_buffer[MAX_DATA_LENGTH];
char tx_buffer[MAX_DATA_LENGTH];

int main(void) {
    int sock_handler;
    struct sockaddr_in addr_remote;
    int ret, rx_size;

    sock_handler = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_handler == -1) {
        printf("[SYSTEM] Error when init socket: %d\n", errno);
        exit(-1);
    }

    addr_remote.sin_family = AF_INET;
    addr_remote.sin_addr.s_addr = inet_addr(SRV_IP);
    addr_remote.sin_port = htons(SRV_PORT);
    bzero(&(addr_remote.sin_zero), 8);

    ret = connect(sock_handler, (struct sockaddr*) &addr_remote, sizeof(struct sockaddr));
    if(ret == -1)
    {
        printf("[SYSTEM] Connect Failed: %d\n", errno);
        exit(-1);
    }
    else
    {
        printf("[SYSTEM] Connect Established: %s : %d\n", SRV_IP, SRV_PORT);
    }
    while(1)
    {
        recv(sock_handler, rx_buffer, MAX_DATA_LENGTH, 0);
        printf("[Server] %s\n", rx_buffer);
        printf("== Input Your Message: ==\n");
        scanf(" %[^\n]s", tx_buffer);
        send(sock_handler, tx_buffer, MAX_DATA_LENGTH, 0);
        memset(tx_buffer, 0, MAX_DATA_LENGTH);
    }
    return 0;
}
