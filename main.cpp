#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "modellist.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ModelList model;
    for (int i = 0; i < 128; i++)
    model.addItem(Item());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("myModel", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


