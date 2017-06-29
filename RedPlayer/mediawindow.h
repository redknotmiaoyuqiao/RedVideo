#ifndef MEDIAWINDOW_H
#define MEDIAWINDOW_H

#include <QMainWindow>
#include <vlc/vlc.h>

namespace Ui {
class MediaWindow;
}

class MediaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaWindow(QWidget *parent = 0);
    ~MediaWindow();

private:
    Ui::MediaWindow *ui;

    libvlc_instance_t * m_Instance;
};

#endif // MEDIAWINDOW_H
