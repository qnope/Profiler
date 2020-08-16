#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "json/JsonParser.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    auto jsonValues = extractCompileEventFromJson("C:/Users/mmorr/Documents/Dossier "
                                                  "Antoine/little-type-library/build-little-type-library-Clang-"
                                                  "Debug/CMakeFiles/LittleTypeLibrary.dir/test.cpp.json");

    return app.exec();
}
