#include "H264decoder.hpp"

H264Decoder::H264Decoder()
{
    pCodec              =NULL;
    pCodecCtx           =NULL;
    pVideoFrame         =NULL;
    img_convert_ctx     =NULL;

    pictureWidth            =0;
    setRecordREsolveState   =0;

    av_register_all();
    avcodec_register_all();

    pCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
    if(pCodec==NULL){
        printf("codec not found!\n");
    }

    pCodecCtx=avcodec_alloc_context3(pCodec);
    if(pCodecCtx == NULL){
        printf("alloc codec ctx failed \n");
    }

    avcodec_open2(pCodecCtx,pCodec,NULL);

    pVideoFrame=avcodec_alloc_frame();

    avpicture_alloc(&outPicture,PIX_FMT_RGB24,640,480);

}
H264Decoder::~H264Decoder()
{
    if(pCodecCtx!=NULL){
        avcodec_close(pCodecCtx);
        pCodecCtx=NULL;
    }
    if(pVideoFrame!=NULL){
        avcodec_free_frame(&pVideoFrame);
        pVideoFrame=NULL;
    }

     avpicture_free(&outPicture);

    if(img_convert_ctx != NULL){
        sws_freeContext(img_convert_ctx);
        img_convert_ctx=NULL;
    }

}

int H264Decoder::DecodeH264Frames(unsigned char* inputBuffer,int aLength,RGBData_Define* outputRGBData)
{
    int gotPicPtr=0;
    int result=0;


    av_init_packet(&pAvPacket);
    pAvPacket.data=(unsigned char*)inputBuffer;
    pAvPacket.size=aLength;

    result=avcodec_decode_video2(pCodecCtx,pVideoFrame,&gotPicPtr,&pAvPacket);

    if(gotPicPtr)
    {
        if(img_convert_ctx == NULL){
            img_convert_ctx=sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,640,360,PIX_FMT_RGB24,SWS_FAST_BILINEAR,NULL,NULL,NULL);
        }
        //转 RGB
        sws_scale(img_convert_ctx,(const uint8_t**)pVideoFrame->data,pVideoFrame->linesize,0,pVideoFrame->height,outPicture.data,outPicture.linesize);

        outputRGBData->width=pCodecCtx->width;
        outputRGBData->height=pCodecCtx->height;
        outputRGBData->bufferSize=outPicture.linesize[0];
        outputRGBData->databuffer=outPicture.data[0];
    }
    av_free_packet(&pAvPacket);


    return 0;

}
