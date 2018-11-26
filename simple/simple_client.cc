#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct sockaddr_in client_addr;

void* f(void* p) {
    int i = *(int *)p;

    int socketfd;
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("socket error: index %d\n", i);
    }

    if(connect(socketfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        printf("connect error: index %d\n", i);
    }

    printf("socketfd: %d, index: %d\n", socketfd, i);

    while(true) {
        sleep(1);
    }

    return NULL;
}

int main() {
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(1234);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    const int thread_num = 15;
    pthread_t pid[thread_num];
    int index[thread_num];
    for(int i = 0; i < thread_num; ++i) {
        index[i] = i;
        pthread_create(&pid[i], NULL, &f, (void *)&index[i]);
    }

    for(int i = 0; i < thread_num; ++i) {
        int* ret;
        pthread_join(pid[i], (void **)&(ret));
    }

    return 0;
}
