#include "socketserver.h"

int m_ListenSocket = -1;
int m_BindSocket = -1;

int client_socket_process(struct sockaddr_in * lpddr)
{
    fd_set read_set;
    int wait_count = 0;

    struct timeval tmval;
    tmval.tv_sec = 20;
    tmval.tv_usec = 0;

    FD_ZERO(&read_set);
    FD_SET(m_BindSocket,&read_set);

    while(wait_count<10){
        int ret = select(m_BindSocket + 1,&read_set,NULL,NULL,&tmval);
        if(ret == -1){

        }
        else if(ret == 0){
            wait_count++;
        }
        else{
            if(FD_ISSET(m_BindSocket,&read_set)){
                //有数据
            }
        }
    }

    return 0;
}

void accept_process()
{
    struct sockaddr_in peerIpAddr;
    socklen_t size;

    while(1)
    {
        size = sizeof(struct sockaddr_in);
        m_BindSocket = accept(m_ListenSocket,(struct sockaddr *)&peerIpAddr,&size);
        if(m_BindSocket > 0){
            //链接成功
            printf("链接成功%d\n",m_BindSocket);
            //client_socket_process();
        }
    }
}

void start_tcp_server()
{
    m_ListenSocket = socket(AF_INET,SOCK_STREAM,0);
    if(m_ListenSocket < 0){
        puts("socket() Fail");
        close(m_ListenSocket);
        m_ListenSocket = -1;
    }

    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(struct sockaddr_in));

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(LISTEN_PORT);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int nRet = bind(m_ListenSocket,(struct sockaddr *)&client_addr,sizeof(client_addr));
    if(nRet < 0){
        puts("bind() Fail");
        close(m_ListenSocket);
        m_ListenSocket = -1;
    }

    nRet = listen(m_ListenSocket,10);
    if(nRet < 0){
        puts("listen() Fail");
        close(m_ListenSocket);
        m_ListenSocket = -1;
    }

    accept_process();
}
