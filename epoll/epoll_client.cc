#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include <sys/epoll.h>

#include "epoll_control.h"
#include "socket_common.h"

const char* kIP_ADDRESS = "127.0.0.1";
const int kPORT = 8888;

char buf[MAXBUFFER];

void Read(int epollfd, int fd, int socketfd) {
    int nread = read(fd, buf, MAXBUFFER);
    if(nread == -1) {
        perror("read error");
        close(fd);
    } else if(nread == 0) {
        fprintf(stderr, "server close\n");
        close(fd);
    } else {
        if(fd == STDIN_FILENO) {
            AddEvent(epollfd, socketfd, EPOLLOUT);
        } else {
            DeleteEvent(epollfd, socketfd, EPOLLIN);
            AddEvent(epollfd, STDOUT_FILENO, EPOLLOUT);
        }
    }
}

void Write(int epollfd, int fd, int socketfd) {
    int nwrite = write(fd, buf, strlen(buf));
    if(nwrite == -1) {
        perror("write error");
        close(fd);
    } else {
        if(fd == STDOUT_FILENO) {
            DeleteEvent(epollfd, fd, EPOLLOUT);
        } else {
            ModifyEvent(epollfd, fd, EPOLLIN);
        }
    }

    memset(buf, 0, MAXBUFFER);
}

void HandleEvents(int epollfd, struct epoll_event* events, int num, int socketfd) {
    for(int i = 0; i < num; ++i) {
        int fd = events[i].data.fd;
        uint32_t evt = events[i].events;
        if(evt & EPOLLIN) {
            Read(epollfd, fd, socketfd);
        } else if(evt & EPOLLOUT) {
            Write(epollfd, fd, socketfd);
        }
    }
}

int main() {
    int socketfd = SocketConnect(kIP_ADDRESS, kPORT);
    EpollCycle(socketfd, STDIN_FILENO, HandleEvents);
    return 0;
}