#ifndef NETWORK_EPOLL_CONTROL
#define NETWORK_EPOLL_CONTROL

#ifndef MAXEVENTS
#define MAXEVENTS 50
#endif

#ifndef MAXBUFFER
#define MAXBUFFER 1024
#endif

#ifndef FDSIZE
#define FDSIZE 1000
#endif

void AddEvent(int epollfd, int fd, int state);
void DeleteEvent(int epollfd, int fd, int state);
void ModifyEvent(int epollfd, int fd, int state);
void EpollCycle(int socketfd, int initfd, void (*handle_events)(int, struct epoll_event*, int, int));

#endif // NETWORK_EPOLL_CONTROL