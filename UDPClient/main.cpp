#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>

int sockfd;
int main(int argc, char*argv[])
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        fprintf(stderr, "socket:%s\n",strerror(errno));
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1",
            &addr.sin_addr.s_addr);


    char buffer[512];
    sendto(sockfd, buffer, sizeof(buffer),
            0, (struct sockaddr*)&addr, sizeof(addr));

    memset(buffer, 0, sizeof(buffer));
    ssize_t n;
    if((n=recv(sockfd, buffer, sizeof(buffer), 0))< 0)
    {
        fprintf(stderr, "recv:%s\n", strerror(errno));
        close(sockfd);
        exit(1);
    }else
    {
        if(write(STDOUT_FILENO, buffer, n)!=n)
        {
            fprintf(stderr, "write:%s\n",
                    strerror(errno));
            close(sockfd);
            exit(1);
        }
    }
    close(sockfd);
}




