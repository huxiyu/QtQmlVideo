#include "qmlvideo.h"
#include <QPainter>
#include <GL/glew.h>
#include <qgl.h>
#include <QTimer>
#include <QMutexLocker>
#include <QImage>
#include <vlc/vlc.h>
#include <QOpenGLContext>

#include <QDebug>

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

QmlVideo::QmlVideo(QQuickItem *parent)
    :QQuickPaintedItem(parent),
      m_state(Stopped),
      m_paintMode(PaintModeTexture)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);


    //Initialize the VLC library;
    const char *argv[] =
    {
        "--no-audio", /* skip any audio track */
        "--no-xlib", /* tell VLC to not use Xlib */
        "--no-video-title-show"
        //        "--network-caching=500"
    };
    int argc = sizeof(argv) / sizeof(*argv);
    m_libVlc = libvlc_new(argc,argv);
}

QmlVideo::~QmlVideo(){
}

void QmlVideo::paint(QPainter* p)
{
    if (m_paintModeFlag)
    {
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            qDebug("error %s", glewGetErrorString(err));
            m_paintMode = PaintModeQPainter;
        }
        else
        {
            if(GLEW_EXT_pixel_buffer_object)
            {
                qDebug() << "GLEW_EXT_pixel_buffer_object: PaintModePBO";
                m_paintMode = PaintModePBO;
            }
            else
            {
                qDebug() << "GLEW_EXT_pixel_buffer_object: PaintModeTexture";
                m_paintMode = PaintModeTexture;
            }
        }
        qDebug("GLEW_VERSION %s", glewGetString(GLEW_VERSION));
        m_paintModeFlag = false;
    }


    switch(m_paintMode)
    {
    case PaintModeQPainter:
    {
        QImage img((uchar *)m_pixelBuff, m_width, m_height, QImage::Format_RGB888);
        p->drawImage(boundingRect(), img.rgbSwapped(), QRect(0,0,m_width, m_height));
    }
        break;
    case PaintModeTexture:
    {
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pixelBuff);
        glBindTexture(GL_TEXTURE_2D, 0);


        p->beginNativePainting();
        //        qDebug() << "Paint...";

        QRectF rect = boundingRect();

//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, m_textureId);

//        glBegin(GL_QUADS);
//        //        glColor3f(1.0f,0.0f,0.0f);
//        glTexCoord2d(0.0,0.0); glVertex2d(-1.0f, -1.0f);
//        glTexCoord2d(0.0,1.0); glVertex2d(-1.0f, 1.0f);
//        glTexCoord2d(1.0,1.0); glVertex2d(1.0f, 1.0f);
//        glTexCoord2d(1.0,0.0); glVertex2d(1.0f, -1.0f);
//        glEnd();

//        glBindTexture(GL_TEXTURE_2D, 0);
//        glDisable(GL_TEXTURE_2D);


        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex2d(-1.0f, -1.0f);
        glTexCoord2d(0.0,1.0); glVertex2d(-1.0f, 0.0f);
        glTexCoord2d(1.0,1.0); glVertex2d(0.0f, 0.0f);
        glTexCoord2d(1.0,0.0); glVertex2d(0.0f, -1.0f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex2d(0.0f, -1.0f);
        glTexCoord2d(0.0,1.0); glVertex2d(0.0f, 0.0f);
        glTexCoord2d(1.0,1.0); glVertex2d(1.0f, 0.0f);
        glTexCoord2d(1.0,0.0); glVertex2d(1.0f, -1.0f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex2d(-1.0f, 0.0f);
        glTexCoord2d(0.0,1.0); glVertex2d(-1.0f, 1.0f);
        glTexCoord2d(1.0,1.0); glVertex2d(0.0f, 1.0f);
        glTexCoord2d(1.0,0.0); glVertex2d(0.0f, 0.0f);
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex2d(0.0f, 0.0f);
        glTexCoord2d(0.0,1.0); glVertex2d(0.0f, 1.0f);
        glTexCoord2d(1.0,1.0); glVertex2d(1.0f, 1.0f);
        glTexCoord2d(1.0,0.0); glVertex2d(1.0f, 0.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        p->endNativePainting();
    }
        break;
    case PaintModePBO:
        break;
    }


    /*
    p->beginNativePainting();

    qDebug() << "Paint...";
    //    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    QRectF rect = boundingRect();

    //    glLoadIdentity();
    //    // draw triangle
    //    glColor3f(1.0f,0.0f,0.0f);
    //    glBegin(GL_TRIANGLES);
    //    glColor3f(1.0f,0.0f,0.0f);
    //    glVertex3f(0.0f, -1.0f, 0.0f);
    //    glColor3f(0.0f,1.0f,0.0f);
    //    glVertex3f(-1.0f, 1.0f, 0.0f);
    //    glColor3f(0.0f,0.0f,1.0f);
    //    glVertex3f(1.0f, 1.0f, 0.0f);
    //    glEnd();
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex2d(0.0f, 0.0f);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex2d(0.0f, 1.0f);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2d(1.0f, 1.0f);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex2d(1.0f, 0.0f);
    glEnd();

    p->endNativePainting();
    */
}
QmlVideo::State QmlVideo::state()
{
    return(m_state);
}

void QmlVideo::play(const QString &fileName)
{
    if(fileName.isNull())
        setFileName(fileName);

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
        libvlc_media_player_stop(m_mediaPlayer);
        emit(stopped());
        break;
    case Playing:
        qDebug() << "Playing";
        libvlc_media_player_play(m_mediaPlayer);
        emit(playing());
        break;
    case Paused:
        qDebug() << "Paused";
        libvlc_media_player_set_pause(m_mediaPlayer,1);
        emit(paused());
        break;
    }

    emit(stateChanged(state));
}

