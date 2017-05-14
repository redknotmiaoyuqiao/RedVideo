#include "h264decode.h"

H264decode::H264decode()
{
    this->pCodec = nullptr;
    this->pCodecCtx = nullptr;
    this->pVideoFrame = nullptr;
    this->img_convert_ctx = nullptr;

    this->pictureWidth = 0;
    this->setRecordREsolveState = 0;

    av_register_all();
    avcodec_register_all();

    this->pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if(this->pCodec == nullptr){
        printf("codec not found");
    }



}

H264decode::~H264decode()
{

}
