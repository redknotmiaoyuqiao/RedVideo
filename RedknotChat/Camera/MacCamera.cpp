#include "MacCamera.h"

MacCamera::MacCamera()
{

}

MacCamera::~MacCamera()
{

}

void MacCamera::OpenCamera(int id)
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    camera = new QCamera(cameras[id]);
    imageCapture = new QCameraImageCapture(camera);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    imageCapture->setBufferFormat(QVideoFrame::Format_BGRA32);

    camera->start();
}

void * MacCamera::QueryFrame()
{
    camera->searchAndLock();
    int id = imageCapture->capture();
    camera->unlock();

    QImage img(100,100,QImage::Format_ARGB32);
    imageCapture->imageCaptured(id,img);

    qDebug()<< id << "   " << img.pixelColor(10,10);
}

int MacCamera::CountCamera()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    return cameras.length();
}

void MacCamera::ShowCamera(int id)
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    if(id >= cameras.length()){
        return;
    }

    qDebug() << "deviceName : " << cameras[id].deviceName() << "\n";
    qDebug() << "description : " << cameras[id].description() << "\n";
}
