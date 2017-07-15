#include "H264/H264Encoder.hpp"
#include <stdlib.h>
#include <string.h>

H264Encoder::H264Encoder(int width,int height) {
    this->width = width;
    this->height = height;

    printf("Init H264 Encode!!!!!!\n\n\n");

    //x264_param_t为结构体的名字，见文件X264.h
    pX264Param = new x264_param_t;

    //* 配置参数
    //* 使用默认参数，在这里因为我的是实时网络传输，所以我使用了zerolatency(零延迟)的选项，使用这个选项之后就不会有delayed_frames，如果你使用的不是这样的话，还需要在编码完成之后得到缓存的编码帧
    int res = x264_param_default_preset(pX264Param,"veryfast","zerolatency");
    //* cpuFlags
    //* 取空缓冲区继续使用不死锁的保证.
    pX264Param->i_threads = X264_SYNC_LOOKAHEAD_AUTO;

    //* 视频选项
    pX264Param->i_width = this->width; //* 要编码的图像宽度.
    pX264Param->i_height = this->height; //* 要编码的图像高度
    pX264Param->i_frame_total = 0; //* 编码总帧数.不知道用0.
    pX264Param->i_keyint_max = 100;

    //* 流参数
    /*
    pX264Param->i_bframe = 0;
    pX264Param->b_open_gop = 0;
    pX264Param->i_bframe_pyramid = 1;
    pX264Param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
    */

    //* Log参数，不需要打印编码信息时直接注释掉就行
    //pX264Param->i_log_level = X264_LOG_DEBUG;

    //X264_LOG_NONE
    //* 速率控制参数
    pX264Param->rc.i_rc_method = X264_RC_CQP;
    pX264Param->rc.i_bitrate = 1024 * 5;//* 码率(比特率,单位Kbps)
    //* muxing parameters
    pX264Param->i_fps_den  = 1; //* 帧率分母
    pX264Param->i_fps_num  = 20;//* 帧率分子
    pX264Param->i_timebase_den = pX264Param->i_fps_num;
    pX264Param->i_timebase_num = pX264Param->i_fps_den;

    //* 设置Profile.使用Baseline profile
    int res2 = x264_param_apply_profile(pX264Param, x264_profile_names[0]);

    pX264Handle = x264_encoder_open(pX264Param);

    //* 编码需要的辅助变量
    pPicIn = new x264_picture_t;
    pPicOut = new x264_picture_t;

    x264_picture_init(pPicOut);//输出图像的初始化
    int res3 = x264_picture_alloc(pPicIn, X264_CSP_I420, pX264Param->i_width, pX264Param->i_height); //输入图像分配数据

    pPicIn->img.i_csp = X264_CSP_I420;//设置初始化图片格式 YUV420p
    pPicIn->img.i_plane = 3;
}

unsigned char * H264Encoder::Decode(unsigned char * yuv420data,int * frameSize)
{
    pPicIn->i_pts = pts;
    pPicIn->i_type = X264_TYPE_AUTO;

    pPicIn->img.plane[0] = yuv420data;
    pPicIn->img.plane[1] = yuv420data + (width * height);
    pPicIn->img.plane[2] = yuv420data + (width * height) + (width * height / 4);

    unsigned char * out = NULL;

    int frame_size = x264_encoder_encode(pX264Handle,&pNals,&iNal,pPicIn,pPicOut);
    if(frame_size > 0)
    {
        *frameSize = frame_size;
        out = (unsigned char *) malloc(frame_size);
        unsigned char * p_out = out;

        pts++;
        for (int i = 0; i < iNal; ++i)
        {
            memcpy(p_out, pNals[i].p_payload, pNals[i].i_payload);
            p_out += pNals[i].i_payload;
            //fwrite(pNals[i].p_payload, 1, pNals[i].i_payload, pFile);
        }
    }

    return out;
}

H264Encoder::~H264Encoder(){
    pX264Handle = NULL;

    delete pPicIn ;
    pPicIn = NULL;

    delete pPicOut;
    pPicOut = NULL;

    delete pX264Param;
    pX264Param = NULL;
}
