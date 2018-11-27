#include "socket_common.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int SocketBind(const char* ip, int port) {
    int socketfd = -1;
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("create socket error");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    if(bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind error");
        exit(1);
    }

    return socketfd;
}

int SocketConnect(const char* ip, int port) {
    int socketfd = -1;
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &client_addr.sin_addr);
    
    if(connect(socketfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        perror("connect error");
        exit(1);
    }

    return socketfd;
}

int Listen(int socketfd, int n) {
    return listen(socketfd, n);
}

int Accept(int epollfd, int socketfd) {
    int clientfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    if((clientfd = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
        perror("accept error");
        return -1;
    }

    printf("accept a new connection: %s:%d\n", inet_ntoa(client_addr.sin_addr),
                                               ntohs(client_addr.sin_port));
    
    return clientfd;
}