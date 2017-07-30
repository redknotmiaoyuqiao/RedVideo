#pragma once

/****
 * Base FFmpeg 2.1.8
****/
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

#define MIN(a,b)    (((a) < (b)) ? (a) : (b))

typedef struct H264FrameDef
{
    unsigned int length;
    unsigned char * dataBuffer;
}H264FrameDef;

typedef struct H264YUVDef
{
    unsigned int width;
    unsigned int height;
    H264FrameDef luma;
    H264FrameDef chromaB;
    H264FrameDef chromaR;
}H264YUVDef;

class H264Decoder
{
private:
    AVCodec * pCodec;
    AVCodecContext * pCodecCtx;
    AVFrame * pVideoFrame;

    AVPacket pAvPachet;

    int pictureWidth;
    int setRecordResoleState;
public:
    H264Decoder();
    ~H264Decoder();

    void init();
    void dealloc();

    int DecodeH264Frames(unsigned char * data, int length, H264YUVDef * yuvFrame);
    void copyDecodedFrame(unsigned char * src,unsigned char * dist, int linesize,int width,int height);
};
