#ifndef MACCAMERA_H
#define MACCAMERA_H

#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>

class MacCamera{

public:
    MacCamera();
    ~MacCamera();

    void OpenCamera(int id);
    void * QueryFrame();

    static int CountCamera();
    static void ShowCamera(int id);

private:
    QCamera *camera;
    QCameraImageCapture *imageCapture;
};

#endif // MACCAMERA_H
