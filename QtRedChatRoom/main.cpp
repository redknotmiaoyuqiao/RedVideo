#include <QApplication>

#include "mainwindow.h"
#include "Camera/Camera.hpp"
#include "H264/H264encode.hpp"

int main(int argc, char *argv[])
{
    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",640,360);

    X264Encoder encode;
    H264encode * h264encode = new H264encode(&encode);

    unsigned char * data = camera->read_frame();
    unsigned char * out;

    h264encode->h264EncoderInit(640,320);
    h264encode->h264CompressFrame(2,data,out);

    printf("\n\n%d",sizeof(out));
    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    */
}
