#include "qmlvideo.h"
#include <QPainter>
#include <GL/glew.h>
#include <qgl.h>
#include <QTimer>
#include <QMutexLocker>
#include <QImage>
#include <vlc/vlc.h>
//#include <QOpenGLContext>

#include <QDebug>

//#ifndef GL_BGR
//#define GL_BGR 0x80E0
//#endif

QmlVideo::QmlVideo(QQuickItem *parent)
    :QQuickPaintedItem(parent),
      m_state(Stopped),
      m_paintMode(PaintModeTexture)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);

    qRegisterMetaType<const libvlc_event_t *>("const libvlc_event_t *");


    memset(m_textureId, 0, sizeof(quint32)*3);
    memset(m_pbo1, 0, sizeof(quint32)*3);
    memset(m_pbo2, 0, sizeof(quint32)*3);

    //Initialize the VLC library;
    const char *argv[] =
    {
        "--no-xlib", /* tell VLC to not use Xlib */
        "--network-caching=500",
        "--no-video-title-show",
        "--disable-screensaver",
    };
    int argc = sizeof(argv) / sizeof(*argv);
    m_libVlc = libvlc_new(argc,argv);


    //    //Initialize the VLC library;
    //    const char *argv[] =
    //    {
    //        "--no-audio", /* skip any audio track */
    //        "--no-xlib", /* tell VLC to not use Xlib */
    //        "--no-video-title-show"
    ////        "--network-caching=500",
    ////        "--disable-screensaver"
    //    };
    //    int argc = sizeof(argv) / sizeof(*argv);
    //    m_libVlc = libvlc_new(argc,argv);
}

QmlVideo::~QmlVideo(){
    clearUp();
    libvlc_release(m_libVlc);
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
        m_paintMode = PaintModeQPainter;
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
        //        qDebug() << "PaintModeTexture";
        //        glGenTextures(1, &m_textureId[0]);
        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);




        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pixelBuff[0]);
        glBindTexture(GL_TEXTURE_2D, 0);


        //        qDebug() << "Updating Textures";
        //        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
        //        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pixelBuff[0]);
        //        glBindTexture(GL_TEXTURE_2D, 0);


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
        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);

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
    case PaintModePBO: {
        p->beginNativePainting();
        qDebug() << "Setting up textures";
//        glGenTextures(1, &m_textureId[0]);
//        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//        glBindTexture(GL_TEXTURE_2D, 0);


        //        qDebug() << "Updating Textures";
//        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pixelBuff[0]);
//        glBindTexture(GL_TEXTURE_2D, 0);


//        qDebug() << "Setting up PBOs";
//        glGenBuffers(1, &m_pbo1[0]);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo1[0]);
//        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 3, 0, GL_STREAM_DRAW);
//        m_pixelBuff[0] = (char *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
//        glGenBuffers(1, &m_pbo2[0]);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo2[0]);
//        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 3, 0, GL_STREAM_DRAW);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);


//        // updatePBOs
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo1[0]);
//        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo2[0]);
//        //Reset the buffer data to make sure we don't block when the buffer is mapped
//        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 3, 0, GL_STREAM_DRAW);
//        m_pixelBuff[0] = (char *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

//        quint32 tmp = m_pbo1[0];
//        m_pbo1[0] = m_pbo2[0];
//        m_pbo2[0] = tmp;

//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo2[0]);
//        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, 0);
//        glBindTexture(GL_TEXTURE_2D, 0);
//        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);




        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);

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

void QmlVideo::clearUp(){
    if(m_mediaPlayer != NULL)libvlc_media_player_stop(m_mediaPlayer);
    if(m_mediaPlayer != NULL)libvlc_media_player_release(m_mediaPlayer);

    cleanupBuffers();
    cleanupTextures();
    cleanupPBOs();
}

QmlVideo::State QmlVideo::state()
{
    return(m_state);
}

