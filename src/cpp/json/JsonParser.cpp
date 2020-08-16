#include "JsonParser.h"
#include "../exceptions/exceptions.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

#include <ltl/operator.h>
#include <ltl/Range/actions.h>

using namespace std::chrono;

static QByteArray readFile(const QString &path) {
    QFile file{path};

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "File" << path << "not opened";
        throw NotOpenedFileException{path};
    }

    return file.readAll();
}

auto toCompileEvent(QJsonValue ref) {
    return CompileEvent{ref["args"]["detail"].toString(), //
                        microseconds{ref["ts"].toInt()},  //
                        microseconds{ref["dur"].toInt()}, //
                        {}};
}

auto constructTreeEvent(const std::vector<CompileEvent> &events) { return events; }

std::vector<CompileEvent> parseJsonObject(QJsonDocument document) {
    auto array = document.object()["traceEvents"].toArray();
    auto sortedEvents = array |                    //
                        ltl::map(toCompileEvent) | //
                        ltl::to_vector |           //
                        ltl::actions::sort_by([](const auto &a, const auto &b) { return a.begin < b.begin; });
    return constructTreeEvent(sortedEvents);
}

std::vector<CompileEvent> extractCompileEventFromJson(const QString &path) {
    auto document = QJsonDocument::fromJson(readFile(path));

    if (document.isObject())
        return parseJsonObject(std::move(document));

    throw NotParsableFileException{path + " is not a JSonObject"};
}
