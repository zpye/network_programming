#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/tcp.h>

int main() {
    int socketfd;
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %d %s\n", errno, strerror(errno));
        exit(1);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(1234);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        printf("bind error: %d %s\n", errno, strerror(errno));
        exit(1);
    } 

    if(listen(socketfd, 20) == -1) {
        printf("listen error: %d %s\n", errno, strerror(errno));
        exit(1);
    }

    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    int client_sockets[100];
    int client_sockets_index = 0;

    fd_set client_fdset;
    struct timeval tv;

    int timeout_count = 0;
    while(true) {
        tv.tv_sec = 10;
        tv.tv_usec = 0;

        FD_ZERO(&client_fdset);
        FD_SET(socketfd, &client_fdset);

        int ret = select(socketfd + 1, &client_fdset, NULL, NULL, &tv);
        if(ret < 0) {
            printf("select error: %d %s\n", errno, strerror(errno));
            break;
        } else if(ret == 0) {
            printf("select timeout %d\n", timeout_count);
            timeout_count += 1;
            if(timeout_count >= 5) {
                break;
            }
            continue;
        } else {
            int socket_client = accept(socketfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
            if(socket_client < 0) {
                printf("accept error: %d %s\n", errno, strerror(errno));
                continue;
            }

            client_sockets[client_sockets_index++] = socket_client;
            printf("add connection %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            sleep(3);
        }
    }

    printf("close connections and exit\n");
    for(int i = 0; i < 100; ++i) {
        if(client_sockets[i] != 0) {
            close(client_sockets[i]);
        }
    }
    close(socketfd);

    return 0;
}
