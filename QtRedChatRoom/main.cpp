#include <QApplication>

#include "mainwindow.h"
#include "Camera/Camera.hpp"
#include "H264/H264encode.hpp"

int main(int argc, char *argv[])
{
    int width = 640;
    int height = 320;
    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",width,height);

    H264encode * h264encode = new H264encode();
    h264encode->h264_encoder_init(width,height);

    unsigned char * out = (uint8_t *) malloc(sizeof(uint8_t) * width * height * 3);

    int i=0;
    while(i<1000){
        unsigned char * data = camera->read_frame();

        int res = h264encode->h264_compress_frame(10,data,out);


        i++;
    }

    //printf("Res::::%d\n\n",res);



//    for(int i=0;i<width * height * 3 / 2;i++){
//        printf("%d\t",data420[i]);
//    }

    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    */
}
