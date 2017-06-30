#pragma once

typedef struct RGBDataDefine{
    int width;
    int height;
    int bufferSize;
    unsigned char * dataBuffer;
}RGBDataDefine;

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

class H264decoder
{
private:
    int pictureWidth;

    AVCodec * pCodec;
    AVCodecContext * pCodecCtx;
    AVFrame * pVideoFrame;

public:
    H264decoder();
    ~H264decoder();

    int DecoderH264Frames(unsigned char * inputBuffer,int length,RGBDataDefine * outPutbuffer);
};
