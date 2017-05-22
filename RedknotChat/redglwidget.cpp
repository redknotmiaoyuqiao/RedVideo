#include "redglwidget.h"

#include <QDebug>
#include <QString>

RedGLWidget::RedGLWidget()
{

}

RedGLWidget::~RedGLWidget()
{

}

void RedGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    printContextInformation();

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RedGLWidget::resizeGL(int width, int height)
{

}

void RedGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void RedGLWidget::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE

    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
