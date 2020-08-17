#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "json/JsonParser.h"

#include <QDebug>

using namespace std::chrono_literals;

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

    auto now = std::chrono::high_resolution_clock::now();

    auto jsonValues = extractCompileEventNodeFromJson(
        "A:/Programmation/Little-Type-Library/Little-Type-Library/build-Little-Type-Library-Clang-Debug/CMakeFiles/"
        "LittleTypeLibrary.dir/test.cpp.json");

    auto end = std::chrono::high_resolution_clock::now();

    qDebug() << std::chrono::duration_cast<std::chrono::microseconds>(end - now).count();
    return app.exec();
}