void QmlVideo::play(const QString &fileName)
{
    if(!fileName.isNull())
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

void QmlVideo::setState(State state)
{
    switch(state)
    {
    case Stopped:
        if(m_state == Stopped)
            return;
        libvlc_media_player_pause(m_mediaPlayer);
        libvlc_media_player_set_time(m_mediaPlayer, 0);
        break;
    case Playing:
        libvlc_media_player_play(m_mediaPlayer);
        break;
    case Paused:
//        enum State
//           {
//               Opening,
//               Buffering,
//               Stopped,
//               Playing,
//               Paused
//           };
        qDebug() << "m_state" << m_state << Playing << (m_state != Playing);
        if(m_state == Paused || m_state == Stopped) {
            qDebug() << "Playing(m_mediaPlayer);";
            play();
        } else {
            qDebug() << "Paused(m_mediaPlayer);";
//            Sleep(2000);
            libvlc_media_player_pause(m_mediaPlayer);
        }
        qDebug() << "m_state" << m_state << Playing;

        break;
    }
}

QString QmlVideo::fileName()
{
    return(m_fileName);
}

void QmlVideo::setFileName(const QString &fileName)
{
//    if (!m_pixelBuff) {
//        qDebug() << "ClearUp!!!!!!!!!!";
//        clearUp();
//    }
    qDebug() << "setFileName" << fileName;

    m_fileName = fileName;

    libvlc_media_t *m;
    m = libvlc_media_new_path(m_libVlc, qPrintable(fileName));
    m_mediaPlayer = libvlc_media_player_new_from_media(m);
    libvlc_media_release(m);

    libvlc_video_set_format_callbacks(m_mediaPlayer, vlcVideoFormatCallback, NULL);
    libvlc_video_set_callbacks(m_mediaPlayer, vlcVideoLockCallBack, vlcVideoUnlockCallback, vlcVideoDisplayCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerOpening, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerBuffering, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerPlaying, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerPaused, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerStopped, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerEndReached, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerEncounteredError, vlcVideoEventCallback, this);
    //libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerTimeChanged, vlcVideoEventCallback, this);
    //libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerPositionChanged, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerSeekableChanged, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerPausableChanged, vlcVideoEventCallback, this);
    libvlc_event_attach(libvlc_media_player_event_manager(m_mediaPlayer), libvlc_MediaPlayerLengthChanged, vlcVideoEventCallback, this);

    if(m_state != Stopped)
        setState(Stopped);
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
    planes[0] = (void *)instance->m_pixelBuff[0];
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
    //        m_paintMode = PaintModeQPainter;
    //    }
    //    else
    //    {
    //        if(GLEW_EXT_pixel_buffer_object)
    //        {
    //            m_paintMode = PaintModeTexture;
    //        }
    //        else
    //        {
    //            m_paintMode = PaintModeTexture;
    //        }
    //    }

    qDebug() << "Paint Mode:" << m_paintMode;

    setupPlanes(chroma, width, height, pitches, lines);
    setupBuffers();
    setupTextures();
    setupPBOs();

    return(m_numPlanes);
}

void QmlVideo::updateTexture(void *picture, void * const *planes)
{
    updateTextures();
    updatePBOs();
}

void QmlVideo::vlcVideoEventCallback(const libvlc_event_t *event, void *object)
{
    libvlc_event_t *tmp = new libvlc_event_t;
    memcpy(tmp,event,sizeof(libvlc_event_t));

    QmlVideo *instance = (QmlVideo *)object;
    QMetaObject::invokeMethod(instance, "playerEvent", Qt::QueuedConnection,
                              Q_ARG(const libvlc_event_t *, tmp));
}

void QmlVideo::playerEvent(const libvlc_event_t *event)
{
    switch(event->type)
    {
    case libvlc_MediaPlayerEndReached:
        setFileName(fileName());
    case libvlc_MediaPlayerStopped:
        qDebug() << "Stopped";
        m_state=Stopped;
        emit(stateChanged(m_state));
        emit(stopped());
        break;
    case libvlc_MediaPlayerOpening:
        qDebug() << "Opening";
        m_state=Opening;
        emit(stateChanged(m_state));
        break;
    case libvlc_MediaPlayerBuffering:
        qDebug() << "Buffering";
        m_state=Buffering;
        emit(stateChanged(m_state));
        break;
    case libvlc_MediaPlayerPlaying:
        qDebug() << "Playing!!!";
        m_state=Playing;
        emit(stateChanged(m_state));
        emit(playing());
        break;
    case libvlc_MediaPlayerPaused:
        qDebug() << "Paused";
        m_state=Paused;
        emit(stateChanged(m_state));
        emit(paused());
        break;
    default:
        qDebug() << "Event: " << libvlc_event_type_name(event->type);
        break;
    }

    delete event;
}

void QmlVideo::setupPlanes(char *chroma, unsigned int *width, unsigned int *height,
                           unsigned int *pitches, unsigned int *lines)
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
        strcpy(chroma, "RV24");
        pitches[0] = *width * 3;
        lines[0] = *height * 3;
        m_width = *width;
        m_height = *height;
        m_numPlanes = 1;
        break;
    case PaintModeTexture:
    case PaintModePBO:
        strcpy(chroma, "RV24");
        pitches[0] = *width * 3;
        lines[0] = *height * 3;
        m_width = *width;
        m_height = *height;
        m_numPlanes = 1;
        break;
    }
}

