#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(QRect(0,0,640,480));

    m_CameraClient = new CameraClient();
    m_CameraClient->setupUpdateGUICallback(updateVideoToGUI,(unsigned long)this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_CameraClient;
}

void MainWindow::updateVideoToGUI(RGBDataDefine * rgbData,unsigned long userData)
{
    MainWindow * window = (MainWindow *)userData;
    if(window != NULL){
        window->onRecvFrame(rgbData->databuffer,rgbData->width,rgbData->height);
    }
}

void MainWindow::onRecvFrame(unsigned char * data,int width,int height)
{
    QMutexLocker locker(&m_mutex);

    //QImage::Format_RGB888等于24
    const int imageSize = (((width * 24 + 31) >> 5) << 2) * height;

    uint8_t *dataTmp = new uint8_t[imageSize];
    //将数据拷贝到一个临时内存中，因为OnRecvFrame是异步执行的，data可能已经无效。
    try
    {
        memcpy(dataTmp, data, imageSize);
    }
    catch (...)
    {
        delete[] dataTmp;
        return;
    }

    //转化rgb数据为可显示的图像对象。
    QImage image = QImage(dataTmp, width, height, QImage::Format_RGB888);
    if (image.isNull())
    {
        qDebug()<<"Receive frame error, width:%d, height:%d."<<width<<height;

        return;
    }

    m_image = image.copy(0, 0, width, height);

    delete[] dataTmp;

    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (!m_image.isNull())
    {
        PaintImage(painter);
    }
}

void MainWindow::PaintImage(QPainter &painter)
{
    QMutexLocker locker(&m_mutex);

    int imageWidth = m_image.width();
    int imageHeight = m_image.height();
    QRect m_imageRect=QRect(0,0,imageWidth,imageHeight);

    painter.drawImage(m_imageRect, m_image);
}


