#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <QString>

struct CompileEvent {
    QString details;
    std::chrono::microseconds begin;
    std::chrono::microseconds duration;
    std::vector<CompileEvent> childEvents;
};

std::vector<CompileEvent> extractCompileEventFromJson(const QString &path);
