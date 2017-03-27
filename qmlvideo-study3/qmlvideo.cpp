#include "qmlvideo.h"
#include <QPainter>
#include <GL/glew.h>
//#include <qgl.h>
#include <QTimer>

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

    m_frameTimer = new QTimer(this);
    QObject::connect(m_frameTimer, SIGNAL(timeout()), this, SLOT(frame()));
    m_frameTimer->setInterval(33);
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
QmlVideo::State QmlVideo::state()
{
    return(m_state);
}

void QmlVideo::play()
{
    setState(Playing);
}

void QmlVideo::pause()
{
    setState(Paused);
}

void QmlVideo::stop()
{
    setState(Stopped);
}

void QmlVideo::playPause()
{
    switch(m_state)
    {
    case Stopped:
        setState(Playing);
        break;
    case Playing:
        setState(Paused);
        break;
    case Paused:
        setState(Playing);
        break;
    }
}

void QmlVideo::setState(State state)
{
    if(state == m_state)
        return;

    m_state = state;

    switch(m_state)
    {
    case Stopped:
        qDebug() << "Stopped";
        m_frameTimer->stop();
        emit(stopped());
        break;
    case Playing:
        qDebug() << "Playing";
        m_frameTimer->start();
        emit(playing());
        break;
    case Paused:
        qDebug() << "Paused";
        m_frameTimer->stop();
        emit(paused());
        break;
    }

    emit(stateChanged(state));
}

void QmlVideo::frame()
{
    update();
}

