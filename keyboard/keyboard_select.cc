#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/select.h>
#include <sys/time.h>

int main() {
    int keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
    assert(keyboard > 0);
    
    while(true) {
        struct timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        fd_set readfd;
        FD_ZERO(&readfd);
        FD_SET(keyboard, &readfd);
        
        int ret = select(keyboard + 1, &readfd, NULL, NULL, &timeout);
        if(ret == -1) {
            perror("error in select");
        } else if(ret == 0) {
            printf("time out, use keyboard to input value\n");
        } else {
            if(FD_ISSET(keyboard, &readfd)) {
                char c;
                read(keyboard, &c, 1);

                if(c == '\n')
                    continue;
                if(c == 'q')
                    break;
                
                printf("keyboard input is: %c\n", c);
            }
        }
    }

    return 0;
}
