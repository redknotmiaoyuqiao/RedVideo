#pragma once

typedef struct rgbDataDefine
{
    int                 width;
    int                 height;
    int                 bufferSize;
    unsigned char*      databuffer;
}RGBData_Define;

extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

class H264Decoder
{
public:
    H264Decoder();
    ~H264Decoder();

    int     DecodeH264Frames(unsigned char* inputBuffer,int aLength,RGBData_Define* outputRGBData);

private:
    int pictureWidth;

    int setRecordREsolveState;

    AVCodec*            pCodec;
    AVCodecContext*     pCodecCtx;
    AVFrame*            pVideoFrame;
    AVPacket            pAvPacket;

    AVPicture           outPicture;

    struct SwsContext*  img_convert_ctx;


};
