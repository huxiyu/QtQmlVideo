
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QtQml>
#include "qmlvideo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    qmlRegisterType<QmlVideo>("QmlVideo", 1, 0, "Video");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}


