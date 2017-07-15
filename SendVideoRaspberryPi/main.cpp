#include <iostream>
#include <stdio.h>
#include "Camera/Camera.hpp"
#include "H264/H264Encoder.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",500,500);

    printf("width:%d\n",camera->width);
    printf("height:%d\n",camera->height);

    H264Encoder * h264Encoder = new H264Encoder(camera->width,camera->height);

    FILE * pFile = fopen("/home/redknot/test.h264", "wb");

    unsigned char * out = NULL;

    while(1){
        unsigned char * yuv420data = camera->read_yuv420_frame();

        int frame_size = 0;
        unsigned char * out = h264Encoder->Decode(yuv420data,&frame_size);

        fwrite(out, 1, frame_size, pFile);

        printf("Frame_Size:%d\n",frame_size);

        if(out != NULL){
            free(out);
        }
    }

    delete camera;
    delete h264Encoder;

    return 0;
}
