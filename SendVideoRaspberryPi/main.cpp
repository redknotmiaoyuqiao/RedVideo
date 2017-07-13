#include <iostream>
#include <stdio.h>
#include "Camera/Camera.hpp"
#include "H264/H264Encoder.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",2000,2000);

    X264Encoder x264Encoder;
    H264Encoder * h264Encoder = new H264Encoder();

    h264Encoder->h264_encoder_init(&x264Encoder,camera->width,camera->height);

    while(1){
        unsigned char * yuv422data = camera->read_frame();

        unsigned char * out = (uint8_t * )malloc(10000);
        int res = h264Encoder->h264_compress_frame(&x264Encoder,10,yuv422data,out);

        //h264Encoder->encodeH264(yuv420data,out);

        puts("--------------------\n");
        printf("frame_size:%d\n",res);
        printf("%d\n",yuv422data[0]);
        printf("%d\n",yuv422data[1]);
        printf("%d\n",yuv422data[2]);
        puts("--------------------\n");
    }

    delete camera;
    delete h264Encoder;

    return 0;
}
