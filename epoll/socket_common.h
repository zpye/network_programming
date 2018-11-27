#ifndef NETWORK_SOCKET_COMMON
#define NETWORK_SOCKET_COMMON

int SocketBind(const char* ip, int port);
int SocketConnect(const char* ip, int port);
int Listen(int socketfd, int n);
int Accept(int epollfd, int socketfd);

#endif // NETWORK_SOCKET_COMMON