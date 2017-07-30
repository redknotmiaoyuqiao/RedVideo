#include "H264Decoder.hpp"

H264Decoder::H264Decoder()
{
    init();
}

H264Decoder::~H264Decoder()
{
    dealloc();
}

void H264Decoder::init()
{
    pCodec = NULL;
    pCodecCtx = NULL;
    pVideoFrame = NULL;

    pictureWidth = 0;
    setRecordResoleState = 0;

    av_register_all();
    avcodec_register_all();

    pCodec = avcodec_find_decoder(CODEC_ID_H264);
    if(!pCodec){
        printf("Codec not find\n");
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(!pCodecCtx){
        printf("Allocate Codec Context Error\n");
    }

    avcodec_open2(pCodecCtx,pCodec,NULL);

    pVideoFrame = avcodec_alloc_frame();
}

void H264Decoder::dealloc(){
    if(!pCodecCtx){
        avcodec_close(pCodecCtx);
        pCodecCtx = NULL;
    }
    if(!pVideoFrame){
        avcodec_free_frame(&pVideoFrame);
        pVideoFrame = NULL;
    }
}

int H264Decoder::DecodeH264Frames(unsigned char * data, int length, H264YUVDef * yuvFrame)
{

    int gotPicPtr = 0;
    int result = 0;

    av_init_packet(&pAvPachet);
    pAvPachet.data = data;
    pAvPachet.size = length;

    //Decode
    result = avcodec_decode_video2(pCodecCtx,pVideoFrame,&gotPicPtr,&pAvPachet);

    if((pictureWidth != 0) && (pictureWidth != pCodecCtx->width)){
        setRecordResoleState = 0;
        pictureWidth = pCodecCtx->width;
        return -1;
    }

    if(gotPicPtr){
        unsigned int lumaLength = (pCodecCtx->height) * (MIN(pVideoFrame->linesize[0],pCodecCtx->width));
        unsigned int chromBLength = ((pCodecCtx->height)/2) * (MIN(pVideoFrame->linesize[1], (pCodecCtx->width)/2));
        unsigned int chromRLength = ((pCodecCtx->height)/2) * (MIN(pVideoFrame->linesize[2], (pCodecCtx->width)/2));

        yuvFrame->luma.length = lumaLength;
        yuvFrame->chromaB.length = chromBLength;
        yuvFrame->chromaR.length = chromRLength;

        if(yuvFrame->luma.dataBuffer != NULL){
            free(yuvFrame->luma.dataBuffer);
        }
        if(yuvFrame->chromaB.dataBuffer != NULL){
            free(yuvFrame->chromaB.dataBuffer);
        }
        if(yuvFrame->chromaR.dataBuffer != NULL){
            free(yuvFrame->chromaR.dataBuffer);
        }

        yuvFrame->luma.dataBuffer = (unsigned char *)malloc(lumaLength);
        yuvFrame->chromaB.dataBuffer = (unsigned char *)malloc(chromBLength);
        yuvFrame->chromaR.dataBuffer = (unsigned char *)malloc(chromRLength);

        copyDecodedFrame(pVideoFrame->data[0],yuvFrame->luma.dataBuffer,pVideoFrame->linesize[0],pCodecCtx->width,pCodecCtx->height);
        copyDecodedFrame(pVideoFrame->data[1],yuvFrame->chromaB.dataBuffer,pVideoFrame->linesize[1],pCodecCtx->width / 2,pCodecCtx->height / 2);
        copyDecodedFrame(pVideoFrame->data[2],yuvFrame->chromaR.dataBuffer,pVideoFrame->linesize[2],pCodecCtx->width / 2,pCodecCtx->height / 2);

        yuvFrame->width = pCodecCtx->width;
        yuvFrame->height = pCodecCtx->height;

        if(setRecordResoleState == 0){
            setRecordResoleState = 1;
        }
    }

    return result;
}

void H264Decoder::copyDecodedFrame(unsigned char * src,unsigned char * dist, int linesize,int width,int height){
    width = MIN(linesize,width);
    for(int i=0;i<height;i++){
        memcpy(dist, src, width);
        dist += width;
        src += linesize;
    }
}
