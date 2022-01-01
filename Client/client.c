#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "socket_error_handler.h"

#define PORT 8080

char str[1024];

int main(int argc, char **argv) {
    uint32_t port = PORT;
    if (argc == 2) {
        uint32_t t = atoi(argv[1]);
        if (t != 0) 
            port = t;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) error("Error: Socket error");
    printf("Socket: %d\n", sock);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(port);

    if(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)<=0) 
        error("Error: Invalid address/ Address not supported"); 

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        error("Error: Bad Connection");
    
    int welcome = read(sock, str, 1024);
    printf("Server says %s", str);
    
    while(1) {
        fgets(str, 1024, stdin);
        if (str[strlen(str)-1] == '\n') 
            str[strlen(str)-1] = 0;
        send(sock, str, strlen(str), 0);  
    }
    return 0;
}