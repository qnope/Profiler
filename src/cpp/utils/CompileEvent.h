#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <QString>
#include <QObject>
#include <QVariant>
#include <QVariantList>

#include <map>

#include <ltl/StrongType.h>

#include <cstdint>

using Pid = ltl::strong_type_t<int, struct PidTag, ltl::EqualityComparable, ltl::LessThan>;
using Tid = ltl::strong_type_t<int, struct TidTag, ltl::EqualityComparable, ltl::LessThan>;

struct CompileEvent {
    Pid pid;
    Tid tid;
    QString name;
    QString details;
    std::chrono::microseconds begin;
    std::chrono::microseconds duration;
};

class CompileEventNode {
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QString details MEMBER details CONSTANT)
    Q_PROPERTY(uint64_t begin READ getBegin CONSTANT)
    Q_PROPERTY(uint64_t end READ end CONSTANT)

    Q_PROPERTY(QVariantList nodeEvents READ getEvents CONSTANT)
  public:
    uint64_t end() const noexcept;

    QString name;
    QString details;
    std::chrono::microseconds begin;
    std::chrono::microseconds duration;
    std::vector<CompileEventNode> nodeEvents;

    CompileEventNode addEvent(CompileEvent event) &&;

  private:
    QVariantList getEvents() const noexcept;
    uint64_t getBegin() const noexcept;
};

Q_DECLARE_METATYPE(CompileEventNode)

using CompileEventTree = std::map<Pid, std::map<Tid, CompileEventNode>>;

CompileEventNode constructTreeEvent(std::vector<CompileEvent> events);
