#include <QApplication>

#include "redglwidget.h"
#include "Camera/MacCamera.h"

int main(int argc, char *argv[])
{

    MacCamera::ShowCamera(0);
    MacCamera * camera = new MacCamera();
    camera->OpenCamera(0);
    camera->QueryFrame();

    while(1){
        camera->QueryFrame();
    }

/*
    QGuiApplication app(argc, argv);

    MacCamera::ShowCamera(0);
    MacCamera * camera = new MacCamera();
    camera->OpenCamera(0);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    RedGLWidget window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    return app.exec();
    */
}
