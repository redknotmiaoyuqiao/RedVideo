#ifndef CAMERACLIENT_H
#define CAMERACLIENT_H

#include "h264decode.h"
#include "cctcpdatadefine.h"

typedef void (*UpdateVideo2GUI_Callback)(RGBDataDefine * data, unsigned long userData);

class CameraClient
{
public:
    CameraClient();
    ~CameraClient();

    bool startTCPSocketConnection(CC_NetConnectInfo*  connectionInfo);
    void stopTCPSocketClient();

    void setupUpdateGUICallback(UpdateVideo2GUI_Callback callback,unsigned long userData);

private:
    char                        m_IPAdress[64];                 //camIP
    int                         m_port;                       //端口
    char                        m_userName[13];                  //用户名
    char                        m_password[13];                  //密码

    int                         m_cSockfd;                      //命令套接字.
    int                         m_dSockfd;                      //数据通道套接字.

    int                         m_videoID;      //视频ID

    struct   sockaddr_in        m_cSockaddr_in;
    struct   sockaddr_in        m_dSockaddr_in;

    bool                        m_recvDataToggle;
    bool                        m_recvCommandToggle;

    pthread_mutex_t             mutex_cRecv;
    pthread_mutex_t             mutex_cSend;
    pthread_mutex_t             mutex_dRecv;
    pthread_mutex_t             mutex_dSend;


    UpdateVideo2GUI_Callback updateCallback;
    unsigned long userData;


    int initTCPCSocketConnection(CC_NetConnectInfo*  connectionInfo);
};

#endif // CAMERACLIENT_H
