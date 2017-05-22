#include "h264decode.h"

H264decode::H264decode()
{
    this->pCodec = NULL;
    this->pCodecCtx = NULL;
    this->pVideoFrame = NULL;
    this->img_convert_ctx = NULL;

    this->pictureWidth = 0;
    this->setRecordREsolveState = 0;

    av_register_all();
    avcodec_register_all();

    this->pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if(this->pCodec == NULL){
        printf("codec not found\n");
    }

    this->pCodecCtx = avcodec_alloc_context3(this->pCodec);
    if(this->pCodecCtx == NULL){
        printf("avcodec_alloc_context3 Fail!\n");
    }

    avcodec_open2(pCodecCtx,pCodec,NULL);

    this->pVideoFrame = avcodec_alloc_frame();

    avpicture_alloc(&outPicture,PIX_FMT_RGB24,VideoConfig::VIDEO_WIDTH,VideoConfig::VIDEO_HEIGHT);
}

H264decode::~H264decode()
{
    if(this->pCodecCtx != NULL){
        avcodec_close(this->pCodecCtx);
        this->pCodecCtx = NULL;
    }

    if(this->pVideoFrame != NULL){
        avcodec_free_frame(&this->pVideoFrame);
        this->pVideoFrame = NULL;
    }

    if(&this->outPicture != NULL){
        avpicture_free(&this->outPicture);
    }

    if(this->img_convert_ctx != NULL){
        sws_freeContext(this->img_convert_ctx);
        this->img_convert_ctx = NULL;
    }
}

int H264decode::DecodeH264Frame(unsigned char * inputBuffer,int aLength,RGBDataDefine * outputRGBData)
{
    int gotPicPtr = 0;
    int result = 0;

    av_init_packet(&this->pAvPacket);
    this->pAvPacket.data = (unsigned char * )inputBuffer;
    this->pAvPacket.size = aLength;

    result = avcodec_decode_video2(this->pCodecCtx,this->pVideoFrame,&gotPicPtr,&this->pAvPacket);

    if(gotPicPtr){
        if(img_convert_ctx == NULL){
            img_convert_ctx = sws_getContext(this->pCodecCtx->width,this->pCodecCtx->height,this->pCodecCtx->pix_fmt,VideoConfig::VIDEO_WIDTH,VideoConfig::VIDEO_HEIGHT,PIX_FMT_RGB24,SWS_FAST_BILINEAR,NULL,NULL,NULL);
        }
        //转RGB
        sws_scale(img_convert_ctx,(const uint8_t**)this->pVideoFrame->data,this->pVideoFrame->linesize,0,this->pVideoFrame->height,this->outPicture.data,this->outPicture.linesize);

        outputRGBData->width = this->pCodecCtx->width;
        outputRGBData->height = this->pCodecCtx->height;
        outputRGBData->bufferSize = this->outPicture.linesize[0];
        outputRGBData->databuffer = this->outPicture.data[0];
    }

    av_free_packet(&this->pAvPacket);

    return 0;
}