QString QmlVideo::fileName()
{
    return(m_fileName);
}

void QmlVideo::setFileName(const QString &fileName)
{
    if(m_state != Stopped)
        setState(Stopped);
    m_fileName = fileName;

    libvlc_media_t *m;
    m = libvlc_media_new_path(m_libVlc, qPrintable(fileName));
    m_mediaPlayer = libvlc_media_player_new_from_media(m);
    libvlc_media_release(m);

    libvlc_video_set_format_callbacks(m_mediaPlayer, vlcVideoFormatCallback, NULL);
    libvlc_video_set_callbacks(m_mediaPlayer, vlcVideoLockCallBack, vlcVideoUnlockCallback, vlcVideoDisplayCallback, this);
}

void QmlVideo::paintFrame()
{
    //Just signal that we need to repaint the item.
    update();
}


unsigned int QmlVideo::vlcVideoFormatCallback(void **object, char *chroma, unsigned int *width, unsigned int *height,
                                              unsigned int *pitches, unsigned int *lines)
{
    unsigned int retval = 0;
    QmlVideo *instance = (QmlVideo *)*object;
    QMetaObject::invokeMethod(instance, "setupFormat", Qt::BlockingQueuedConnection, Q_RETURN_ARG(quint32, retval),
                              Q_ARG(char *, chroma), Q_ARG(unsigned int *, width), Q_ARG(unsigned int *, height),
                              Q_ARG(unsigned int *, pitches), Q_ARG(unsigned int *, lines));
    return(retval);
}

void *QmlVideo::vlcVideoLockCallBack(void *object, void **planes)
{
    //Lock the pixel mutex, and hand the pixel buffer to VLC
    QmlVideo *instance = (QmlVideo *)object;
    //    QMutexLocker((instance->m_pixelMutex));
    *planes = (void *)instance->m_pixelBuff;
    return NULL;
}

void QmlVideo::vlcVideoUnlockCallback(void *object, void *picture, void * const *planes)
{
    QmlVideo *instance = (QmlVideo *)object;
    QMetaObject::invokeMethod(instance, "updateTexture", Qt::BlockingQueuedConnection,
                              Q_ARG(void *, picture), Q_ARG(void * const *, planes));
}

void QmlVideo::vlcVideoDisplayCallback(void *object, void *picture)
{
    //Call the paintFrame function in the main thread.
    QmlVideo *instance = (QmlVideo *)object;
    QMetaObject::invokeMethod(instance, "paintFrame", Qt::BlockingQueuedConnection);
}

quint32 QmlVideo::setupFormat(char *chroma, unsigned int *width, unsigned int *height, unsigned int *pitches, unsigned int *lines)
{

    qDebug() << "Got format request:" << chroma << *width << *height;

    //    glewExperimental = GL_TRUE;
    //    GLenum err = glewInit();
    //    if (GLEW_OK != err)
    //    {
    //        qDebug("error %s", glewGetErrorString(err));
    //        m_paintMode = PaintModeQPainter;
    //    }
    //    else
    //    {
    //        if(GLEW_EXT_pixel_buffer_object)
    //        {
    //            qDebug() << "GLEW_EXT_pixel_buffer_object: PaintModePBO";
    //            m_paintMode = PaintModePBO;
    //        }
    //        else
    //        {
    //            qDebug() << "GLEW_EXT_pixel_buffer_object: PaintModeTexture";
    //            m_paintMode = PaintModeTexture;
    //        }
    //    }
    //    qDebug("GLEW_VERSION %s", glewGetString(GLEW_VERSION));


    strcpy(chroma, "RV24");
    pitches[0] = *width * 3;
    lines[0] = *height * 3;
    m_width = *width;
    m_height = *height;

    switch(m_paintMode)
    {
    case PaintModeQPainter:
        m_pixelBuff = (char *)malloc((*width)*(*height)*3);
        break;
    case PaintModeTexture:
        m_pixelBuff = (char *)malloc((*width)*(*height)*3);
        glGenTextures(1, &m_textureId);

        //        glBindTexture(GL_TEXTURE_2D, m_textureId);
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        //        glBindTexture(GL_TEXTURE_2D, 0);
        break;
    case PaintModePBO:
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, 0);

        //        glGenBuffers(1, &m_pbo1);
        //        glGenBuffers(1, &m_pbo2);
        break;
    }

    return(1);
}

void QmlVideo::updateTexture(void *picture, void * const *planes)
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
        break;
    case PaintModeTexture: {
        //        qDebug() << "Decode";
        //        glBindTexture(GL_TEXTURE_2D, m_textureId);
        //        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pixelBuff);
        //        glBindTexture(GL_TEXTURE_2D, 0);
    }
        break;
    case PaintModePBO:
        break;
    }
}



