#ifndef QMLVIDEO_H
#define QMLVIDEO_H

#include <QtQuick/QQuickPaintedItem>

class QTimer;

class QmlVideo : public QQuickPaintedItem
{
    Q_OBJECT
    Q_ENUMS(State);
    Q_PROPERTY(State state READ state WRITE setState);
public:
    enum State
    {
        Stopped,
        Playing,
        Paused
    };

    explicit QmlVideo(QQuickItem *parent = 0);
    ~QmlVideo();

    void paint(QPainter* p);

    Q_INVOKABLE State state();

signals:
    void stateChanged(State state);
    void stopped();
    void playing();
    void paused();

public slots:
    void play();
    void pause();
    void stop();
    void playPause();
    void setState(State state);

protected slots:
    virtual void frame();

private:
    QTimer *m_frameTimer;
    State m_state;
};

#endif // QMLVIDEO_H
