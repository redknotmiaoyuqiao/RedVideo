#include <QDebug>
#include "mediawindow.h"
#include "ui_mediawindow.h"

MediaWindow::MediaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaWindow)
{
    ui->setupUi(this);

    const char * const vlc_args[] = {
                  "-I", "dummy", /* Don't use any interface */
                  "--ignore-config", /* Don't use VLC's config */
                  "--plugin-path=/Users/redknot/RedVideo/RedPlayer/3rdparty/mac/libvlc/plugins" };

    //m_Instance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m_Instance = libvlc_new(0,NULL);
    if(m_Instance == NULL){
        qDebug()<<"Instance Fail";
    }else{
        qDebug()<<"Instance Call OK";
    }
}

MediaWindow::~MediaWindow()
{
    delete ui;
}
