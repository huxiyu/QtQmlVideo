#include "qmlvideo.h"
#include <QPainter>
#include <GL/glew.h>
#include <qgl.h>
#include <QDebug>



QmlVideo::QmlVideo(QQuickItem *parent)
    :QQuickPaintedItem(parent)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    GLenum err = glewInit();
        if (GLEW_OK != err)
        {
        }
        else
        {
            if(GLEW_EXT_pixel_buffer_object)
            {
            }
            else
            {
            }
        }
}

QmlVideo::~QmlVideo(){
}



void QmlVideo::paint(QPainter* p)
{
    p->beginNativePainting();

    qDebug() << "Paint...";
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    QRectF rect = boundingRect();

    glLoadIdentity();
        // draw triangle
        glColor3f(1.0f,0.0f,0.0f);
        glBegin(GL_TRIANGLES);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(0.0f, -1.0f, 0.0f);
                glColor3f(0.0f,1.0f,0.0f);
                glVertex3f(-1.0f, 1.0f, 0.0f);
                glColor3f(0.0f,0.0f,1.0f);
                glVertex3f(1.0f, 1.0f, 0.0f);
        glEnd();
    p->endNativePainting();
}

