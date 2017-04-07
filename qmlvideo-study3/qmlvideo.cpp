#include "qmlvideo.h"
#include <QPainter>
#include <qgl.h>
#include <QTimer>
#include <QMutexLocker>
#include <QImage>
#include <vlc/vlc.h>
#include <QString>

#include <QDebug>

QmlVideo::QmlVideo(QQuickItem *parent)
    :QQuickPaintedItem(parent),
      m_state(Stopped)
{
    // 设置 绘制方式
    //    setRenderTarget(QQuickPaintedItem::FramebufferObject);

    m_pixelBuff = NULL;
    m_fileName = QString::null;
    m_mediaPlayer = NULL;

    //Initialize the VLC library;
    const char *argv[] =
    {
        "--no-audio", /* skip any audio track */
        "--no-xlib", /* tell VLC to not use Xlib */
        "--no-video-title-show",
        "--network-caching=500",
    };
    int argc = sizeof(argv) / sizeof(*argv);
    m_libVlc = libvlc_new(argc,argv);
}

QmlVideo::~QmlVideo(){
    if(m_mediaPlayer != NULL)libvlc_media_player_stop(m_mediaPlayer);
    if(m_mediaPlayer != NULL)libvlc_media_player_release(m_mediaPlayer);
    if(m_pixelBuff != NULL) delete m_pixelBuff;
    libvlc_release(m_libVlc);
    qDebug() << "exit " << m_fileName;
}

void QmlVideo::paint(QPainter* p)
{

    if(m_state == Playing || m_state == Paused)
    {
        //        qDebug() << "Paint...";

        QImage img((uchar *)m_pixelBuff, m_width, m_height, QImage::Format_RGB888);
        p->drawImage(boundingRect(), img.rgbSwapped(), QRect(0,0,m_width, m_height));


        /*
        // OpenGL播放模式
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
        //        QRectF rect = boundingRect();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glBegin(GL_QUADS);
        glColor3f(1.0f,0.0f,0.0f);
        glTexCoord2d(0.0,0.0); glVertex2d(-1.0f, -1.0f);
        glTexCoord2d(0.0,1.0); glVertex2d(-1.0f, 1.0f);
        glTexCoord2d(1.0,1.0); glVertex2d(1.0f, 1.0f);
        glTexCoord2d(1.0,0.0); glVertex2d(1.0f, -1.0f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);


        //            glEnable(GL_TEXTURE_2D);
        //            glBindTexture(GL_TEXTURE_2D, m_textureId);

        //            glBegin(GL_QUADS);
        //            glTexCoord2d(0.0,0.0); glVertex2d(-1.0f, -1.0f);
        //            glTexCoord2d(0.0,1.0); glVertex2d(-1.0f, 0.0f);
        //            glTexCoord2d(1.0,1.0); glVertex2d(0.0f, 0.0f);
        //            glTexCoord2d(1.0,0.0); glVertex2d(0.0f, -1.0f);
        //            glEnd();

        //            glBegin(GL_QUADS);
        //            glTexCoord2d(0.0,0.0); glVertex2d(0.0f, -1.0f);
        //            glTexCoord2d(0.0,1.0); glVertex2d(0.0f, 0.0f);
        //            glTexCoord2d(1.0,1.0); glVertex2d(1.0f, 0.0f);
        //            glTexCoord2d(1.0,0.0); glVertex2d(1.0f, -1.0f);
        //            glEnd();

        //            glBegin(GL_QUADS);
        //            glTexCoord2d(0.0,0.0); glVertex2d(-1.0f, 0.0f);
        //            glTexCoord2d(0.0,1.0); glVertex2d(-1.0f, 1.0f);
        //            glTexCoord2d(1.0,1.0); glVertex2d(0.0f, 1.0f);
        //            glTexCoord2d(1.0,0.0); glVertex2d(0.0f, 0.0f);
        //            glEnd();

        //            glBegin(GL_QUADS);
        //            glTexCoord2d(0.0,0.0); glVertex2d(0.0f, 0.0f);
        //            glTexCoord2d(0.0,1.0); glVertex2d(0.0f, 1.0f);
        //            glTexCoord2d(1.0,1.0); glVertex2d(1.0f, 1.0f);
        //            glTexCoord2d(1.0,0.0); glVertex2d(1.0f, 0.0f);
        //            glEnd();

        //            glBindTexture(GL_TEXTURE_2D, 0);
        //            glDisable(GL_TEXTURE_2D);

        p->endNativePainting();
        */
    }
}

QmlVideo::State QmlVideo::state()
{
    return(m_state);
}

void QmlVideo::play(const QString &fileName)
{
//    if(m_fileName.isNull())
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
        libvlc_media_player_set_pause(m_mediaPlayer,1);
        libvlc_media_player_set_time(m_mediaPlayer, 0);
        libvlc_media_player_stop(m_mediaPlayer);
        update();
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

void QmlVideo::qmlVideoUpdate() {
//    update();
}


unsigned int QmlVideo::vlcVideoFormatCallback(void **object, char *chroma, unsigned int *width, unsigned int *height,
                                              unsigned int *pitches, unsigned int *lines)
{
    unsigned int retval = 0;
    QmlVideo *instance = (QmlVideo *)*object;
    // problem
    QMetaObject::invokeMethod(instance, "setupFormat", Qt::DirectConnection, Q_RETURN_ARG(quint32, retval),
                              Q_ARG(char *, chroma), Q_ARG(unsigned int *, width), Q_ARG(unsigned int *, height),
                              Q_ARG(unsigned int *, pitches), Q_ARG(unsigned int *, lines));
    return(retval);
}
void *QmlVideo::vlcVideoLockCallBack(void *object, void **planes)
{
    //Lock the pixel mutex, and hand the pixel buffer to VLC
    QmlVideo *instance = (QmlVideo *)object;
//        QMutexLocker((instance->m_pixelMutex));
    *planes = (void *)instance->m_pixelBuff;
    return NULL;

}

void QmlVideo::vlcVideoUnlockCallback(void *object, void *picture, void * const *planes)
{
    QmlVideo *instance = (QmlVideo *)object;
    QMetaObject::invokeMethod(instance, "updateTexture", Qt::DirectConnection,
                              Q_ARG(void *, picture), Q_ARG(void * const *, planes));
}

void QmlVideo::vlcVideoDisplayCallback(void *object, void *picture)
{
    //Call the paintFrame function in the main thread.
    QmlVideo *instance = (QmlVideo *)object;
    QMetaObject::invokeMethod(instance, "paintFrame", Qt::QueuedConnection);
}

quint32 QmlVideo::setupFormat(char *chroma, unsigned int *width, unsigned int *height, unsigned int *pitches, unsigned int *lines)
{
    qDebug() << "Got format request:" << chroma << *width << *height;
    strcpy(chroma, "RV24");
    pitches[0] = *width * 3;
    lines[0] = *height * 3;
    m_pixelBuff = new char[(*width)*(*height)*3];
    m_width = *width;
    m_height = *height;
    //    glGenTextures(1, &m_textureId);

    return(1);
}

void QmlVideo::updateTexture(void *picture, void * const *planes)
{
}


