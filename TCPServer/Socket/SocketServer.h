#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

#include <sys/wait.h>

#include <unistd.h>
#include <errno.h>

#include <fcntl.h>
#include <err.h>

#include <pthread.h>
#include <time.h>
#include <net/if.h>

#include <ifaddrs.h>

#define LISTEN_PORT 30001

int m_listenSocket = -1;
int m_bindSocket = -1;

int tcp_send(int sock_fd ,char * sendBuffer,unsigned int sendLen);

int detach_thread_create(pthread_t * thread,void * start_routine,void * args)
{
    pthread_attr_t attr;
    pthread_t thread_t;

    int nRet = 0;
    if(thread == NULL){
        thread = &thread_t;
    }

    if(pthread_attr_init(&attr)){
        printf("pthread_attr_init fail!\n");
        return -1;
    }

    if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)){
        printf("pthread_attr_setdetachstate Fail!\n");
        pthread_attr_destroy(&attr);
        return -1;
    }

    nRet = pthread_create(&thread_t,&attr,start_routine,args);
    if(nRet < 0){
        printf("pthread_create Fail!\n");
        pthread_attr_destroy(&attr);
        return -1;
    }

    nRet = pthread_detach(&thread_t);
}

//非阻塞套接字接受
int tcp_receive(int sock_fd,char * pBuffer,unsigned int nSize)
{
    if((pBuffer == NULL) || (nSize <= 0)){
        return -1;
    }

    if(sock_fd < 0){
        return -1;
    }


    int ret = 0;
    fd_set read_set;

    struct timeval tmval;
    memset(&tmval,0,sizeof(struct timeval));

    int recvedSize = 0;

    while(recvedSize < nSize){
        FD_ZERO(&read_set);
        FD_SET(sock_fd,&read_set);

        tmval.tv_sec = 10;
        tmval.tv_usec = 0;

        ret = select(sock_fd + 1,&read_set,NULL,NULL,&tmval);
        if(ret == 0){
            //超时
            //printf("超时\n");
            continue;
        }
        if(ret == -1){
            return -2;
        }

        if(FD_ISSET(sock_fd,&read_set)){
            ret = recv(sock_fd,pBuffer + recvedSize,nSize - recvedSize,0);
            if(ret < 0 && (errno == EAGAIN || errno == EINTR)){
                continue;
            }
            if(ret <= 0){
                return -1;
            }
            //printf("ret:%d",ret);
            recvedSize += ret;
        }
    }

    return recvedSize;
}

int tcp_send(int sock_fd ,char * sendBuffer,unsigned int sendLen)
{
    if((sendBuffer == NULL) || (sendLen <= 0)){
        return -1;
    }

    if(sock_fd < 0){
        return -1;
    }

    fd_set write_set;
    struct timeval tmval;

    memset(&tmval,0,sizeof(tmval));

    int nRet = 0;

    int sendedSize = 0;

    while(sendedSize < sendLen)
    {
        FD_ZERO(&write_set);
        FD_SET(sock_fd,&write_set);

        tmval.tv_sec = 3;
        tmval.tv_usec = 0;

        nRet = select(sock_fd + 1,&write_set,NULL,NULL,&tmval);
        if(nRet == 0){
            continue;
        }
        if((nRet < 0) && (errno == EINTR)){
            continue;
        }
        if(nRet < 0){
            return -1;
        }

        if(FD_ISSET(sock_fd,&write_set)){
            while(1){
                nRet = send(sock_fd,sendBuffer + sendedSize, sendLen - sendedSize,0);
                if(nRet < 0 && errno == EINTR){
                    //重新发送
                    continue;
                }
                if(nRet > 0){
                    sendLen -= nRet;
                    sendedSize += nRet;
                }
                break;
            }
        }
    }

    return sendedSize;
}

int set_socket_params(int sockfd)
{
    int flags1 = fcntl(sockfd,F_GETFL,0);
    fcntl(sockfd,F_SETFL,flags1 &(~O_NONBLOCK));

    struct timeval tmval;
    tmval.tv_sec = 5;
    tmval.tv_usec = 0;

    setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,&tmval,sizeof(struct timeval));

    return 0;
}

int client_socket_process()
{
    fd_set read_set;
    struct timeval tmval;

    tmval.tv_sec = 20;
    tmval.tv_usec = 0;

    set_socket_params(m_bindSocket);

    FD_ZERO(&read_set);
    FD_SET(m_bindSocket,&read_set);

    int wait_count = 0;

    while(wait_count < 10){
        int ret = select(m_bindSocket + 1,&read_set,NULL,NULL,&tmval);
        if(ret == -1){
            return -1;
        }
        if(ret == 0){
            wait_count++;
        }

        if(FD_ISSET(m_bindSocket,&read_set)){
            //printf("接受数据。。。。。\n");

            int size = 1;
            char * buffer = (char *)malloc(sizeof(char) * size);
            tcp_receive(m_bindSocket,buffer,size);

            for(int i=0;i<size;i++){
                printf("%c",buffer[i]);
            }

            free(buffer);

            char str[] = {'H','E','L','L','O','\t','W','O','R','L','D','\r','\n'};
            //tcp_send(m_bindSocket,str,13);
        }
    }

    char str[] = {'H','E','L','L','O','\t','W','O','R','L','D','\r','\n'};
    tcp_send(m_bindSocket,str,13);

    return 0;
}

void accept_process()
{
    struct sockaddr_in peerIpAddr;
    socklen_t size;

    pid_t pid;

    while(1){
        size = sizeof(struct sockaddr_in);
        m_bindSocket = accept(m_listenSocket,(struct sockaddr *)&peerIpAddr,&size);
        if(m_listenSocket > 0){
            printf("连接成功!\n");
            client_socket_process();
        }

        //close(m_listenSocket);
    }
}

void start_tcp_server()
{
    m_listenSocket = socket(AF_INET,SOCK_STREAM,0);
    if(m_listenSocket < 0){
        close(m_listenSocket);
        m_listenSocket = -1;
        return;
    }

    struct sockaddr_in client_addr;

    bzero(&client_addr,sizeof(struct sockaddr_in));

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(LISTEN_PORT);
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);

    int opt = 1;
    setsockopt(m_listenSocket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

    int nRet = bind(m_listenSocket,(struct sockaddr *)&client_addr,sizeof(struct sockaddr_in));
    if(nRet < 0){
        close(m_listenSocket);
        m_listenSocket = -1;
        return;
    }

    nRet = listen(m_listenSocket,10);
    if(nRet < 0){
        close(m_listenSocket);
        m_listenSocket = -1;
        return;
    }

    accept_process();
}


