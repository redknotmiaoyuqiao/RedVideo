#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QWidget>
#include <QDebug>
#include <QPainter>

#include "cameraclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void onRecvFrame(unsigned char * data,int width,int height);

private:
    QMutex m_mutex;

    QImage m_image;

    Ui::MainWindow *ui;

    CameraClient * m_CameraClient;

    void paintEvent(QPaintEvent *);
    void PaintImage(QPainter &ainter);

    void static updateVideoToGUI(RGBDataDefine * rgbData,unsigned long userData);
};

#endif // MAINWINDOW_H
