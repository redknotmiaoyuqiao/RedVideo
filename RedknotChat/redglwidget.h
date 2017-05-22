#ifndef REDGLWIDGET_H
#define REDGLWIDGET_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class RedGLWidget : public QOpenGLWindow,protected QOpenGLFunctions
{
public:
    RedGLWidget();
    ~RedGLWidget();

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    void printContextInformation();
};

#endif // REDGLWIDGET_H