void QmlVideo::setupBuffers()
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
    case PaintModeTexture:
        qDebug() << "Setting up buffers";
        m_pixelBuff[0] = new char[m_width*m_height*3];
        break;
    case PaintModePBO:
        break;
    }
}

void QmlVideo::cleanupBuffers()
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
    case PaintModeTexture:
        delete m_pixelBuff[0];
        break;
    case PaintModePBO:
        break;
    }
}

void QmlVideo::setupTextures()
{
    switch(m_paintMode) {
    case PaintModeQPainter:
        break;
    case PaintModeTexture:
    case PaintModePBO:
        qDebug() << "Setting up textures";
        glGenTextures(1, &m_textureId[0]);
        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, 0);
        break;
    }
}

void QmlVideo::cleanupTextures()
{
    switch(m_paintMode) {
    case PaintModeQPainter:
        break;
    case PaintModeTexture:
    case PaintModePBO:
        if(m_textureId[0] != 0){glDeleteTextures(1,&m_textureId[0]);}
        break;
    }
}

void QmlVideo::updateTextures()
{
    switch(m_paintMode) {
    case PaintModeQPainter:
        break;
    case PaintModeTexture:
        //        qDebug() << "Updating Textures";
        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pixelBuff[0]);
        glBindTexture(GL_TEXTURE_2D, 0);
        break;
    case PaintModePBO:
        break;
    }
}

void QmlVideo::setupPBOs()
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
    case PaintModeTexture:
        break;
    case PaintModePBO:
        qDebug() << "Setting up PBOs";
        glGenBuffers(1, &m_pbo1[0]);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo1[0]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 3, 0, GL_STREAM_DRAW);
        m_pixelBuff[0] = (char *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        glGenBuffers(1, &m_pbo2[0]);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo2[0]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 3, 0, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        break;
    }
}

void QmlVideo::cleanupPBOs()
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
    case PaintModeTexture:
        break;
    case PaintModePBO:
        if(m_pbo1[0] != 0){glDeleteBuffers(1, &m_pbo1[0]);}
        if(m_pbo2[0] != 0){glDeleteBuffers(1, &m_pbo2[0]);}
        break;
    }
}

void QmlVideo::updatePBOs()
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
    case PaintModeTexture:
        break;
    case PaintModePBO:
        // updatePBOs
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo1[0]);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo2[0]);
        //Reset the buffer data to make sure we don't block when the buffer is mapped
        glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 3, 0, GL_STREAM_DRAW);
        m_pixelBuff[0] = (char *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

        quint32 tmp = m_pbo1[0];
        m_pbo1[0] = m_pbo2[0];
        m_pbo2[0] = tmp;

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo2[0]);
        glBindTexture(GL_TEXTURE_2D, m_textureId[0]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        break;
    }
}


