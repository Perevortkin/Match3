#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#include <QDebug>
#include "modellist.h"
#include "gameconfig.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameConfig config;
    ModelList model(config);

    for (int i = 0; i <2 * config.rows() * config.columns(); i++)
        model.addItem(Item());

    model.searchForMatch();
    model.setFirstSearchExecuted(true);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("myModel", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}


