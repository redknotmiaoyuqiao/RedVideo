#include <QApplication>

#include "mainwindow.h"
#include "Camera/Camera.hpp"

int main(int argc, char *argv[])
{
    Camera * camera = new Camera();
    camera->OpenCamera("/dev/video0",640,360);

    unsigned char * data = camera->read_frame();

    for (int idx=0; idx<120; idx++){
        printf("%d\t",data[idx]);
    }


    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    */
}
