#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

int sockfd;

void out_addr(struct sockaddr_in addr)
{
    char ip[16];
    unsigned short port = ntohs(addr.sin_port);
    inet_ntop(AF_INET, &addr.sin_addr.s_addr, ip, sizeof(addr));
    printf("ip:%s(%u) connect...\n",ip, port);
}

void do_service(struct sockaddr_in addr)
{
    out_addr(addr);
    long t = time(0);
    char* s = ctime(&t);
    if(sendto(sockfd, s, strlen(s), 0,
            (struct sockaddr*)&addr,sizeof(addr))<0)
    {
        fprintf(stderr, "sendto:%s\n",strerror(errno));
        return;
    }
}

int main(int argc, char*argv[])
{

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        fprintf(stderr, "socket:%s\n",
                strerror(errno));
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr*)&addr,
                sizeof(addr)) < 0){
        fprintf(stderr, "bind:%s\n",strerror(errno));
        exit(1);
    }

    char buffer[512];
    memset(buffer, 0, 512);
    struct sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    while(1){
        if(recvfrom(sockfd, buffer, sizeof(buffer),0, (struct sockaddr*)&caddr, &len)<0){
            fprintf(stderr, "recvform:%s\n",
                    strerror(errno));
            continue;
        }
        do_service(caddr);
        out_addr(caddr);
    }


    return 0;
}


