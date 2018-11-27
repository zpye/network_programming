#include "epoll_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

void AddEvent(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void DeleteEvent(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void ModifyEvent(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void EpollCycle(int socketfd, int initfd, void (*handle_events)(int, struct epoll_event*, int, int)) {
    int epollfd = -1;
    if((epollfd = epoll_create(FDSIZE)) < 0) {
        perror("epoll create error");
        exit(1);
    }

    AddEvent(epollfd, initfd, EPOLLIN);

    int ret = -1;
    struct epoll_event events[MAXEVENTS];
    while(true) {
        ret = epoll_wait(epollfd, events, MAXEVENTS, -1);
        handle_events(epollfd, events, ret, socketfd);
    }
    close(epollfd);
}