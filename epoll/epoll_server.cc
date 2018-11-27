#include <unordered_map>

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
const int backlog = 10;

std::unordered_map< int, char* > bufs;

char* GetBufFromFd(int fd) {
    char* buf = nullptr;
    auto iter = bufs.find(fd);
    if(iter != bufs.end()) {
        buf = iter->second;
    } else {
        buf = new char[MAXBUFFER];
        bufs[fd] = buf;
    }

    return buf;
}

void ReleaseBufFromFd(int fd) {
    auto iter = bufs.find(fd);
    if(iter != bufs.end()) {
        char* buf = iter->second;
        delete[] buf;
        bufs.erase(iter);
    }
}

void Read(int epollfd, int fd) {
    char* buf = GetBufFromFd(fd);
    int nread = read(fd, buf, MAXBUFFER);
    if(nread == -1) {
        perror("read error");
        close(fd);
        DeleteEvent(epollfd, fd, EPOLLIN);
        ReleaseBufFromFd(fd);
    } else if(nread == 0) {
        fprintf(stderr, "client close\n");
        close(fd);
        DeleteEvent(epollfd, fd, EPOLLIN);
        ReleaseBufFromFd(fd);
    } else {
        printf("read message: %s", buf);
        ModifyEvent(epollfd, fd, EPOLLOUT);
    }
}

void Write(int epollfd, int fd) {
    char* buf = GetBufFromFd(fd);
    int nwrite = write(fd, buf, strlen(buf));
    if(nwrite == -1) {
        perror("write error");
        close(fd);
        DeleteEvent(epollfd, fd, EPOLLOUT);
        ReleaseBufFromFd(fd);
    } else {
        ModifyEvent(epollfd, fd, EPOLLIN);
    }

    memset(buf, 0, MAXBUFFER);
}

void HandleEvents(int epollfd, struct epoll_event* events, int num, int socketfd) {
    for(int i = 0; i < num; ++i) {
        int fd = events[i].data.fd;
        uint32_t evt = events[i].events;
        if((fd == socketfd) && (evt & EPOLLIN)) {
            // accpet new connection
            int clientfd = -1;
            if((clientfd = Accept(epollfd, socketfd)) >= 0) {
                AddEvent(epollfd, clientfd, EPOLLIN);
            }
        } else if(evt & EPOLLIN) {
            Read(epollfd, fd);
        } else if(evt & EPOLLOUT) {
            Write(epollfd, fd);
        }
    }
}

int main() {
    int socketfd = SocketBind(kIP_ADDRESS, kPORT);
    Listen(socketfd, backlog);
    EpollCycle(socketfd, socketfd, HandleEvents);
    return 0;
}