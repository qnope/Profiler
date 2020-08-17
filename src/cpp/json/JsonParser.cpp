#include "JsonParser.h"
#include "../exceptions/exceptions.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDebug>

#include <cassert>

#include <ltl/algos.h>
#include <ltl/operator.h>
#include <ltl/functional.h>
#include <ltl/Range/actions.h>

using namespace std::chrono;

using namespace ltl;
using namespace ltl::actions;

static QByteArray readFile(const QString &path) {
    QFile file{path};

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "File" << path << "not opened";
        throw NotOpenedFileException{path};
    }

    return file.readAll();
}

static auto toCompileEvent(QJsonValue ref) {
    return CompileEvent{
        Pid{ref["pid"].toInt()},          //
        Tid{ref["tid"].toInt()},          //
        ref["name"].toString(),           //
        ref["args"]["detail"].toString(), //
        microseconds{ref["ts"].toInt()},  //
        microseconds{ref["dur"].toInt()}  //
    };
}

CompileEventTree parseJsonObject(QJsonDocument document) {
    CompileEventTree tree;
    auto sortedEventsByPid = document.object()["traceEvents"].toArray() | //
                             map(toCompileEvent) |                        //
                             to_vector |                                  //
                             sort_by(ascending(&CompileEvent::pid));

    for (auto [pid, events] : sortedEventsByPid | group_by(&CompileEvent::pid)) {
        auto sortedEventsByTid = events | to_vector | sort_by(ascending(&CompileEvent::tid));
        for (auto [tid, events] : sortedEventsByTid | group_by(&CompileEvent::tid)) {
            tree[pid][tid] = constructTreeEvent(events | to_vector | sort_by(ascending(&CompileEvent::begin)));
        }
    }

    return tree;
}

CompileEventTree extractCompileEventNodeFromJson(const QString &path) {
    auto document = QJsonDocument::fromJson(readFile(path));

    if (document.isObject())
        return parseJsonObject(std::move(document));

    throw NotParsableFileException{path + " is not a JSonObject"};
}
