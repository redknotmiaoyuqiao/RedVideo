#pragma once

#include <stdio.h>
#include <stdint.h>
extern "C"
{
#include <x264.h>
}

class H264Encoder
{
private:
    int width = 0;
    int height = 0;

    int pts = 0;

    x264_param_t* pX264Param = NULL;

    //编码器句柄
    x264_t* pX264Handle = NULL;

    //Input And Output
    x264_picture_t* pPicIn = NULL;
    x264_picture_t* pPicOut = NULL;

    int iNal = 0;//修改部分
    x264_nal_t *pNals = NULL;//修改部分

public:
    H264Encoder(int width,int height);
    unsigned char * Decode(unsigned char * yuv420data,int * frameSize);
    ~H264Encoder();
};
