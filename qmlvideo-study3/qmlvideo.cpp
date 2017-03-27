#include "qmlvideo.h"
#include <QPainter>
#include <qgl.h>
#include <QDebug>



QmlVideo::QmlVideo(QQuickItem *parent)
    :QQuickPaintedItem(parent)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

QmlVideo::~QmlVideo(){
}



void QmlVideo::paint(QPainter* p)
{
    p->drawLine(QPoint(0,0), QPoint(10, 10));

    p->beginNativePainting();

    qDebug() << "Paint...";
//    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    QRectF rect = boundingRect();
    qDebug() << rect.x() << " " << rect.y() << " " <<
             rect.width() << " " << rect.height();


    glLoadIdentity();
    glBegin(GL_QUADS);
        glColor3f(0,0,0);
        glVertex2d(rect.x(), rect.y());
        glVertex2d(rect.x(), rect.y() + rect.height());
        glVertex2d(rect.x() + rect.width(), rect.y() + rect.height());
        glVertex2d(rect.x() + rect.width(), rect.y());
    glEnd();

    p->endNativePainting();
}

