#ifndef QMLVIDEO_H
#define QMLVIDEO_H

#include <QtQuick/QQuickPaintedItem>

class QmlVideo : public QQuickPaintedItem
{
    Q_OBJECT
public:
//    explicit
    QmlVideo(QQuickItem *parent = 0);
    ~QmlVideo();

    void paint(QPainter* p);

signals:


public slots:

};

#endif // QMLVIDEO_H
