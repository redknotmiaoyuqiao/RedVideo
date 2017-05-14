#ifndef H264DECODE_H
#define H264DECODE_H

typedef struct RGBDataDefine
{
    int width;
    int height;
    int bufferSize;
    unsigned char * databuffer;
}RGBDataDefine;

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

class H264decode
{
public:
    H264decode();
    ~H264decode();

    int DecodeH264Frame(unsigned char * inputBuffer,int aLength,RGBDataDefine * outputRGBData);

private:
    int pictureWidth;

    int setRecordREsolveState;

    AVCodec * pCodec;
    AVCodecContext * pCodecCtx;
    AVFrame * pVideoFrame;
    AVPacket  pAvPacket;

    AVPicture outPicture;

    struct SwsContext * img_convert_ctx;
};

#endif // H264DECODE_H
