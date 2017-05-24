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

    X264Encoder encode;
    H264encode * h264encode = new H264encode(&encode);

    unsigned char * data420 = new unsigned char[width * height * 3 / 2];

    int i=0;
    while(i<100){
        unsigned char * data = camera->read_frame();
        h264encode->YUV422ToYUV420(data,data420,width,height);
        i++;
    }


    for(int i=0;i<width * height * 3 / 2;i++){
        printf("%d\t",data420[i]);
    }

    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    */
}
